# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo3)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17  -Wall -Wno-unused-variable ")
include_directories(
    "${PROJECT_SOURCE_DIR}/inc"
    "${PROJECT_SOURCE_DIR}/3rd/libco",
)
# 指定生成目标 
add_executable(Demo main.cc ${DIR_SRCS})

