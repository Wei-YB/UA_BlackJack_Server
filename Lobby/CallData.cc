#include "CallData.h"
#include <spdlog/spdlog.h>

CallData::CallData(LobbyService::AsyncService* service,
                   ServerCompletionQueue*      cq,
                   BlockList<CallData*>&       request_list): service_(service),
                                                              cq_(cq),
                                                              parser_(nullptr),
                                                              request_list_(request_list),
                                                              responder_(&ctx_),
                                                              status_(CREATE) {
    Proceed();
}

void CallData::Proceed(Response* rpc_result) {
    if (status_ == CREATE) {
        ProceedCreate();
    }
    else if (status_ == NEW_REQUEST) {
        ProceedNewRequest();
    }
    else if (status_ == PROCESS) {
        new CallData(service_, cq_, request_list_);
        ProceedProcess(rpc_result);
    }
    else {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
}

int64_t CallData::GetStamp() const { return parser_->GetStamp(); }

CallData::CallStatus CallData::GetCallStatus() const {
    return status_;
}

void CallData::SetParserFactory(ParserFactory* parser_factory) {
    parser_factory_ = parser_factory;
}

void CallData::ProceedCreate() {
    status_ = NEW_REQUEST;
    service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void CallData::ProceedNewRequest() {
    const auto request_type = request_.requesttype();
    spd::info("new request come from {0}", ctx_.peer());
    parser_ = parser_factory_->NewParser(request_type);
    request_list_.PushBack(this);
    status_ = PROCESS;
}

void CallData::ProceedProcess(Response* rpc_result) {
    spd::trace("process the request ");
    const auto result = parser_->Parse(request_, rpc_result, reply_);
    if (result) {
        status_ = FINISH;
        responder_.Finish(reply_, Status::OK, this);
    }
}

ParserFactory* CallData::parser_factory_ = nullptr;
