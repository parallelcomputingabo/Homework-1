#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint> 

void multiply_matrices(double* result, double* matrixA, double* matrixB, uint32_t rowsA, uint32_t colsA, uint32_t colsB) {
    for (uint32_t i = 0; i < rowsA; ++i) {
        for (uint32_t j = 0; j < colsB; ++j) {
            result[i * colsB + j] = 0.0;
            for (uint32_t k = 0; k < colsA; ++k) {
                result[i * colsB + j] += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
        }
    }
}

bool load_matrix(const std::string& file_path, double*& matrix, uint32_t& rows, uint32_t& cols) {
    std::ifstream file(file_path);
    if (!file) {
        std::cerr << "Unable to open file: " << file_path << "\n";
        return false;
    }
    file >> rows >> cols;
    matrix = new double[rows * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) {
        file >> matrix[i];
    }
    return true;
}

bool save_matrix(const std::string& file_path, double* matrix, uint32_t rows, uint32_t cols) {
    std::ofstream file(file_path);
    if (!file) {
        std::cerr << "Unable to write file: " << file_path << "\n";
        return false;
    }
    file << rows << " " << cols << "\n";
    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            file << matrix[i * cols + j];
            if (j != cols - 1) file << " ";
        }
        file << "\n";
    }
    return true;
}

bool compare_matrices(const std::string& expected_path, const std::string& result_path, double tolerance = 1e-6, int max_prints = 10) {
    std::ifstream expected_file(expected_path), result_file(result_path);
    if (!expected_file || !result_file) return false;

    uint32_t rows1, cols1, rows2, cols2;
    expected_file >> rows1 >> cols1;
    result_file >> rows2 >> cols2;
    if (rows1 != rows2 || cols1 != cols2) return false;

    double expected_value, result_value;
    int print_count = 0;
    bool failed_comparison = false;

    for (uint32_t i = 0; i < rows1 * cols1; ++i) {
        expected_file >> expected_value;
        result_file >> result_value;

        // Apply smaller tolerance for comparison
        if (std::abs(expected_value - result_value) > tolerance) {
            if (print_count < max_prints) {
                std::cout << "Mismatch at index " << i << ": Expected " << expected_value << ", Got " << result_value << "\n";
                print_count++;
            }
            failed_comparison = true;
        }
    }

    if (failed_comparison && print_count >= max_prints) {
        std::cout << "More mismatches found. Showing the first " << max_prints << " mismatches only.\n";
    }

    return !failed_comparison;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./matmul <case_number>\n";
        return 1;
    }

    std::string case_dir = "data/" + std::string(argv[1]);
    std::string matrixA_path = case_dir + "/input0.raw";
    std::string matrixB_path = case_dir + "/input1.raw";
    std::string reference_output_path = case_dir + "/output.raw";  
    std::string result_output_path = case_dir + "/result.raw";      

    double *A = nullptr, *B = nullptr, *C = nullptr;
    uint32_t rowsA, colsA, rowsB, colsB;

    if (!load_matrix(matrixA_path, A, rowsA, colsA)) return 1;
    if (!load_matrix(matrixB_path, B, rowsB, colsB)) return 1;

    if (colsA != rowsB) {
        std::cerr << "Matrix size mismatch: A (" << rowsA << "x" << colsA << ") and B (" << rowsB << "x" << colsB << ")\n";
        return 1;
    }

    C = new double[rowsA * colsB];
    multiply_matrices(C, A, B, rowsA, colsA, colsB);

    if (!save_matrix(result_output_path, C, rowsA, colsB)) return 1;

    if (compare_matrices(reference_output_path, result_output_path)) {
        std::cout << "Test case " << argv[1] << ": Passed.\n";
    } else {
        std::cout << "Test case " << argv[1] << ": Failed.\n";
    }

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}
