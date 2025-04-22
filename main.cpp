#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


/**
 * @brief Performs naive matrix multiplication: C = A x B
 *
 * Multiplies two matrices A (m x n) and B (n x p) and stores the result in C (m x p).
 *
 * @param C Pointer to the result matrix.
 * @param A Pointer to the first input matrix.
 * @param B Pointer to the second input matrix.
 * @param m Number of rows in matrix A.
 * @param n Number of columns in matrix A and rows in matrix B.
 * @param p Number of columns in matrix B.
 */
void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    for (uint32_t i = 0; i < m; ++i) {
        // For each row of A
        for (uint32_t j = 0; j < p; ++j) {
            // For each column of B
            C[i * p + j] = 0.0f;
            for (uint32_t k = 0; k < n; ++k) {
                // Sum dot product
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
            // Round the value to two decimals if the output has more than two decimals
            if (std::floor(C[i * p + j] * 100) != C[i * p + j] * 100) {
                C[i * p + j] = std::round(C[i * p + j] * 100.0f) / 100.0f;
            }
        }
    }
}

/**
 * @brief Compares two matrix result files for exact match.
 *
 * Successful test will print "Files match exactly!".
 * Unsuccessful test will print the first mismatch found.
 *
 * @param result_file Path to the generated result matrix file.
 * @param output_file Path to the expected output matrix file.
 */
void compare_results(const char* result_file, const char* output_file) {
    std::ifstream result(result_file);
    std::ifstream output(output_file);

    // Check if files opened successfully
    if (!result || !output) {
        std::cerr << "Validation: Error Opening File" << std::endl;
        return;
    }

    int m_result = 0, p_result = 0;
    int m_output = 0, p_output = 0;
    std::string line;

    // Read dimensions from result.raw
    if (std::getline(result, line)) {
        std::stringstream ss(line);
        ss >> m_result >> p_result;
    }

    // Read dimensions from output.raw
    if (std::getline(output, line)) {
        std::stringstream ss(line);
        ss >> m_output >> p_output;
    }

    // Check if dimensions match
    if (m_result != m_output || p_result != p_output) {
        std::cerr << "Validation: Matrix Dimensions Do Not Match!" << std::endl;
        return;
    }

    // Compare matrices element by element
    float value_result = 0.0f, value_output = 0.0f;
    for (int i = 0; i < m_result; ++i) {
        for (int j = 0; j < p_result; ++j) {
            result >> value_result;
            output >> value_output;
            if (value_result != value_output) {
                std::cerr << "Mismatch at (" << i << ", " << j << "): "
                          << value_result << " != " << value_output << std::endl;
                return;
            }
        }
    }
    std::cout << "Files match exactly!" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: matmul <test_number>" << std::endl;
        return 1;
    }

    std::string current_index = argv[1];

    std::cout << "\nRunning Naive Matrix Multiplication for test " << current_index << std::endl;

    // Open and read input files
    std::ifstream file0("data/" + current_index + "/input0.raw");
    std::ifstream file1("data/" + current_index + "/input1.raw");

    // Check if files opened successfully
    if (!file0 || !file1) {
        std::cerr << "Dimension Extraction: Error Opening File" << std::endl;
        return 1;
    }

    int m = 0, n=0, p=0;
    std::string firstLine;

    // Read dimensions from input0.raw
    if (std::getline(file0, firstLine)) {
        std::stringstream ss(firstLine);
        std::string part1, part2;
        ss >> part1 >> part2;
        m = std::stoi(part1);
        n = std::stoi(part2);
    }
    // Read dimensions from input1.raw
    if (std::getline(file1, firstLine)) {
        std::stringstream ss(firstLine);
        std::string part1, part2;
        ss >> part1 >> part2;
        if (std::stoi(part1) != n) {
            std::cerr << "Dimension Extraction: Matrix Dimensions Do Not Match" << std::endl;
        }
        p = std::stoi(part2);
    }
    // Allocate memory for matrices A, B, and C using new or malloc
    float* matrixA = (float*)malloc(m * n * sizeof(float));
    float* matrixB = (float*)malloc(n * p * sizeof(float));
    float* matrixC = (float*)malloc(m * p * sizeof(float));

    std::string line;

    // Read matrix A (row-major order)
    for (int i = 0; i < m; i++) {
        std::getline(file0, line);

        std::stringstream ss(line);
        for (int j = 0; j < n; j++) {
            ss >> matrixA[i * n + j];
        }
    }

    // Read matrix B (row-major order)
    for (int i = 0; i < n; i++) {
        std::getline(file1, line);
        std::stringstream ss(line);
        for (int j = 0; j < p; j++) {
            ss >> matrixB[i * p + j];
        }
    }

    // Step 2: Perform matrix multiplication
    naive_matmul(matrixC, matrixA, matrixB, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    std::ofstream resultFile("data/" + current_index + "/result.raw");
    // Check if file opened successfully
    if (!resultFile) {
        std::cerr << "Error Opening File" << std::endl;
    } else {
        resultFile << m << " " << p << "\n";
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < p; ++j) {
                resultFile << matrixC[i * p + j];
                if (j < p - 1) resultFile << " ";
            }
            resultFile << "\n";
        }
        resultFile.close();
    }

    // Validate the result
    std::string resultFilePath = "data/" + current_index + "/result.raw";
    std::string outputFilePath = "data/" + current_index + "/output.raw";
    compare_results(resultFilePath.c_str(), outputFilePath.c_str());

    // Step 5: Clean up memory
    free(matrixA);
    free(matrixB);
    free(matrixC);

    return 0;
}