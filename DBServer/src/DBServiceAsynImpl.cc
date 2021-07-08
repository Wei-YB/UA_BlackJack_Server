#include "DBServerAsynImpl.h"



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

    spdlog::info("Server listening on {}", server_address);

    HandleRpcs();
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
