cmake_minimum_required(VERSION 3.12)
project(measuring)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(test_lib)
set(SRC_LIST ./test_lib/doctest.h measure.cpp list.h sort.h sort.cpp hash.h mm.h avltree.h)

include_directories(./)

add_executable(measure ${SRC_LIST})