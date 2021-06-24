cmake_minimum_required(VERSION 3.5.1)

project(Tests C CXX)

include(../common.cmake)

# proto file
get_filename_component(common_proto "../include/protos/common.proto" ABSOLUTE)
get_filename_component(proto_path "${common_proto}" PATH)
MESSAGE(STATUS "proto_path: ${proto_path}")


set(inc_path "${proto_path}/..")
# generate sources
set(common_proto_srcs "${inc_path}/grpc/common.pb.cc")
set(proxy_proto_srcs "${inc_path}/grpc/proxy.pb.cc")
set(lobby_proto_srcs "${inc_path}/grpc/lobby.pb.cc")
set(room_proto_srcs "${inc_path}/grpc/room.pb.cc")
set(social_proto_srcs "${inc_path}/grpc/social.pb.cc")

set(common_proto_hdrs "${inc_path}/grpc/common.pb.h")
set(proxy_proto_hdrs "${inc_path}/grpc/proxy.pb.h")
set(lobby_proto_hdrs "${inc_path}/grpc/lobby.pb.h")
set(room_proto_hdrs "${inc_path}/grpc/room.pb.h")
set(social_proto_hdrs "${inc_path}/grpc/social.pb.h")

set(proxy_grpc_srcs "${inc_path}/grpc/proxy.grpc.pb.cc")
set(lobby_grpc_srcs "${inc_path}/grpc/lobby.grpc.pb.cc")
set(social_grpc_srcs "${inc_path}/grpc/social.grpc.pb.cc")
set(room_grpc_srcs "${inc_path}/grpc/room.grpc.pb.cc")

set(proxy_grpc_hdrs "${inc_path}/grpc/proxy.grpc.pb.h")
set(lobby_grpc_hdrs "${inc_path}/grpc/lobby.grpc.pb.h")
set(social_grpc_hdrs "${inc_path}/grpc/social.grpc.pb.h")
set(room_grpc_hdrs "${inc_path}/grpc/room.grpc.pb.h")


# include generated *.pb.h files
include_directories("${inc_path}/grpc" "${inc_path}/net" "${inc_path}/protocols" "${inc_path}")

# service_grpc_proto
add_library(grpc_server_lib
    ${proxy_grpc_srcs}
    ${proxy_grpc_hdrs}
    ${lobby_grpc_srcs}
    ${lobby_grpc_hdrs}
    ${social_grpc_srcs}
    ${social_grpc_hdrs}
    ${room_grpc_srcs}
    ${room_grpc_hdrs}
    ${proxy_proto_srcs}
    ${proxy_proto_hdrs}
    ${lobby_proto_srcs}
    ${lobby_proto_hdrs}
    ${social_proto_srcs}
    ${social_proto_hdrs}
    ${room_proto_srcs}
    ${room_proto_hdrs}
    ${common_proto_srcs}
    ${common_proto_hdrs})
target_link_libraries(grpc_server_lib
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

foreach(target_ proxyServer simpleBackEnd client)
    add_executable(${target_} "${target_}.cpp")
    target_link_libraries(${target_}
        grpc_server_lib
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
endforeach()
