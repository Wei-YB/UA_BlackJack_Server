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

# Proto file
get_filename_component(bj_proto "protos/UA_BlackJack.proto" ABSOLUTE)
get_filename_component(bj_proto_path "${bj_proto}" PATH)
get_filename_component(project_inc_path "${bj_proto_path}/../include/ProxyServer.h" PATH)
get_filename_component(project_src_path "${bj_proto_path}/../src/ProxyServer.cpp" PATH)
get_filename_component(project_lib_path "${bj_proto_path}/../libs/libblackjack_grpc.a" PATH)

message(STATUS "project include path: ${project_inc_path}\
                project srouce path: ${project_src_path}\
                project library path: ${project_lib_path}\
                protobuf files path: ${bj_proto_path}")

# Include generated *.pb.h files
include_directories("${project_inc_path}"
                    "${project_inc_path}/grpc"
                    "/include")

set(libblackjack_grpc "${project_lib_path}/libblackjack_grpc.a")

# Targets route_guide_(client|server)
file(GLOB CcSources ${project_src_path}/*.cc)
add_library(net "${CcSources}")
target_link_libraries(net
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)
# aux_source_directory("${project_src_path}" CppSources)

add_executable(proxy "${project_src_path}/main.cpp;${CcSources}")
target_link_libraries(proxy
  ${libblackjack_grpc}
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
)

