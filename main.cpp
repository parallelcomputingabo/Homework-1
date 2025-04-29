#include <iostream>
#include <fstream>
#include <cstdint>

// Naive matrix multiplication C = A x B
// A is m x n, B is n x p, C is m x p
void naive_matmul(double* C, double* A, double* B, uint32_t m, uint32_t n, uint32_t p) {
    // Initialize the result matrix C to zero
    for (uint32_t i = 0; i < m * p; ++i) {
        C[i] = 0.0f;
    }
    
    // Perform the matrix multiplication
    for (uint32_t i = 0; i < m; ++i) { 
        for (uint32_t j = 0; j < p; ++j) { 
            for (uint32_t k = 0; k < n; ++k) { 
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

// Read a matrix from text file (row-major)
double* read_matrix(const std::string& path, uint32_t& rows, uint32_t& cols) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: cannot open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }
    in >> rows >> cols;
    double* mat = new double[static_cast<size_t>(rows) * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) {
        in >> mat[i];
    }
    in.close();
    return mat;
}

// Write a matrix to text file (row-major)
void write_matrix(const std::string& path, const double* mat,
                  uint32_t rows, uint32_t cols) {
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: cannot write to file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }
    out << rows << " " << cols << '\n';
    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            out << mat[i * cols + j];
            if (j + 1 < cols) out << ' ';
        }
        out << '\n';
    }
    out.close();
}

int main() {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory


    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p;  // A is m x n, B is n x p, C is m x p

    // TODO: Allocate memory for matrices A, B, and C using new or malloc

    // TODO: Read matrix elements into A and B (row-major order)

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul

    // Step 3: Write result to result.raw in the same directory as input files

    // TODO: Write dimensions and elements to result.raw

    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw


    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C

    return 0;
}