cmake_minimum_required(VERSION 3.5.1)

project(Tests C CXX)

include(../common.cmake)
# Proto file
get_filename_component(_dummy "." ABSOLUTE)
get_filename_component(project_path "${_dummy}" PATH)

set(project_inc_path "${project_path}/include")
set(project_src_path "${project_path}/src")
set(project_test_path "${project_path}/test")
set(project_lib_path "${project_path}/libs")

# Include generated *.pb.h files
include_directories("${project_inc_path}"
                    "${project_inc_path}/grpc"
                    "${project_test_path}"
                    "/include")

set(libblackjack_grpc "${project_lib_path}/libblackjack_grpc.a")

file(GLOB CcSources ${project_src_path}/*.cc)
foreach(target_ proxy client)
    add_executable(${target_} "${project_test_path}/${target_}.cpp;${CcSources}")
    target_link_libraries(${target_}
      ${libblackjack_grpc}
      ${_REFLECTION}
      ${_GRPC_GRPCPP}
      ${_PROTOBUF_LIBPROTOBUF}
    )
endforeach()


