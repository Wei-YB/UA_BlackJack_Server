#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <unordered_map>
#include <functional>

#include "common.pb.h"
#include "circ_buf.h"
#include "ClientProxyProtocol.h"
#include "./asyncServiceClient.h"
#include "EventLoop.h"

using common::Request;
using common::Response;

typedef int FileDesc;
typedef int64_t UserId;

class TcpConnection
{
    const int bufferSize_ = 4096;
public:
    TcpConnection(FileDesc connfd, struct sockaddr_in addr)
                : writeBuffer_(bufferSize_), readBuffer_(bufferSize_), 
                eventsSource_(connfd, std::bind(&TcpConnection::OnInput, this), 
                                std::bind(&TcpConnection::OnOutput, this), 
                                std::bind(&TcpConnection::OnError, this)) {}
    ~TcpConnection() {close(eventsSource_.fd());}
    // callbacks
    int OnInput()
    {
        FileDesc clientfd = eventsSource_.fd();
        int byteRead, pkgProcessed = 0;
        bool needToRead = true;
        std::vector<Request> requests;
        std::vector<Response> responses;
        while (needToRead)
        {
            byteRead = Net::read(clientfd, readBuffer_);
            if (byteRead < 0)
            {   // fatal error
                return -1;
            }
            if (readBuffer_.size() != readBuffer_.capacity())
		    {   // the tcp buffer has been cleared, don't have to read anymore
			    needToRead = false;
		    }
            // handle packages in buffer one by one
            while (readBuffer_.size() >= 8)
            {
                int32_t msgType, msgLength; 
                Net::readAs(readBuffer_, 0, msgType, true);
                Net::readAs(readBuffer_, sizeof(msgType), msgLength, true);
                if (msgLength == -1 || readBuffer_.size() - 8 < msgLength)
                {   // not a complete package, wait for the next read event
                    return pkgProcessed;
                }
                readBuffer_.free(sizeof(msgType) + sizeof(msgLength));

                std::string rawMsg;
                Net::circularBufferToString(readBuffer_, msgLength, rawMsg);
                readBuffer_.free(msgLength);
                if (msgType == NS::REQUEST)
                {
                    Request request;
                    request.ParseFromString(rawMsg);
                    requests.push_back(request);
                }
                else if (msgType == NS::RESPONSE)
                {
                    Response response;
                    response.ParseFromString(rawMsg);
                    responses.push_back(response);
                }
                pkgProcessed++;
            }
        }
        // TODO: call Client to handle this
        inputCallBack_(requests, responses);
        return pkgProcessed;;
    }
    
    int OnOutput()
    {
        // check whether we can get some data from upper layer
        outputCallBack_();
        if (writeBuffer_.empty())
        {   
            eventsSource_.Update(Net::EV_IN | Net::EV_ERR | Net::EV_ERR);
            return 0;
        }
        return write(eventsSource_.fd(), writeBuffer_);
    }
    
    int OnError()
    {
        errorCallBack_();
        return -1;
    }
    //
    int Send(const std::string &pkgsData)
    {
        return Net::put(writeBuffer_, pkgsData.c_str(), pkgsData.size());
    }

    int SockFd() const {return eventsSource_.fd();}
    int GetWriteBufferRoom() const {return writeBuffer_.capacity() - writeBuffer_.size();}
    // 
    void SetInputCallBack(const std::function<void(std::vector<Request>, std::vector<Response>)> &cb) {inputCallBack_ = cb;}
    void SetOutPutCallBack(const std::function<void()> &cb) {outputCallBack_ = cb;}
    void SetErrorCallBack(const std::function<void()> &cb) {errorCallBack_ = cb;}

private:
    Net::EventsSource eventsSource_;
    std::function<void(std::vector<Request>, std::vector<Response>)> inputCallBack_;
    std::function<void()> outputCallBack_;
    std::function<void()> errorCallBack_;
    Net::CircularBuffer writeBuffer_;
    Net::CircularBuffer readBuffer_;
};

class Client
{
public:
    Client(std::shared_ptr<TcpConnection> conn, 
           const std::function<void(FileDesc, const Request &)> &requestCallBack,
           const std::function<void(const Response &)> &responseCallBack,
           const std::function<void(FileDesc)> &errorCallBack) 
           : conn_(conn), 
            requestCallBack_(requestCallBack),
            responseCallBack_(responseCallBack),
            errorCallBack_(errorCallBack) 
    {
        conn_->SetInputCallBack(std::bind(&Client::OnMessages, this, std::placeholders::_1, std::placeholders::_2));
        conn_->SetOutPutCallBack(std::bind(&Client::OnSendReady, this));
        conn_->SetErrorCallBack(std::bind(&Client::OnError, this));
    }
public:
    //
    int SendRequest(const Request &request)
    {
        std::string rawRequest = request.SerializeAsString();
        std::string pkgData(8 + rawRequest.size(), '\0');
        NS::pack(NS::REQUEST, rawRequest, &pkgData[0], pkgData.size());
        {
            std::lock_guard<std::mutex> guard(connLock_);
            if (pkgData.size() > conn_->GetWriteBufferRoom())
            {   // TODO: maybe we can buffer this request
                return -1;
            }
            conn_->Send(pkgData);
        }
        return 0;
    }

    int SendResponse(const Response &response)
    {
        std::string rawResponse = response.SerializeAsString();
        std::string pkgData(8 + rawResponse.size(), '\0');
        NS::pack(NS::RESPONSE, rawResponse, &pkgData[0], pkgData.size());
        {
            std::lock_guard<std::mutex> guard(connLock_);
            if (pkgData.size() > conn_->GetWriteBufferRoom())
            {   // TODO: maybe we can buffer this request
                return -1;
            }
            conn_->Send(pkgData);
        }
        return 0;
    }
    
    UserId uid() const {return uid_;}
    void SetUid(UserId id) {uid_ = id;}
    void SetUnloginStamp(int64_t stamp) {unloginStamp_ = stamp;}
    // callbacks
    void OnMessages(const std::vector<Request> &requests, const std::vector<Response> &responses)
    {
        for (int i = 0; i < requests.size(); ++i)
        {
            requestCallBack_(requests[i]);
        }
        for (int i = 0; i < responses.size(); ++i)
        {
            responseCallBack_(responses[i]);
        }
    }
    void OnSendReady() {/*TODO: write requests/responses buffer to tcp connection*/;}
    void OnError() {errorCallBack_(conn_->SockFd());}
private:
    UserId uid_ = -1;
    int64_t unloginStamp_;
    std::mutex connLock_;
    std::shared_ptr<TcpConnection> conn_;
    std::function<void(FileDesc, const Request &)> requestCallBack_;
    std::function<void(const Response &)> responseCallBack_;
    std::function<void(FileDesc)> errorCallBack_;
};

class Proxy {
public:
    // callbacks
    void OnNewClient(std::shared_ptr<TcpConnection> conn)
    {
        auto requestCallBack = std::bind(&Proxy::OnClientRequest, this, std::placeholders::_1, std::placeholders::_2);
        auto responseCallBack = std::bind(&Proxy::OnClientResponse, this, std::placeholders::_1);
        auto errorCallBack = std::bind(&Proxy::OnError, this, std::placeholders::_1);
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

    void SetClientResponseCallBack(const std::function<void(const Response &)> &clientResponseCallBack)
    {clientResponseCallBack_ = clientResponseCallBack;}
private:
    std::mutex uidToClientLock_;
    std::unordered_map<UserId, std::shared_ptr<Client>> uidToClient_;   
    std::unordered_map<FileDesc, std::shared_ptr<Client>> fdToClient_;  // only accessible from the main thread, no lock protected
    std::mutex stampToClientLock_;
    std::unordered_map<int64_t, std::weak_ptr<Client>> stampToClient_;
    std::unordered_map<Request::RequestType, std::weak_ptr<ServiceClient>> requestTypeToServiceClient_;
    std::function<void(const Response &)> clientResponseCallBack_;
};

