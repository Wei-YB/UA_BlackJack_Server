cmake_minimum_required(VERSION 3.5.1)

project(Proxy C CXX)

include(../common.cmake)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

get_filename_component(project_inc_path "${PROJECT_SOURCE_DIR}/../include/ProxyServer.h" PATH)
get_filename_component(project_src_path "${PROJECT_SOURCE_DIR}/../src/ProxyServer.cpp" PATH)
get_filename_component(project_lib_path "${PROJECT_SOURCE_DIR}/../lib/libblackjack_grpc.a" PATH)

include_directories("${project_inc_path}" "${project_inc_path}/grpc" "/include")

message(STATUS "project include path: ${project_inc_path}\
                project srouce path: ${project_src_path}\
                project library path: ${project_lib_path}\
                protobuf files path: ${bj_proto_path}")

# # generate static library of this project
# file(GLOB CcSources ${project_src_path}/*.cc)
# set(LIBRARY_OUTPUT_PATH "${project_lib_path}") 
# add_library(net "${CcSources}")
# target_link_libraries(net
#   ${_REFLECTION}
#   ${_GRPC_GRPCPP}
#   ${_PROTOBUF_LIBPROTOBUF}
# )

set(blackjack_grpc "${project_lib_path}/libblackjack_grpc.a")

# now we generate the server 
file(GLOB CcSources ${project_src_path}/*.cc)
foreach(target_ client simpleBackEnd)
    add_executable(${target_} "${target_}.cpp;${CcSources}")
    target_link_libraries(${target_}
      ${blackjack_grpc}
      ${_REFLECTION}
      ${_GRPC_GRPCPP}
      ${_PROTOBUF_LIBPROTOBUF}
    )
endforeach()