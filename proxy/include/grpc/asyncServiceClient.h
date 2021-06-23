#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <mutex>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#include "EventLoop.h"
#include "common.pb.h"
#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"
#include "proxy.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using grpc::CreateChannel;
using grpc::InsecureChannelCredentials;

using common::Request;
using common::Response;
using lobby::Lobby;
using room::Room;
using social::Social;

struct AsyncClientCall 
{
    Request request;
    Response reply;
    ClientContext context;
    Status status;
    std::unique_ptr<ClientAsyncResponseReader<Response>> response_reader;
};

template<typename ServiceType>
class Client
{
public:
    explicit Client(const std::string &serviceAddr, 
                    std::unordered_map<int64_t, ClientHandler*> uidToClient,
                    std::mutex *lock)
            : stub_(ServiceType::NewStub(CreateChannel(serviceAddr, InsecureChannelCredentials())), 
            uidToClient_(uidToClient), lock_(lock) {}

    void Notify(const Request &request)
    {
        AsyncClientCall *call = new AsyncClientCall;
        call->request = request;
        call->response_reader = 
            stub_->PrepareAsyncNotify(&call->context, call->request, &cq_);
        
        call->response_reader->StartCall();
        call->response_reader->Finish(&call->reply, &call->status, (void*)call);
    }

    void AsyncCompleteRpc()
    {
        void *got_tag;
        bool ok = false;

        while (cq_.Next(&got_tag, &ok))
        {
            // get a response, forward to the corresponding client
            AsyncClientCall *call = static_cast<AsyncClientCall*>(got_tag);
            int64_t uid = call->reply.uid();
            ClientHandler *client = NULL;
            {
            std::lock_guard<std::mutex> guard(lock_);
            if (uidToClient.find(uid) != uidToClient.end())
            {
                client = uidToClient[uid];
            }
            }
            if (client)
            {  
                Response *res = new Response;
                *res = call->reply;
                if (0 > client->pushRpcResponse(res))
                {
                    delete res;
                }
            }

            delete call;
        }
    }

private:
    std::unique_ptr<ServiceType::Stub> stub_;
    CompletionQueue cq_;
    int asyncNotifyPipe_; 
    std::unordered_map<int64_t, ClientHandler *> uidToClient_;
    std::mutex lock_;
};



// int main()
// {
//     // create three pipes for three async clients
//     int pipes[3][2];
//     for (int i = 0; i < 3; ++i)
//     {
//         pipe2(pipes[i], O_DIRECT | O_NONBLOCK);
//     } 

//     Client<Lobby> lobbyClient(grpc::CreateChannel(lobbyAddress, grpc::InsecureChannelCredentials()), pipes[0][1]);
//     Client<Room> roomClient(grpc::CreateChannel(roomAddress, grpc::InsecureChannelCredentials()), pipes[1][1]);
//     Client<Social> socialClient(grpc::CreateChannel(socialAddress, grpc::InsecureChannelCredentials()), pipes[2][1]);

//     std::thread lobbyListener = std::thread(&Client<Lobby>::AsyncCompleteRpc, &lobbyClient);
//     std::thread roomListener = std::thread(&Client<Room>::AsyncCompleteRpc, &roomClient);
//     std::thread socialListener = std::thread(&Client<Social>::AsyncCompleteRpc, &socialClient);

//     lobbyListener.join();
//     roomListener.join();
//     socialListener.join();

//     return 0;
// }