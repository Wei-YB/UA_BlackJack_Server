
cmake_minimum_required (VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(DBServer)


add_library(database RedisService.cc nameFormat.cc)

add_executable(demo RedisTest.cc )
target_link_libraries(demo database _acl_cpp _protocol _acl pthread)

macro(add_test_target TEST_TARGET)
    add_executable("test_${TEST_TARGET}" "./test/test_${TEST_TARGET}.cc")
    target_link_libraries("test_${TEST_TARGET}" database _acl_cpp _protocol _acl gtest gtest_main pthread)
endmacro(add_test_target)

add_test_target(nameFormat)
add_test_target(RedisService)