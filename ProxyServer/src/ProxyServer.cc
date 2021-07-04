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

ProxyServer::ProxyServer(const char *ip, unsigned short port, EventLoop *loop)
{
    server_ = std::make_shared<TcpServer>(ip, port, loop, 
                                    std::bind(&ProxyServer::OnNewClient, this, std::placeholders::_1),
                                    std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
}

// this should be called by tcp server
void ProxyServer::OnNewClient(std::shared_ptr<TcpConnection> conn)
{
    // logger_ptr->info("In main thread: Get a new client connection (sockfd: {})", conn->SockFd());
    std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2),
                                                                std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnDisConnection, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
    fdToClient_.emplace(conn->SockFd(), newClient);
}

void ProxyServer::OnClientRequest(FileDesc fd, Request request)
{   
    // check whether the request valid
    if (requestTypeToModule.find(request.requesttype()) == requestTypeToModule.end()) 
    {
        // logger_ptr->info("In main thread: Get unkonwn request (type number: {2}) from client (uid: {0}, sockfd: {1})", request.uid(), fd, request.requesttype());
        return;
    }
    // logger_ptr->info("In main thread: Get request (type: {2}, stamp: {3}) from client (uid: {0}, sockfd: {1})", request.uid(), fd, requestTypeToStr[request.requesttype()], request.stamp());
    // check whether the service exist
    std::shared_ptr<ServiceClient> serviceClient = requestTypeToServiceClient_[request.requesttype()].lock();
    if (!serviceClient)
    {
        // logger_ptr->info("In main thread: Service unavailable!");
        return;
    }
    // for unlogin user, we only handle signin and login request
    if (fdToClient_[fd]->uid() == -1 && request.requesttype() == Request::LOGIN)
    {
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as its identity,
        // since stamps from multiple clients might conflict
        int64_t stamp = (int64_t)client.get();
        // logger_ptr->info("In main thread: setting the request stamp to {}", stamp);
        {
        std::lock_guard<std::mutex> guard(stampToUnloginClientLock_);
        stampToUnloginClient_.emplace(stamp, client);
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
        // logger_ptr->info("In main thread: setting the request stamp to {}", stamp);
        {
        std::lock_guard<std::mutex> guard(stampToSignupClientLock_);
        stampToSignupClient_.emplace(stamp, client);
        }
        client->SetSignupStamp(request.stamp());
        // modify the stamp so we can recognize when we get response 
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else if (fdToClient_[fd]->uid() != -1)
    {
        // logger_ptr->info("In main thread: client (uid: {}) directly call", fdToClient_[fd]->uid());
        serviceClient->Call(request);
    }
    else
    {
        // logger_ptr->info("In main thread: Client (uid: {0}, sockfd: {1}) sends illegal request, drop it.", request.uid(), fd);
        // simply drop it
    }
}

// this callback directly forward the response to RpcServer
void ProxyServer::OnClientResponse(Response response)
{
    // logger_ptr->info("In main thread: Get response (stamp: {1}) from client (uid: {0})", response.uid(), response.stamp());
    if (clientResponseCallBack_)
        clientResponseCallBack_(response);
}

void ProxyServer::OnServiceResponse(Response& response)
{
    UserId uid = response.uid();
    int64_t stamp = response.stamp();
    // logger_ptr->info("In service client thread: Get response for client (uid: {0}, stamp: {1})", uid, stamp);
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
            // logger_ptr->info("In service client thread: This response is for an unlogin client.");
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
            // logger_ptr->info("In service client thread: This response is for an signup client.");
            client_weak = stampToSignupClient_[stamp];
            stampToSignupClient_.erase(stamp);
        }
    }
    if (client = client_weak.lock())
    {   
        // restore the original stamp
        if (flag)
            response.set_stamp(client->unloginStamp());
        else
            response.set_stamp(client->signupStamp());
        // if this is a login (and successful) response, set its uid and put into login client set
        if (flag & response.status() == 0)
        {
            // logger_ptr->info("In service client thread: setting client uid to {}.", uid);
            client->SetUid(uid);
            // add client to login set if it is login client
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.emplace(uid, client);
        }
        client->SendResponse(response);
    }
    else
    {
        // logger_ptr->info("In service client thread: Unknown response for uid: {}.", uid);
    }
}

void ProxyServer::OnDisConnection(FileDesc fd)
{
    std::shared_ptr<Client> client = fdToClient_[fd];
    // logger_ptr->info("In main thread: found the exiting client by fd: {}, deleting it...", fd);
    fdToClient_.erase(fd);
    if (client->uid() != -1)
    {
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (uidToClient_.find(client->uid()) != uidToClient_.end())
            uidToClient_.erase(client->uid());
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
    // logger_ptr->info("In main thread: Client (uid: {0} sockfd: {1}) out.", client->uid(), fd);
}

void ProxyServer::OnError(FileDesc fd)
{
    // if it was not client, it must be the tcp server
    if (fd == server_->listenFd())
    {
        // logger_ptr->info("In main thread: Fatal error in tcp server, exiting now...");
        abort();
    }
    // logger_ptr->info("In main thread: Fatal error in client (sockfd: {})", fd);
    OnDisConnection(fd);
}

// this function should be called by the RpcServer
int ProxyServer::SendRequest(Request &request)
{
    UserId uid = request.uid();
    std::shared_ptr<Client> client;
    // if the request is to a logined client
    {      
        std::lock_guard<std::mutex> guard(uidToClientLock_);
        if (uidToClient_.find(uid) != uidToClient_.end())
        {
            client = uidToClient_[uid];
        }
    }
    if (client)
    {
        // logger_ptr->info("In gRPC server thread: Send request (type: {0}) to client (uid: {1})", requestTypeToStr[request.requesttype()], uid);
        return client->SendRequest(request);
    }
    // logger_ptr->info("In gRPC server thread: Send request (type: {0}) to unknown client (uid: {1})", requestTypeToStr[request.requesttype()], uid);
    return -1;
}
