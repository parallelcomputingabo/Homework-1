#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>  
using namespace std;  
void naive_matmul(float *C, float *A, float *B, uint32_t m, uint32_t n, uint32_t p)
{
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p

    for (uint32_t i = 0; i < m * p; i++) {
        C[i] = 0.0f;
    }

    // Perform matrix multiplication C = A x B
    for (uint32_t i = 0; i < m; i++) {         // For each row of A
        for (uint32_t j = 0; j < p; j++) {     // For each column of B
            for (uint32_t k = 0; k < n; k++) {  // For each element in row/column
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

bool validate_result(const std::string& result_file, const std::string& expected_file) {
    ifstream res(result_file);
    ifstream exp(expected_file);
    
    if (!res.is_open() || !exp.is_open()) {
        cout << "Error opening validation files!" << std::endl;
        return false;
    }

    int result_row, result_col, exp_row, exp_col;
    res >> result_row >> result_col;
    exp >> exp_row >> exp_col;

    if (result_row != exp_row || result_col != exp_col) {
        cout << "Dimension mismatch in validation!" << std::endl;
        return false;
    }

    float res_value, exp_value;
    bool valid = true;

    for (int i = 0; i < result_row * result_col; ++i) {
        res >> res_value;
        exp >> exp_value;
        if (res_value != exp_value) {
            cout << "Mismatch at element " << i << ": " 
                      << res_value << " vs " << exp_value << std::endl;
            valid = false;
        }
    }

    res.close();
    exp.close();
    return valid;
}

int main(int argc, char* argv[])
{
    // Check if test case number is provided
    if (argc != 2) {
        cerr << "Please provide test case number as argument(0-9): " << endl;
        cerr << argv[0] << " <test_case_number>" << endl;
        return 1;
    }

    int test_case = atoi(argv[1]);
    if (test_case < 0 || test_case > 9) {
        cerr << "Test case number must be between 0 and 9" << endl;
        return 1;
    }

    // Construct file paths
    string base_path = "../data/" + to_string(test_case) + "/";
    string inputA_path = base_path + "input0.raw";
    string inputB_path = base_path + "input1.raw";
    string output_path = base_path + "output.raw";
    string result_path = base_path + "result.raw";

    // Step 1: Open and read input files
    ifstream fileA(inputA_path);
    ifstream fileB(inputB_path);

  
    int m, n, p; // A is m x n, B is n x p, C is m x p

    fileA >> m >> n;

    // Read dimensions from input1.raw (matrix B)
    int n2;
    fileB >> n2 >> p;

    // Check if matrix multiplication is possible 
    if (n != n2)
    {
        std::cerr << "Matrix dimensions mismatch! Cannot multiply "
                  << m << "x" << n << " with " << n2 << "x" << p << std::endl;
        return 1;
    }
    cout << "Matrix A dimensions: " << m << "x" << n << std::endl;
    cout << "Matrix B dimensions: " << n2 << "x" << p << std::endl;

    

    // Allocate memory for matrices A, B, and C
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];

    // Read matrix in row-major order
    for (int i = 0; i < m * n; ++i) fileA >> A[i];
    for (int i = 0; i < n * p; ++i) fileB >> B[i];

    // Close input files
    fileA.close();
    fileB.close();

    // Step 2: Perform matrix multiplication
    
    naive_matmul(C, A, B, m, n, p);


    // Step 3: Write result to result.raw in the same directory as input files
    std::ofstream outFile(result_path);
    if (!outFile.is_open()) {
        cout << "Error opening result file!" << std::endl;
        delete[] A; delete[] B; delete[] C;
        return 1;
    }

    outFile << m << " " << p << "\n";
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            outFile << C[i * p + j];
            if (j < p - 1) outFile << " ";
        }
        outFile << "\n";
    }
    outFile.close();


    // Step 4: validate the result
    if (validate_result(result_path, output_path)) {
        cout << "Validation successful! Result match output file." << std::endl;
    } else {
        cout << "Validation failed! Results don't match." << std::endl;
    }

    // Step 5: Clean up memory
    delete[] A;
    delete[] B;
    delete[] C;
    
    return 0;
}