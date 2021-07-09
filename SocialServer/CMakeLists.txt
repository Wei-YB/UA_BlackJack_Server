cmake_minimum_required(VERSION 3.5.1)

# project(social C CXX)

# include("${PROJECT_SOURCE_DIR}/cmake/common.cmake")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

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

add_custom_target(social ALL DEPENDS "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/grpc")

# add_library(hw_grpc_proto
#   ${hw_grpc_srcs}
#   ${hw_grpc_hdrs}
#   ${hw_proto_srcs}
#   ${hw_proto_hdrs})
# target_link_libraries(hw_grpc_proto
#   ${_REFLECTION}
#   ${_GRPC_GRPCPP}
#   ${_PROTOBUF_LIBPROTOBUF})

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc")

add_executable(SocialServer "${SOURCES}")
target_link_libraries(SocialServer
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  spdlog
)


file(GLOB Stress-syn-SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc" 
                         "${CMAKE_CURRENT_SOURCE_DIR}/stress-test/stress-test-syn.cc")
add_executable(Stress-Social-syn "${Stress-syn-SOURCES}")
target_link_libraries(Stress-Social-syn
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  spdlog)

file(GLOB Stress-asyn-SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc" 
                         "${CMAKE_CURRENT_SOURCE_DIR}/stress-test/stress-test-asyn.cc")
add_executable(Stress-Social-asyn "${Stress-asyn-SOURCES}")
target_link_libraries(Stress-Social-asyn
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  spdlog)

file(GLOB Stress-syn-time-SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/grpc/*.cc" 
                         "${CMAKE_CURRENT_SOURCE_DIR}/stress-test/stress-test-syn-time.cc")
add_executable(Stress-Social-syn-time "${Stress-syn-time-SOURCES}")
target_link_libraries(Stress-Social-syn-time
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF}
  spdlog)