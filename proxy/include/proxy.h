#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <utility>
#include <unordered_map>
#include <unordered_set>

#include "grpc/asyncProxyServer.h"
#include "grpc/asyncServiceClient.h"

#include "net/EventLoop.h"
#include "net/circ_buf.h"
#include "protocols/ClientProxyProtocol.h"   
#include "common.h"

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

// void print(std::ostream &os, client::Command &command)
// {
//     int cmdType = (int)command.cmdtype();
//     os << "command type: " << cmdTypeMap[cmdType] << std::endl;
//     os << "arguments: ";
//     if (command.arg_size())
//     {
//         for (int i = 0; i < command.arg_size(); ++i)
//         {
//             const std::string &arg = command.arg(i);
//             os << arg << " ";
//         }
//         return;
//     }
//     os << "null" << std::endl;
// }

class ClientHandler : public Net::EventsHandler {
public:
    ClientHandler(int sockfd, const struct sockaddr_in addr, 
                int bufferSize = BUFFER_SIZE, int queueSize = QUEUE_SIZE) 
        : Net::EventsHandler(), m_clientfd(sockfd), m_addr(addr), 
        m_readBuffer(bufferSize), m_writeBuffer(bufferSize),
        m_requestWaittingQueue(queueSize), m_responseWaittingQueue(queueSize) 
    {}

    ~ClientHandler() 
    {
        close(m_clientfd);
    }

public:
    // this method should be called by the request/response producer thread
    int notifyInAdvance(int sockfd, Net::Event events, void *data)
    {
        // request from back-end module
        if (m_requestSources.find(sockfd) != m_requestSources.end())
        {
            AsyncCall *call = static_cast<AsyncCall*>(data);
            return m_requestWaittingQueue.push(std::make_pair(sockfd, call));
        }
        // response from back-end module
        else if (m_responseSources.find(sockfd) != m_responseSources.end())
        {
            common::Response *response = static_cast<common::Response*>(data);
            return m_responseWaittingQueue.push(response);
        }
        return -1;
    }

    // this method is called by 
    int addToEventLoop(int sockfd, Net::Event events, Net::EventLoop *eventLoop)
    {
        if (m_eventLoop || sockfd != m_clientfd)
        {
            return -1;
        }
        m_eventLoop = eventLoop;
        return eventLoop->add(m_clientfd, Net::toEpollEvent(events), this);
    }

    // 
    int addRequestSource(int pipefd)
    {
        m_requestSources.insert(pipefd);
        return 0;
    }

    int addResponseSource(int pipefd)
    {
        m_responseSources.insert(pipefd);
        return 0;
    }

    int removeFromEventLoop(int sockfd, Net::EventLoop *loop)
    {
        if (sockfd != m_clientfd || loop != m_eventLoop)
        {
            return -1;
        }
        return loop->del(sockfd);
    }

    int handleEvents(int sockfd, Net::Event events)
    {
        if (sockfd == m_clientfd)
        {
            return handleClient(events);
        }
        else if (m_requestSources.find(sockfd) != m_requestSources.end())
        {
            return handleRequest(sockfd);
        }
        else if (m_responseSources.find(sockfd) != m_responseSources.end())
        {
            return handleResponse(sockfd);
        }
        else 
        {
            return -1;
        }
    }

private:
    int handleClient(Net::Event events)
    {
        int ret = 0;
        if (events & Net::EV_IN)
        {
            ret = onRecv() == 0 ? ret : -1;
        }
        if (events & Net::EV_OUT)
        {
            ret = onSend() == 0 ? ret : -1;
        }
        if (events & Net::EV_ERR)
        {
            ret = onError() == 0 ? ret : -1;
        }
        return ret;
    }

    // simply register EV_OUT event for clientfd
    int handleRequest(int pipefd)
    {
        // resgister EV_OUT for clientfd
        if (!m_eventLoop)
        {
            return -1;
        }
        if (m_events & Net::EV_OUT)
        {
            return 0;
        }
        return m_eventLoop->mod(m_clientfd, m_events | Net::EV_OUT | Net::EV_ET, this);
    }
    
    // simply register EV_OUT event for clientfd
    int handleResponse(int pipefd)
    {
        return handleRequest(pipefd);    
    }

    int onRecv()
    {
        int ret, byteRead, pkgProcessed = 0;
        bool needToRead = true;
        while (needToRead)
        {
            byteRead = read(m_clientfd, m_readBuffer); 
            if (byteRead < 0)
            {
                return -1;
            }
            if (m_readBuffer.size() != m_readBuffer.capacity())
		    {
			    needToRead = false;
		    }
            // handle packages in buffer one by one
            while (true)
            {
                int32_t msgType = Net::readAs(m_readBuffer, 0, msgType, true);
                int32_t msgLength = Net::readAs(m_readBuffer, sizeof(msgType), msgLength, true);
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
                    common::Request request;
                    request.ParseFromString(rawMsg);
                    forwardRequest(request);
                }
                else if (msgType == NS::RESPONSE)
                {
                    common::Response response;
                    response.ParseFromString(rawMsg);
                    forwardResponse(response);
                }
                pkgProcessed++;

                // check whether there is package left
                if (m_readBuffer.size() <= 8)
                {
                    break;
                }
            }
        }
        
        return pkgProcessed;
    }

    int onSend()
    {
        // request first state
        if (m_writeRequest)
        {
            int ret = streamRequestToBuffer();
            if (ret < 0)
                return -1;
            if (ret > 0)
                m_writeRequest = false;
            // if there are spaces left in buffer, try responseQueue
            if (m_writeBuffer.capacity() - m_writeBuffer.size() > 9)
            {
                ret = streamResponseToBuffer();
            }
        }
        else
        {
            int ret = streamResponseToBuffer();
            if (ret < 0)
                return -1;
            if (ret > 0)
                m_writeRequest = true;
            // if there are spaces left in buffer, try responseQueue
            if (m_writeBuffer.capacity() - m_writeBuffer.size() > 9)
            {
                ret = streamRequestToBuffer();
            }
        }
        return write(m_clientfd, m_writeBuffer);
    }

    int onError()
    {
        return -1;
    }

    int forwardRequest(const common::Request &request)
    {
        if (cmdTypeToModule.find(request.requesttype()) == cmdTypeToModule.end())
        {
            return -1;
        }
        BackEndModule modu = cmdTypeToModule.find(request.requesttype())->second;
        if (modu == BackEndModule::Lobby)
        {
            m_asyncLobbyClient->Notify(request);
        }
        else if (modu == BackEndModule::Room)
        {
            m_asyncRoomClient->Notify(request);
        }
        else if (modu == BackEndModule::Social)
        {
            m_asyncSocialClient->Notify(request);
        }
        else 
        {
            return -1;
        }
        return 0;
    }

    int forwardResponse(common::Response &response)
    {
        int64_t key = response.stamp();
        // find the corresponding request
        if (m_requestsMap.find(key) == m_requestsMap.end())
        {
            // no matched request, drop it without raising an error 
            return 0;
        }
        auto iter = m_requestsMap.find(key);
        // restore to original stamp
        response.set_stamp(iter->second.second);
        iter->second.first->setReply(response);
        iter->second.first->Proceed();
    }

    int streamRequestToBuffer()
    {
        int ret = 0;
        // write all the request from queue until no request
        while (true)
        {
            std::pair<int, AsyncCall *> item;
            common::Request &request = item.second->getRequest();
            if (0 > m_requestWaittingQueue.front(item))
            {   // empty queue, go check response queue
                break;
            }
            // insert this requestHandler to requestsMap
            m_requestsMap.emplace(item.first * request.stamp(), item.second, request.stamp());
            // don't pop() now, because we don't known whether 
            // we can write this request completely
            // check whether this is a big msg
            size_t msgLen = request.ByteSizeLong();
            if (msgLen + 8 > m_writeBuffer.capacity())
            {   // drop this bad msg...
                common::Response res;
                res.set_uid(request.uid());
                res.set_status(-1);
                res.set_stamp(request.stamp());
                res.add_args("Message too large, should not be larger than 8 KB.");
                item.second->setReply(res);
                item.second->Proceed();
                m_requestWaittingQueue.pop();
                m_requestsMap.erase(item.first * request.stamp());
                continue;
            }
            // check whether the current write buffer can hold this msg
            if (m_writeBuffer.capacity() - m_writeBuffer.size() < msgLen + 8)
            {
                break;
            }
            ret++;
            request.set_stamp(item.first * request.stamp());
            std::string msg = request.SerializeAsString();
            NS::pack(NS::REQUEST, msg, m_writeBuffer);
            m_requestWaittingQueue.pop();
        }
        return ret;
    }

    int streamResponseToBuffer()
    {
        int ret = 0;
        // write all the response from queue until no request
        while (true)
        {
            common::Response *response;
            if (0 > m_responseWaittingQueue.front(response))
            {   // empty queue, go check response queue
                break;
            }
            // don't pop() now, because we don't known whether 
            // we can write this request completely
            // check whether this is a big msg
            size_t msgLen = response->ByteSizeLong();
            if (msgLen + 8 > m_writeBuffer.capacity())
            {   
                return ret;
            }
            // check whether the current write buffer can hold this msg
            if (m_writeBuffer.capacity() - m_writeBuffer.size() < msgLen + 8)
            {
                break;
            }
            ret++;
            std::string msg = response->SerializeAsString();
            NS::pack(NS::RESPONSE, msg, m_writeBuffer);
            m_responseWaittingQueue.pop();
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
    
    std::unordered_set<int> m_requestSources;
    std::unordered_set<int> m_responseSources;
    std::unordered_map<int64_t, std::pair<AsyncCall*, int64_t>> m_requestsMap;
    Net::CircularQueue<std::pair<int, AsyncCall*>> m_requestWaittingQueue;
    Net::CircularQueue<common::Response*> m_responseWaittingQueue;

    Client<lobby::Lobby> *m_asyncLobbyClient = NULL;
    Client<room::Room> *m_asyncRoomClient = NULL;
    Client<social::Social> *m_asyncSocialClient = NULL;

    bool m_writeRequest = false;
};

