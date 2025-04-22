cmake_minimum_required(VERSION 3.10)
project(MatMul CXX)
set(CMAKE_CXX_STANDARD 11)
add_executable(matmul main.cpp)

# Enable testing
enable_testing()

# Create tests for each directory 0-9
foreach(i RANGE 0 9)
    # Convert i to a string
    string(STRIP "${i}" dir_num)

    # Set the result and output file paths dynamically based on i
    set(current_index "${dir_num}")

    # Add a test for each directory
    add_test(NAME Test${dir_num}
            COMMAND matmul "${current_index}")
    set_tests_properties(Test${dir_num} PROPERTIES
            PASS_REGULAR_EXPRESSION "Files match exactly!"
    )
endforeach()