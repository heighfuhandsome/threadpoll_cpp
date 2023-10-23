cmake_minimum_required(VERSION 3.0.0)

project(ThreadPoll C CXX)

set(CMAKE_CXX_STANDARD 17)

#定义变量 静态库输出路径
set(lib_path ${CMAKE_CURRENT_SOURCE_DIR}/lib/static)

#定义变量 静态库名称
set(lib_name thread_poll)

#定义变量 头文件路劲
set(inc_path ${CMAKE_CURRENT_SOURCE_DIR}/inc)

#添加生产静态库的CMakeLists.txt
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

#添加生产example的可执行程序
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/example)