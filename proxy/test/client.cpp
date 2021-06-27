#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <unordered_map>

#include "EventLoop.h"
#include "CircularBuffer.h"
#include "UA_BlackJack.pb.h"
#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "common.h"

typedef std::chrono::time_point<std::chrono::steady_clock> time_point;

using namespace Net;
using ua_blackjack::Request;
using ua_blackjack::Response;

#define DEFAULT_BUFFER_SIZE 4096

std::vector<double> stats;
bool flag = false;

void stop_client(int)
{
    flag = true;
}

static int setNonBlocking(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

class BlackJackClient
{
public:
    BlackJackClient(const char *server_ip, 
                    unsigned short server_port, 
                    EventLoop *loop, int64_t objId,
                    size_t bufferSize = DEFAULT_BUFFER_SIZE) 
                    : conn_(server_ip, server_port, loop), objId_(objId),
                    readBuffer_(bufferSize), 
                    writeBuffer_(bufferSize) {}
public:
    int Connect()
    {
        int ret = conn_.Connect();
        if (ret != -1)
        {
            conn_.SetInputCallBack(std::bind(&BlackJackClient::OnMessages, this, std::placeholders::_1, std::placeholders::_2));
            conn_.SetOutPutCallBack(std::bind(&BlackJackClient::OnSendReady, this));
            conn_.SetErrorCallBack(std::bind(&BlackJackClient::OnError, this));
            return setNonBlocking(conn_.SockFd());
        }
        return -1;
    }

    int sendRequest(Request &request) 
    {
        if (uid_ == -1 
            && request.requesttype() != Request::LOGIN
            && request.requesttype() != Request::SIGNUP)
        {
            std::cout << "invalid request." << std::endl;
            return -1;
        }
        if (conn_.GetWriteBufferRoom() < request.ByteSizeLong() + 8)
        {   // should wait
            return 0;
        }
        request.set_uid(uid_ == -1 ? 0 : uid_);
        time_point now = std::chrono::steady_clock::now();
        int64_t stamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
        request.set_stamp(stamp);
        stampToRequest_.emplace(stamp, request);

        std::string rawRequest = request.SerializeAsString();
        std::string pkgData(8 + rawRequest.size(), '\0');
        NS::pack(NS::REQUEST, rawRequest, &pkgData[0], pkgData.size());
        if (request.requesttype() == Request::LOGOUT)
        {
            uid_ = -1;
        }
        // record
        conn_.Send(pkgData);
        
        //print(std::cout, request);
        return 1;
    }
    void SetRequests(const std::queue<Request> &requests) {requests_ = requests;}
    UserId uid() const {return uid_;}

private:
    void OnMessages(std::vector<Request> &requests, std::vector<Response> &responses)
    {
        for (int i = 0; i < requests.size(); ++i)
        {
            std::cout << "request from server: " << std::endl;
            //print(std::cout, requests[i]);
        }
        for (int i = 0; i < responses.size(); ++i)
        {
            std::cout << "response from server" << std::endl;
            waittingResponse_ = false;
            int64_t stamp = responses[i].stamp();
            if (uid_ == -1)
            {
                std::cout << "setting client's uid" << std::endl;
                uid_ = responses[i].uid();
            }
            else if (stampToRequest_[stamp].requesttype() == Request::LOGOUT)
            {
                uid_ = -1;
            }
            stampToRequest_.erase(stamp);
            time_point now = std::chrono::steady_clock::now();
            stamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() - stamp;
            stamp = stamp < 0 ? 0 : stamp;
            std::cout << "response time: " << stamp << " ms" << std::endl;
            // update statistic
            responseTime_ = responseTime_ * ((double)requestSent_ / (requestSent_ + 1)) + (double)stamp / (requestSent_ + 1);
            requestSent_++;
            // std::cout << "response from server: " << std::endl;
            // print(std::cout, responses[i]);
          
        }
        if (!waittingResponse_ && !requests_.empty())
        {
            Request request = requests_.front();
            if (sendRequest(request) > 0)
            {
                std::cout << "successfully send request to proxy." << std::endl;
                waittingResponse_ = true;
                requests_.pop();
            }
        }
        else if (!waittingResponse_ && requests_.empty())
        {
            std::cout << "we have sent all the request to proxy" << std::endl;
            conn_.DisConnect();
            stats[objId_] = responseTime_;
        }
    }

    void OnSendReady() 
    {
        std::cout << "call to OnSendReady" << std::endl;
        if (!waittingResponse_ && !requests_.empty())
        {
            Request request = requests_.front();
            if (sendRequest(request) > 0)
            {
                std::cout << "successfully send a request to proxy." << std::endl;
                requests_.pop();
                waittingResponse_ = true;
            }
        }
        else if (!waittingResponse_ && requests_.empty())
        {
            conn_.DisConnect();
            stats[objId_] = responseTime_;
        }
    }
    void OnError() 
    {
        conn_.DisConnect();
    }

private:
    int64_t objId_ = 0;
    UserId uid_ = -1;
    TcpConnection conn_;
    CircularBuffer readBuffer_;
    CircularBuffer writeBuffer_;
    //
    std::unordered_map<int64_t, Request> stampToRequest_;
    int requestSent_ = 0;
    double responseTime_ = 0;
    std::queue<Request> requests_;
    bool waittingResponse_ = false;
};

void parseFile(const char *filepath, std::queue<Request> &res)
{
    std::ifstream fin(filepath, std::ifstream::in);
    while (fin.good())
    {
        Request request;
        std::string line;
        std::string item;
        getline(fin, line);
        std::istringstream iss(line);
        iss >> item;
        if (strToRequestType.find(item) == strToRequestType.end())
        {
            continue;
        }
        request.set_requesttype(strToRequestType[item]);
        while (iss.good())
        {
            iss >> item;
            request.add_args(item);
        }
        res.push(request);
    }
    fin.close();
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port samples_path client_number." << std::endl;
        exit(0);
    }
    // prepare all the request
    std::queue<Request> requests;
    parseFile(argv[3], requests);
    if (requests.size() == 0)
    {
        std::cout << "fail to parse samples file." << std::endl;
        exit(0);
    }

    Net::EventLoop loop;
    
    std::vector<std::shared_ptr<BlackJackClient>> clients;
    int clientno = atoi(argv[4]);
    if (clientno <= 0 || clientno > 20000)
    {
        std::cout << "invalid client number or client number too large." << std::endl;
        exit(0);
    }
    // create clients
    for (int i = 0; i < clientno; ++i)
    {
        clients.push_back(std::make_shared<BlackJackClient>(argv[1], (unsigned short)atoi(argv[2]), &loop, i));
    }
    stats = std::vector<double>(clientno, 0.0);
    // connect to server one by one
    int connCnt = 0;
    for (int i = 0; i < clientno; ++i)
    {
        if (clients[i]->Connect() != -1)
        {
            connCnt++;
            clients[i]->SetRequests(requests);
        }
    }
    if (connCnt == 0)
    {
        std::cout << "no clients connected to proxy." << std::endl;
        exit(0);
    }
    std::cout << connCnt << " clients connected to proxy." << std::endl;
    
    signal(SIGINT, stop_client);
    // now start to flood the proxy
    while (!flag && loop.loopOnce(1000) != -1);
    
    if (flag)
    {
        std::cout << "program stopped by SIGINT." << std::endl;
    }
    else
    {
        double sum = 0.0;
        for (int i = 0; i < stats.size(); ++i) sum += stats[i];
        sum /= stats.size();
        std::cout << "average response time from proxy: " << sum << " ms" << std::endl;
    }
    //delete client;
    return 0;
}
