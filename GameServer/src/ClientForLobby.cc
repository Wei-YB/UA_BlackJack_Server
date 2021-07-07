#include "ClientForLobby.h"
#include <chrono>
#include "spdlog/spdlog.h"
#include <sstream>
void ua_blackjack::Game::ClientForLobby::matchEnd(const BlackJackRoomID roomid)
{

    spdlog::info("Start lobby match end request roomid {0}", roomid);

    Request request;
    std::stringstream ss;
    ss << roomid;
    std::string tmpStr = ss.str();
    request.set_requesttype(ua_blackjack::Request_RequestType::Request_RequestType_MATCH_END); //requestType
    request.add_args(tmpStr);
    // Call object to store rpc data
    AsyncClientCall *call = new AsyncClientCall;

    call->response_reader =
        stub_->PrepareAsyncNotify(&call->context, request, &cq_);

    // StartCall initiates the RPC call
    call->response_reader->StartCall();

    call->response_reader->Finish(&call->reply, &call->status, (void *)call);
}