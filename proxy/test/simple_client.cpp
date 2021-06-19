#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

#include "../protobuf/client_proxy.pb.h"
#include "../tcp/poller.h"
#include "../tcp/tcplistener.h"

#define BUFFER_SIZE 2048

std::unordered_map<std::string, client::Command_CommandType> 
cmdTypeMap = {
    {"NULL", client::Command::NULL_},
    {"LOGIN", client::Command::LOGIN},
    {"LOGOUT", client::Command::LOGOUT},
    {"ROOM_LIST", client::Command::ROOM_LIST},
    {"JOIN_ROOM", client::Command::JOIN_ROOM},
    {"CREATE_ROOM", client::Command::CREATE_ROOM},
    {"QUICK_MATCH", client::Command::QUICK_MATCH},
    // forward to room
    {"LEAVE_ROOM", client::Command::LEAVE_ROOM},
    {"READY", client::Command::READY},
    {"BET", client::Command::BET},
    {"HIT", client::Command::HIT},
    {"STAND", client::Command::STAND},
    {"DOUBLE", client::Command::DOUBLE},
    {"SPLIT", client::Command::SPLIT},
    {"SURRENDER", client::Command::SURRENDER},
    // forward to social
    {"SIGNIN", client::Command::SIGNIN},
    {"INFO", client::Command::INFO},
    {"RANK_ME", client::Command::RANK_ME},
    {"RANK_TOP", client::Command::RANK_TOP},
    {"ADD_FRIEND", client::Command::ADD_FRIEND},
    {"ACCEPT_FRIEND", client::Command::ACCEPT_FRIEND},
    {"DELETE_FRIEND", client::Command::DELETE_FRIEND},
    {"LIST_FRIEND", client::Command::LIST_FRIEND}
};

void print(std::ostream &os, client::Response &response)
{
    int status = (int)response.status();
    if (response.status() == client::Response_Status_SUCCEED)
    {
        os << "Response status: succeed" << std::endl;
    }
    else
    {
        os << "Response status: fail" << std::endl;
    }
    os << "description: " << response.description() << std::endl;
}

std::queue<client::Command> g_commands;

class ConnectionHandler : public tcp::EventsHandler {
public:
    ConnectionHandler(const char *ip, unsigned short port) : EventsHandler()
    {
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &m_addr.sin_addr) == 0)
        {
            throw "ConnectionHandler: Invalid IP addr.\n";
        }

        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            throw "ConnectionHandler: fail to create socket.\n";
        }
    }


    ~ConnectionHandler() {close(m_sockfd);}

public:
    int connect()
    {
        return ::connect(m_sockfd, (struct sockaddr*)&m_addr, sizeof(m_addr));
    }

    int addToPoller(tcp::SocketPoller *poller)
    {
        if (poller->add(m_sockfd, EPOLLOUT | EPOLLERR | EPOLLET, this) < 0)
        {
            return -1;
        }
        m_poller = poller;
        return 0;
    }

    int handleEvents(int sockfd, int events)
    {
        if (sockfd != m_sockfd)
            return 0;
        int ret = 0;
        if (events & EPOLLIN)
        {
            ret = onRecv() == 0 ? ret : -1;
        }
        if (events & EPOLLOUT)
        {
            ret = onSend() == 0 ? ret : -1;
        }
        if (events & EPOLLERR)
        {
            ret = onError() == 0 ? ret : -1;
        }
        return ret;
    }

private:
    int onRecv()
    {
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
        std::string rawResponse(m_buffer, ret);
        client::Response response;
        
        if (m_status = response.ParseFromString(rawResponse))
        {
            print(std::cout, response);
        }
        else
        {
            std::cout << "fail to parse the response" << std::endl;
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
        if (g_commands.empty())
        {
            return -1;
        }
        client::Command command = g_commands.front();
        g_commands.pop();

        std::string rawCommand = command.SerializeAsString();
        if (rawCommand.empty())
        {
            std::cout << "fail to serialize command." << std::endl;
            return -1;
        }

        // write to proxy
        int ret = write(m_sockfd, rawCommand.c_str(), rawCommand.size());
        if (ret < rawCommand.size())
        {
            std::cout << "fail to write the whole command message back." << std::endl;
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
    tcp::SocketPoller *m_poller = NULL;
    int m_sockfd;
    struct sockaddr_in m_addr;
    char m_buffer[BUFFER_SIZE];
    bool m_status;
    //tcp::CircularBuffer m_buffer;
};

const char *host = "127.0.0.1";
unsigned port = 12345;
const char *samples_path = "./samples.txt";

int main()
{
    std::ifstream fin(samples_path, std::ifstream::in);
    while (fin.good())
    {
        client::Command command;
        std::string line;
        std::string item;
        getline(fin, line);
        std::cout << "get one line: " << line << std::endl;
        std::istringstream iss(line);
        iss >> item;
        std::cout << "cmdType: " << item << std::endl;
        if (cmdTypeMap.find(item) == cmdTypeMap.end())
        {
            continue;
        }
        command.set_cmdtype(cmdTypeMap[item]);
        int idx = 0;
        while (iss.good())
        {
            iss >> item;
            std::cout << "arg: " << item << std::endl;
            command.add_arg(item);
        }

        g_commands.push(command);
    }
    fin.close();
    std::cout << g_commands.size() << " commands in total" << std::endl;

    tcp::SocketPoller poller;
    ConnectionHandler *conn = new ConnectionHandler(host, port);

    if (conn->connect() < 0)
    {
        std::cout << "fail to connect to host" << std::endl;
        delete conn;
        return 0;
    }
    std::cout << "successfully connect to host" << std::endl;

    conn->addToPoller(&poller);
    while (true)
    {
        if (poller.poll(1) < 0)
        {
            std::cout << "fail to poll" << std::endl;
            break;
        }
    }
    
    delete conn;
    return 0;
}
