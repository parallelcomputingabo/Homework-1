cmake_minimum_required(VERSION 3.10)
project(MatMul CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_executable(matmul main.cpp)