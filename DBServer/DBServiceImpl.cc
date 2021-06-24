#include <spdlog/spdlog.h>

#include "DBServiceImpl.h"


grpc::Status DBServiceImpl::RequestDB(grpc::ServerContext* context, const player::Request* request,
                                      player::Response*    response) {

    // fixed set log
    spdlog::debug("new request come from {0}", context->peer());
    // std::cout << "new request come from " << context->peer() << std::endl;

    response->set_stamp(request->stamp());
    response->set_uid(request->uid());
    if (!parser_.Parse(*request, *response)) {
        // bad request
        response->set_status(-1);
        return grpc::Status::CANCELLED;
    }
    response->set_status(1);
    return grpc::Status::OK;
}
