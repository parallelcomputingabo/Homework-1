#include <iostream>
#include <fstream>
#include <cassert>  // for assert
#include <cstring>  // for std::memset
#include <cmath>    // for std::fabs

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for( uint32_t i = 0; i < m; i++) {
        for( uint32_t j = 0; j < p; j++) {
            for( uint32_t k = 0; k < n; k++) {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

int main() {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    std::ifstream input0("data/8/input0.raw");
    if (!input0.is_open()) {
        std::cerr << "Error: Could not open input0.raw" << std::endl;
        return 1;
    }

    std::ifstream input1("data/8/input1.raw");
    if (!input0.is_open()) {
        std::cerr << "Error: Could not open input0.raw" << std::endl;
        return 1;
    }

    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p;  // A is m x n, B is n x p, C is m x p
    int rol_input0, col_input0, rol_input1, col_input1;
    input0 >> rol_input0 >> col_input0;
    input1 >> rol_input1 >> col_input1;
    std::cout<< "Matrix A dimensions: " << rol_input0 << " x " << col_input0 << std::endl;
    std::cout<< "Matrix B dimensions: " << rol_input1 << " x " << col_input1 << std::endl;
    assert( col_input0 == rol_input1 && "Matrix dimensions do not match for multiplication");
    m = rol_input0;
    n = col_input0;
    p = col_input1;

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];
    assert(A != nullptr && B != nullptr && C != nullptr && "Memory allocation failed");
    // initilaizing all elements in C to 0 here right after allocation
    std::memset(C, 0, sizeof(float) * m * p);
    // TODO: Read matrix elements into A and B (row-major order)
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            input0 >> A[i * n + j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            input1 >> B[i * p + j];
        }
    }
    // Close input files
    input0.close();
    input1.close();
    // for( int i = 0; i < m * n; i++ ) {
    //     std::cout << A[i] << " ";
    // }

    // for( int i = 0; i < n * p; i++ ) {
    //     std::cout << B[i] << " ";
    // }


    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);
    

    // Step 3: Write result to result.raw in the same directory as input files
    std::ofstream result("data/8/result.raw");
    if (!result.is_open()) {
        std::cerr << "Error: Could not open result.raw" << std::endl;
        return 1;
    }

    // TODO: Write dimensions and elements to result.raw
    result << m << " " << p << std::endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            result << C[i * p + j] << " ";
        }
        result << std::endl;
    }
    result.close();

    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    // I kept forgetting to change the path to this output file for verification, pretty dumb but took a while to figure out
    std::ifstream output("data/8/output.raw");
    if (!output.is_open()) {
        std::cerr << "Error: Could not open output.raw" << std::endl;
        return 1;
    }
    int rol_output, col_output;
    output >> rol_output >> col_output;
    // I tested several values of epsilon, 1e-3 seems to be the error that floating point precision can handle here
    const float epsilon = 1e-3;
    for( int i = 0; i < rol_output; i++ ) {
        for( int j = 0; j < col_output; j++ ) {
            float expected;
            output >> expected;
            // can't just use != for comparing floating point numbers
            if( std::fabs(C[i * col_output + j] - expected) > epsilon ) {
                std::cerr << "Validation failed at (" << i << ", " << j << "): expected " << expected << ", got " << C[i * col_output + j] << std::endl;
            }
        }
    }


    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}