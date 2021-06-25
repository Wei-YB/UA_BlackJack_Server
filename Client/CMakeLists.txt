cmake_minimum_required(VERSION 3.16.1)
project(client)

set (CMAKE_CXX_STANDARD 11)
include(common.cmake)
include_directories("/root/.local/include")

add_executable(Client Client.cc Rio.cc client.pb.cc)

target_link_libraries(Client   
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
)