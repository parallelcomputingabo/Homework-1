#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

void naive_matmul(float *C, const float *A, const float *B, int m, int n, int p)
{
    // Initialize result matrix C to zero
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            C[i * p + j] = 0.0f;
        }
    }
    // Triple nested loop for naive matrix multiplication
    for (int i = 0; i < m; ++i)
    {
        for (int k = 0; k < n; ++k)
        {
            float a_val = A[i * n + k];
            for (int j = 0; j < p; ++j)
            {
                C[i * p + j] += a_val * B[k * p + j];
            }
        }
    }
}

bool validate_result(const std::string &ref_path, float *C, int m, int p)
{
    std::ifstream ref_file(ref_path);
    if (!ref_file)
    {
        std::cerr << "Error: Cannot open reference file " << ref_path << std::endl;
        return false;
    }
    int rm, rp;
    ref_file >> rm >> rp;
    if (rm != m || rp != p)
    {
        std::cerr << "Dimension mismatch: reference is " << rm << "x" << rp
                  << ", but computed is " << m << "x" << p << std::endl;
        return false;
    }
    bool ok = true;
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            float ref_val;
            ref_file >> ref_val;
            float comp_val = C[i * p + j];
            // Compare at cent precision: round both to nearest hundredth
            int ref_scaled = static_cast<int>(std::round(ref_val * 100.0f));
            int comp_scaled = static_cast<int>(std::round(comp_val * 100.0f));
            if (ref_scaled != comp_scaled)
            {
                std::cerr << "Mismatch at (" << i << "," << j << "): expected "
                          << ref_val << ", got " << comp_val << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <case_number (0-9)>" << std::endl;
        return 1;
    }
    std::string case_id = argv[1];
    std::string folder = std::string("data/") + case_id + "/";
    std::string inputA_path = folder + "input0.raw";
    std::string inputB_path = folder + "input1.raw";
    std::string result_path = folder + "result.raw";
    std::string ref_path = folder + "output.raw";

    // Read dimensions and data of A
    std::ifstream fileA(inputA_path);
    if (!fileA)
    {
        std::cerr << "Error: Cannot open " << inputA_path << std::endl;
        return 1;
    }
    int m, n, p;
    fileA >> m >> n;
    float *A = new float[m * n];
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            fileA >> A[i * n + j];
        }
    }
    fileA.close();

    // Read dimensions and data of B
    std::ifstream fileB(inputB_path);
    if (!fileB)
    {
        std::cerr << "Error: Cannot open " << inputB_path << std::endl;
        delete[] A;
        return 1;
    }
    int bn, bp;
    fileB >> bn >> bp;
    if (bn != n)
    {
        std::cerr << "Inner dimension mismatch: A is " << m << "x" << n
                  << ", B is " << bn << "x" << bp << std::endl;
        delete[] A;
        return 1;
    }
    p = bp;
    float *B = new float[n * p];
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            fileB >> B[i * p + j];
        }
    }
    fileB.close();

    // Allocate C and perform multiplication
    float *C = new float[m * p];
    naive_matmul(C, A, B, m, n, p);

    // Write result matrix to file with dynamic precision
    std::ofstream out(result_path);
    if (!out)
    {
        std::cerr << "Error: Cannot open " << result_path << " for writing" << std::endl;
        delete[] A;
        delete[] B;
        delete[] C;
        return 1;
    }
    out << m << " " << p << "\n";
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < p; ++j)
        {
            float val = C[i * p + j];
            int scaled = static_cast<int>(std::round(val * 100.0f));
            std::ostringstream oss;
            oss << std::fixed;
            if (scaled % 100 == 0)
            {
                // Whole number
                oss << std::setprecision(0) << val;
            }
            else if (scaled % 10 == 0)
            {
                // One decimal place
                oss << std::setprecision(1) << val;
            }
            else
            {
                // Two decimal places
                oss << std::setprecision(2) << val;
            }
            out << oss.str();
            if (j + 1 < p)
                out << " ";
        }
        out << "\n";
    }
    out.close();

    // Validate result
    bool valid = validate_result(ref_path, C, m, p);
    if (valid)
    {
        std::cout << "Validation passed: result matches reference." << std::endl;
    }
    else
    {
        std::cerr << "Validation failed." << std::endl;
    }

    // Clean up
    delete[] A;
    delete[] B;
    delete[] C;

    return valid ? 0 : 1;
}
