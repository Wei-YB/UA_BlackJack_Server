#include "ProxyRPCClient.h"


UID ClientSyn::Login(std::string username, std::string password) {
    Request request;
    //request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_requesttype(Request_RequestType::Request_RequestType_LOGIN);
    request.set_stamp(getCurrentLocalTimeStamp());
    request.add_args(username);
    request.add_args(password);

    Response reply;
    ClientContext context;

    Status status = stub_->Notify(&context, request, &reply);
    UID uid = reply.uid();
    // client这边只判断reply.status 不判断uid成功与否
    // Act upon its status.
    if (status.ok()) {
      spdlog::info("Login succeed ");
      
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
    }
    return uid;
  }

  bool ClientSyn::JoinRoom(UID uid, RoomID rid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());
    request.add_args(std::to_string(rid));

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start JoinRoom request with stamp------------{0:d}", request.stamp());

    if (status.ok()) {
      
        spdlog::info("join room succeed ");
        return true;
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
      return false;
    }
  }

  RoomID ClientSyn::CreateRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_CREATE_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start CreateRoom request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    if (status.ok()) {
      
        spdlog::info("create room succeed");
        return std::stoi(reply.args(0));
      

    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
      return -1;
    }
  }

  bool ClientSyn::LeaveRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_LEAVE_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start LeaveRoom request with stamp------------{0:d}", request.stamp());

    // Act upon its status.
    if (status.ok()) {
      
        spdlog::info("leave room succeed");
        return true;
    

    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
      return false;
    }
  }

  bool ClientSyn::PlayerReady(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_READY);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start PlayerReady request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    if (status.ok()) {
      
        spdlog::info("player ready succeed");
        return true;


    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
      return false;
    }
  }

  RoomID ClientSyn::QuickMatch(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_QUICK_MATCH);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start QuickMatch request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    if (status.ok()) {
      
        spdlog::info("quick match succeed");
        return std::stoi(reply.args(0));
      
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
      return -1;
    }
  }

  std::vector<RoomID> ClientSyn::GetAvailableRoomList() {
    Request request;
  
    request.set_requesttype(Request_RequestType::Request_RequestType_ROOM_LIST);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start RoomList request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    std::vector<RoomID> roomlist;
    if (status.ok()) {
        for(int i = 0; i<reply.args_size(); i++){
          roomlist.push_back(std::stoi(reply.args(i)));
        }
        spdlog::info("get roomlist succeed");
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");
    }

    return roomlist;

  }

  void ClientSyn::MatchEnd(RoomID rid) {
    Request request;
    request.add_args(std::to_string(rid));
    request.set_requesttype(Request_RequestType::Request_RequestType_MATCH_END);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start MatchEnd request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    if (status.ok()) {
        spdlog::info("match end succeed");
      
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");

    }
  }

  void ClientSyn::Logout(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_LOGOUT);
    request.set_stamp(getCurrentLocalTimeStamp());

    Response reply;
    ClientContext context;

    // The actual RPC.
    Status status = stub_->Notify(&context, request, &reply);
    spdlog::trace("start Logout request with stamp------------{0:d}", request.stamp());
    // Act upon its status.
    if (status.ok()) {
      spdlog::info("logout succeed");
      
    } else {
      spdlog::warn( "{0:d}:{1} ", status.error_code(), status.error_message());
      spdlog::warn( "rpc failed ");

    }
  }

  int64_t ClientSyn::getCurrentLocalTimeStamp()
  {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
    return tmp.count();
    
    // return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }



UID ClientAsyn::Login(std::string username, std::string password) {
    Request request;
    //request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_requesttype(Request_RequestType::Request_RequestType_LOGIN);
    request.set_stamp(getCurrentLocalTimeStamp());
    request.add_args(username);
    request.add_args(password);
  
    AsynClientCall(request);
    
  }

  bool ClientAsyn::JoinRoom(UID uid, RoomID rid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_JOIN_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());
    request.add_args(std::to_string(rid));

    AsynClientCall(request);
  }

  bool ClientAsyn::LeaveRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_LEAVE_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());

    AsynClientCall(request);
  }

  RoomID ClientAsyn::CreateRoom(UID uid) {
    Request request;
    request.set_uid(uid);
    request.set_requesttype(Request_RequestType::Request_RequestType_CREATE_ROOM);
    request.set_stamp(getCurrentLocalTimeStamp());

    AsynClientCall(request);
  }

// Assembles the client's payload and sends it to the server.
void ClientAsyn::AsynClientCall(Request request){
  // Call object to store rpc data
  AsyncClientCall* call = new AsyncClientCall;
  call->response_reader = stub_->PrepareAsyncNotify(&call->context, request, &cq_);
  //StartCall()和Finish()都会立即返回，无需等待服务端的响应！
  call->response_reader->StartCall();
  call->response_reader->Finish(&call->reply, &call->status, (void*)call);

}

void ClientAsyn::AsyncCompleteRpc() {
  void* got_tag;
  bool ok = false;

  // Block until the next result is available in the completion queue "cq".
  // 故虽然是异步，但当服务端没有回应时就会阻塞在cq_.Next里，所以此成员函数应该放在新的线程里运行，让此阻塞不会妨碍主线程的运行！
  // 需要循环调用cq_里的Next方法！
  // 这个while是一个无限循环，用于监听来自服务器的响应
  while (cq_.Next(&got_tag, &ok)) {
    std::cout << "cq_.Next has returned." << std::endl;
    AsyncClientCall* call = static_cast<AsyncClientCall*>(got_tag);
    //GPR_ASSERT(ok);
    if (call->status.ok()){
      spdlog::info("rpc succeed ");
    }
    else{
      spdlog::warn( "{0:d}:{1} ", call->status.error_code(), call->status.error_message());
      spdlog::warn( "rpc failed ");
    }
    delete call;
  }
}

int64_t ClientAsyn::getCurrentLocalTimeStamp()
  {
    std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> tp = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
    return tmp.count();
    
    // return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }