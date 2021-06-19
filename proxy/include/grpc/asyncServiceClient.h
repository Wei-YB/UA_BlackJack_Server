#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>
#include <thread>

#include "../net/EventLoop.h"
#include "common.pb.h"
#include "lobby.grpc.pb.h"
#include "room.grpc.pb.h"
#include "social.grpc.pb.h"

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

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
    explicit Client(std::shared_ptr<Channel> channel, 
                    Net::HandlerManager<int64_t> *clientManager, 
                    int asyncNotifyPipe)
            : stub_(ServiceType::NewStub(channel)), 
            clientManager_(clientManager), 
            asyncNotifyPipe_(asyncNotifyPipe) {}

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
            Net::EventsHandler *client = clientManager_->find(uid);
            // deal with this reponse only when client exists
            if (client)
            {
                Response *res = new Response;
                *res = call->reply;
                if (0 == client->notifyInAdvance(asyncNotifyPipe_, Net::EV_IN, (void*)res))
                {
                    write(asyncNotifyPipe_, (void*)&uid, sizeof(uid));
                }
                else
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
    Net::HandlerManager<int64_t> *clientManager_;
};

const std::string lobbyAddress = "localhost:50051";
const std::string roomAddress = "localhost:50051";
const std::string socialAddress = "localhost:50051";

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