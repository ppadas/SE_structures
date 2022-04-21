cmake_minimum_required(VERSION 2.8)
project(memory_manager)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(test_lib)
set(SRC_LIST_SL ./test_lib/doctest.h test.cpp mm.h)

add_executable(test ${SRC_LIST_SL})
