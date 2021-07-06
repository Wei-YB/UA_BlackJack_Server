#include "DBServerAsynImpl.h"

using ua_blackjack::data_base_server::RequestParser;

ServerAsynImpl::ServerAsynImpl(const std::string& rpc_host,
                               const std::string& redis_host,
                               const int          init_score) : rpc_host_(rpc_host),
                                                                redis_host_(redis_host),
                                                                parser_(redis_host, init_score) {
}

ServerAsynImpl::~ServerAsynImpl() {
    server_->Shutdown();
    cq_->Shutdown();
}

void ServerAsynImpl::Run() {
    stop_ = false;
    const std::string server_address(rpc_host_);
    ServerBuilder     builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_     = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    SPDLOG_INFO("Server listening on {}", server_address);

    HandleRpcs();
}

class CallData {
public:
    CallData(DatabaseService::AsyncService* service, ServerCompletionQueue* cq, RequestParser& parser);

    void Proceed();

private:
    DatabaseService::AsyncService* service_;
    ServerCompletionQueue*         cq_;
    ServerContext                  ctx_;

    Request                             request_;
    Response                            response_;
    RequestParser&                      parser_;
    ServerAsyncResponseWriter<Response> responder_;

    enum CallStatus { CREATE, PROCESS, FINISH };

    CallStatus status_;
};

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

void ServerAsynImpl::HandleRpcs() {
    new CallData(&service_, cq_.get(), parser_);
    void* tag;
    bool  ok = true;
    gpr_timespec deadline;
    deadline.tv_nsec = 0;
    deadline.tv_sec = 1;
    deadline.clock_type = GPR_TIMESPAN;
    while (!stop_) {
        auto status = cq_->AsyncNext(&tag, &ok, deadline);
        if(status == grpc::CompletionQueue::NextStatus::GOT_EVENT)
            static_cast<CallData*>(tag)->Proceed();
        else if(stop_){
            cq_->Shutdown();
            server_->Shutdown();
        }
    }
    
}
