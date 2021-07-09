cmake_minimum_required(VERSION 3.5.1)

# add_subdirectory(protos)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
cmake_policy(SET CMP0069 NEW)
include(CheckIPOSupported)
check_ipo_supported(RESULT _IsIPOSupported)
  if(_IsIPOSupported)
  message(STATUS "Turn on INTERPROCEDURAL_OPTIMIZATION")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

get_filename_component(bj_proto "${PROJECT_SOURCE_DIR}/protos/UA_BlackJack.proto" ABSOLUTE)
get_filename_component(bj_proto_path "${bj_proto}" PATH)

get_filename_component(proxy_inc_path "${CMAKE_CURRENT_SOURCE_DIR}/include/ProxyServer.h" PATH)
get_filename_component(proxy_src_path "${CMAKE_CURRENT_SOURCE_DIR}/src/ProxyServer.cpp" PATH)

set(bj_proto_srcs "${proxy_inc_path}/grpc/UA_BlackJack.pb.cc")
set(bj_proto_hdrs "${proxy_inc_path}/grpc/UA_BlackJack.pb.h" )
set(bj_grpc_srcs "${proxy_inc_path}/grpc/UA_BlackJack.grpc.pb.cc" )
set(bj_grpc_hdrs "${proxy_inc_path}/grpc/UA_BlackJack.grpc.pb.h")

add_custom_command(
      OUTPUT "${bj_proto_srcs}" "${bj_proto_hdrs}" "${bj_grpc_srcs}" "${bj_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${proxy_inc_path}/grpc"
        --cpp_out "${proxy_inc_path}/grpc"
        -I "${bj_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${bj_proto}"
      DEPENDS "${bj_proto}")
  
# this library is produced by building protos
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib") 
add_library(blackjack_grpc
    ${bj_proto_srcs}
    ${bj_proto_hdrs}
    ${bj_grpc_srcs}
    ${bj_grpc_hdrs}
)
target_include_directories(blackjack_grpc PRIVATE "${proxy_inc_path}/grpc")
target_link_libraries(blackjack_grpc
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)

# now we generate the proxy server 
file(GLOB CcSources ${proxy_src_path}/*.cc)
add_executable(proxy "${proxy_src_path}/main.cpp;${CcSources}")
target_include_directories(proxy PRIVATE "${proxy_inc_path}" "${proxy_inc_path}/grpc")

target_link_libraries(proxy
  spdlog::spdlog
  blackjack_grpc
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

if (ENABLE_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test ${PROJECT_BINARY_DIR}/proxy/test)
endif()
