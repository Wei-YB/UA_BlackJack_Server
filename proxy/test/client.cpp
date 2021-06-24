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

#include "../include/net/EventLoop.h"
#include "../include/net/circ_buf.h"
#include "../include/grpc/common.pb.h"
#include "../include/protocols/ClientProxyProtocol.h"

using Net::EventsHandler;
using Net::EventLoop;
using Net::Event;
using Net::CircularBuffer;
using common::Request;
using common::Response;

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
    os << "send request." << std::endl; 
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

class TcpClient : public EventsHandler
{
public:
    TcpClient(size_t bufferSize) : EventsHandler(), m_readBuffer(bufferSize), m_writeBuffer(bufferSize)
    {
        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw "Socket: fail to create socket.\n";
        }
    }

    ~TcpClient() {close(m_sockfd);}

public:
    int connect(const char *ipAddr, unsigned short port)
    {
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ipAddr, &m_addr.sin_addr) == 0)
        {
            return -1;
        }
        if (::connect(m_sockfd, (struct sockaddr *)&m_addr, sizeof(m_addr)) == 0)
        {
            fcntl(m_sockfd, F_SETFL, fcntl(m_sockfd, F_GETFL) | O_NONBLOCK);
            return 0;
        }
        return -1;
    }

    int addToEventLoop(EventLoop *loop)
    {
        m_eventLoop = loop;
        m_events = Net::EV_IN | Net::EV_ET | Net::EV_ERR;
        return m_eventLoop->add(m_sockfd, m_events, this);
    }

    int handleEvents(int sockfd, Event events)
    {
        int ret;
        if (events & Net::EV_OUT)
        {
            if ((ret = onSend()) < 0)
            {
                return -1;
            }
        }
        if (events & Net::EV_IN)
        {
            if ((ret = onRecv()) < 0)
            {
                return -1;
            }
        }
        if (events & Net::EV_ERR)
        {
            return -1;
        }
        return ret;
    }

    int sendRequest(const Request &request) 
    {
        std::string rawRequest = request.SerializeAsString();
        if (0 > NS::pack(NS::REQUEST, rawRequest, m_writeBuffer))
        {
            return -1;
        }
        m_events |= Net::EV_OUT | Net::EV_ET;
        m_eventLoop->mod(m_sockfd, m_events, this);
        return 0;
    }

    int sendResponse(const Response &response) 
    {
        std::string rawResponse = response.SerializeAsString();
        if (0 > NS::pack(NS::RESPONSE, rawResponse, m_writeBuffer))
        {
            return -1;
        }
        m_events |= Net::EV_OUT | Net::EV_ET;
        m_eventLoop->mod(m_sockfd, m_events, this);
        return 0;
    }

    int64_t uid() const {return m_uid;}
private:
    int onRecv()
    {
        int ret, byteRead, pkgProcessed = 0;
        bool needToRead = true;
        while (needToRead)
        {
            byteRead = read(m_sockfd, m_readBuffer); 
            std::cout << "read " << ret << " bytes." << std::endl;
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
                if (msgType == NS::RESPONSE)
                {
                    Response response;
                    response.ParseFromString(rawMsg);
                    if (m_uid == -1)
                    {
                        m_uid = response.uid();
                    }
                    print(std::cout, response);
                }
                pkgProcessed++;
            }
        }
        
        return pkgProcessed;
    }

    int onSend()
    {
        int ret = Net::write(m_sockfd, m_writeBuffer, true);
        if (ret < 0)
        {
            return -1;
        }
        if (m_writeBuffer.empty())
        {
            m_events &= ~Net::EV_OUT;
            m_eventLoop->mod(m_sockfd, m_events, this);
        }
        return ret;
    }


private:
    int m_sockfd = -1;
    int64_t m_uid = -1;
    struct sockaddr_in m_addr;
    EventLoop *m_eventLoop = NULL;
    Event m_events;
    CircularBuffer m_readBuffer;
    CircularBuffer m_writeBuffer;
};

const char *samples_path = "/root/ricki/UA_BlackJack_Server/proxy/test/";

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "usage: " << std::string(argv[0]) << " ip port." << std::endl;
        exit(0);
    }
    // prepare all the request
    std::queue<Request> requests;
    std::ifstream fin((std::string(samples_path) + std::string("samples.txt").c_str()), std::ifstream::in);
    while (fin.good())
    {
        Request request;
        std::string line;
        std::string item;
        getline(fin, line);
        std::istringstream iss(line);
        iss >> item;
        if (strToType.find(item) == strToType.end())
        {
            continue;
        }
        request.set_uid(12);
        request.set_requesttype(strToType[item]);
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
    TcpClient *client = new TcpClient(4096);
    if (0 > client->connect(argv[1], atoi(argv[2])))
    {
        std::cout << "fail to connect to proxy." << std::endl;
        delete client;
        exit(0);
    }
    std::cout << "successfully connect to host" << std::endl;

    client->addToEventLoop(&loop);
    // write request to proxy one by one
    while (!requests.empty())
    {
        Request request = requests.front();
        requests.pop();
        // if (client->uid() != -1 || (request.requesttype() == Request::LOGIN))
        // {
        //     if (client->sendRequest(request) != -1)
        //     {
        //         print(std::cout, request);
        //     }
        // }
        if (client->sendRequest(request) != -1)
        {
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