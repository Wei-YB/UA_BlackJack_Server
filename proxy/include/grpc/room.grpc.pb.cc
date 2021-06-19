// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: room.proto

#include "room.pb.h"
#include "room.grpc.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/channel_interface.h>
#include <grpcpp/impl/codegen/client_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/rpc_service_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/sync_stream.h>
namespace room {

static const char* Room_method_names[] = {
  "/room.Room/Notify",
};

std::unique_ptr< Room::Stub> Room::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< Room::Stub> stub(new Room::Stub(channel, options));
  return stub;
}

Room::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_Notify_(Room_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status Room::Stub::Notify(::grpc::ClientContext* context, const ::common::Request& request, ::common::Response* response) {
  return ::grpc::internal::BlockingUnaryCall< ::common::Request, ::common::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_Notify_, context, request, response);
}

void Room::Stub::experimental_async::Notify(::grpc::ClientContext* context, const ::common::Request* request, ::common::Response* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::common::Request, ::common::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Notify_, context, request, response, std::move(f));
}

void Room::Stub::experimental_async::Notify(::grpc::ClientContext* context, const ::common::Request* request, ::common::Response* response, ::grpc::experimental::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_Notify_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::common::Response>* Room::Stub::PrepareAsyncNotifyRaw(::grpc::ClientContext* context, const ::common::Request& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::common::Response, ::common::Request, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_Notify_, context, request);
}

::grpc::ClientAsyncResponseReader< ::common::Response>* Room::Stub::AsyncNotifyRaw(::grpc::ClientContext* context, const ::common::Request& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncNotifyRaw(context, request, cq);
  result->StartCall();
  return result;
}

Room::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      Room_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< Room::Service, ::common::Request, ::common::Response, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](Room::Service* service,
             ::grpc::ServerContext* ctx,
             const ::common::Request* req,
             ::common::Response* resp) {
               return service->Notify(ctx, req, resp);
             }, this)));
}

Room::Service::~Service() {
}

::grpc::Status Room::Service::Notify(::grpc::ServerContext* context, const ::common::Request* request, ::common::Response* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace room

