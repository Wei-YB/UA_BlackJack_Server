cmake_minimum_required(VERSION 3.16.1)
project(client)

set (CMAKE_CXX_STANDARD 11)
include(common.cmake)
include_directories("/root/.local/include")


# add_executable(test test.cc)
# target_link_libraries(test spdlog)

add_executable(Client Client.cc Rio.cc UA_BlackJack.pb.cc Display.cc)

target_link_libraries(Client   
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
spdlog
)

# add_executable(Server Server-test.cc Rio.cc UA_BlackJack.pb.cc Display.cc)

# target_link_libraries(Server  
# ${_REFLECTION}
# ${_GRPC_GRPCPP}
# ${_PROTOBUF_LIBPROTOBUF}
# spdlog
# )