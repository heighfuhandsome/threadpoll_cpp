cmake_minimum_required(VERSION 3.0.0)

include_directories(${inc_path})

file(GLOB LIB_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(LIBRARY_OUTPUT_PATH ${lib_path})

add_library(${lib_name} STATIC ${LIB_LIST})

