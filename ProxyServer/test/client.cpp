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
#include <chrono>

#include "EventLoop.h"
#include "CircularBuffer.h"
#include "UA_BlackJack.pb.h"
#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "common.h"
#include "log.h"

typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;
typedef std::chrono::milliseconds MilliSeconds;

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
                    EventLoop *loop, 
                    int64_t objId,
                    size_t bufferSize = DEFAULT_BUFFER_SIZE) 
                    : conn_(server_ip, server_port, loop), 
                    objId_(objId),
                    readBuffer_(bufferSize), 
                    writeBuffer_(bufferSize),
                    createTime_(SteadyClock::now()) {}
public:
    int Connect()
    {
        int ret = conn_.Connect();
        if (ret != -1)
        {
            conn_.SetInputCallBack(std::bind(&BlackJackClient::OnMessages, this, std::placeholders::_1));
            conn_.SetOutPutCallBack(std::bind(&BlackJackClient::OnSendReady, this));
            conn_.SetHupCallBack(std::bind(&BlackJackClient::OnError, this));
            conn_.SetErrorCallBack(std::bind(&BlackJackClient::OnError, this));
            conn_.SetEncoder(std::bind(pack, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            // conn_.SetDecoder(std::bind(unpack, std::placeholders::_1, std::placeholders::_2));
            conn_.SetDecoder(std::bind(unpack_sp, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            setNonBlocking(conn_.SockFd());
            return 0;
        }
        return -1;
    }

    int sendRequest(Request &request) 
    {
        if (uid_ == -1 
            && request.requesttype() != Request::LOGIN
            && request.requesttype() != Request::SIGNUP)
        {
            // logger_ptr->warn("client (uid: {0} fd: {1}) try to send invalid request.", uid_, conn_.SockFd());
            return -1;
        }
        request.set_uid(uid_ == -1 ? 0 : uid_);
        int64_t stamp = std::chrono::duration_cast<MilliSeconds>(SteadyClock::now() - createTime_).count();
        request.set_stamp(stamp);
        stampToRequest_.emplace(stamp, request);

        std::string rawRequest(std::move(request.SerializeAsString()));
        if (request.requesttype() == Request::LOGOUT)
        {
            uid_ = -1;
        }
        // record
        conn_.Send(REQUEST, rawRequest);
        
        return 0;
    }
    
    void SetRequests(const std::queue<Request> &requests) {requests_ = requests;}
    
    UserId uid() const {return uid_;}

private:
    //void OnMessages(std::vector<std::pair<int32_t, std::string>> msgs)
    void OnMessages(std::vector<std::pair<int32_t, StringPiece>> msgs)
    {
        for (int i = 0; i < msgs.size(); ++i)
        {
            if (msgs[i].first != RESPONSE)
            {
                msgs[i].second.free();
                continue;
            }
            Response response;
            ParseFromStringPiece(response, msgs[i].second);
            //response.ParseFromString(std::get<1>(msgs[i]));
            // logger_ptr->info("client (uid: {0}, fd: {1}) gets response from proxy", uid_, conn_.SockFd());
            waittingResponse_ = false;
            int64_t stamp = response.stamp();
            if (uid_ == -1)
            {
                uid_ = response.uid();
                // logger_ptr->info("setting client's uid to {}", uid_);
            }
            stampToRequest_.erase(stamp);
            stamp = std::chrono::duration_cast<MilliSeconds>(SteadyClock::now() - createTime_).count() - stamp;
            stamp = stamp < 0 ? 0 : stamp;
            // logger_ptr->info("response time: {} ms", stamp);
            // update statistic
            responseTime_ = responseTime_ * ((double)requestSent_ / (requestSent_ + 1)) + (double)stamp / (requestSent_ + 1);
            requestSent_++;
        }
        if (!waittingResponse_ && !requests_.empty())
        {
            if (sendRequest(requests_.front()) > -1)
            {
                // logger_ptr->info("client (uid: {0}, fd: {1}) successfully send {2} request to proxy.", uid_, conn_.SockFd(), requestTypeToStr[requests_.front().requesttype()]);
                waittingResponse_ = true;
                requests_.pop();
            }
        }
        else if (!waittingResponse_ && requests_.empty())
        {
            // logger_ptr->info("client (uid: {0}, fd: {1}) has sent all requests to proxy.", uid_, conn_.SockFd());
            conn_.DisConnect();
            stats[objId_] = responseTime_;
        }
    }

    void OnSendReady() 
    {
        if (!waittingResponse_ && !requests_.empty())
        {
            if (sendRequest(requests_.front()) > -1)
            {
                // logger_ptr->info("client (uid: {0}, fd: {1}) successfully send {2} request to proxy.", uid_, conn_.SockFd(), requestTypeToStr[requests_.front().requesttype()]);
                requests_.pop();
                waittingResponse_ = true;
            }
        }
        else if (!waittingResponse_ && requests_.empty())
        {
            // logger_ptr->info("client (uid: {0}, fd: {1}) has sent all requests to proxy.", uid_, conn_.SockFd());
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
    TimePoint createTime_;
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
    if (argc < 6)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port samples_path client_number log_path." << std::endl;
        exit(0);
    }

    logger_flush_on();
    set_logger_name("client_logger");
    set_log_path(argv[5]);
    create_logger();
    
    // prepare all the request
    std::queue<Request> requests;
    parseFile(argv[3], requests);
    if (requests.size() == 0)
    {
        // logger_ptr->error("fail to parse samples file.");
        exit(0);
    }

    Net::EventLoop loop;
    
    std::vector<std::shared_ptr<BlackJackClient>> clients;
    int clientno = atoi(argv[4]);
    if (clientno <= 0 || clientno > 20000)
    {
        // logger_ptr->error("invalid client number or client number too large.");
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
        // logger_ptr->error("no clients connected to proxy.");
        exit(0);
    }
    // logger_ptr->info("{} clients connected to proxy.", connCnt);
    
    signal(SIGINT, stop_client);
    // now start to flood the proxy
    TimePoint begin = SteadyClock::now();
    while (!flag && loop.loopOnce(1000) != -1);
    TimePoint end = SteadyClock::now();

    if (flag)
    {
        // logger_ptr->info("program stopped by SIGINT.");
    }
    else
    {
        double sum = 0.0;
        for (int i = 0; i < stats.size(); ++i) sum += stats[i];
        sum /= stats.size();
        std::cout << "QPS: " 
                  << requests.size() * clientno * 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
                  << std::endl;
        std::cout << "average response time: " << sum << " ms" << std::endl;
    }
    //delete client;
    return 0;
}
