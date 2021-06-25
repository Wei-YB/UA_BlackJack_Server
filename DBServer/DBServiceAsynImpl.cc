#include "DBServerAsynImpl.h"

using ua_black_jack_server::data_base_server::RequestParser;

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
    const std::string server_address(rpc_host_);
    ServerBuilder     builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service_);

    cq_     = builder.AddCompletionQueue();
    server_ = builder.BuildAndStart();

    std::cout << "Server listening on " << server_address << std::endl;

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
        spdlog::info("new request from {0}", ctx_.peer());
        response_.set_uid(request_.uid());
        response_.set_stamp(request_.stamp());
        parser_.Parse(request_, response_);
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
    while (ok) {
        GPR_ASSERT(cq_->Next(&tag, &ok));
        static_cast<CallData*>(tag)->Proceed();
    }
}
