cmake_minimum_required(VERSION 3.5.1)

set (CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/grpc" ${CMAKE_CURRENT_BINARY_DIR}/grpc EXCLUDE_FROM_ALL)
message(STATUS "Using gRPC via add_subdirectory.")

# After using add_subdirectory, we can now use the grpc targets directly from
# this build.
set(_PROTOBUF_LIBPROTOBUF libprotobuf)
set(_REFLECTION grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
    message(STATUS "CMAKE_CROSSCOMPILING.")
    find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()
set(_GRPC_GRPCPP grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog" "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/spdlog" EXCLUDE_FROM_ALL)
message(STATUS "Using spdlog via add_subdirectory.")

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED
                PATHS "${CMAKE_CURRENT_BINARY_DIR}/spdlog")
endif()
