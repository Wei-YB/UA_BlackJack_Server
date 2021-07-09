cmake_minimum_required (VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 17)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl_cpp/include/acl_cpp")

get_filename_component(hw_proto "${PROJECT_SOURCE_DIR}/protos/UA_BlackJack.proto" ABSOLUTE)  # demo dependent
get_filename_component(hw_proto_path "${hw_proto}" PATH)

set(hw_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/grpc/UA_BlackJack.pb.cc") # demo dependent
set(hw_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/grpc/UA_BlackJack.pb.h")  # demo dependent
set(hw_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/grpc/UA_BlackJack.grpc.pb.cc") # demo dependent
set(hw_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/grpc/UA_BlackJack.grpc.pb.h")  # demo dependent
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}/grpc"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}/grpc"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/grpc")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_protocol/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl_cpp/)

# include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)

file(GLOB GRPC_SOURCE  "${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc")
file(GLOB DATABASE_SCOURE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc" 
                 )


add_executable(DatabaseServer "${GRPC_SOURCE}" "${DATABASE_SCOURE}")

target_link_libraries(DatabaseServer 
${_REFLECTION} 
${_GRPC_GRPCPP} 
${_PROTOBUF_LIBPROTOBUF} 
acl_cpp_static 
protocol_static 
acl_static 
pthread 
spdlog)

add_executable(DatabaseBench "./Bench/bench.cc" ${GRPC_SOURCE})
target_link_libraries(DatabaseBench 
${_REFLECTION} 
${_GRPC_GRPCPP} 
${_PROTOBUF_LIBPROTOBUF} 
pthread 
spdlog)

