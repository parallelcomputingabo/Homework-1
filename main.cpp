#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <string>
//this is a test if github works

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            C[i * p + j] = 0;
            for (uint32_t k = 0; k < n; ++k) {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case_number>" << std::endl;
        return -1;
    }

    // Get the test case number from the command-line argument
    int test_case = std::stoi(argv[1]);
    std::cout << test_case;
    std::string folder = "data/" + std::to_string(test_case) + "/";

    

    std::ifstream input0(folder + "input0.raw", std::ios::binary);
    std::ifstream input1(folder + "input1.raw", std::ios::binary);

    

    if (!input0 || !input1) {
        std::cerr << "Error opening input files in folder: " << folder << std::endl;
        return -1;
    }

    int m, n, p;
    input0.read(reinterpret_cast<char*>(&m), sizeof(int));
    input0.read(reinterpret_cast<char*>(&n), sizeof(int));
    input1.read(reinterpret_cast<char*>(&n), sizeof(int));
    input1.read(reinterpret_cast<char*>(&p), sizeof(int));

    if (!input0.read(reinterpret_cast<char*>(&m), sizeof(int)) ||
        !input0.read(reinterpret_cast<char*>(&n), sizeof(int))) {
        std::cerr << "Error reading dimensions from input0.raw" << std::endl;
        return -1;
    }
    if (!input0.read(reinterpret_cast<char*>(&n), sizeof(int)) ||
        !input0.read(reinterpret_cast<char*>(&p), sizeof(int))) {
        std::cerr << "Error reading dimensions from input1.raw" << std::endl;
        return -1;
    }

    if (m <= 0 || n <= 0 || p <= 0) {
        std::cerr << "Invalid matrix dimensions: m=" << m << ", n=" << n << ", p=" << p << std::endl;
        return -1;
    }

    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];

    input0.read(reinterpret_cast<char*>(A), m * n * sizeof(float));
    input1.read(reinterpret_cast<char*>(B), n * p * sizeof(float));

    std::cout << "Matrix A dimensions: " << m << " x " << n << std::endl;
    std::cout << "Matrix B dimensions: " << n << " x " << p << std::endl;

    naive_matmul(C, A, B, m, n, p);

    std::ofstream result(folder + "result.raw", std::ios::binary);
    if (!result) {
        std::cerr << "Error opening result file in folder: " << folder << std::endl;
        delete[] A;
        delete[] B;
        delete[] C;
        return -1;
    }

    result.write(reinterpret_cast<char*>(&m), sizeof(int));
    result.write(reinterpret_cast<char*>(&p), sizeof(int));
    result.write(reinterpret_cast<char*>(C), m * p * sizeof(float));

    std::ifstream output(folder + "output.raw", std::ios::binary);
    if (!output) {
        std::cerr << "Error opening output file in folder: " << folder << std::endl;
        delete[] A;
        delete[] B;
        delete[] C;
        return -1;
    }

    float* expected = new float[m * p];
    output.read(reinterpret_cast<char*>(expected), m * p * sizeof(float));

    bool valid = true;
    for (int i = 0; i < m * p; ++i) {
        if (std::abs(C[i] - expected[i]) > 1e-6) {
            valid = false;
            break;
        }
    }

    if (valid) {
        std::cout << "Test case " << test_case << ": Validation successful!" << std::endl;
    } else {
        std::cerr << "Test case " << test_case << ": Validation failed!" << std::endl;
    }

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] expected;

    return 0;
}