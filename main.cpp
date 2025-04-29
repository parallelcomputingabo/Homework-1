#include <iostream>
#include <fstream>

using namespace std;

void naive_matmul(float* C, float* A, float* B, int m, int n, int p) {
    // Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            // Initialize elements in C
            C[i * p + j] = 0;
            for (int k = 0; k < n; ++k) {
                // Access elements by Row-Major indexing, multiply using given formula
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

int main() {
    // Step 1: Open and read input files
    ifstream FileA("data/0/input0.raw");
    // Validate that file is opened correctly
    if (!FileA.is_open()) {
        cerr << "Error opening file";
        return 1;
    }

    ifstream FileB("data/0/input1.raw");
    // Validate that file is opened correctly
    if (!FileB.is_open()) {
        cerr << "Error opening file";
        return 1;
    }

    //Read dimensions from input0.raw and input1.raw
    int m, n, p;  // A is m x n, B is n x p, C is m x p
    FileA >> m >> n;
    FileB >> n >> p;

    //Allocate memory for matrices A, B, and C using given dimensions for each matrix
    float* A = (float*)malloc(m * n * sizeof(float));
    // Validate that memory is allocated correctly
    if (A == NULL) {
        cerr << "Memory allocation failed";
        return 1;
    }

    float* B = (float*)malloc(n * p * sizeof(float));
    if (B == NULL) {
        cerr << "Memory allocation failed";
        return 1;
    }

    float* C = (float*)malloc(m * p * sizeof(float));
    if (C == NULL) {
        cerr << "Memory allocation failed";
        return 1;
    }


    //Read matrix elements into A and B (row-major order), close file after reading
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            FileA >> A[i * n + j];
        }
    }
    FileA.close();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p; ++j) {
            FileB >> B[i * p + j];
        }
    }
    FileB.close();

    // Step 2: Perform matrix multiplication
    // Call naive_matmul
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    // Write dimensions and elements to result.raw
    ofstream result("data/0/result.raw");
    // Validate that file is created correctly
    if (!result) {
        cerr << "Unable to open file";
        exit(1);
    }

    // Write the dimensions of C on the first line
    result << m << " " << p << endl;
    // Iterate C and write each element to result.raw
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            result << C[i * p + j] << " ";
        }
        result << endl;
    }
    // Close file after writing
    result.close();

    // Step 4: validate the result
    // Implement validation function to check if the result is correct by comparing results.raw and output.raw

    // Open both output.raw and result.raw
    ifstream comparison("data/0/output.raw");
    if (!comparison.is_open()) {
        // Validate that file opened correctly
        cerr << "Unable to open file";
        exit(1);
    }

    ifstream res("data/0/result.raw");
    if (!res.is_open()) {
        // Validate that file opened correctly
        cerr << "Unable to open file";
        exit(1);
    }

    float Comp, ResValue;
    // Iterate using the dimensions of C.
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            // Get element from both matrix by index, compare values and throw error if values don't match
            res >> ResValue;
            comparison >> Comp;
            if (ResValue != Comp) {
                cerr << "Value mismatch";
                exit(1);
            }
        }
    }
    // Close both files once comparison is done
    comparison.close();
    res.close();

    // Step 5: Clean up memory
    // Deallocate A, B, and C
    free(A);
    free(B);
    free(C);

    return 0;
}