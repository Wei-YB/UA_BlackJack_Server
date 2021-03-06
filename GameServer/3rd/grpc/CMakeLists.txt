cmake_minimum_required(VERSION 3.5.1)

# project(Demo C CXX)

# include(common.cmake)

get_filename_component(hw_proto "UA_BlackJack.proto" ABSOLUTE)  # demo dependent
get_filename_component(hw_proto_path "${hw_proto}" PATH)

set(hw_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/grpc_cpp/UA_BlackJack.pb.cc") # demo dependent
set(hw_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/grpc_h/UA_BlackJack.pb.h")  # demo dependent
set(hw_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/grpc_cpp/UA_BlackJack.grpc.pb.cc") # demo dependent
set(hw_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/grpc_h/UA_BlackJack.grpc.pb.h")  # demo dependent
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}/grpc_cpp"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}/grpc_cpp"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/grpc_h")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}") 
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})