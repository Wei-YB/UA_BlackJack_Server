#include <iostream>
#include <string>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <functional>

#include "asyncServiceClient.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "CircularBuffer.h"
#include "Client.h"
#include "ClientProxyProtocol.h"   
#include "common.h"
#include "ProxyServer.h"
#include "UA_BlackJack.pb.h"
#include "log.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

using namespace Net;

#define BUFFER_SIZE 1024 * 4     // 4 KB

void print(std::ostream &os, const Response &response)
{
    os << "Response details:" << std::endl; 
    os << "status: " << response.status() << std::endl;
    os << "stamp: " << response.stamp() << std::endl;
    os << "uid: " << response.uid() << std::endl; 
    if (response.args_size())
    {
        os << "args: ";
        for (int i = 0; i < response.args_size(); ++i)
        {
            const std::string &arg = response.args(i);
            os << arg << " ";
        }
        os << std::endl;
        return;
    }
    os << "null" << std::endl;
}

void print(std::ostream &os, const Request &request)
{
    os << "Request details:" << std::endl; 
    os << "type: " << requestTypeToStr[request.requesttype()] << std::endl;
    os << "stamp: " << request.stamp() << std::endl;
    os << "uid: " << request.uid() << std::endl; 
    if (request.args_size())
    {
        os << "args: ";
        for (int i = 0; i < request.args_size(); ++i)
        {
            const std::string &arg = request.args(i);
            os << arg << " ";
        }
        os << std::endl;
        return;
    }
    os << "null" << std::endl;
}

void print(std::ostream &os, Net::Event events)
{
    os << "events: ";
    if (events & Net::EV_IN) os << "EV_IN ";
    if (events & Net::EV_OUT) os << "EV_OUT ";
    if (events & Net::EV_ET) os << "EV_ET ";
    if (events & Net::EV_ERR) os << "EV_ERR ";
    os << std::endl;
}

ProxyServer::ProxyServer(const char *ip, unsigned short port, EventLoop *loop)
{
    server_ = std::make_shared<Net::TcpServer>(ip, port, loop, 
                                    std::bind(&ProxyServer::OnNewClient, this, std::placeholders::_1),
                                    std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
}

void ProxyServer::OnNewClient(std::shared_ptr<TcpConnection> conn)
{
    logger_ptr->info("In main thread: Get a new client connection")
    std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2),
                                                                std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
    fdToClient_.emplace(conn->SockFd(), newClient);
}

void ProxyServer::OnClientRequest(FileDesc fd, Request &request)
{   
     std::cout << "ProxyServer::OnClientRequest: get request from " << (request.uid() ? request.uid() : fd) << std::endl;  
     std::cout << "requestType: " << request.requesttype() << std::endl;
     // check whether the service exist
     if (requestTypeToModule.find(request.requesttype()) == requestTypeToModule.end()) 
     {
         std::cout << "an request type that is not known by proxy!" <<std::endl;
         return;
     }
     std::shared_ptr<ServiceClient> serviceClient = requestTypeToServiceClient_[request.requesttype()].lock();
    if (!serviceClient)
    {
        std::cout << "service client down!" << std::endl;
        return;
    }
    // for unlogin user, we only handle signin and login request
    if (fdToClient_[fd]->uid() == -1 
        && (request.requesttype() == Request::LOGIN
            || request.requesttype() == Request::SIGNUP))
    {
        std::cout << "comes a unlogin client want to login." << std::endl;
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as its identity,
        // since stamps from multiple clients might conflict
        int64_t stamp = (int64_t)client.get();
        {
        std::lock_guard<std::mutex> guard(stampToClientLock_);
        stampToClient_.emplace(stamp, client);
        }
        client->SetUnloginStamp(request.stamp());
        // modify the stamp so we can recognize when we get response 
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else if (fdToClient_[fd]->uid() != -1)
    {
        std::cout << "login client's request (uid: " << fdToClient_[fd]->uid() << ")" <<std::endl;
        serviceClient->Call(request);
    }
    else
    {
        std::cout << "unlogin client try to send illegal request" <<std::endl;
        // simply drop it
    }
}

// this callback directly forward the response to RpcServer
void ProxyServer::OnClientResponse(Response &response)
{
    if (clientResponseCallBack_)
        clientResponseCallBack_(response);
}

void ProxyServer::OnServiceResponse(const Response& response)
{
    std::cout << "On service response (uid: " << response.uid() << std::endl;
    UserId uid = response.uid();
    int64_t stamp = response.stamp();
    // if the response is for a logined client
    std::shared_ptr<Client> client;
    {
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (uidToClient_.find(uid) != uidToClient_.end())
        {
            client = uidToClient_[uid];
        }
    }
    if (client)
    {
        std::cout << "a login client's response" << std::endl;
        client->SendResponse(response);
        return;
    }
    // if the response is for unlogin client
    std::weak_ptr<Client> client_weak;
    {
        std::lock_guard<std::mutex> guard(stampToClientLock_);
        if (stampToClient_.find(stamp) != stampToClient_.end())
        {
            client_weak = stampToClient_[stamp];
            stampToClient_.erase(stamp);
        }
    }
    if (client = client_weak.lock())
    {
        std::cout << "an unlogin client." << std::endl;
        std::cout << "set its uid to be " << uid << std::endl;
        client->SetUid(uid);
        {
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.emplace(uid, client);
        }
        client->SendResponse(response);
    }
    else
    {
        std::cout << "an unknown response for uid: " << uid  << std::endl;
    }
}

void ProxyServer::OnDisConnection(FileDesc fd)
{
    std::shared_ptr<Client> client = fdToClient_[fd];
    fdToClient_.erase(fd);
    {
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (client->uid() != -1)
        {
            uidToClient_.erase(client->uid());
        }
    }  
}

void ProxyServer::OnError(FileDesc fd)
{
    // if it was not client, it must be the tcp server
    if (fdToClient_.find(fd) == fdToClient_.end())
    {
        std::cout << "fatal error, exiting now." << std::endl;
        abort();
    }
    OnDisConnection(fd);
}

// this function should be called by the RpcServer
int ProxyServer::SendRequest(Request &request)
{
    UserId uid = request.uid();
    std::shared_ptr<Client> client;
    // if the response is for a logined client
    {      
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (uidToClient_.find(uid) != uidToClient_.end())
        {
            client = uidToClient_[uid];
        }
    }
    if (client)
    {
        return client->SendRequest(request);
    }
    return -1;
}
