/*
 * Program overview:
 * 
 * Compilation and execution: 
 * I used the provided CMAKE txt with the default settings. 
 * Code was executed with the same structure of folders with cmake command lines as shown here: 
 * mkdir -p build
 * cd build
 * cmake .. 
 * make
 * 
 * for i in {0..9}; do
 *   ./matmul $i
 * done
 * 
 * Logic:
 * 1- Read matrices:
 *    - Open “input0.raw” and “input1.raw” in the chosen data folder.
 *    - Read the first two numbers as dimensions (rows and cols).
 *    - Read the remaining values into C-style arrays A and B (row-major order).
 *
 * 2- Compute product:
 *    - Allocate a C-style array C of size (rows of A) × (cols of B).
 *    - Use a triple-nested loop to compute C[i][j] = sum_k A[i][k] * B[k][j].
 *
 * 3- Write result:
 *    - Open “result.raw” for writing.
 *    - Output the dimensions, then each value of C:
 *        • Round to two decimals.
 *        • Strip trailing zeros (so "2.30" → "2.3", “5.00” → “5.”).
 *    - Close the file to ensure all data is flushed to disk.
 *
 * 4- Validate output:
 *    - Open “result.raw” and the reference “output.raw”.
 *    - For each line:
 *        • If lines match textually, continue.
 *        • Otherwise, split both lines into floats.
 *        • Compare each pair of floats with an absolute tolerance of 0.001.
 *    - Pass if every value is within tolerance and both files have the same number of lines.
 */



#include <iostream>
#include <fstream>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

// 1) Naive triple-loop multiply C = A × B
void naive_matmul(float* C, const float* A, const float* B,
                  uint32_t m, uint32_t n, uint32_t p) {
    for (uint32_t i = 0; i < m; ++i)
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < n; ++k)
                sum += A[i*n + k] * B[k*p + j];
            C[i*p + j] = sum;
        }
}

// 2) Compare floats within ±0.001 (tolerance)
bool compare_floats(float a, float b) {
    return std::fabs(a - b) <= 0.001f;
}

// 3) Line-by-line, tolerance-based compare
bool compare_matrix_files(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1), f2(p2);
    if (!f1 || !f2) return false;
    std::string l1, l2; size_t line = 0;
    while (std::getline(f1, l1)) {
        ++line;
        if (!std::getline(f2, l2)) return false;
        if (l1 == l2) continue;
        std::istringstream s1(l1), s2(l2);
        std::vector<float> v1, v2; float x;
        while (s1 >> x) v1.push_back(x);
        while (s2 >> x) v2.push_back(x);
        if (v1.size()!=v2.size()) return false;
        for (size_t i=0; i<v1.size(); ++i)
            if (!compare_floats(v1[i], v2[i])) return false;
    }
    return f1.eof() && !std::getline(f2, l2);
}

int main(int argc, char* argv[]) {
    assert(argc==2 && "Usage: matmul <case#>");
    std::string dir = "../data/" + std::string(argv[1]) + "/";

    // read dims
    std::ifstream inA(dir+"input0.raw"), inB(dir+"input1.raw");
    assert(inA && inB);
    uint32_t m,n,n2,p;
    inA>>m>>n; inB>>n2>>p; assert(n==n2);

    // alloc & load
    float *A=new float[m*n], *B=new float[n*p], *C=new float[m*p];
    for(uint32_t i=0;i<m*n;++i) inA>>A[i];
    for(uint32_t i=0;i<n*p;++i) inB>>B[i];

    // multiply
    naive_matmul(C,A,B,m,n,p);

    // write result
    std::string out_path = dir+"result.raw";
    std::ofstream out(out_path);
    out<<m<<" "<<p<<"\n";
    for(uint32_t i=0;i<m;++i){
      for(uint32_t j=0;j<p;++j){
        float v2 = std::round(C[i*p+j]*100.0f)/100.0f;
        std::ostringstream ss;
        ss<<std::fixed<<std::setprecision(2)<<v2;
        std::string s=ss.str();
        while(s.back()=='0') s.pop_back();
        out<<s<<(j+1<p?" ":"\n");
      }
    }
    out.close();  // <-- ensure full flush

    // validate
    bool ok = compare_matrix_files(out_path, dir+"output.raw");
    std::cout<<(ok?"PASS\n":"FAIL\n");

    delete[] A; delete[] B; delete[] C;
    return ok?0:1;
}
