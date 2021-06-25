cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})

enable_testing()

add_executable(client_test client_test.cc ../Client.cc)
target_link_libraries(client_test gtest ${CMAKE_THREAD_LIBS_INIT})
