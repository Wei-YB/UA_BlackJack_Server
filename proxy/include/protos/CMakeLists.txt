cmake_minimum_required(VERSION 3.5.1)

project(ProxyProto C CXX)

include(../../common.cmake)

# proto file
get_filename_component(common_proto "common.proto" ABSOLUTE)
get_filename_component(proto_path "${common_proto}" PATH)
set(inc_path "${proto_path}/..")
set(proxy_proto "${proto_path}/proxy.proto")
set(lobby_proto "${proto_path}/lobby.proto")
set(room_proto "${proto_path}/room.proto")
set(social_proto "${proto_path}/social.proto")

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
set(lobby_proto_srcs "${inc_path}/grpc/lobby.grpc.pb.cc")
set(social_proto_srcs "${inc_path}/grpc/social.grpc.pb.cc")
set(room_proto_srcs "${inc_path}/grpc/room.grpc.pb.cc")

set(proxy_grpc_hdrs "${inc_path}/grpc/proxy.grpc.pb.h")
set(lobby_grpc_hdrs "${inc_path}/grpc/lobby.grpc.pb.h")
set(social_grpc_hdrs "${inc_path}/grpc/social.grpc.pb.h")
set(room_grpc_hdrs "${inc_path}/grpc/room.grpc.pb.h")

add_custom_command(
    OUTPUT "${common_proto_srcs}" "${common_proto_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --cpp_out "${inc_path}/grpc"
        -I "${proto_path}"
        "${common_proto}"
    DEPENDS "${common_proto}")

add_custom_command(
    OUTPUT "${proxy_proto_srcs}" "${proxy_proto_hdrs}"
            "${proxy_grpc_srcs}" "${proxy_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${inc_path}/grpc"
        --cpp_out "${inc_path}/grpc"
        -I "${inc_path}/grpc:${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${proxy_proto}"
    DEPENDS "${proxy_proto}")

add_custom_command(
    OUTPUT "${lobby_proto_srcs}" "${lobby_proto_hdrs}"
            "${lobby_grpc_srcs}" "${lobby_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${inc_path}/grpc"
        --cpp_out "${inc_path}/grpc"
        -I "${inc_path}/grpc:${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${lobby_proto}"
    DEPENDS "${lobby_proto}")

add_custom_command(
    OUTPUT "${room_proto_srcs}" "${room_proto_hdrs}"
            "${room_grpc_srcs}" "${room_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${inc_path}/grpc"
        --cpp_out "${inc_path}/grpc"
        -I "${inc_path}/grpc:${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${room_proto}"
    DEPENDS "${room_proto}")

add_custom_command(
    OUTPUT "${social_proto_srcs}" "${social_proto_hdrs}"
            "${social_grpc_srcs}" "${social_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${inc_path}/grpc"
        --cpp_out "${inc_path}/grpc"
        -I "${inc_path}/grpc:${proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${social_proto}"
    DEPENDS "${social_proto}")

# include generated *.pb.h files
# include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

# service_grpc_proto
add_library(service_grpc_proto
    ${proxy_grpc_srcs}
    ${proxy_grpc_hdrs}
    ${lobby_grpc_srcs}
    ${lobby_grpc_hdrs}
    ${social_grpc_srcs}
    ${social_grpc_hdrs}
    ${room_proto_srcs}
    ${room_proto_hdrs}
    ${common_proto_srcs}
    ${common_proto_hdrs})
target_link_libraries(service_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# targets proxy_async_server lobby_async_client room_async_client social_async_client
# foreach(_target
#     proxy_async_server lobby_async_client room_async_client social_async_client)
#     add_executable(${_target} "${_target}.cc")
#     target_link_libraries(${_traget}
#         service_grpc_proto
#         ${_REFLECTION}
#         ${_GRPC_GRPCPP}
#         ${_PROTOBUF_LIBPROTOBUF})
# endforeach()


