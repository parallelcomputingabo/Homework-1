#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cmath>

void naive_matmul(float *C, const float *A, const float *B, uint32_t m, uint32_t n, uint32_t p) {
    // Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i * p + j] = 0.00f;
            for (int k = 0; k < n; k++) {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

// Writing the files values into the matrix
void read_file(std::ifstream &input, float *matrix, uint32_t x, uint32_t y) {
    for (int j = 0; j < x; j++) {
        for (int k = 0; k < y; k++) {
            input >> matrix[j * y + k];
        }
    }
}

// Comparing value per value the expected output with the achieved one.
bool compare_matrices(std::ifstream &output, const float *R) {
    float *O;
    uint32_t x, y;

    if (output.is_open()) {
        output >> x >> y;
        O = new float[x * y];
        read_file(output, O, x, y);
    } else {
        exit(1);
    }

    const float EPSILON = 1e-2f;

    for (int j = 0; j < x; j++) {
        for (int k = 0; k < y; k++) {
            if (std::fabs(R[j * y + k] - O[j * y + k]) > EPSILON) {
                delete[] O;
                return false;
            }
        }
    }

    delete[] O;
    return true;
}

int main() {
    for (int i = 0; i < 10; i++) {
        std::string current_path = "../data/" + std::to_string(i) + "/";
        float *A, *B, *C;
        uint32_t m, n, p;  // A is m x n, B is n x p, C is m x p

        // Step 1: Open and read input files
        // Read dimensions from input0.raw and input1.raw available in the data directory
        std::ifstream input0(current_path + "input0.raw");
        std::ifstream input1(current_path + "input1.raw");

        // Read dimensions from input0.raw and input1.raw
        // Allocate memory for matrices A, B, and C using new or malloc
        // Read matrix elements into A and B (row-major order)
        if (input0.is_open()) {
            input0 >> m >> n;
            A = new float[m * n];

            read_file(input0, A, m, n);
        } else {
            exit(1);
        }

        input0.close();

        if (input1.is_open()) {
            input1 >> n >> p;
            B = new float[n * p];

            read_file(input1, B, n, p);
        } else {
            exit(1);
        }

        input1.close();

        C = new float[m * p];

        // Step 2: Perform matrix multiplication
        // Call naive_matmul
        naive_matmul(C, A, B, m, n, p);

        // Step 3: Write result to result.raw in the same directory as input files

        // Write dimensions and elements to result.raw
        std::ofstream result(current_path + "result.raw");

        result << m << " " << p << "\n";
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < p; k++) {
                result << C[j * p + k];

                if (k != p - 1) {
                    result << " ";
                }
            }
            if (j != m - 1) {
                result << "\n";
            }
        }

        // Step 4: validate the result
        // Implement validation function to check if the result is correct by comparing results.raw and output.raw
        std::ifstream output(current_path + "output.raw");
        bool result_comp = compare_matrices(output, C);

        if (result_comp) {
            std::cout << "For matrices with index " << i << " the multiplication was correct!" << std::endl;
        } else {
            std::cout << "For matrices with index " << i << " the multiplication was incorrect!" << std::endl;
        }

        output.close();
        result.close();

        // Step 5: Clean up memory
        // Deallocate A, B, and C
        delete[] A;
        delete[] B;
        delete[] C;
    }

    return 0;
}