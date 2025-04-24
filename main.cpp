#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
#include <iomanip>

// Step 2: Perform matrix multiplication
void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

bool load_matrix(const std::string& path, float*& data, uint32_t& rows, uint32_t& cols) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open " << path << std::endl;
        return false;
    }
    file >> rows >> cols;
    data = new float[rows * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) {
        file >> data[i];
    }
    return true;
}

bool write_matrix(const std::string& path, float* data, uint32_t rows, uint32_t cols) {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "Failed to write to " << path << std::endl;
        return false;
    }
    file << rows << " " << cols << "\n";
    // failing test 6 and 9
    //file << std::fixed << std::setprecision(1); //
    //now failing 9, need more decimal point
    file << std::fixed << std::setprecision(2); //

    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            file << data[i * cols + j] << " ";
        }
        file << "\n";
    }
    return true;
}


// Step 4: validate the result
//single test validation
bool validate_result(const std::string& expected_path, const std::string& result_path) {
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    std::ifstream expected(expected_path), result(result_path);
    if (!expected || !result) {
        std::cerr << "Validation failed: could not open output files.\n";
        return false;
    }

    uint32_t e_rows, e_cols, r_rows, r_cols;
    expected >> e_rows >> e_cols;
    result >> r_rows >> r_cols;

    if (e_rows != r_rows || e_cols != r_cols) {
        std::cerr << "Dimension mismatch in validation.\n";
        return false;
    }

    for (uint32_t i = 0; i < e_rows * e_cols; ++i) {
        float a, b;
        expected >> a;
        result >> b;
        if (std::abs(a - b) > 1e-3) {
            std::cerr << "Mismatch at index " << i << ": " << a << " vs " << b << "\n";
            return false;
        }
    }
    std::cout << "Validation passed!\n";
    return true;
}

int main(int argc, char** argv) {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case_number (0-9)>" << std::endl;
        return 1;
    }

    std::string case_num = argv[1];
    std::string folder = "data/" + case_num + "/";
    std::string inputA = folder + "input0.raw";
    std::string inputB = folder + "input1.raw";
    std::string outputPath = folder + "result.raw";
    std::string expectedPath = folder + "output.raw";

    float* A = nullptr;
    float* B = nullptr;
    float* C = nullptr;
    uint32_t m, n, nB, p;

    // TODO: Read dimensions from input0.raw and input1.raw
    if (!load_matrix(inputA, A, m, n)) return 1;
    if (!load_matrix(inputB, B, nB, p)) return 1;

    if (n != nB) {
        std::cerr << "Matrix dimensions incompatible for multiplication.\n";
        return 1;
    }

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    // (A and B already allocated in load_matrix)
    C = new float[m * p];

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    // TODO: Write dimensions and elements to result.raw
    if (!write_matrix(outputPath, C, m, p)) return 1;

    // Step 4: validate the result
    validate_result(expectedPath, outputPath);

    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
