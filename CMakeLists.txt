cmake_minimum_required(VERSION 3.17)
project(cds)

set(CMAKE_CXX_STANDARD 17)

include_directories(containers/deque)

add_executable(cds main.cpp containers/deque/deque.h)