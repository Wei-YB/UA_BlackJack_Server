#include <iostream>
#include <string>
#include <mutex>
#include <utility>
#include <unordered_map>
#include <functional>

#include "asyncServiceClient.h"
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

#define BUFFER_SIZE 1024 * 2     // 4 KB

ProxyServer::ProxyServer(const char *ip, 
                         unsigned short port, 
                         EventLoop *loop, 
                         int healthReportPeriod)
                         : timer_(loop, std::bind(&ProxyServer::OnHealthReport, this)),
                         server_(std::make_shared<TcpServer>(ip, port, loop, 
                                std::bind(&ProxyServer::OnNewClient, this, std::placeholders::_1),
                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1)))
{
    if (healthReportPeriod > 0)
    {
        timer_.SetPeriod(healthReportPeriod);
    }
}

// this should be called by tcp server
void ProxyServer::OnNewClient(std::shared_ptr<TcpConnection> conn)
{
    logger_ptr->info("In ProxyServer::OnNewClient(): Get a new client connection (sockfd: {})", conn->SockFd());
    std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2),
                                                                std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnDisConnection, this, std::placeholders::_1),
                                                                std::bind(&ProxyServer::OnError, this, std::placeholders::_1));
    if (fdToClient_.find(conn->SockFd()) != fdToClient_.end())
        fdToClient_[conn->SockFd()] = newClient;    // fuck the original client out
    else
        fdToClient_.emplace(conn->SockFd(), newClient);
}

void ProxyServer::OnClientRequest(FileDesc fd, Request request)
{   
    // check whether the request valid
    if (requestTypeToModule.find(request.requesttype()) == requestTypeToModule.end()) 
    {
        logger_ptr->warn("In ProxyServer::OnClientRequest(): Get unkonwn request (type number: {2}) from client (uid: {0}, sockfd: {1})", request.uid(), fd, request.requesttype());
        return;
    }
    logger_ptr->trace("In ProxyServer::OnClientRequest(): Get request (type: {2}, stamp: {3}) from client (uid: {0}, sockfd: {1})", request.uid(), fd, requestTypeToStr[request.requesttype()], request.stamp());
    // check whether the service exist
    std::shared_ptr<ServiceClient> serviceClient = requestTypeToServiceClient_[request.requesttype()].lock();
    if (!serviceClient)
    {
        logger_ptr->warn("In ProxyServer::OnClientRequest(): Service ({}) unavailable!", requestTypeToStr[request.requesttype()]);
        return;
    }
    // for unlogin user, we only handle signin and login request
    if (fdToClient_[fd]->uid() == -1)
    {
        std::shared_ptr<Client> client = fdToClient_[fd];
        // we use the memory addr of the client as its identity,
        // since stamps from multiple clients might conflict
        int64_t stamp = (int64_t)client.get();
        if (request.requesttype() == Request::LOGIN)
        {
            std::lock_guard<std::mutex> guard(stampToUnloginClientLock_);
            // conflict happens! must check whether the previous client's still alive
            if (stampToUnloginClient_.find(stamp) != stampToUnloginClient_.end())
            {   logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict happens when inserting a unlogin client (fd: {}) to unlogin client set.", fd);
                if (!stampToUnloginClient_[stamp].lock())
                {   // it is ok to replace it, since it is a expired client
                    logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict resolved for client (fd: {})", fd);
                    stampToUnloginClient_[stamp] = client;
                }
                else
                {
                    logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict cannot be resolved for client (fd: {})", fd);
                    return;
                }
            }
            else
            {
                stampToUnloginClient_.emplace(stamp, client);
            }
        }
        else if (request.requesttype() == Request::SIGNUP)
        {
            std::lock_guard<std::mutex> guard(stampToSignupClientLock_);
            if (stampToSignupClient_.find(stamp) != stampToSignupClient_.end())
            {   logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict happens when inserting a signup client (fd: {}) to signup client set.", fd);
                if (!stampToSignupClient_[stamp].lock())
                {   // it is ok to replace it, since it is a expired client
                    logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict resolved for client (fd: {})", fd);
                    stampToSignupClient_[stamp] = client;
                }
                else
                {
                    logger_ptr->warn("In ProxyServer::OnClientRequest(): Conflict cannot be resolved for client (fd: {})", fd);
                    return;
                }
            }
            else
            {
                stampToSignupClient_.emplace(stamp, client);
            }
        }
        else 
        {
            logger_ptr->warn("In ProxyServer::OnClientRequest(): Client (uid: {0}, sockfd: {1}) sends illegal request, drop it.", request.uid(), fd);
            return;
        }
        client->SetUnloginStamp(request.stamp());
        // modify the stamp so we can recognize when we get response 
        logger_ptr->trace("In ProxyServer::OnClientRequest(): setting client (fd: {1}) request's stamp to {0}", stamp, fd);
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else if (request.requesttype() == Request::LOGOUT)
    {   // we have to handle logout request since we should remove it
        logger_ptr->info("In ProxyServer::OnClientRequest(): client (fd: {0}, uid: {1}) logout.", fd, request.uid()); 
        std::shared_ptr<Client> logoutClient = fdToClient_[fd];
        int64_t stamp = (int64_t)logoutClient.get();
        {
        std::lock_guard<std::mutex> guard(stampToLogoutClientLock_);
        if (stampToLogoutClient_.find(stamp) != stampToLogoutClient_.end())
        {   logger_ptr->warn("In ProxyServer::OnClientRequest(): logout conflict happends, resolving...");
            stampToLogoutClient_[stamp] = logoutClient;
        }
        else
            stampToLogoutClient_.emplace(stamp, logoutClient);
        }
        logoutClient->SetLogoutStamp(request.stamp());
        request.set_stamp(stamp);
        serviceClient->Call(request);
    }
    else
    {
        logger_ptr->trace("In ProxyServer::OnClientRequest(): client (uid: {0}) directly call {1}", fdToClient_[fd]->uid(), requestTypeToStr[request.requesttype()]);
        serviceClient->Call(request);
    }
}

// this callback directly forward the response to RpcServer
void ProxyServer::OnClientResponse(Response response)
{
    logger_ptr->trace("ProxyServer::OnClientResponse(): Get response (stamp: {1}) from client (uid: {0})", response.uid(), response.stamp());
    // int cnt = 0;
    // for (auto arg: response.args())
    // {
    //     logger_ptr->info("In main thread: arg{0}: {1}", cnt++, arg);
    // }
    if (clientResponseCallBack_)
        clientResponseCallBack_(response);
}

void ProxyServer::OnServiceResponse(Response& response)
{
    UserId uid = response.uid();
    int64_t stamp = response.stamp();
    logger_ptr->trace("In ProxyServer::OnServiceResponse: Get response for client (uid: {0}, stamp: {1})", uid, stamp);
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
        bool isLogout = false;
        // check whether it is a logout response
        {
            std::lock_guard<std::mutex> guard(stampToLogoutClientLock_);
            if (stampToLogoutClient_.find(stamp) != stampToLogoutClient_.end())
            {   
                response.set_stamp(client->logoutStamp());
                stampToLogoutClient_.erase(stamp);
                isLogout = true;
            }
        }
        if (isLogout)
        {
            logger_ptr->trace("In ProxyServer::OnServiceResponse(): logout response, setting client's uid to -1");
            client->SetUid(-1); // set it to -1, to avoid sending uneccessary logout again
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.erase(uid);
        }
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
            logger_ptr->trace("In ProxyServer::OnServiceResponse: This response is for an unlogin client.");
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
            logger_ptr->trace("In ProxyServer::OnServiceResponse: This response is for an signup client.");
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
            logger_ptr->trace("In ProxyServer::OnServiceResponse: setting client uid to {}.", uid);
            client->SetUid(uid);
            // add client to login set if it is login client
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            uidToClient_.emplace(uid, client);
        }
        client->SendResponse(response);
    }
    else
    {
        logger_ptr->trace("In ProxyServer::OnServiceResponse: receive response for an offline client (uid: {}).", uid);
    }
}

void ProxyServer::OnDisConnection(FileDesc fd)
{
    std::shared_ptr<Client> client = fdToClient_[fd];
    logger_ptr->trace("In ProxyServer::OnDisConnection: found the exiting client by fd: {}, deleting it...", fd);
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
    logger_ptr->info("In ProxyServer::OnDisConnection: Client (sockfd: {}) out.", fd);
}

void ProxyServer::OnError(FileDesc fd)
{
    // if it was not client, it must be the tcp server
    if (fd == server_->listenFd())
    {
        logger_ptr->error("In main thread: Fatal error in tcp server, exiting now...");
        abort();
    }
    logger_ptr->warn("In main thread: Fatal error in client (sockfd: {})", fd);
    OnDisConnection(fd);
}

void ProxyServer::OnHealthReport()
{
    // the only thread will modify the following two maps is the main thread,
    // so we don't need to hold lock
    int clientTotal = fdToClient_.size();
    int clientLogined = uidToClient_.size();
    int clientLogining, clientSignuping, clientLogouting;
    // these two should hold locks
    {
    std::lock_guard<std::mutex> guard(stampToUnloginClientLock_);
    clientLogining = stampToUnloginClient_.size();
    }
    {
    std::lock_guard<std::mutex> guard(stampToSignupClientLock_);
    clientSignuping = stampToSignupClient_.size();
    }
    {
    std::lock_guard<std::mutex> guard(stampToLogoutClientLock_);
    clientLogouting = stampToLogoutClient_.size();
    }
    logger_ptr->info("ProxyServer Health Report: \
                      unlogined clients : {0}, \
                      logined clients: {1}, \
                      logining clients: {2}, \
                      signuping clients: {3} \
                      logouting clients: {4}", 
                      clientTotal - clientLogined,
                      clientLogined, 
                      clientLogining,
                      clientSignuping,
                      clientLogouting);
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
        logger_ptr->trace("In ProxyServer::SendRequest: Send request (type: {0}) to client (uid: {1})", requestTypeToStr[request.requesttype()], uid);
        return client->SendRequest(request);
    }
    logger_ptr->warn("In ProxyServer::SendRequest: Send request (type: {0}) to offline client (uid: {1})", requestTypeToStr[request.requesttype()], uid);
    return -1;
}
