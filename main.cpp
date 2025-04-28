#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <cmath>
#include <iomanip>
using namespace std;

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; i++){
        for (uint32_t j = 0; j < p; j++){
            C[i * p + j] = 0.0f;
            for (uint32_t k = 0; k < n; k++){
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
            // rounding it
            C[i * p + j] = std::round(C[i * p + j] * 100.0f) / 100.0f;
        }
    }
}

bool validate_matmul(const std::string& result, const std::string& output){
    // input is file path to result.raw and output.raw
    // read files line for line and compare
    // return true or false
    std::ifstream resultFile(result);
    std::ifstream outputFile(output);

    int res_m, res_p, out_m, out_p;

    resultFile >> res_m >> res_p;
    outputFile >> out_m >> out_p;

    for (uint32_t i = 0; i < static_cast<uint32_t>(res_m); i++){
        for (uint32_t j = 0; j < static_cast<uint32_t>(res_p); j++){
            float result_val, expected_val;
            resultFile >> result_val;
            outputFile >> expected_val;

            //adding a small error tolerance for floating point because sometimes its not exact (421.51, 421.5)
            // const float error_tol = 0.000001f;

            if (result_val != expected_val){
                std::cerr << "error at " << i << ", " << j << ". Expected " << expected_val << ", got " << result_val << std::endl;
                return false;
            }

        }
    }
    std::cout << "Matrix is same, validation complete";
    return true;
}

// needed to get the exact match for output.raw and result.raw
// in output.raw an even value with no decimal is represented with dot after, e.g. "580."
// this will make comparison wrong even if correct.
// could not make it to remove the trailing 0 in the example value 580.50, output says it should be 580.5
void write_dot_after_value(std::ofstream& file, float value) {
    if (std::floor(value) == value) {
        file << std::fixed << std::setprecision(1) << value;
    } else {
        file << std::fixed << std::setprecision(2) << value;
    }
}

// argc = argument count, argv = argument vector
int main(int argc, char* argv[]) {

    if (argc != 2){
        std::cerr << "wrong arguments, 1 allowed";
        return 1;
    }

    std::string testNumber = argv[1];
    std::string testPath0 = "../data/" + testNumber + "/input0.raw";
    std::string testPath1 = "../data/" + testNumber + "/input1.raw";
    std::string resultPath = "../data/" + testNumber + "/result.raw";
    std::string validatePath = "../data/" + testNumber + "/output.raw";



    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    // TODO: Read dimensions from input0.raw and input1.raw

    std::ifstream file0(testPath0);
    std::ifstream file1(testPath1);

    int m, n, p;  // A is m x n, B is n x p, C is m x p

    file0 >> m >> n;
    file1 >> n >> p;

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float* A = new float[m*n];
    float* B = new float[n*p];
    float* C = new float[m*p];

    std::cout << "A dimensions: " << m << " x " << n << std::endl;
    std::cout << "B dimensions: " << n << " x " << p << std::endl;
    std::cout << "C dimensions: " << m << " x " << p << std::endl;

    // TODO: Read matrix elements into A and B (row-major order)

    // A matrix
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            file0 >> A[i* n + j];
        }
    }
    // B matrix
    for (int i = 0; i<n; i++){
        for (int j = 0; j < p; j++){
            file1 >> B[i*p + j];
        }
    }

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C,A,B,m,n,p);

    // Step 3: Write result to result.raw in the same directory as input files
    // TODO: Write dimensions and elements to result.raw
    // trunc removes old first
    std::ofstream result(resultPath, std::ios::out | std::ios::trunc);

    result << m << " " << p << "\n";
    for (uint32_t i = 0; i < static_cast<uint32_t>(m); i++){
        for (uint32_t j = 0; j < static_cast<uint32_t>(p); j++) {
            write_dot_after_value(result, C[i * p + j]);
            if(j < static_cast<uint32_t>(p) - 1){
                result << " ";
            }

        }
        result << "\n";
    }

    result.flush(); 
    result.close();

    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    validate_matmul(resultPath, validatePath);
    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}