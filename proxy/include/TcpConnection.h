#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_
#include <functional>
#include <netinet/in.h>
#include "common.h"
#include "common.pb.h"
#include "EventLoop.h"
#include "CircularBuffer.h"
#include "ClientProxyProtocol.h"

using common::Request;
using common::Response;

class TcpConnection
{
    static const int bufferSize_ = 4096;
public:
    TcpConnection(FileDesc connfd, const struct sockaddr_in &addr, Net::EventLoop *loop)
                : writeBuffer_(bufferSize_), readBuffer_(bufferSize_), 
                eventsSource_(connfd, loop,
                                std::bind(&TcpConnection::OnInput, this), 
                                std::bind(&TcpConnection::OnOutput, this), 
                                std::bind(&TcpConnection::OnError, this)) 
    {eventsSource_.Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);}
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
        if (inputCallBack_)
        {
            inputCallBack_(requests, responses);
        }
        return pkgProcessed;;
    }
    
    int OnOutput()
    {
        // check whether we can get some data from upper layer
        if (outputCallBack_)
        {
            outputCallBack_();
        }
        if (writeBuffer_.empty())
        {   
            eventsSource_.Update(Net::EV_IN | Net::EV_ET | Net::EV_ERR);
            return 0;
        }
        return write(eventsSource_.fd(), writeBuffer_);
    }
    
    int OnError()
    {
        if (errorCallBack_)
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

#endif
