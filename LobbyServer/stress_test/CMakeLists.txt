cmake_minimum_required (VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
project("ProxyClient")

include(../common.cmake)
include_directories(../include)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	 add_definitions(-D_DEBUG)
	 message("DEBUG MODE ENABLE")
endif()

#查找指定目录下的所有源文件( .cc)，然后将结果存进指定变量名：aux_source_directory(<dir> <variable>)
#aux_source_directory(proxy_client.cc  LobbyFile)

#将源文件编译链接生成可执行文件LobbyServer
#add_executable (proxy_client  ${LobbyFile}   )

set(GRPC_CC ../src/UA_BlackJack.grpc.pb.cc ../src/UA_BlackJack.pb.cc)
add_executable (proxy_client  proxy_client.cc ProxyRPCClient.cc ${GRPC_CC})
target_link_libraries(proxy_client ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} pthread spdlog)