#include <iostream>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

// Function to perform matrix multiplication (naive method)
void naive_matmul(float* A, float* B, float* C, int M, int K, int N) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            float sum = 0;
            for (int k = 0; k < K; ++k) {
                sum += A[i * K + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Function to read a matrix from a file
float* read_matrix(const std::string& filename, int& rows, int& cols) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file >> rows >> cols;
    float* mat = new float[rows * cols];

    for (int i = 0; i < rows * cols; ++i) {
        file >> mat[i];
    }

    file.close();
    return mat;
}

// Function to write a matrix to a file
void write_matrix(const std::string& filename, float* mat, int rows, int cols) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file << rows << " " << cols << "\n";
    for (int i = 0; i < rows * cols; ++i) {
        file << mat[i] << " ";
        if ((i + 1) % cols == 0) {
            file << "\n";
        }
    }

    file.close();
}

// Function to process matrices from the folder based on the case number
void process_matrices_from_case(int case_num) {
    // Construct the path to the data folder and case subfolder
    std::string folder = "data/" + std::to_string(case_num);

    // Build the file paths for input0.raw, input1.raw, and reference output
    std::string input0_path = folder + "/input0.raw";
    std::string input1_path = folder + "/input1.raw";
    std::string output_path = folder + "/output.raw";

    // Read matrices A and B
    int M, K, K2, N;
    float* A = read_matrix(input0_path, M, K);
    float* B = read_matrix(input1_path, K2, N);

    // Ensure the dimensions match for multiplication
    if (K != K2) {
        std::cerr << "Matrix dimensions do not match for multiplication in case " << case_num << "\n";
        delete[] A;
        delete[] B;
        return;
    }

    // Allocate memory for the result matrix C
    float* C = new float[M * N];

    // Perform the matrix multiplication
    naive_matmul(A, B, C, M, K, N);

    // Write the result to the result.raw file
    std::string result_path = folder + "/result.raw";
    write_matrix(result_path, C, M, N);

    // Free memory
    delete[] A;
    delete[] B;
    delete[] C;
}

int main(int argc, char* argv[]) {
    // Check if the case number is passed as a command-line argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <case_number>\n";
        return 1;
    }

    // Parse the case number from the command-line argument
    int case_num = std::stoi(argv[1]);

    // Ensure the case number is within the valid range (0-9)
    if (case_num < 0 || case_num > 9) {
        std::cerr << "Invalid case number. Please provide an integer between 0 and 9.\n";
        return 1;
    }

    try {
        // Process the matrices for the specified case number
        process_matrices_from_case(case_num);
        std::cout << "Matrix multiplication for case " << case_num << " completed successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
