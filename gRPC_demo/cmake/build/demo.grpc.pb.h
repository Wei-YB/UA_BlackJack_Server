// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: demo.proto
// Original file comments:
// Copyright 2015 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef GRPC_demo_2eproto__INCLUDED
#define GRPC_demo_2eproto__INCLUDED

#include "demo.pb.h"

#include <functional>
#include <grpc/impl/codegen/port_platform.h>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace demo {

class GetNameService final {
 public:
  static constexpr char const* service_full_name() {
    return "demo.GetNameService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status GetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::demo::IDReply* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>> AsyncGetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>>(AsyncGetIDRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>> PrepareAsyncGetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>>(PrepareAsyncGetIDRaw(context, request, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, std::function<void(::grpc::Status)>) = 0;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      virtual void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, ::grpc::ClientUnaryReactor* reactor) = 0;
      #else
      virtual void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) = 0;
      #endif
    };
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    typedef class experimental_async_interface async_interface;
    #endif
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    async_interface* async() { return experimental_async(); }
    #endif
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>* AsyncGetIDRaw(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::demo::IDReply>* PrepareAsyncGetIDRaw(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    ::grpc::Status GetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::demo::IDReply* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::demo::IDReply>> AsyncGetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::demo::IDReply>>(AsyncGetIDRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::demo::IDReply>> PrepareAsyncGetID(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::demo::IDReply>>(PrepareAsyncGetIDRaw(context, request, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, std::function<void(::grpc::Status)>) override;
      #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, ::grpc::ClientUnaryReactor* reactor) override;
      #else
      void GetID(::grpc::ClientContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response, ::grpc::experimental::ClientUnaryReactor* reactor) override;
      #endif
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::demo::IDReply>* AsyncGetIDRaw(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::demo::IDReply>* PrepareAsyncGetIDRaw(::grpc::ClientContext* context, const ::demo::IDRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_GetID_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status GetID(::grpc::ServerContext* context, const ::demo::IDRequest* request, ::demo::IDReply* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_GetID() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetID(::grpc::ServerContext* context, ::demo::IDRequest* request, ::grpc::ServerAsyncResponseWriter< ::demo::IDReply>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_GetID<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithCallbackMethod_GetID() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::demo::IDRequest, ::demo::IDReply>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::demo::IDRequest* request, ::demo::IDReply* response) { return this->GetID(context, request, response); }));}
    void SetMessageAllocatorFor_GetID(
        ::grpc::experimental::MessageAllocator< ::demo::IDRequest, ::demo::IDReply>* allocator) {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::GetHandler(0);
    #else
      ::grpc::internal::MethodHandler* const handler = ::grpc::Service::experimental().GetHandler(0);
    #endif
      static_cast<::grpc::internal::CallbackUnaryHandler< ::demo::IDRequest, ::demo::IDReply>*>(handler)
              ->SetMessageAllocator(allocator);
    }
    ~ExperimentalWithCallbackMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* GetID(
      ::grpc::CallbackServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* GetID(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/)
    #endif
      { return nullptr; }
  };
  #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
  typedef ExperimentalWithCallbackMethod_GetID<Service > CallbackService;
  #endif

  typedef ExperimentalWithCallbackMethod_GetID<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_GetID() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_GetID() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetID(::grpc::ServerContext* context, ::grpc::ByteBuffer* request, ::grpc::ServerAsyncResponseWriter< ::grpc::ByteBuffer>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    ExperimentalWithRawCallbackMethod_GetID() {
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
      ::grpc::Service::
    #else
      ::grpc::Service::experimental().
    #endif
        MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackUnaryHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
                   ::grpc::CallbackServerContext*
    #else
                   ::grpc::experimental::CallbackServerContext*
    #endif
                     context, const ::grpc::ByteBuffer* request, ::grpc::ByteBuffer* response) { return this->GetID(context, request, response); }));
    }
    ~ExperimentalWithRawCallbackMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    #ifdef GRPC_CALLBACK_API_NONEXPERIMENTAL
    virtual ::grpc::ServerUnaryReactor* GetID(
      ::grpc::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #else
    virtual ::grpc::experimental::ServerUnaryReactor* GetID(
      ::grpc::experimental::CallbackServerContext* /*context*/, const ::grpc::ByteBuffer* /*request*/, ::grpc::ByteBuffer* /*response*/)
    #endif
      { return nullptr; }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetID : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithStreamedUnaryMethod_GetID() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::internal::StreamedUnaryHandler<
          ::demo::IDRequest, ::demo::IDReply>(
            [this](::grpc::ServerContext* context,
                   ::grpc::ServerUnaryStreamer<
                     ::demo::IDRequest, ::demo::IDReply>* streamer) {
                       return this->StreamedGetID(context,
                         streamer);
                  }));
    }
    ~WithStreamedUnaryMethod_GetID() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetID(::grpc::ServerContext* /*context*/, const ::demo::IDRequest* /*request*/, ::demo::IDReply* /*response*/) override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetID(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::demo::IDRequest,::demo::IDReply>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_GetID<Service > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_GetID<Service > StreamedService;
};

}  // namespace demo


#endif  // GRPC_demo_2eproto__INCLUDED