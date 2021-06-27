# Copyright 2018 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# cmake build file for C++ route_guide example.
# Assumes protobuf and gRPC have been installed using cmake.
# See cmake_externalproject/CMakeLists.txt for all-in-one cmake build
# that automatically builds all the dependencies before building route_guide.

cmake_minimum_required(VERSION 3.5.1)

project(Proxy C CXX)

include(common.cmake)


get_filename_component(bj_proto "${PROJECT_SOURCE_DIR}/protos/UA_BlackJack.proto" ABSOLUTE)
get_filename_component(bj_proto_path "${bj_proto}" PATH)

include_directories("${PROJECT_SOURCE_DIR}/include" "/include")


# Now we build the project
get_filename_component(project_inc_path "${bj_proto_path}/../include/ProxyServer.h" PATH)
get_filename_component(project_src_path "${bj_proto_path}/../src/ProxyServer.cpp" PATH)
get_filename_component(project_lib_path "${bj_proto_path}/../lib/libblackjack_grpc.a" PATH)

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

set(backjack_grpc "${project_lib_path}/libbackjack_grpc.a")

# now we generate the server 
file(GLOB CcSources ${project_src_path}/*.cc)
add_executable(proxy "${project_src_path}/main.cpp;${CcSources}")
target_link_libraries(proxy
  ${backjack_grpc}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

