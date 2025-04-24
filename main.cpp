#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <sstream>
#include <cmath>
#include <iomanip>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            float sum = 0.0f;
            for (uint32_t k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

//Built and ran in CLion, to run, add 0 1 2 3 4 5 6 7 8 9 in program arguments for run configurations.
int main(int argc, char* argv[]) {
    //We validate and parse command-line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <case_number> [<case_number> ...]" << std::endl;
        return 1;
    }

    for (int arg = 1; arg < argc; ++arg) {
        int case_number;
        try {
            case_number = std::stoi(argv[arg]);
        } catch (...) {
            std::cerr << "Invalid case number '" << argv[arg] << "' Must be 0-9" << std::endl;
            continue;
        }
        if (case_number < 0 || case_number > 9) {
            std::cerr << "Case number out of range: " << case_number << ". Skipping" << std::endl;
            continue;
        }
        std::cout << "Testing case " << case_number << "..." << std::endl;

        std::string dir = "data/" + std::to_string(case_number) + "/";

        // Step 1: Open and read input files
        std::ifstream inA(dir + "input0.raw");
        std::ifstream inB(dir + "input1.raw");
        if (!inA.is_open() || !inB.is_open()) {
            std::cerr << "Error opening input files in " << dir << std::endl;
            continue;
        }

        int m, n, p;
        inA >> m >> n;
        inB >> n >> p;
        float* A = new float[m * n];
        float* B = new float[n * p];
        float* C = new float[m * p];
        for (int i = 0; i < m * n; ++i) inA >> A[i];
        for (int i = 0; i < n * p; ++i) inB >> B[i];

        // Step 2: Perform matrix multiplication
        naive_matmul(C, A, B, m, n, p);
        for (int i = 0; i < m * p; ++i) {
            float raw = C[i];
            float scaled = std::floor(raw * 100.0f + 0.5f);
            C[i] = scaled / 100.0f;
        }

        // Step 3: Write result to result.raw in the same directory as input files
        std::ofstream out(dir + "result.raw");
        if (!out.is_open()) {
            std::cerr << "Error opening result.raw file for writing in " << dir << std::endl;
            delete[] A;
            delete[] B;
            delete[] C;
            continue;
        }
        out << m << " " << p << '\n';

        for (int i = 0; i < m * p; ++i) {
            float val = C[i];
            int scaled = static_cast<int>(std::floor(val * 100.0f + 0.5f));
            int ip = scaled / 100;
            int frac2 = scaled % 100;
            if (frac2 == 0) {
                out << ip << '.';
            } else if (frac2 % 10 == 0) {
                out << ip << '.' << (frac2 / 10);
            } else {
                int high = frac2 / 10;
                int low = frac2 % 10;
                out << ip << '.' << high << low;
            }
            if ((i + 1) % p) out << ' ';
            else out << '\n';
        }
        out.close();

        // Step 4: validate the result
        std::ifstream ref(dir + "output.raw");
        int rm, rp;
        ref >> rm >> rp;
        bool check = (rm == m && rp == p);
        for (int i = 0; i < m * p && check; ++i) {
            float expected;
            ref >> expected;
            if (std::fabs(C[i] - expected) > 1e-6f) check = false;
        }
        std::cout << "Case " << case_number << ": "
                  << (check ? "Validation pass" : "Validation fail")
                  << std::endl;

        // Step 5: Clean up memory
        delete[] A;
        delete[] B;
        delete[] C;
    }
    return 0;
}
