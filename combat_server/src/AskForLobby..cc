#include "AskForLobby.h"
#include <chrono>
#include <sstream>
void ClientForLobby::matchEnd(const BlackJackRoomID roomid)
{
#ifdef PRINT_LOG
    std::cout << "Start lobby match end request" << std::endl;
#endif
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