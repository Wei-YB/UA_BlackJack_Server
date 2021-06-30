cmake_minimum_required(VERSION 3.5.1)

project(UA_BLACKJACK_SERVER C CXX)

set(CMAKE_CXX_STANDARD 11)

include(cmake/common.cmake)

add_subdirectory(proxy "${CMAKE_CURRENT_BINARY_DIR}/proxy")