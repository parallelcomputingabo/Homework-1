#include <iostream>
#include <cstdlib>    
#include <iomanip>   
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

// Triple‐loop, row‐major, C = A*B
void naive_matmul(const float* A, const float* B, float* C,
                  int M, int K, int N)
{

    int MN = M * N;
    for (int i = 0; i < MN; ++i)
        C[i] = 0.0f;
    // C = A * B
    for (int i = 0; i < M; ++i) {
        for (int k = 0; k < K; ++k) {
            float a = A[i*K + k];
            for (int j = 0; j < N; ++j) {
                C[i*N + j] += a * B[k*N + j];
            }
        }
    }
}

// Read a matrix: first two ints are rows, cols, then row‐major floats
bool read_matrix(const std::string& filename, float*& mat,
                 int& rows, int& cols)
{
    std::ifstream in(filename.c_str());
    if (!in) return false;
    in >> rows >> cols;
    //array allocation
    mat = (float*)std::malloc(rows * cols * sizeof(float));
    if (!mat) return false;
    // Read the matrix data
    for (int i = 0, total = rows*cols; i < total; ++i)
        in >> mat[i];
    return true;
}

// Write rows cols, then row‐major floats
bool write_matrix(const std::string& filename,
                  const float* mat, int rows, int cols)
{
    std::ofstream out(filename.c_str());
    if (!out) return false;
    out << rows << " " << cols << "\n"
        << std::fixed << std::setprecision(2);
    // Write the matrix data
    for (int i = 0, total = rows*cols; i < total; ++i) {
        out << mat[i]
            << ((i % cols == cols-1) ? "\n" : " ");
    }
    return true;
}

// Compare result.raw vs. expected output.raw within eps
bool validate_matrix(const std::string& result_file,
                     const std::string& expected_file,
                     float eps = 1e-4f)
{
    // Read result and expected matrices
    float *R = nullptr, *E = nullptr;
    int r1, c1, r2, c2;

    // Check if the matrices are read successfully
    if (!read_matrix(result_file, R, r1, c1) ||
        !read_matrix(expected_file, E, r2, c2))
    {
        std::cerr << "Validation: failed to read files.\n";
        std::free(R); std::free(E);
        return false;
    }

    // Check if the dimensions match
    if (r1 != r2 || c1 != c2) {
        std::cerr << "Validation: dimension mismatch.\n";
        std::free(R); std::free(E);
        return false;
    }
    bool ok = true;
    // Compare the matrices
    for (int i = 0, total = r1*c1; i < total; ++i) {
        if (std::fabs(R[i] - E[i]) > eps) {
            int row = i / c1, col = i % c1;
            std::cerr << "  Mismatch at [" << row << "," << col
                      << "]: got " << R[i]
                      << ", expected " << E[i] << "\n";
            ok = false;
            break;
        }
    }
    // Free allocated memory
    std::free(R); std::free(E);
    return ok;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <case_id 0-9>\n";
        return -1;
    }
    //The case_id must be a single digit 0–9
    // Check if case_id is a single digit
    std::string case_id = argv[1];
    if (case_id.size() != 1 || case_id[0] < '0' || case_id[0] > '9') {
        std::cerr << "Error: case_id must be a single digit 0–9.\n";
        return -1;
    }

    // Build filenames by string concatenation
    std::string base = "data/" + case_id + "/";
    std::string inA  = base + "input0.raw";
    std::string inB  = base + "input1.raw";
    std::string outR = base + "result.raw";
    std::string exp  = base + "output.raw";

    int m, n, p;
    //Array declarations
    // A is m x n, B is n x p, C is m x p
    float *A = nullptr, *B = nullptr, *C = nullptr;
    // Read input matrices
    if (!read_matrix(inA, A, m, n) ||
        !read_matrix(inB, B, n, p))
    {
        // Error handling: check if matrices are read successfully
        std::cerr << "Error: couldn’t read input matrices.\n";
        std::free(A); std::free(B);
        return EXIT_FAILURE;
    }
    // memory allocation for C
    // Check if the dimensions are valid for matrix multiplication
    C = (float*)std::malloc(m * p * sizeof(float));
    if (!C) {
        std::cerr << "Error: allocation for C failed.\n";
        std::free(A); std::free(B);
        return EXIT_FAILURE;
    }
    // Perform matrix multiplication
    naive_matmul(A, B, C, m, n, p);

    // Write result to file
    // Check if the result is written successfully
    if (!write_matrix(outR, C, m, p))
        std::cerr << "Warning: failed to write " << outR << "\n";

    // Validate result against expected output
    // Check if the validation is successful
    if (validate_matrix(outR, exp))
        std::cout << "Validation passed: result matches expected.\n";
    else
        std::cerr << "Validation FAILED: result differs.\n";
   
    // Free allocated memory
    std::free(A);
    std::free(B);
    std::free(C);
    return 0;
}