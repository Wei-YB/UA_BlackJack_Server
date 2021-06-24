#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "../Player.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using player::DatabaseService;
using player::Request;
using player::Response;

class Client {
public:
    Client(std::shared_ptr<Channel> channel)
        : stub_(DatabaseService::NewStub(channel)) {
    }

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    int64_t SignUp(const std::string& nickname, const std::string& password) {

        Request  request;
        Response response;
        request.add_args(nickname);
        request.add_args(password);
        request.set_requesttype(player::Request_RequestType_SIGNUP);


        ClientContext context;


        // The actual RPC.
        Status status = stub_->RequestDB(&context, request, &response);

        // Act upon its status.
        if (status.ok()) {
            return response.uid();
        }
        else {
            std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
            return -1;
        }
    }

private:
    std::unique_ptr<DatabaseService::Stub> stub_;
};

int main(int argc, char** argv) {
    std::string target_str = "localhost:50051";
    Client      client(
        grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials())
    );

    std::string name     = "owen1";
    std::string password = "owenpass";


    auto reply = client.SignUp(name, password);
    std::cout << "Client received: " << reply << std::endl;

    return 0;
}
