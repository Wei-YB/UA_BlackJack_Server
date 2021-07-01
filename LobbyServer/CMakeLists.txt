cmake_minimum_required(VERSION 3.5.1)

# set(CMAKE_CXX_STANDARD 17)
# project("Lobby")

# include(common.cmake)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	 add_definitions(-D_DEBUG)
	 message("DEBUG MODE ENABLE")
endif()

#添加_debug宏
#add_definitions(-D_DEBUG)

#查找指定目录下的所有源文件( .cc)，然后将结果存进指定变量名：aux_source_directory(<dir> <variable>)
aux_source_directory(. LobbyFile)

#将源文件编译链接生成可执行文件LobbyServer
add_executable (LobbyServer  ${LobbyFile}   )
target_link_libraries(LobbyServer ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} pthread spdlog)