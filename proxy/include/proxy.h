#ifndef _PROXY_H_
#define _PROXY_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <mutex>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "asyncProxyServer.h"
#include "asyncServiceClient.h"

#include "EventLoop.h"
#include "circ_buf.h"
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

enum BackEndModule {Proxy = 0, Lobby, Room, Social, DataBase};

const std::unordered_map<common::Request_RequestType, BackEndModule> cmdTypeToModule = {
    // Lobby request
    {common::Request_RequestType_LOGIN, BackEndModule::Lobby}, 
    {common::Request_RequestType_LOGOUT, BackEndModule::Lobby},
    {common::Request_RequestType_ROOM_LIST, BackEndModule::Lobby},
    {common::Request_RequestType_CREATE_ROOM, BackEndModule::Lobby},
    {common::Request_RequestType_QUICK_MATCH, BackEndModule::Lobby},
    {common::Request_RequestType_READY, BackEndModule::Lobby},
    // Room request
    {common::Request_RequestType_LEAVE_ROOM, BackEndModule::Room},
    {common::Request_RequestType_BET, BackEndModule::Room},
    {common::Request_RequestType_HIT, BackEndModule::Room},
    {common::Request_RequestType_STAND, BackEndModule::Room},
    {common::Request_RequestType_DOUBLE, BackEndModule::Room},
    {common::Request_RequestType_SURRENDER, BackEndModule::Room},
    // Social request
    {common::Request_RequestType_SIGNUP, BackEndModule::Social},
    {common::Request_RequestType_INFO, BackEndModule::Social},
    {common::Request_RequestType_RANK_ME, BackEndModule::Social},
    {common::Request_RequestType_RANK_TOP, BackEndModule::Social},
    {common::Request_RequestType_ADD_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_ACCEPT_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_DELETE_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_LIST_FRIEND, BackEndModule::Social},
    {common::Request_RequestType_LIST_MATCH, BackEndModule::Social},
    {common::Request_RequestType_LIST_WAITTING, BackEndModule::Social},
    // Proxy request
    {common::Request_RequestType_NOTIFY_USER, BackEndModule::Proxy},
};

const std::string typeToStr[] = {
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

std::unordered_map<std::string, Request::RequestType> 
strToType = {
    {"NULL", Request::INVAL},
    {"LOGIN", Request::LOGIN},
    {"LOGOUT", Request::LOGOUT},
    {"ROOM_LIST", Request::ROOM_LIST},
    {"JOIN_ROOM", Request::JOIN_ROOM},
    {"CREATE_ROOM", Request::CREATE_ROOM},
    {"QUICK_MATCH", Request::QUICK_MATCH},
    // forward to room
    {"LEAVE_ROOM", Request::LEAVE_ROOM},
    {"READY", Request::READY},
    {"BET", Request::BET},
    {"HIT", Request::HIT},
    {"STAND", Request::STAND},
    {"DOUBLE", Request::DOUBLE},
    {"SURRENDER", Request::SURRENDER},
    // forward to social
    {"SIGNIN", Request::SIGNUP},
    {"INFO", Request::INFO},
    {"RANK_ME", Request::RANK_ME},
    {"RANK_TOP", Request::RANK_TOP},
    {"ADD_FRIEND", Request::ADD_FRIEND},
    {"ACCEPT_FRIEND", Request::ACCEPT_FRIEND},
    {"DELETE_FRIEND", Request::DELETE_FRIEND},
    {"LIST_FRIEND", Request::LIST_FRIEND}
};

void print(std::ostream &os, const Response &response)
{
    os << "get response." << std::endl; 
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
    os << "get request." << std::endl; 
    os << "type: " << typeToStr[request.requesttype()] << std::endl;
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

class ClientHandler : public Net::EventsHandler {
public:
    ClientHandler(int sockfd, const struct sockaddr_in addr, 
                std::unordered_map<int64_t, Net::EventsHandler*> *uidToClient,
                std::mutex *lock,
                RpcClient<lobby::Lobby> *asyncLobbyClient,
                RpcClient<room::Room> *asyncRoomClient,
                RpcClient<social::Social> *asyncSocialClient,
                int bufferSize = BUFFER_SIZE, int queueSize = QUEUE_SIZE) 
        : Net::EventsHandler(), m_clientfd(sockfd), m_addr(addr), 
        m_uidToClient(uidToClient), m_lock(lock), 
        m_readBuffer(bufferSize), m_writeBuffer(bufferSize),
        m_asyncLobbyClient(asyncLobbyClient), m_asyncRoomClient(asyncRoomClient), 
                m_asyncSocialClient(asyncSocialClient) {}

    ~ClientHandler() 
    {
        close(m_clientfd);
    }

public:
    int pushRpcRequest(void *data)
    {
        AsyncCall *call = static_cast<AsyncCall*>(data);
        std::lock_guard<std::mutex> guard(m_asyncCallQueueLock);
        
        if (!(m_events & Net::EV_OUT))
        {
            m_events |= Net::EV_OUT | Net::EV_ET;
            if (m_eventLoop->mod(m_clientfd, m_events, this) < 0)
            {
                m_events &= ~(Net::EV_OUT | Net::EV_ET);
                return -1;
            }
        }
        m_asyncCallQueue.push(call);
        return 0;
    }

    int pushRpcResponse(void *data)
    {
        Response *response = static_cast<Response*>(data);
        if (!(m_events & Net::EV_OUT))
        {
            m_events |= Net::EV_OUT | Net::EV_ET;
            if (m_eventLoop->mod(m_clientfd, m_events, this) < 0)
            {
                m_events &= ~Net::EV_OUT;
                return -1;
            }
        }
        std::lock_guard<std::mutex> guard(m_responseQueueLock);
        m_responseQueue.push(response);
        return 0;
    }

    int addToEventLoop(Net::EventLoop *eventLoop)
    {
        if (m_eventLoop)
        {
            return -1;
        }
        m_eventLoop = eventLoop;
        m_events = Net::EV_IN | Net::EV_ET | Net::EV_ERR;
        return eventLoop->add(m_clientfd, m_events, this);
    }

    int handleEvents(int sockfd, Net::Event events)
    {
        int ret = 0;
        if (events & Net::EV_IN)
        {
            ret = onRecv() != -1 ? ret : -1;
        }
        if (events & Net::EV_OUT)
        {
            ret = onSend() != -1 ? ret : -1;
        }
        if (events & Net::EV_ERR)
        {
            ret = onError() != -1 ? ret : -1;
        }
        if (ret < 0 && m_userId != -1)
        {   // remove client from uidToClient
            std::lock_guard<std::mutex> guard(*m_lock);
            if (m_uidToClient->find(m_userId) != m_uidToClient->end())
            {
                m_uidToClient->erase(m_userId);
            }
        }
        return ret;
    }

private:
    int onRecv()
    {
        int ret, byteRead, pkgProcessed = 0;
        bool needToRead = true;
        while (needToRead)
        {
            byteRead = read(m_clientfd, m_readBuffer);
            if (byteRead < 0)
            {   // fatal error
                return -1;
            }
            if (m_readBuffer.size() != m_readBuffer.capacity())
		    {   // the tcp buffer has been cleared, don't have to read anymore
			    needToRead = false;
		    }
            // handle packages in buffer one by one
            while (m_readBuffer.size() >= 8)
            {
                int32_t msgType, msgLength; 
                Net::readAs(m_readBuffer, 0, msgType, true);
                Net::readAs(m_readBuffer, sizeof(msgType), msgLength, true);
                if (msgLength == -1 || m_readBuffer.size() - 8 < msgLength)
                {   // not a complete package, wait for the next read event
                    return pkgProcessed;
                }
                m_readBuffer.free(sizeof(msgType) + sizeof(msgLength));

                std::string rawMsg;
                Net::circularBufferToString(m_readBuffer, msgLength, rawMsg);
                m_readBuffer.free(msgLength);
                if (msgType == NS::REQUEST)
                {
                    Request request;
                    request.ParseFromString(rawMsg);
                    forwardRequest(request);
                }
                else if (msgType == NS::RESPONSE)
                {
                    Response response;
                    response.ParseFromString(rawMsg);
                    forwardResponse(response);
                }
                pkgProcessed++;
            }
        }
        
        return pkgProcessed;
    }

    int onSend()
    {
        // request first state
        if (m_writeRequest)
        {
            std::lock_guard<std::mutex> requestLockGuard(m_asyncCallQueueLock);
            if (streamRequestToBuffer() > 0)
                m_writeRequest = false;
            // try responseQueue now
            std::lock_guard<std::mutex> responseLockGuard(m_responseQueueLock);
            streamResponseToBuffer();
            // if no response and request in queues, shut the EV_OUT
            if (m_responseQueue.empty() && m_asyncCallQueue.empty())
            {
                m_events &= ~Net::EV_OUT;
                m_eventLoop->mod(m_clientfd, m_events, this);
            }
        }
        else
        {
            std::lock_guard<std::mutex> responseLockGuard(m_responseQueueLock);
            if (streamResponseToBuffer() > 0)
                m_writeRequest = true;
            // try asyncCallQueue now
            std::lock_guard<std::mutex> requestLockGuard(m_asyncCallQueueLock);
            streamRequestToBuffer();
            // if no response and request in queues, shut the EV_OUT
            if (m_responseQueue.empty() && m_asyncCallQueue.empty())
            {
                m_events &= ~Net::EV_OUT;
                m_eventLoop->mod(m_clientfd, m_events, this);
            }
        }
        
        return write(m_clientfd, m_writeBuffer);
    }

    int onError()
    {
        return -1;
    }

    int forwardRequest(const Request &request)
    {
        if (cmdTypeToModule.find(request.requesttype()) == cmdTypeToModule.end())
        {
            return -1;
        }
        if (m_userId == -1 && request.uid() != 0)
        {
            m_userId = request.uid();
            {
            std::lock_guard<std::mutex> guard(*m_lock);
            m_uidToClient->emplace(m_userId, this);
            }
        }
        BackEndModule modu = cmdTypeToModule.find(request.requesttype())->second;
        if (modu == BackEndModule::Lobby)
        {
            std::cout << "forward to lobby" << std::endl;
            m_asyncLobbyClient->Notify(request);
        }
        else if (modu == BackEndModule::Room)
        {
            std::cout << "forward to room" << std::endl;
            m_asyncRoomClient->Notify(request);
        }
        else if (modu == BackEndModule::Social)
        {
            std::cout << "forward to social" << std::endl;
            m_asyncSocialClient->Notify(request);
        }
        else 
        {
            return -1;
        }
        return 0;
    }

    int forwardResponse(Response &response)
    {
        int64_t key = response.stamp();
        // find the corresponding asyncCall
        if (m_requestsMap.find(key) == m_requestsMap.end())
        {   // no matched request, drop it without raising an error 
            return 0;
        }
        auto iter = m_requestsMap.find(key);
        // if uid == -1, check whether this response is corresponding to LOGIN request
        if (m_userId == -1)
        {
            AsyncCall *call = iter->second.first;
            Request &request = call->getRequest();
            if (cmdTypeMap[request.requesttype()] == "LOGIN" && response.status() == 0)
            {
                m_userId = response.uid();
                {
                std::lock_guard<std::mutex> guard(*m_lock);
                m_uidToClient->emplace(m_userId, this);
                }
            }
        }
        // restore to original stamp
        response.set_stamp(iter->second.second);
        iter->second.first->setReply(response);
        iter->second.first->Proceed();
        m_requestsMap.erase(key);
        return 0;
    }

    int streamRequestToBuffer()
    {
        int ret = 0;
        // write all the request from queue until no request
        while (!m_asyncCallQueue.empty())
        {
            AsyncCall *call = m_asyncCallQueue.front();
            // don't pop() now, because we don't known whether 
            // we can write this request completely
            // check whether this is a big msg
            Request &request = call->getRequest();
            size_t msgLen = request.ByteSizeLong();
            if (msgLen + 8 > m_writeBuffer.capacity())
            {   // drop this bad msg...
                m_asyncCallQueue.pop();
                Response res;
                res.set_uid(request.uid());
                res.set_status(-1);
                res.set_stamp(request.stamp());
                res.add_args("Message too large, should not be larger than 8 KB.");
                call->setReply(res);
                call->Proceed();
                continue;
            }
            // check whether the current write buffer can hold this msg
            if (m_writeBuffer.capacity() - m_writeBuffer.size() < msgLen + 8)
            {
                break;
            }
            // insert this requestHandler to requestsMap
            m_requestsMap.emplace((int64_t)(&request), std::make_pair(call, request.stamp()));
            request.set_stamp((int64_t)(&request));
            std::string msg = request.SerializeAsString();
            NS::pack(NS::REQUEST, msg, m_writeBuffer);
            m_asyncCallQueue.pop();
            ret++;
        }
        return ret;
    }

    int streamResponseToBuffer()
    {
        int ret = 0;
        // write all the response from queue until no request
        while (m_responseQueue.size())
        {
            Response *response = m_responseQueue.front();
            // don't pop() now, because we don't known whether 
            // we can write this response completely
            // check whether this is a big msg
            size_t msgLen = response->ByteSizeLong();
            if (msgLen + 8 > m_writeBuffer.capacity())
            {   
                m_responseQueue.pop();
                continue;
            }
            // check whether the current write buffer can hold this msg
            if (m_writeBuffer.capacity() - m_writeBuffer.size() < msgLen + 8)
            {
                break;
            }
            std::string msg = response->SerializeAsString();
            NS::pack(NS::RESPONSE, msg, m_writeBuffer);
            //delete response;
            m_responseQueue.pop();
            delete response;
            ret++;
        }
        return ret;
    }

private:
    enum ClientState {CLOSE = 0, LOGOUT, LOGIN, INGAME};
private:
    int m_clientfd = -1;   
    int m_userId = -1;
    struct sockaddr_in m_addr;
    Net::Event m_events;
    ClientState m_status;
    Net::EventLoop *m_eventLoop = NULL;
    Net::CircularBuffer m_readBuffer;
    Net::CircularBuffer m_writeBuffer;
    
    std::unordered_map<int64_t, std::pair<AsyncCall*, int64_t>> m_requestsMap;
    std::mutex m_asyncCallQueueLock;
    std::queue<AsyncCall*> m_asyncCallQueue;
    std::mutex m_responseQueueLock;
    std::queue<Response*> m_responseQueue;

    RpcClient<lobby::Lobby> *m_asyncLobbyClient = NULL;
    RpcClient<room::Room> *m_asyncRoomClient = NULL;
    RpcClient<social::Social> *m_asyncSocialClient = NULL;

    std::unordered_map<int64_t, Net::EventsHandler*> *m_uidToClient;
    std::mutex *m_lock;

    bool m_writeRequest = false;
};

#endif