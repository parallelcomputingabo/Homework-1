#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>


using namespace std;


bool validate_output(float* A, float* B, int rows, int cols) {
    // Compares two matrices A and B element-wise after rounding to 2 decimal places.
    // In the output.raw files, integers have a dot (e.g., 312.), but results.raw dont (e.g., 312)
    // So outputs are not EXACTLY same but this function ignores this difference by rounding both
    // values to two decimal places before comparing.
    // Returns true if all elements match; otherwise, prints failure and returns false.
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Round both elements to 2 decimal places to avoid floating-point precision errors
            float a = round(A[i * cols + j] * 100.0f) / 100.0f;
            float b = round(B[i * cols + j] * 100.0f) / 100.0f;

            if (a != b) {
                cout << "Validation Failed" << endl;
                return false;
            }
        }
    }
    cout << "Validation Passed" << endl;
    return true;
}

float* load_matrix(const string& path, int& rows, int& cols) {
    // Loads a matrix from a .raw file at the given path.
    // The first line of the file contains two integers: number of rows and columns.
    // The remaining values are the matrix elements in row-major order.
    // Returns a pointer to a dynamically allocated float array containing the matrix.
    // The dimensions are returned via reference parameters `rows` and `cols`.
    ifstream file(path);
    file >> rows >> cols;

    float* data = new float[rows * cols]; // Allocate memory for the matrix
    for (int i = 0; i < rows * cols; ++i) {
        file >> data[i];
    }

    file.close();
    return data;
}



void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // Performs naive matrix multiplication: C = A x B
    // A is an m x n matrix
    // B is an n x p matrix
    // C is the resulting m x p matrix, stored in row-major order
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = roundf(sum * 100.0f) / 100.0f;
        }
    }
}

int main() {
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    int folder_num;
    cout << "Enter folder number (0-9):";
    cin >> folder_num;

    string folder = "data/" + to_string(folder_num) + "/";
    string fileA_path = folder + "input0.raw";
    string fileB_path = folder + "input1.raw";
    string fileC_path = folder + "results.raw";

    cout << "Opening A from: " << fileA_path << endl;
    cout << "Opening B from: " << fileB_path << endl;
    cout << "Writing result to: " << fileC_path << endl;



    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p, dummy;  // A is m x n, B is n x p, C is m x p
    ifstream fileA(fileA_path);
    ifstream fileB(fileB_path);

    fileA >> m >> n;       // A is m x n
    fileB >> dummy >> p;   // B is n x p

    cout << "Dimensions of A: " << m << " x " << n << endl;
    cout << "Dimensions of B: " << dummy << " x " << p << endl;


    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];

    // TODO: Read matrix elements into A and B (row-major order)
    for (int i = 0; i < m * n; ++i) {
        fileA >> A[i];
    }
    fileA.close();

    for (int i = 0; i < n * p; ++i) {
        fileB >> B[i];
    }
    fileB.close();

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files

    // TODO: Write dimensions and elements to result.raw
    ofstream fileC(fileC_path);
    // Write the matrix dimensions (rows and columns) as the first line
    fileC << m << " " << p << endl;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            // Write each value, followed by a space (unless it's the last in the row)
            fileC << C[i * p + j];
            if (j != p - 1) fileC << " ";
        }
        // Write a newline after each row (except the last one)
        if (i != m - 1) fileC << endl;
    }


    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw

    int output_m, output_p;
    float* given_C = load_matrix(folder + "output.raw", output_m, output_p);
    validate_output(C, given_C, m, p);



    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;
    delete[] given_C;

    return 0;
}