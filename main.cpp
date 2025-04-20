#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <iomanip> 


bool read_matrix(const std::string& filename, float*& matrix, uint32_t& rows, uint32_t& cols) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error opening file: " << filename << "\n";
        return false;
    }

    infile >> rows >> cols;
    matrix = new float[rows * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) {
        infile >> matrix[i];
    }

    return true;
}

void write_matrix(const std::string& filename, const float* matrix, uint32_t rows, uint32_t cols) {
    std::ofstream outfile(filename);
    outfile << rows << " " << cols << "\n";
    outfile << std::fixed << std::setprecision(2);  // Set rounding to 2 decimal places
    for (uint32_t i = 0; i < rows * cols; ++i) {
        outfile << matrix[i] << "\n";
    }
}

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0;
            for (uint32_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

bool validate(const std::string& resultFile, const std::string& referenceFile, float epsilon = 1e-5) {
    std::ifstream result(resultFile), reference(referenceFile);
    if (!result || !reference) {
        std::cerr << "Validation failed: could not open files.\n";
        return false;
    }

    uint32_t r1, c1, r2, c2;
    result >> r1 >> c1;
    reference >> r2 >> c2;

    if (r1 != r2 || c1 != c2) {
        std::cerr << "Dimension mismatch during validation.\n";
        return false;
    }

    float a, b;
    for (uint32_t i = 0; i < r1 * c1; ++i) {
        result >> a;
        reference >> b;
        if (std::fabs(a - b) > epsilon) {
            std::cerr << "Mismatch at index " << i << ": " << a << " vs " << b << "\n";
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./matmul <case_number>\n";
        return 1;
    }

    std::string caseDir = "data/" + std::string(argv[1]) + "/";
    std::string fileA = caseDir + "input0.raw";
    std::string fileB = caseDir + "input1.raw";
    std::string fileC = caseDir + "result.raw";
    std::string refOut = caseDir + "output.raw";

    float* A = nullptr;
    float* B = nullptr;
    float* C = nullptr;
    uint32_t m, n, n2, p;

    // Read matrix A
    if (!read_matrix(fileA, A, m, n)) return 1;

    // Read matrix B
    if (!read_matrix(fileB, B, n2, p)) return 1;
    if (n != n2) {
        std::cerr << "Matrix dimension mismatch: A is " << m << "x" << n << ", B is " << n2 << "x" << p << "\n";
        delete[] A;
        delete[] B;
        return 1;
    }

    // Allocate matrix C
    C = new float[m * p];

    // Multiply
    naive_matmul(C, A, B, m, n, p);

    // Write result
    write_matrix(fileC, C, m, p);

    // Validate
    if (validate(fileC, refOut)) {
        std::cout << "Output matches reference.\n";
    } else {
        std::cerr << "Output does NOT match reference.\n";
    }

    // Cleanup
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
