cmake_minimum_required (VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
project("Lobby")

include(common.cmake)


aux_source_directory(. LobbyFile)

message(${LobbyFile})
add_executable (LobbyServer  ${LobbyFile}   )
target_link_libraries(LobbyServer ${_REFLECTION} ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF} pthread spdlog)