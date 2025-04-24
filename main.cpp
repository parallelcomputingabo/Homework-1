#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <iomanip>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < p; j++) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

// Run by editing run configuration with program arguments set to
// anything from 0-9
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case_number>" << std::endl;
        return 1;
    }

    int test_case = std::atoi(argv[1]);
    if (test_case < 0 || test_case > 9) {
        std::cerr << "Test case number must be between 0 and 9" << std::endl;
        return 1;
    }

    // Define all the paths used
    std::string base_path = "../data/" + std::to_string(test_case) + "/";
    std::string input0_path = base_path + "input0.raw";
    std::string input1_path = base_path + "input1.raw";
    std::string result_path = base_path + "result.raw";
    std::string output_path = base_path + "output.raw";

    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    std::ifstream input0_file(input0_path);
    std::ifstream input1_file(input1_path);

    if (!input0_file.is_open() || !input1_file.is_open()) {
        std::cerr << "Failed to open input files" << std::endl;
        return 1;
    }

    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p; // A is m x n, B is n x p, C is x p
    input0_file >> m >> n;
    input1_file >> n >> p;

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];

    // TODO: Read matrix elements into A and B (row-major order)
    for (int i = 0; i < m * n; i++) {
        input0_file >> A[i];
    }
    for (int i = 0; i < n * p; i++) {
        input1_file >> B[i];
    }

    input0_file.close();
    input1_file.close();

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files

    // TODO: Write dimensions and elements to result.raw
    std::ofstream result_file(result_path);
    if (!result_file.is_open()) {
        std::cerr << "Failed to open result file for writing" << std::endl;
        delete[] A;
        delete[] B;
        delete[] C;
        return 1;
    }

    // Write dimensions first
    result_file << m << " " << p << "\n";


    // Write matrix elements
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            result_file << std::fixed << std::setprecision(2) << C[i * p + j];
            if (j < p - 1) {
                result_file << " ";
            }
        }
        if (i < m - 1) {
            result_file << "\n";
        }
    }

    result_file.close();



    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    std::ifstream result(result_path);
    std::ifstream output(output_path);
    bool success = true;
    if (!result.is_open() || !output.is_open()) {
        success = false;
    }
    // Compare dimensions first
    int m1, n1, m2, n2;
    result >> m1 >> n1;
    output >> m2 >> n2;

    if (m1 != m2 || n1 != n2) {
        success = false;
    }

    // Compare each element
    float tolerance = 1e-5f;
    float val1, val2;
    while (result >> val1 && output >> val2) {
        if (std::abs(val1 - val2) > tolerance) {
            success = false;
            break;
        }
    }

    // Check if both files reached EOF at the same time
    if (result.eof() != output.eof()) {
        success = false;
    }
    if (success) {
        std::cout << "Test case " << test_case << " passed" << std::endl;
    } else {
        std::cout << "Test case " << test_case << " failed" << std::endl;
    }

    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}