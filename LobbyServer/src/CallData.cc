#include "CallData.h"
#include <spdlog/spdlog.h>

//CallData对象创建之后会调用proceed(),则status会变为NEW_REQUEST
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

//Proceed里是有限状态机
void CallData::Proceed(Response* rpc_result) {
    if (status_ == CREATE) {
        ProceedCreate();
    }
    else if (status_ == NEW_REQUEST) {
        ProceedNewRequest();
    }
    else if (status_ == PROCESS) {
        //真正的处理这个proxy的request，ProceedProcess有可能需要调用多次
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

//当proxy request, game request到来时，把存放着此request的calldata对象放在阻塞队列里，必须先更新状态机，再pushback到request_list_里激活workthread重新进入proceed里的相应状态(status == PROCESS)进行处理
//race发生时status_未改变为PROCESS，将会重新进入此函数引发gRPC崩溃
void CallData::ProceedNewRequest() {
    // in main thread: listen request from proxy.
    const auto request_type = request_.requesttype();
    SPDLOG_INFO("new request come from {0} with stamp---------------------{1}, request type is {2}, uid is {3}", ctx_.peer(), request_.stamp(), Request::RequestType_Name(request_type), request_.uid());
    parser_ = parser_factory_->NewParser(request_type);

    //race condition fixed
    status_ = PROCESS;
    request_list_.PushBack(this);
}

//由wordthread执行
void CallData::ProceedProcess(Response* rpc_result) {
    SPDLOG_TRACE("process the request with stamp---------------------{0:d}",  request_.stamp());
    //Parser解析器去解析此request并完成此请求的处理，请求结果放在reply_
    const auto result = parser_->Parse(request_, rpc_result, reply_);
    if (result) {
        status_ = FINISH;
        //向client发回结果
        responder_.Finish(reply_, Status::OK, this);
    }
}

ParserFactory* CallData::parser_factory_ = nullptr;
