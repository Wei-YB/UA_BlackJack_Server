#pragma once

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif


#include "Lobby.h"
#include "RPCClient.h"
#include "UA_BlackJack.grpc.pb.h"


namespace ua_blackjack::lobby {

using ua_blackjack::Request;
using ua_blackjack::Response;

class Parser {
public:
    Parser(Lobby& lobby);

    Parser(const Parser&) = delete;
    Parser(Parser&&)      = delete;

    Parser& operator=(const Parser&) = delete;
    Parser& operator=(Parser&&)      = delete;

    virtual bool Parse(const Request& request, Response* rpc_result, Response& response);

    int64_t GetStamp() const;

    virtual ~Parser();

protected:
    int64_t stamp_;
    Lobby&  lobby_;
};

class LoginParser final : public Parser {
public:
    LoginParser(Lobby& lobby, RPCClient& client);

    bool Parse(const Request& request, Response* rpcResult, Response& response) override;

    ~LoginParser() override;

private:
    enum class RPC_State {
        START,
        GET_UID,
        GET_PASSWORD,
    };

    std::string password;
    std::string nickname;
    RPCClient&  client_;
    RPC_State   state_;
    char        padding_[4];
};

class ReadyParser final : public Parser {
public:
    ReadyParser(Lobby& lobby, RPCClient& client);


    bool Parse(const Request& request, Response* rpcResult, Response& response) override;

    ~ReadyParser() override;

    enum class RPC_State {
        START,
        CALL_MATCH_START,
    };

    RPCClient& client_;
    RPC_State  state_;
    char       padding_[4];
};

class ParserFactory {
public:
    ParserFactory(Lobby& lobby, RPCClient& client);

    std::shared_ptr<Parser> NewParser(ua_blackjack::Request_RequestType type);

private:
    Lobby      lobby_;
    RPCClient& rpc_client_;
};

}
