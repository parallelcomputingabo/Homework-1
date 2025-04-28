#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void naive_matmul(float *C, float *A, float *B, uint32_t m, uint32_t n, uint32_t p)
{
    // TODO: Implement naive matrix multiplication C = A x B
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            float sum = 0;
            for (int k = 0; k < n; ++k)
            {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

float *read_matrix(const std::string &filename, int &rows, int &cols)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    file >> rows >> cols;
    float *mat = new float[rows * cols];

    for (int i = 0; i < rows * cols; ++i)
    {
        file >> mat[i];
    }

    file.close();
    return mat;
}

void write_matrix(const std::string &filename, float *mat, int rows, int cols)
{
    std::ofstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    file << rows << " " << cols << "\n";
    for (int i = 0; i < rows * cols; ++i)
    {
        file << mat[i] << " ";
        if ((i + 1) % cols == 0)
        {
            file << "\n";
        }
    }

    file.close();
}

bool validate_matrices(float *mat1, float *mat2, int rows, int cols)
{
    const float EPSILON = 1e-4f;

    for (int i = 0; i < rows * cols; ++i)
    {
        if (std::abs(mat1[i] - mat2[i]) > EPSILON)
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    // Step 1: Open and read input files
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <case_number>\n";
        return 1;
    }
    int case_num = std::stoi(argv[1]);

    if (case_num < 0 || case_num > 9)
    {
        std::cerr << "Invalid case number. Please provide an integer between 0 and 9.\n";
        return 1;
    }

    std::string folder = "data/" + std::to_string(case_num);

    std::string input0_path = folder + "/input0.raw";
    std::string input1_path = folder + "/input1.raw";
    std::string output_path = folder + "/output.raw";

    int m, n, p; // A is m x n, B is n x p, C is m x p
    float *A = read_matrix(input0_path, m, n);
    float *B = read_matrix(input1_path, n, p);

    float *C = new float[m * p];

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);
    // Step 3: Write result to result.raw in the same directory as input files
    std::string result_path = folder + "/result.raw";
    write_matrix(result_path, C, m, p);
    // TODO: Write dimensions and elements to result.raw
    result_path = folder + "/result.raw";
    output_path = folder + "/output.raw";

    int res_rows, res_cols, out_rows, out_cols;
    float *result = read_matrix(result_path, res_rows, res_cols);
    float *output = read_matrix(output_path, out_rows, out_cols);
    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    if (res_rows != out_rows || res_cols != out_cols)
    {
        std::cerr << "Dimension mismatch between result and output\n";
    }
    else if (validate_matrices(result, output, res_rows, res_cols))
    {
        std::cout << "Validation successful: result matches output\n";
    }
    else
    {
        std::cout << "Validation failed: result does not match output\n";
    }

    delete[] result;
    delete[] output;

    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}