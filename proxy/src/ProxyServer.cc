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

using ua_blackjack::Request;
using ua_blackjack::Response;
using namespace Net;

#define MAX_FORWORD_FAILURE 5

#define BUFFER_SIZE 1024 * 4     // 4 KB
#define QUEUE_SIZE  128

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

ProxyServer::ProxyServer(const char *ip, unsigned short port, Net::EventLoop *loop)
{
    server_ = std::make_shared<Net::TcpServer>(ip, port, loop, 
                                    std::bind(&ProxyServer::OnNewClient, this, std::placeholders::_1),
                                    std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
}

void ProxyServer::OnNewClient(std::shared_ptr<TcpConnection> conn)
{
    // std::cout << "ProxyServer::OnNewClient" << std::endl;
    std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2),
                                                                std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
    fdToClient_.emplace(conn->SockFd(), newClient);
}

void ProxyServer::OnClientRequest(FileDesc fd, Request &request)
{   
    // std::cout << "ProxyServer::OnClientRequest: get request from " << (request.uid() ? request.uid() : fd)<<  
    // check whether the service exist
    std::shared_ptr<ServiceClient> serviceClient = requestTypeToServiceClient_[request.requesttype()].lock();
    if (!serviceClient)
    {
        return;
    }
    // remenber an unlogin user
    if (request.uid() == -1)
    {
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as the identity of this client
        // since the stamps from multiple client might conflict
        int64_t stamp = (int64_t)client.get();
        {
        std::lock_guard<std::mutex> guard(stampToClientLock_);
        stampToClient_.emplace(stamp, client);
        }
        client->SetUnloginStamp(request.stamp());
        // modify the stamp so we can reconize the corresponding response
        request.set_stamp(stamp);
    }
    serviceClient->Call(request);
}

// this callback directly forward the response to RpcServer
void ProxyServer::OnClientResponse(Response &response)
{
    if (clientResponseCallBack_)
        clientResponseCallBack_(response);
}

void ProxyServer::OnServiceResponse(const Response& response)
{
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
        client->SendResponse(response);
        return;
    }
    // if the response is for non-login client
    std::weak_ptr<Client> client_weak;
    {
        std::lock_guard<std::mutex> guard(stampToClientLock_);
        if (stampToClient_.find(stamp) != stampToClient_.end())
        {
            client_weak = stampToClient_[stamp];
            stampToClient_.erase(stamp);
        }
    }
    client = client_weak.lock();
    if (client)
    {
        client->SetUid(uid);
        {
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.emplace(uid, client);
        }
        client->SendResponse(response);
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
