#ifndef _PROXYSERVER_H_
#define _PROXYSERVER_H_

#include <mutex>
#include <unordered_map>
#include <functional>

#include "UA_BlackJack.pb.h"
#include "common.h"

using ua_blackjack::Request;
using ua_blackjack::Response;

#define MAX_FORWORD_FAILURE 5

#define BUFFER_SIZE 1024 * 4     // 4 KB
#define QUEUE_SIZE  128

// forward declarations
class ServiceClient;
class Client;

namespace Net {
    class TcpServer;
    class TcpConnection;
    class EventLoop;
}


class ProxyServer {
public:
    // ip: which ip to bind
    // port: which port to listen
    // loop: pointer to the eventloop of the main thread
    ProxyServer(const char *ip, unsigned short port, Net::EventLoop *loop);

    // conn: shared ptr to new tcp connection
    void OnNewClient(std::shared_ptr<TcpConnection> conn);

    // fd: file descriptor for the tcp connection of the client
    // request: request from client
    void OnClientRequest(FileDesc fd, Request &request);

    // response: response from client
    void OnClientResponse(Response &response);

    // response: response from rpc server
    void OnServiceResponse(const Response& response);

    // fd: file descriptor for the tcp connection of the client
    void OnDisConnection(FileDesc fd);

    void OnError(FileDesc fd);
    
    // request: request from remote rpc caller
    int SendRequest(Request &request);

    // clientResponseCallBack: callback function when client sent response
    void SetClientResponseCallBack(const std::function<void(Response &)> &clientResponseCallBack)
    {clientResponseCallBack_ = clientResponseCallBack;}

    //  register rpc client
    void RegisterServiceClient(Request::RequestType requestType, std::shared_ptr<ServiceClient> client)
    {requestTypeToServiceClient_.emplace(requestType, client);}
private:
    std::mutex uidToClientLock_;
    std::unordered_map<UserId, std::shared_ptr<Client>> uidToClient_;   
    std::unordered_map<FileDesc, std::shared_ptr<Client>> fdToClient_;  // only accessible from the main thread, no lock protected
    std::mutex stampToClientLock_;
    std::unordered_map<int64_t, std::weak_ptr<Client>> stampToClient_;
    std::unordered_map<Request::RequestType, std::weak_ptr<ServiceClient>> requestTypeToServiceClient_;
    std::function<void(Response &)> clientResponseCallBack_;
    std::shared_ptr<Net::TcpServer> server_;
};

#endif