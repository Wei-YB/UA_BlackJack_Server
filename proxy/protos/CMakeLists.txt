cmake_minimum_required(VERSION 3.5.1)

project(ProxyProto C CXX)

include(../common.cmake)

# Proto file
get_filename_component(bj_proto "UA_BlackJack.proto" ABSOLUTE)
get_filename_component(bj_proto_path "${bj_proto}" PATH)
set(project_inc_path "${bj_proto_path}/../include/grpc")

# Generated sources
set(bj_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/UA_BlackJack.pb.cc")
set(bj_proto_hdrs "${project_inc_path}/UA_BlackJack.pb.h")
set(bj_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/UA_BlackJack.grpc.pb.cc")
set(bj_grpc_hdrs "${project_inc_path}/UA_BlackJack.grpc.pb.h")
add_custom_command(
      OUTPUT "${bj_proto_hdrs}" "${bj_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${project_inc_path}"
        --cpp_out "${project_inc_path}"
        -I "${bj_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${bj_proto}"
      DEPENDS "${bj_proto}")
  
add_custom_command(
      OUTPUT "${bj_proto_srcs}" "${bj_grpc_srcs}" 
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${bj_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${bj_proto}"
      DEPENDS "${bj_proto}")


# include generated *.pb.h files
# include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("/include" "${project_inc_path}")

# service_grpc_proto
set(LIBRARY_OUTPUT_PATH "${bj_proto_path}/../lib") 
add_library(backjack_grpc
    ${bj_proto_srcs}
    ${bj_proto_hdrs}
    ${bj_grpc_srcs}
    ${bj_grpc_hdrs}
)

target_link_libraries(blackjack_grpc
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)

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


