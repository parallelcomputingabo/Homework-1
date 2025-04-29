#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cstdlib>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // Implementing matrix multiplication
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            C[i * p + j] = 0;
            for (uint32_t k = 0; k < n; ++k) {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

bool process_case(int case_number) {
    // Set the absolute base directory for file locations
    std::string base_dir = "F:/Homework-1-main/data/"; // Update with your correct path
    std::string case_dir = base_dir + std::to_string(case_number) + "/";

    //Print the paths to check
    std::cout << "Trying to open: " << case_dir + "input0.raw" << std::endl;
    std::cout << "Trying to open: " << case_dir + "input1.raw" << std::endl;


    // Open input files (input0.raw, input1.raw) from the case directory
    std::ifstream inputA(case_dir + "input0.raw", std::ios::binary);
    std::ifstream inputB(case_dir + "input1.raw", std::ios::binary);

    if (!inputA.is_open() || !inputB.is_open()) {
        std::cout << "Error opening files for case " << case_number << "." << std::endl;
        return false;
    }

    uint32_t m, n_A, n_B, p;

    // Read matrix dimensions
    inputA >> m >> n_A;
    inputB >> n_B >> p;

    // Check for dimension is matched or not
    if (n_A != n_B) {
        std::cout << "Dimension mismatch for case " << case_number << std::endl;
        return false;
    }

    // Allocate memory for matrices A, B, and C using malloc
    float* A = (float*) malloc(m * n_A * sizeof(float));
    float* B = (float*) malloc(n_B * p * sizeof(float));
    float* C = (float*) malloc(m * p * sizeof(float));

    if (A == nullptr || B == nullptr || C == nullptr) {
        std::cout << "Memory allocation failed!" << std::endl;
        return false;
    }

    // Read matrix A from input file
    for (uint32_t i = 0; i < m * n_A; ++i) {
        inputA >> A[i];
    }

    // Read matrix B from input file
    for (uint32_t i = 0; i < n_B * p; ++i) {
        inputB >> B[i];
    }

    inputA.close();
    inputB.close();

    // calculate matrix multiplication
    naive_matmul(C, A, B, m, n_A, p);

    // Write result to result.raw
    std::ofstream outputC(case_dir + "result.raw", std::ios::binary);
    if (!outputC.is_open()) {
        std::cout << "Error opening result.raw for writing in case " << case_number << "." << std::endl;
        free(A);
        free(B);
        free(C);
        return false;
    }

    // Write matrix dimensions to result.raw
    outputC << m << " " << p << std::endl;

    // Write the matrix C elements to result.raw
    for (uint32_t i = 0; i < m * p; ++i) {
        outputC << C[i] << " ";
    }

    outputC.close();

    // Free dynamically allocated memory
    free(A);
    free(B);
    free(C);

    return true;
}

int main() {
    int passed = 0;

    // verify test cases (0 to 9) are failed or passed
    for (int case_number = 0; case_number <= 9; ++case_number) {
        if (process_case(case_number)) {
            std::cout << "Case " << case_number << " passed." << std::endl;
            passed++;
        } else {
            std::cout << "Case " << case_number << " failed." << std::endl;
        }
    }

    // Print the summary
    std::cout << "\nSummary: " << passed << "/10 test cases passed." << std::endl;

    return 0;
}
