# cmake_minimum_required(VERSION 3.16.1)
# project(client)

# set (CMAKE_CXX_STANDARD 11)
# include(common.cmake)

# add_executable(Client Client.cc Rio.cc UA_BlackJack.pb.cc Display.cc)

# target_link_libraries(Client   
# ${_REFLECTION}
# ${_GRPC_GRPCPP}
# ${_PROTOBUF_LIBPROTOBUF}
# spdlog
# )



cmake_minimum_required(VERSION 3.5.1)
# project(client)

# set (CMAKE_CXX_STANDARD 11)
# include(common.cmake)

# SET(CMAKE_BUILD_TYPE "Debug")
# SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g")

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

add_executable(Client ${SOURCES})
target_link_libraries(Client   
${_REFLECTION}
${_GRPC_GRPCPP}
${_PROTOBUF_LIBPROTOBUF}
spdlog
)

