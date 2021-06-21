set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. LogCode)

add_library(myLog STATIC ${LogCode})

macro(add_base_test_target TEST_TARGET)
    add_executable("test_${TEST_TARGET}" "./test/test_${TEST_TARGET}.cpp")
    target_link_libraries("test_${TEST_TARGET}" myLog gtest gtest_main pthread)
endmacro(add_example_target)

add_executable(test test.cpp)
target_link_libraries(test myLog)