#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // TODO: Implement naive matrix multiplication C = A x B
    // Matrix multiplication correctness (30%) done
    // Correct use of row-major indexing (15%) done
    // A is m x n, B is n x p, C is m x p
    // put everything to zero
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            C[i * p + j] = 0.0f;
        }
    }

    // triple nested loop
    for (uint32_t i = 0; i < m; ++i) { // rows of A
        for (uint32_t k = 0; k < n; ++k) { // colm A row B
            for (uint32_t j = 0; j < p; ++j) { // colm of B
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    // File reading and memory allocation 20% done

    int case_num = std::atoi(argv[1]);


    std::stringstream path0, path1, path_result;
    path0 << "data/" << case_num << "/input0.raw";
    path1 << "data/" << case_num << "/input1.raw";
    path_result << "data/" << case_num << "/result.raw";


    std::ifstream input0(path0.str());
    std::ifstream input1(path1.str());
    if (!input0.is_open() || !input1.is_open()) {
        std::cerr << "Failed to open input files!" << std::endl;
        return 1;
    }

    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p;
    input0 >> m >> n;
    input1 >> n >> p;

    //std::cout << "Matrix sizes: m = " << m << ", n = " << n << ", p = " << p << std::endl;

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];


    // TODO: Read matrix elements into A and B (row-major order)
    for (int i = 0; i < m * n; ++i) input0 >> A[i];
    for (int i = 0; i < n * p; ++i) input1 >> B[i];
    input0.close();
    input1.close();

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);
    // Step 3: Write result to result.raw in the same directory as input files
    // TODO: Write dimensions and elements to result.raw
    // File writing and deallocation (Step 3 och 5) 15% done
    std::ofstream output(path_result.str());


    output << std::fixed << std::setprecision(2);
    output << m << " " << p << "\n";
    for (int i = 0; i < m * p; ++i) {
        output << C[i];
        if ((i + 1) % p == 0)
            output << "\n";
        else
            output << " ";
    }


    output.close();

    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    // Successful test comparison with output.raw 15%
    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    free(A);
    free(B);
    free(C);

    return 0;
}
