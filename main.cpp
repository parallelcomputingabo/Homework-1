#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iterator>
#include <cstdint>

float* load_matrix(const std::string& path,
                   uint32_t& rows, uint32_t& cols)
{
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open " + path);
    in >> rows >> cols;

    float* M = new float[rows * cols];
    for (uint32_t i = 0; i < rows * cols; ++i) in >> M[i];
    return M;
}

void save_matrix(const std::string& path,
                 uint32_t rows, uint32_t cols,
                 const float* M)
{
    std::ofstream out(path);
    out << rows << ' ' << cols << '\n';
    for (uint32_t i = 0; i < rows * cols; ++i) {
        out << M[i];
        if ((i + 1) % cols == 0) out << '\n';
        else                      out << ' ';
    }
}

bool floats_equal(float a, float b, float epsilon = 1e-2f) {
    return std::fabs(a - b) < epsilon;
}

bool matrices_equal(const std::string& a, const std::string& b) {
    std::ifstream f1(a), f2(b);
    if (!f1 || !f2) return false;

    uint32_t rows1, cols1, rows2, cols2;
    f1 >> rows1 >> cols1;
    f2 >> rows2 >> cols2;
    if (rows1 != rows2 || cols1 != cols2) return false;

    for (uint32_t i = 0; i < rows1 * cols1; ++i) {
        float val1, val2;
        f1 >> val1;
        f2 >> val2;
        if (!floats_equal(val1, val2)) {
            std::cout << "Mismatch at index " << i
                      << ": " << val1 << " vs " << val2 << '\n';
            return false;
        }
    }
    return true;
}


void naive_matmul(float* C, const float* A, const float* B,
                  uint32_t m, uint32_t n, uint32_t p)
{
    for (uint32_t i = 0; i < m; ++i)
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0.f;
            for (uint32_t k = 0; k < n; ++k)
                sum += A[i * n + k] * B[k * p + j];
            C[i * p + j] = sum;
        }
}



int main(int argc, char* argv[])

{
    //I used this for testing
    std::cout << "Program started\n";
    if (argc != 2) {
        std::cerr << "usage: ./matmul <case 0‑9>\n";
        return 1;
    }
    std::filesystem::path folder = std::filesystem::path("data") / argv[1];

    uint32_t m, n, n_check, p;
    float* A = load_matrix((folder / "input0.raw").string(), m, n);
    float* B = load_matrix((folder / "input1.raw").string(), n_check, p);
    if (n_check != n) {
        std::cerr << "dimension mismatch between A and B\n";
        return 2;
    }
    float* C = new float[m * p];
    naive_matmul(C, A, B, m, n, p);

    

    std::filesystem::path result = folder / "result.raw";
    save_matrix(result.string(), m, p, C);

    bool ok = matrices_equal(result.string(),
                         (folder / "output.raw").string());

    std::cout << (ok ? "PASS" : "FAIL") << '\n';


    delete[] A; delete[] B; delete[] C;
    return ok ? 0 : 1;
}
