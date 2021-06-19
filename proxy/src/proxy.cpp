#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include <limits.h>
#include <memory>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unordered_map>
#include <unordered_set>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include "../include/grpc/asyncProxyServer.h"
#include "../include/grpc/asyncServiceClient.h"

#include "../include/net/EventLoop.h"
#include "../include/net/TcpListener.h"
#include "../include/net/circ_buf.h"

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerBuilder;
using grpc::ServerCompletionQueue;
using grpc::ServerContext;
using grpc::Status;

using proxy::Proxy;
using common::Response;
using common::Request;

#define MAX_FORWORD_FAILURE 5

#define BUFFER_SIZE 4096    // 4 KB

std::string cmdTypeMap[] = {
    "NULL",
    "LOGIN",
    "LOGOUT",
    "ROOM_LIST",
    "JOIN_ROOM",
    "CREATE_ROOM",
    "QUICK_MATCH",
    // forward to room
    "LEAVE_ROOM",
    "READY",
    "BET",
    "HIT",
    "STAND",
    "DOUBLE",
    "SPLIT",
    "SURRENDER",
    // forward to social
    "SIGNIN",
    "INFO",
    "RANK_ME",
    "RANK_TOP",
    "ADD_FRIEND",
    "ACCEPT_FRIEND",
    "DELETE_FRIEND",
    "LIST_FRIEND"
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
    // 
    enum ClientState {CLOSE = 0, LOGOUT, LOGIN, INGAME};
public:
    ClientHandler(int sockfd, const struct sockaddr_in addr) : 
        Net::EventsHandler(), m_clientfd(sockfd), m_addr(addr) {}

    ~ClientHandler() {close(m_clientfd);}

public:
    int notifyInAdvance(int sockfd, int events, void *data)
    {
        if (m_requestSources.find(sockfd) != m_requestSources.end())
        {
            m_requests.push_back(data);
        }
        else if (m_responseSources.find(sockfd) != m_responseSources.end())
        {
            m_reponses.push_back(data);
        }
    }

    // this method is called by 
    int addToEventLoop(int sockfd, int events, Net::EventLoop *eventLoop)
    {
        if (sockfd != m_clientfd || m_eventLoop)
        {
            return -1;
        }
        m_eventLoop = eventLoop;
        return eventLoop->add(m_clientfd, events, this);
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
            return handleRequest();
        }
        else if (m_responseSources.find(sockfd) != m_responseSources.end())
        {
            return handleResponse();
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

    int handleResponse()
    {
        // TODO: need to add synchronous primitives to protect the list
        Response *response = static_cast<Response *>(m_responses.front());
        m_responses.pop_front();
        // 
        
    }

    int handleRequest();

    int onRecv()
    {
        // if this is from client
            // read data into buffer
            // parse all commands in the buffer until there is no command or no complete command
            // dispatch the command to the corresponding rpc
        // if this is from RPC callee
            // collect the return parameter into the toBeSend list
        // if this is from RPC server
            // collect the input parameter into the toBeSend list

        // read the binary message
        int ret = read(m_sockfd, m_buffer, BUFFER_SIZE);        
        if (ret <= 0)
        {
            if (ret < 0 && (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR))
            {
                return 0;
            }
            return ret;
        }
        // parse the message
        std::string rawCommand(m_buffer, ret);
        client::Command command;
        if (m_status = command.ParseFromString(rawCommand))
        {
            print(std::cout, command);
        }
        // prepare for the response
        if (m_poller->mod(m_sockfd, EPOLLOUT | EPOLLERR | EPOLLET, this) < 0)
        {
            return -1;
        }
        return 0;
    }

    int onSend()
    {
        // if this is from client socket
            // check the toBeSend list, serialize the items and put in the socket send buffer, until no room
        client::Response response;
        if (m_status)
        {
            response.set_status(client::Response_Status_SUCCEED);
            response.set_description("succeessfully parsed your command.");
        }
        else
        {
            response.set_status(client::Response_Status_FAIL);
            response.set_description("fail to parse your command.");
        }

        // write back to client
        std::string rawResponse = response.SerializeAsString();
        if (rawResponse.empty())
        {
            std::cout << "fail to serialize the response." << std::endl;
            return -1;
        }
        int ret = write(m_sockfd, rawResponse.c_str(), rawResponse.size());
        //
        if (ret < rawResponse.size())
        {
            std::cout << "fail to write the whole response message back." << std::endl;
            return -1;
        }

        m_poller->mod(m_sockfd, EPOLLIN | EPOLLERR | EPOLLET, this);
        return 0;
        
        
    }

    int onError()
    {
        return -1;
    }

private:
    Net::EventLoop *m_eventLoop = NULL;
    int m_clientfd = -1;   
    int m_userId = -1;
    struct sockaddr_in m_addr;
    char m_buffer[BUFFER_SIZE];
    bool m_status;
    std::unordered_set<int> m_requestSources;
    std::unordered_set<int> m_responseSources;
    std::list<void*> m_requests;
    std::list<void*> m_responses;
};

const char *ip = "127.0.0.1";
unsigned short port = 12345;
bool flag = false;

void stop_server(int)
{
    flag = true;
}

int main()
{
    ::SocketPoller poller;
    tcp::TcpListener<CommandHandler> *listener = new tcp::TcpListener<CommandHandler>(ip, port);

    signal(SIGINT, stop_server);

    if (listener->addToPoller(&poller) < 0)
    {
        flag = true;
    }

    while (!flag)
    {
        if (poller.poll(1) < 0)
        {
            flag = true;
        }
    }

    delete listener;
    return 0;
}
