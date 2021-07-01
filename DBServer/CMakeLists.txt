cmake_minimum_required (VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
project(DBServer)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_protocol/)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl_cpp/)

# include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common.cmake)


add_library(database RedisService.cc nameFormat.cc Database.cc )
target_link_libraries(database PRIVATE spdlog)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/third-party/acl/lib_acl_cpp/include/acl_cpp")

add_executable(DataBaseService "DBService.cc"  "UA_BlackJack.grpc.pb.cc" "UA_BlackJack.pb.cc" "DBServiceAsynImpl.cc" "RequestParser.cc" )

target_link_libraries(DataBaseService ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} acl_cpp_static protocol_static acl_static database  pthread spdlog)
