cmake_minimum_required(VERSION 3.5.1)

project(UA_BLACKJACK_SERVER C CXX)

set(CMAKE_CXX_STANDARD 11)

include(cmake/common.cmake)

add_subdirectory(ProxyServer "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(LobbyServer "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(SocialServer "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(game "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(player "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(database "${CMAKE_CURRENT_BINARY_DIR}/proxy")
add_subdirectory(client "${CMAKE_CURRENT_BINARY_DIR}/proxy")

