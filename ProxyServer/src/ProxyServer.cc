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
    logger_ptr->info("In main thread: Get a new client connection (sockfd: {})", conn->SockFd());
    std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2),
                                                                std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnDisConnection, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
    fdToClient_.emplace(conn->SockFd(), newClient);
}

void ProxyServer::OnClientRequest(FileDesc fd, Request &request)
{   
    // check whether the request valid
    if (requestTypeToModule.find(request.requesttype()) == requestTypeToModule.end()) 
    {
        logger_ptr->info("In main thread: Get unkonwn request (type number: {2}) from client (uid: {0}, sockfd: {1})", 
                                        request.uid(), fd, request.requesttype());
        return;
    }
    logger_ptr->info("In main thread: Get request (type: {2}) from client (uid: {0}, sockfd: {1})", 
                                        request.uid(), fd, requestTypeToStr[request.requesttype()]);
    // check whether the service exist
    std::shared_ptr<ServiceClient> serviceClient = requestTypeToServiceClient_[request.requesttype()].lock();
    if (!serviceClient)
    {
        logger_ptr->info("In main thread: Service unavailable!");
        return;
    }
    // for unlogin user, we only handle signin and login request
    if (fdToClient_[fd]->uid() == -1 && request.requesttype() == Request::LOGIN)
    {
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as its identity,
        // since stamps from multiple clients might conflict
        int64_t stamp = (int64_t)client.get();
        logger_ptr->info("In main thread: login request, setting the request stamp to {}", stamp);
        {
        std::lock_guard<std::mutex> guard(stampToUnloginClientLock_);
        if (!stampToUnloginClient_.emplace(stamp, client).second)
        {   // don't send it, since it has conflicts
            logger_ptr->warn("In main thread: Conflict happens when inserting a unlogin client (fd: {}) to unlogin client set.", fd);
            return;
        }
        }
        client->SetUnloginStamp(request.stamp());
        // modify the stamp so we can recognize when we get response 
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else if (fdToClient_[fd]->uid() == -1 && request.requesttype() == Request::SIGNUP)
    {
        
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as its identity,
        // since stamps from multiple clients might conflict
        int64_t stamp = (int64_t)client.get();
        logger_ptr->info("In main thread: signup request, setting the request stamp to {}", stamp);
        {
        std::lock_guard<std::mutex> guard(stampToSignupClientLock_);
        if (!stampToSignupClient_.emplace(stamp, client).second)
        {   // don't send it, since it has conflicts
            logger_ptr->warn("In main thread: Conflict happens when inserting a signup client (fd: {}) to signup client set.", fd);
            return;
        }
        }
        client->SetSignupStamp(request.stamp());
        // modify the stamp so we can recognize when we get response 
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else if (fdToClient_[fd]->uid() != -1)
    {
        logger_ptr->info("In main thread: client (uid: {}) directly call", fdToClient_[fd]->uid());
        serviceClient->Call(request);
    }
    else
    {
        logger_ptr->info("In main thread: Client (uid: {0}, sockfd: {1}) sends illegal request, drop it.", 
                                        request.uid(), fd);
        // simply drop it
    }
}

// this callback directly forward the response to RpcServer
void ProxyServer::OnClientResponse(Response &response)
{
    logger_ptr->info("In main thread: Get response (stamp: {1}) from client (uid: {0})", response.uid(), response.stamp());
    if (clientResponseCallBack_)
        clientResponseCallBack_(response);
}

void ProxyServer::OnServiceResponse(const Response& response)
{
    UserId uid = response.uid();
    int64_t stamp = response.stamp();
    logger_ptr->info("In service client thread: Get response for client (uid: {0}, stamp: {1})", uid, stamp);
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
    // if the response is for unlogin/signup client
    bool flag = false;
    std::weak_ptr<Client> client_weak;
    {
        // if the response for logining client
        std::lock_guard<std::mutex> guard(stampToUnloginClientLock_);
        if (stampToUnloginClient_.find(stamp) != stampToUnloginClient_.end())
        {
            logger_ptr->info("In service client thread: This response is for an unlogin client.");
            flag = true;
            client_weak = stampToUnloginClient_[stamp];
            stampToUnloginClient_.erase(stamp);
        }
    }
    if (!flag)
    {
        // if the response for signup client
        std::lock_guard<std::mutex> guard(stampToSignupClientLock_);
        if (stampToSignupClient_.find(stamp) != stampToSignupClient_.end())
        {
            logger_ptr->info("In service client thread: This response is for an signup client.");
            client_weak = stampToSignupClient_[stamp];
            stampToSignupClient_.erase(stamp);
        }
    }
    if (client = client_weak.lock())
    {   
        // if this is a login (and successful) response , set its uid and put into login client set
        if (flag & response.status() == 0)
        {
            client->SetUid(uid);
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.emplace(uid, client);
        }
        client->SendResponse(response);
    }
    else
    {
        logger_ptr->info("In service client thread: Unknown response for uid: {}.", uid);
    }
}

void ProxyServer::OnDisConnection(FileDesc fd)
{
    std::shared_ptr<Client> client = fdToClient_[fd];
    logger_ptr->info("In main thread: found the exiting client by fd: {}, deleting it...", fd);
    fdToClient_.erase(fd);
    {
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (client->uid() != -1)
        {
            uidToClient_.erase(client->uid());
        }
    }  
    // if the client is a login client, notify the lobby that the client has logouted
    if (client->uid() != -1)
    {
        if (std::shared_ptr<ServiceClient> lobbyClient = requestTypeToServiceClient_[Request::LOGOUT].lock())
        {
            Request logoutRequest;
            logoutRequest.set_uid(client->uid());
            logoutRequest.set_requesttype(Request::LOGOUT);
            lobbyClient->Call(logoutRequest);
        }
    }
    logger_ptr->info("In main thread: Client (uid: {0} sockfd: {1}) out.", client->uid(), fd);
}

void ProxyServer::OnError(FileDesc fd)
{
    // if it was not client, it must be the tcp server
    if (fdToClient_.find(fd) == fdToClient_.end())
    {
        logger_ptr->info("In main thread: Fatal error in tcp server, exiting now...");
        abort();
    }
    logger_ptr->info("In main thread: Fatal error in client (sockfd: {})", fd);
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
        logger_ptr->info("In gRPC server thread: Send request (type: {0}) to client (uid: {1})", 
                                                requestTypeToStr[request.requesttype()], uid);
        return client->SendRequest(request);
    }
    logger_ptr->info("In gRPC server thread: Send request (type: {0}) to unknown client (uid: {1})", 
                                                requestTypeToStr[request.requesttype()], uid);
    return -1;
}
