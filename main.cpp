#include <iostream>
#include <fstream>

void naive_matmul(float *C, float *A, float *B, uint32_t m, uint32_t n, uint32_t p)
{
    for (uint32_t i = 0; i < m; ++i)
    {
        for (uint32_t j = 0; j < p; ++j)
        {
            double sum = 0.0;
            for (uint32_t k = 0; k < n; ++k)
            {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

bool read_matrix(const std::string &path,
                 float *&M, uint32_t &rows, uint32_t &cols)
{
    std::ifstream in(path);
    if (!in)
        return false;
    in >> rows >> cols;
    M = new float[static_cast<size_t>(rows) * cols];
    for (size_t i = 0; i < (size_t)rows * cols; ++i)
    {
        in >> M[i];
    }
    return true;
}

bool write_matrix(const std::string &path,
                  float *M, uint32_t rows, uint32_t cols)
{
    std::ofstream out(path);
    if (!out)
        return false;
    out << rows << " " << cols << '\n';
    for (size_t i = 0; i < (size_t)rows * cols; ++i)
    {
        out << M[i] << ((i + 1) % cols ? ' ' : '\n');
    }
    return true;
}

bool validate(const std::string &resultFile, const std::string &referenceFile, float epsilon = 1e-5)
{
    std::ifstream result(resultFile), reference(referenceFile);
    if (!result || !reference)
    {
        std::cerr << "Validation failed: could not open files.\n";
        return false;
    }

    uint32_t r1, c1, r2, c2;
    result >> r1 >> c1;
    reference >> r2 >> c2;

    if (r1 != r2 || c1 != c2)
    {
        std::cerr << "Dimension mismatch during validation.\n";
        return false;
    }

    float a, b;
    for (uint32_t i = 0; i < r1 * c1; ++i)
    {
        result >> a;
        reference >> b;
        if (std::fabs(a - b) > epsilon)
        {
            std::cerr << "Mismatch at index " << i << ": " << a << " vs " << b << "\n";
            return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <case_number>\n";
        return EXIT_FAILURE;
    }
    std::string case_id = argv[1];
    std::string base = "data/" + case_id + "/";

    float *A = nullptr, *B = nullptr, *C = nullptr;
    uint32_t m, n, p;
    // Step 1: Open and read input files
    if (!read_matrix(base + "input0.raw", A, m, n) ||
        !read_matrix(base + "input1.raw", B, n, p))
    {
        std::cerr << "Error reading input files" << std::endl;
        return EXIT_FAILURE;
    }

    // Allocate result
    C = new float[(size_t)m * p];

    // Compute
    naive_matmul(C, A, B, m, n, p);

    // Write result
    if (!write_matrix(base + "result.raw", C, m, p))
    {
        std::cerr << "Error writing result.raw" << std::endl;
        return EXIT_FAILURE;
    }

    std::string fileC = base + "result.raw";
    std::string refOut = base + "output.raw";
    // Validate
    if (validate(fileC, refOut))
    {
        std::cout << "Output matches reference.\n";
    }
    else
    {
        std::cerr << "Output does NOT match reference.\n";
    }

    // Cleanup
    delete[] A;
    delete[] B;
    delete[] C;

    return EXIT_SUCCESS;
}