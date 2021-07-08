#include "CallData.h"
#include "DBServerAsynImpl.h"


CallData::CallData(DatabaseService::AsyncService* service,
                   ServerCompletionQueue*         cq,
                   RequestParser&                 parser): service_(service),
                                                           cq_(cq),
                                                           parser_(parser),
                                                           responder_(&ctx_),
                                                           status_(CREATE) {
    Proceed();
}

void CallData::Proceed() {
    if (status_ == CREATE) {
        status_ = PROCESS;
        service_->RequestNotify(&ctx_, &request_, &responder_, cq_, cq_, this);
    }
    else if (status_ == PROCESS) {
        new CallData(service_, cq_, parser_);
        SPDLOG_INFO("new request from {0}: type = {1}, uid = {2}", ctx_.peer(),Request::RequestType_Name(request_.requesttype()),request_.uid() );
        SPDLOG_TRACE("call response_.set_uid");
        response_.set_uid(request_.uid());
        SPDLOG_TRACE("call response_.set_stamp");
        response_.set_stamp(request_.stamp());
        SPDLOG_TRACE("call parser_.Parse");
        if(!parser_.Parse(request_, response_)){
            response_.set_status(-1);
        }
        SPDLOG_INFO("reply to {0}: status:{1}, arg.size={2}", ctx_.peer(), response_.status(), response_.args_size());
        status_ = FINISH;
        responder_.Finish(response_, Status::OK, this);
    }
    else {
        GPR_ASSERT(status_ == FINISH);
        delete this;
    }
}