cmake_minimum_required(VERSION 3.10)
project(naive_matmul)

set(CMAKE_CXX_STANDARD 17)

# Set executable output path to build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copy data directory to build directory 
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

add_executable(naive_matmul src/main.cpp)