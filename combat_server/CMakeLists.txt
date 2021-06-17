# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.11)

# 项目信息
project (Demo3)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src DIR_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17  -Wall -Wno-unused-variable -O3 \
                        -pthread  -lstdc++ -Wl,--no-as-needed -ldl")
set(COLIB_DIRECTORY "${PROJECT_SOURCE_DIR}/3rd/libco")
set(EXAMPLE_DIRECTORY "${PROJECT_SOURCE_DIR}/example")
include_directories(
    "${PROJECT_SOURCE_DIR}/inc"
    ${COLIB_DIRECTORY}
)
LINK_DIRECTORIES(${COLIB_DIRECTORY})
link_libraries(colib)
# 指定生成目标 
add_executable(Main main.cc ${DIR_SRCS} )       



# Macro for add example target
macro(add_example_target EXAMPLE_TARGET)
    add_executable("example_${EXAMPLE_TARGET}" "${EXAMPLE_DIRECTORY}/example_${EXAMPLE_TARGET}.cc" ${DIR_SRCS})
endmacro(add_example_target)
# 指定生成目标 
add_example_target(create_delete_room)
add_example_target(game_single_play)