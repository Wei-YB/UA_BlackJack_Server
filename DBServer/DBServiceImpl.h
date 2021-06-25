#pragma once

#include "UA_BlackJack.grpc.pb.h"
#include "RequestParser.h"

class DBServiceImpl : public ua_blackjack::DatabaseService::Service {

public:
    using RequestParser = ua_black_jack_server::data_base_server::RequestParser;
    using Status = grpc::Status;
    using ServerContext = grpc::ServerContext;
    using Request = ua_blackjack::Request;
    using Response = ua_blackjack::Response;


    DBServiceImpl(const std::string& redis_host, int init_score): parser_(redis_host, init_score) {
    }

    DBServiceImpl(const DBServiceImpl&)            = delete;
    DBServiceImpl(DBServiceImpl&&)                 = delete;
    DBServiceImpl& operator=(const DBServiceImpl&) = delete;
    DBServiceImpl& operator=(DBServiceImpl&&)      = delete;


    Status RequestDB(ServerContext* context, const Request* req, Response* rsp) override;
    ~DBServiceImpl() override = default;

private:
    RequestParser parser_;
}; 
