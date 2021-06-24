#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <unordered_map>
#include <functional>

#include "common.pb.h"
#include "circ_buf.h"

using common::Request;
using common::Response;

typedef int FileDesc;
typedef int64_t UserId;

class TcpConnection;

class Proxy {
public:
    // callbacks
    void OnConnection(std::shared_ptr<TcpConnection> conn);
    void OnClientMessage(const std::string &message);
    void OnServiceResponse(const Response& response);
    void OnBackEndRequest(const Request& request);
    void OnDisConnection(std::shared_ptr<TcpConnection> conn);
    void OnError(std::shared_ptr<TcpConnection> conn);
    //
    int SendResponse(Response &response);
    int SendRequest(Request &request);
private:
    std::unordered_map<UserId, std::shared_ptr<TcpConnection>> uidToConnection_;
    std::unordered_map<FileDesc, std::shared_ptr<TcpConnection>> fdToConnection_;
    std::mutex uidMapLock_;
};

class EventsSource
{
public:
    int HandleEvents(int events);
private:
    std::function<void()> inEventCallBack_;
    std::function<void()> outEventCallBack_;
    std::function<void()> errEventCallBack_;
};

class TcpConnection
{
public:
    TcpConnection(FileDesc connfd, struct sockaddr_in addr);
    ~TcpConnection();
    // callbacks
    void OnInput();
    void OnOutput();
    void OnError(int errors);
    //
    int Send(const std::string &pkgsData, int pkgNum);
    // 
    void SetInputCallBack(const std::function<void(const std::string &)> &);
    void SetOutPutCallBack(const std::function<void(int)> &);
    void SetHangUpCallBack(const std::function<void()> &);
    void SetErrorCallBack(const std::function<void(int)> &);

private:
    FileDesc connfd_;
    struct sockaddr_in addr_;
    std::function<void(const std::string &)> inputCallBack_;
    std::function<void(int)> outputCallBack_;
    std::function<void(int)> errorCallBack_;
    Net::CircularBuffer writeBuffer_;
    Net::CircularBuffer readBuffer_;
};

template<typename ServiceType>
class ServerClient
{
public:
    int Call(const Request &request);
    void SetResponseCallBack(const std::function<void(const Response&)> &);
private:
    std::function<void(const Response&)> responseCallBack_;
};

class ProxyRpcServer
{
public:
    void Response();
    void SetRequestCallBack(const std::function<void(const Request &)>&);
private:
    std::function<void(const Request &)> requestCallBack_;
};