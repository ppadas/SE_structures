cmake_minimum_required(VERSION 3.0)
project(hash)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(test_lib)
set(SRC_LIST_SL ./test_lib/doctest.h hash_test.cpp hash.h)

add_executable(hash_test ${SRC_LIST_SL})