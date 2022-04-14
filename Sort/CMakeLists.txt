cmake_minimum_required(VERSION 2.8)
project(sort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_LIST test.cpp sort.h sort.cpp)

include_directories(./)

add_executable(sort_test ${SRC_LIST})