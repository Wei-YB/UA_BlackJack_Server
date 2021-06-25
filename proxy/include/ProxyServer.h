#ifndef _PROXYSERVER_H_
#define _PROXYSERVER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <mutex>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "asyncProxyServer.h"
#include "asyncServiceClient.h"

#include "EventLoop.h"
#include "TcpConnection.h"
#include "CircularBuffer.h"
#include "Client.h"
#include "ClientProxyProtocol.h"   
#include "common.h"
#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"

using common::Request;
using common::Response;
using Net::CircularQueue;

#define MAX_FORWORD_FAILURE 5

#define BUFFER_SIZE 1024 * 4     // 4 KB
#define QUEUE_SIZE  128

const std::string cmdTypeMap[] = {
        "INVAL",  
        // client to proxy, proxy to lobby 
        "LOGIN",
        "LOGOUT", 
        "ROOM_LIST",
        "JOIN_ROOM",
        "CREATE_ROOM",
        "QUICK_MATCH",
        "READY",
        // client to proxy, proxy to room
        "LEAVE_ROOM",
        "BET",
        "HIT",
        "STAND",
        "DOUBLE",
        "SURRENDER",
        // client to proxy, proxy to social
        "SIGNUP",
        "INFO",  
        "RANK_ME",
        "RANK_TOP",  
        "ADD_FRIEND",
        "ACCEPT_FRIEND",
        "DELETE_FRIEND",
        "LIST_FRIEND",
        "LIST_MATCH",
        "LIST_WAITTING",
        // room, lobby, and social to proxy, proxy to client
        "NOTIFY_USER"
        // 
};





// void print(std::ostream &os, const Response &response)
// {
//     os << "get response." << std::endl; 
//     os << "status: " << response.status() << std::endl;
//     os << "stamp: " << response.stamp() << std::endl;
//     os << "uid: " << response.uid() << std::endl; 
//     if (response.args_size())
//     {
//         os << "args: ";
//         for (int i = 0; i < response.args_size(); ++i)
//         {
//             const std::string &arg = response.args(i);
//             os << arg << " ";
//         }
//         os << std::endl;
//         return;
//     }
//     os << "null" << std::endl;
// }

// void print(std::ostream &os, const Request &request)
// {
//     os << "get request." << std::endl; 
//     os << "type: " << typeToStr[request.requesttype()] << std::endl;
//     os << "stamp: " << request.stamp() << std::endl;
//     os << "uid: " << request.uid() << std::endl; 
//     if (request.args_size())
//     {
//         os << "args: ";
//         for (int i = 0; i < request.args_size(); ++i)
//         {
//             const std::string &arg = request.args(i);
//             os << arg << " ";
//         }
//         os << std::endl;
//         return;
//     }
//     os << "null" << std::endl;
// }

// void print(std::ostream &os, Net::Event events)
// {
//     os << "events: ";
//     if (events & Net::EV_IN) os << "EV_IN ";
//     if (events & Net::EV_OUT) os << "EV_OUT ";
//     if (events & Net::EV_ET) os << "EV_ET ";
//     if (events & Net::EV_ERR) os << "EV_ERR ";
//     os << std::endl;
// }

class ProxyServerServer {
public:
    // callbacks
    void OnNewClient(std::shared_ptr<TcpConnection> conn)
    {
        auto requestCallBack = std::bind(&ProxyServer::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2);
        auto responseCallBack = std::bind(&ProxyServer::OnClientResponse, this, std::placeholders::_1);
        auto errorCallBack = std::bind(&ProxyServer::OnError, this, std::placeholders::_1);
        std::shared_ptr<Client> newClient = std::make_shared<Client>(conn, 
                                                                    requestCallBack,
                                                                    responseCallBack,
                                                                    errorCallBack);
        fdToClient_.emplace(conn->SockFd(), newClient);
    }

    void OnClientRequest(FileDesc fd, const Request &request)
    {   // remenber a unlogin user
        if (request.uid() == -1)
        {
            std::shared_ptr<Client> client = fdToClient_[fd];
            int64_t stamp = (int64_t)client.get();
            {
            std::lock_guard<std::mutex> guard(stampToClientLock_);
            stampToClient_.emplace(stamp, client);
            }
            client->SetUnloginStamp(request.stamp());
        }
        std::weak_ptr<ServiceClient> weakServiceClient = requestTypeToServiceClient_[request.requesttype()];
        std::shared_ptr<ServiceClient> serviceClient = weakServiceClient.lock();
        if (serviceClient)
        {
            serviceClient->Call(request);
        }
    }

    void OnServiceResponse(const Response& response)
    {
        UserId uid = response.uid();
        int64_t stamp = response.stamp();
        // if the response is for a logined client
        {
            std::lock_guard<std::mutex> guard(uidToClientLock_);
            if (uidToClient_.find(uid) != uidToClient_.end())
            {
                std::shared_ptr<Client> client = uidToClient_[uid];
                client->SendResponse(response);
                return;
            }
        }
        // if the response is for non-login client
        std::weak_ptr<Client> weakClient;
        {
            std::lock_guard<std::mutex> guard(stampToClientLock_);
            if (stampToClient_.find(stamp) != stampToClient_.end())
            {
                weakClient = stampToClient_[stamp];
                stampToClient_.erase(stamp);
            }
        }
        std::shared_ptr<Client> client = weakClient.lock();
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

    void OnClientResponse(const Response &response)
    {
        // TODO: check the validity of clientResponseCallBack_
        clientResponseCallBack_(response);
    }

    void OnDisConnection(FileDesc fd)
    {
        std::shared_ptr<Client> client = fdToClient_[fd];
        uidToClient_.erase(fd);
        if (client->uid() != -1)
        {
            uidToClient_.erase(client->uid());
        }
    }

    void OnError(FileDesc fd)
    {
        OnDisConnection(fd);
    }
    //
    int SendRequest(Request &request)
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
            client->SendRequest(request);
        }
    }

    void SetClientResponseCallBack(const std::function<void(Response &)> &clientResponseCallBack)
    {clientResponseCallBack_ = clientResponseCallBack;}
private:
    std::mutex uidToClientLock_;
    std::unordered_map<UserId, std::shared_ptr<Client>> uidToClient_;   
    std::unordered_map<FileDesc, std::shared_ptr<Client>> fdToClient_;  // only accessible from the main thread, no lock protected
    std::mutex stampToClientLock_;
    std::unordered_map<int64_t, std::weak_ptr<Client>> stampToClient_;
    std::unordered_map<Request::RequestType, std::weak_ptr<ServiceClient>> requestTypeToServiceClient_;
    std::function<void(Response &)> clientResponseCallBack_;
};

#endif