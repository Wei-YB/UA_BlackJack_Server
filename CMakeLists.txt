cmake_minimum_required(VERSION 3.5.1)

project(UA_BLACKJACK_SERVER C CXX)

set(CMAKE_CXX_STANDARD 11)

include(cmake/common.cmake)

add_subdirectory(ProxyServer "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(LobbyServer "${CMAKE_CURRENT_BINARY_DIR}/lobby")
add_subdirectory(SocialServer "${CMAKE_CURRENT_BINARY_DIR}/social")
add_subdirectory(GameServer "${CMAKE_CURRENT_BINARY_DIR}/game")
add_subdirectory(PlayerServer "${CMAKE_CURRENT_BINARY_DIR}/player")
# add_subdirectory(DBServer "${CMAKE_CURRENT_BINARY_DIR}/database")
add_subdirectory(Client "${CMAKE_CURRENT_BINARY_DIR}/client")

