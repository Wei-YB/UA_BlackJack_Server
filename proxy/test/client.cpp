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
#include <fstream>
#include <sstream>
#include <queue>

#include "EventLoop.h"
#include "CircularBuffer.h"
#include "UA_BlackJack.pb.h"
#include "TcpConnection.h"
#include "ClientProxyProtocol.h"
#include "common.h"

using namespace Net;
using ua_blackjack::Request;
using ua_blackjack::Response;

#define DEFAULT_BUFFER_SIZE 4096

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
                    size_t bufferSize = DEFAULT_BUFFER_SIZE) 
                    : conn_(server_ip, server_port, loop),
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

    int sendRequest(const Request &request) 
    {
        if (conn_.GetWriteBufferRoom() < request.ByteSizeLong() + 8)
        {
            std::cout << "fail to send request." << std::endl;
            return -1;
        }
        std::string rawRequest = request.SerializeAsString();
        std::string pkgData(8 + rawRequest.size(), '\0');
        NS::pack(NS::REQUEST, rawRequest, &pkgData[0], pkgData.size());
        return conn_.Send(pkgData);
    }

    int sendResponse(const Response &response) 
    {
        if (conn_.GetWriteBufferRoom() < response.ByteSizeLong() + 8)
        {
            return -1;
        }
        std::string rawResponse = response.SerializeAsString();
        std::string pkgData(8 + rawResponse.size(), '\0');
        NS::pack(NS::REQUEST, rawResponse, &pkgData[0], pkgData.size());
        return conn_.Send(pkgData);
    }

private:
    void OnMessages(std::vector<Request> &requests, std::vector<Response> &responses)
    {
        for (int i = 0; i < requests.size(); ++i)
        {
            std::cout << "request from server: " << std::endl;
            print(std::cout, requests[i]);
        }
        for (int i = 0; i < responses.size(); ++i)
        {
            if (uid_ == -1 && responses[i].uid() != 0)
            {
                uid_ = responses[i].uid();
            }
            std::cout << "response from server: " << std::endl;
            print(std::cout, responses[i]);
        }
    }
    void OnSendReady() {/*TODO: write requests/responses buffer to tcp connection*/;}
    void OnError() {/*do nothing, since the loop will exit by itself*/}

private:
    UserId uid_ = -1;
    TcpConnection conn_;
    CircularBuffer readBuffer_;
    CircularBuffer writeBuffer_;
};


int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port samples_path." << std::endl;
        exit(0);
    }
    // prepare all the request
    std::queue<Request> requests;
    std::ifstream fin(argv[3], std::ifstream::in);
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
        request.set_uid(12);
        request.set_requesttype(strToRequestType[item]);
        request.set_stamp((int64_t)&request);
        while (iss.good())
        {
            iss >> item;
            request.add_args(item);
        }
        requests.push(request);
    }
    fin.close();
    std::cout << requests.size() << " requests in total" << std::endl;

    Net::EventLoop loop;
    
    BlackJackClient client(argv[1], (unsigned short)atoi(argv[2]), &loop);
    if (client.Connect() != 0)
    {
        std::cout << "fail to connect to proxy." << std::endl;
        exit(0);
    }
    std::cout << "successfully connect to host" << std::endl;

    // write request to proxy one by one
    while (!requests.empty())
    {
        Request request = requests.front();
        requests.pop();
        if (client.sendRequest(request) != -1)
        {
            std::cout << "Send request: " << std::endl;
            print(std::cout, request);
        }
        if (0 > loop.loopOnce(1000))
        {
            break;
        }
    }
    
    //delete client;
    return 0;
}