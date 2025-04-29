#include <iostream>
#include <fstream>
#include <cstdint>

// Naive matrix multiplication C = A x B
// A is m x n, B is n x p, C is m x p
void naive_matmul(double* C, double* A, double* B, uint32_t m, uint32_t n, uint32_t p) {
    // Initialize the result matrix C to zero
    for (uint32_t i = 0; i < m * p; ++i) {
        C[i] = 0.0f;
    }
    
    // Perform the matrix multiplication
    for (uint32_t i = 0; i < m; ++i) { 
        for (uint32_t j = 0; j < p; ++j) { 
            for (uint32_t k = 0; k < n; ++k) { 
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

// Read a matrix from text file (row-major)
double* read_matrix(const std::string& path, uint32_t& rows, uint32_t& cols) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Error: cannot open file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }
    in >> rows >> cols;
    double* mat = new double[static_cast<size_t>(rows) * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) {
        in >> mat[i];
    }
    in.close();
    return mat;
}

// Write a matrix to text file (row-major)
void write_matrix(const std::string& path, const double* mat,
                  uint32_t rows, uint32_t cols) {
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: cannot write to file " << path << std::endl;
        std::exit(EXIT_FAILURE);
    }
    out << rows << " " << cols << '\n';
    for (uint32_t i = 0; i < rows; ++i) {
        for (uint32_t j = 0; j < cols; ++j) {
            out << mat[i * cols + j];
            if (j + 1 < cols) out << ' ';
        }
        out << '\n';
    }
    out.close();
}

// Compare two matrices and return true if they match within epsilon
bool compare_matrices(const double* C, const double* D, int size, double epsilon = 1e-9) {
    for (int i = 0; i < size; i++) {
        if (std::abs(C[i] - D[i]) > epsilon) {
            std::cout << "Mismatch at index " << i << ": " 
                      << "Result = " << C[i] << ", Expected = " << D[i] 
                      << ", Diff = " << std::abs(C[i] - D[i]) << std::endl;
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <case number>" << std::endl;
        return EXIT_FAILURE;
    }

    int case_num = std::atoi(argv[1]);
    std::string base = "data/" + std::to_string(case_num) + "/";
    std::string pathA = base + "input0.raw";
    std::string pathB = base + "input1.raw";
    std::string pathC = base + "result.raw";
    std::string pathD = base + "output.raw";

    // Read dimensions and matrices from input0.raw and input1.raw
    uint32_t m, n_A, n_B, n, p;  // A is m x n, B is n x p, C is m x p
    std::cout << "Reading matrix A from: " << pathA << std::endl;
    double* A = read_matrix(pathA, m, n_A);
    std::cout << "Reading matrix B from: " << pathB << std::endl;
    double* B = read_matrix(pathB, n_B, p);
    std::cout << "Reading matrix D from: " << pathB << std::endl;
    double* D = read_matrix(pathD, n_B, p);
    std::cout << "Result matrix C will be written to: " << pathC << std::endl;

    if (n_A != n_B) {
        std::cerr << "Error: inner dimensions must match (" << n_A << " vs " << n_B << ")" << std::endl;
        return EXIT_FAILURE;
    }

    n = n_A; // or n_B, they are the same

    // Allocate memory for matrix C 
    double* C = new double[m * p];

    // Perform matrix multiplication
    std::cout << "Performing matrix multiplication..." << std::endl;
    naive_matmul(C, A, B, m, n, p);

    // Write result to result.raw in the same directory as input files
    std::cout << "Writing result matrix C to: " << pathC << std::endl;
    write_matrix(pathC, C, m, p);

    // Validate the result
    std::cout << "Validating result..." << std::endl;
    bool isValid = compare_matrices(C, D, m * p);
    if (isValid) {
        std::cout << "\n✓ Validation successful! Result matches expected output." << std::endl;
    } else {
        std::cout << "\n✗ Validation failed! Result does not match expected output." << std::endl;
    }

    // Clean up memory
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] D;

    return EXIT_SUCCESS;
}