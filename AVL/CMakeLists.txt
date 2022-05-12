cmake_minimum_required(VERSION 3.12)
project(avl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(test_lib)
set(SRC_LIST_SL ./test_lib/doctest.h test.cpp avltree.h)

add_executable(avl_test ${SRC_LIST_SL})