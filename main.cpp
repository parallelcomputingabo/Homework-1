#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdint>

void naive_matmul(float *C, float *A, float *B, uint32_t m, uint32_t n, uint32_t p)
{
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; i++)
    {
        for (uint32_t j = 0; j < p; j++)
        {
            C[i * p + j] = 0.0f;
            for (uint32_t k = 0; k < n; k++)
            {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

bool validate_result(const std::string &result_path, const std::string &reference_path)
{
    std::ifstream result_file(result_path, std::ios::binary);
    std::ifstream reference_file(reference_path, std::ios::binary);

    if (!result_file.is_open() || !reference_file.is_open())
    {
        std::cerr << "Failed to open result or reference file for validation" << std::endl;
        return false;
    }

    int result_m, result_n;
    result_file >> result_m >> result_n;

    int ref_m, ref_n;
    reference_file >> ref_m >> ref_n;

    if (result_m != ref_m || result_n != ref_n)
    {
        std::cerr << "Matrix dimensions don't match" << std::endl;
        return false;
    }

    for (int i = 0; i < result_m * result_n; i++)
    {
        float result_val, ref_val;
        result_file >> result_val;
        reference_file >> ref_val;

        if (std::abs(result_val - ref_val) > 1e-5)
        {
            std::cerr << "Values don't match at element " << i << ": "
                      << result_val << " vs " << ref_val << std::endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <test_case_number>" << std::endl;
        return 1;
    }

    std::string test_case = argv[1];
    int test_case_num = std::stoi(test_case);
    if (test_case_num < 0 || test_case_num > 9)
    {
        std::cerr << "Invalid case number. Please provide a number between 0 and 9." << std::endl;
        return 1;
    }


    std::string data_dir = "data/" + test_case + "/";

    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    std::string input0_path = data_dir + "input0.raw";
    std::string input1_path = data_dir + "input1.raw";
    std::string result_path = data_dir + "result.raw";
    std::string output_path = data_dir + "output.raw";

    std::ifstream input0_file(input0_path);
    std::ifstream input1_file(input1_path);

    if (!input0_file.is_open() || !input1_file.is_open())
    {
        std::cerr << "Failed to open input files" << std::endl;
        return 1;
    }

    // TODO: Read dimensions from input0.raw and input1.raw
    int m, n, p; // A is m x n, B is n x p, C is m x p
    input0_file >> m >> n;
    input1_file >> n >> p;

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
    float *A = new float[m * n];
    float *B = new float[n * p];
    float *C = new float[m * p];

    // TODO: Read matrix elements into A and B (row-major order)
    for (int i = 0; i < m * n; i++)
    {
        input0_file >> A[i];
    }

    for (int i = 0; i < n * p; i++)
    {
        input1_file >> B[i];
    }

    input0_file.close();
    input1_file.close();

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    std::ofstream result_file(result_path);
    if (!result_file.is_open())
    {
        std::cerr << "Failed to open result file for writing" << std::endl;
        delete[] A;
        delete[] B;
        delete[] C;
        return 1;
    }

    // Set precision 2 decimal places
    result_file.precision(2);
    result_file << std::fixed;

    // TODO: Write dimensions and elements to result.raw
    result_file << m << " " << p << std::endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            result_file << C[i * p + j];
            if (j < p - 1)
            {
                result_file << " ";
            }
        }
        result_file << std::endl;
    }
    result_file.close();

    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
    bool valid = validate_result(result_path, output_path);
    std::cout << "Validation " << (valid ? "PASSED" : "FAILED") << std::endl;

    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
    delete[] A;
    delete[] B;
    delete[] C;

    return valid ? 0 : 1;
}