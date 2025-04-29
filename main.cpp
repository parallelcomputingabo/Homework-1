#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdint>
#include <iomanip>

// Function to perform naive matrix multiplication (C = A * B)
void naive_matmul(float* matrix_C, float* matrix_A, float* matrix_B, uint32_t m, uint32_t n, uint32_t p)
{
    for (std::size_t i = 0; i < m; ++i)
    {
        for (std::size_t j = 0; j < p; ++j)
        {
            float sum = 0.0f;
            for (std::size_t k = 0; k < n; ++k)
            {
                sum += matrix_A[i * n + k] * matrix_B[k * p + j];
            }
            matrix_C[i * p + j] = sum;
        }
    }
}

// Function to validate that the result matches the expected output
bool validate(const std::string& result_path, const std::string& output_path)
{
    std::ifstream result(result_path);
    std::ifstream output(output_path);

    if (!result || !output)
        return false;

    int m1, n1, m2, n2;
    result >> m1 >> n1;
    output >> m2 >> n2;

    if (m1 != m2 || n1 != n2)
        return false;

    constexpr float tolerance = 1e-3f;
    float val1 = 0.0f, val2 = 0.0f;

    while (result >> val1 && output >> val2)
    {
        if (std::fabs(val1 - val2) >= tolerance)
            return false;
    }

    return result.eof() && output.eof();
}

// Main driver function
int main(int argc, char* argv[])
{
    // Check for correct number of arguments
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <test_case_number>" << std::endl;
        return EXIT_FAILURE;
    }

    int test_case = std::atoi(argv[1]);
    if (test_case < 0 || test_case > 9)
    {
        std::cerr << "Test case number must be between 0 and 9." << std::endl;
        return EXIT_FAILURE;
    }

    // Define file paths based on test case
    const std::string base_path = "data/" + std::to_string(test_case) + "/";
    const std::string input0_path = base_path + "input0.raw";
    const std::string input1_path = base_path + "input1.raw";
    const std::string result_path = base_path + "result.raw";
    const std::string output_path = base_path + "output.raw";

    // Open input files
    std::ifstream input0(input0_path);
    std::ifstream input1(input1_path);

    if (!input0.is_open() || !input1.is_open())
    {
        std::cerr << "Failed to open input files." << std::endl;
        return EXIT_FAILURE;
    }

    // Read matrix dimensions
    int m = 0, n = 0, p = 0;
    input0 >> m >> n;
    int n_check = 0;
    input1 >> n_check >> p;

    if (n != n_check)
    {
        std::cerr << "Dimension mismatch between input matrices." << std::endl;
        return EXIT_FAILURE;
    }

    // Allocate memory for matrices
    float* matrix_A = new float[m * n];
    float* matrix_B = new float[n * p];
    float* matrix_C = new float[m * p];

    // Read matrix data from files
    for (std::size_t i = 0; i < static_cast<std::size_t>(m * n); ++i)
        input0 >> matrix_A[i];

    for (std::size_t i = 0; i < static_cast<std::size_t>(n * p); ++i)
        input1 >> matrix_B[i];

    input0.close();
    input1.close();

    // Perform matrix multiplication
    naive_matmul(matrix_C, matrix_A, matrix_B, m, n, p);

    // Write result matrix to output file
    std::ofstream result_file(result_path);
    if (!result_file.is_open())
    {
        std::cerr << "Failed to open result file for writing." << std::endl;
        delete[] matrix_A;
        delete[] matrix_B;
        delete[] matrix_C;
        return EXIT_FAILURE;
    }

    result_file << m << " " << p << "\n";
    for (std::size_t i = 0; i < static_cast<std::size_t>(m); ++i)
    {
        for (std::size_t j = 0; j < static_cast<std::size_t>(p); ++j)
        {
            result_file << std::fixed << std::setprecision(2) << matrix_C[i * p + j];
            if (j != p - 1)
                result_file << " ";
        }
        if (i != m - 1)
            result_file << "\n";
    }

    result_file.close();

    // Validate the result against expected output
    bool valid = validate(result_path, output_path);

    if (valid)
        std::cout << "Test case " << test_case << " passed" << std::endl;
    else
        std::cout << "Test case " << test_case << " failed" << std::endl;

    // Clean up allocated memory
    delete[] matrix_A;
    delete[] matrix_B;
    delete[] matrix_C;

    return EXIT_SUCCESS;
}
