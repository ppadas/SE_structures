cmake_minimum_required(VERSION 2.8)
project(list)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_LIST_SL ./test_lib/doctest.h testSL.cpp list.h)
set(SRC_LIST_DL ./test_lib/doctest.h testDL.cpp list.h)

add_executable(SL_test ${SRC_LIST_SL})
add_executable(DL_test ${SRC_LIST_DL})
