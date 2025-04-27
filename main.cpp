#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p)
{
    //A = (m*n)
    //B = (n*p)

    //C = (m*p)

    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; i++) {
        //Each column/row/whatever of A
        for (uint32_t j = 0; j < p; j++){
            //Each (^^) of B
            //float sum = 0.0f;
            C[i * p + j] =0.0f;
            for (uint32_t k = 0; k < n  ; k++){
                //Current A (^^) up to n, from the current (^^) B up to n
                //sum += A[i * n + k] * B[k * p + j];
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            };
            //C[i * p + j] = sum;

        };

    };
}

int main(){
    std::string base = ("../data/");
    //m matrix
    std::string m1 = ("/input0.raw");
    std::string m2 = ("/input1.raw");
    std::string mt = ("/output.raw"); //test
    std::string mw = ("/result.raw"); //write target

    int m, n, p;  // A is m x n, B is n x p, C is m x p
    // Step 1: Open and read input files
    // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
    for (int i =0; i < 10; i++)
    {
        std::string m1_path = base + std::to_string(i) + m1;
        std::string m2_path = base + std::to_string(i) + m2;
        std::string mt_path = base + std::to_string(i) + mt;
        std::string mw_path = base + std::to_string(i) + mw;

        // TODO: Read dimensions from input0.raw and input1.raw
        std::ifstream fA(m1_path);
        std::ifstream fB(m2_path);
        if (!fA.is_open() || !fB.is_open())
        {
            std::cerr << "Opening files failed"<< "\n" << m1_path<<"\n" <<m2_path<< std::endl;
        }
        int n_t;

        fA >> m >> n;
        fB >> n_t >> p;
        if (n != n_t)
        {
            std::cerr << "Error in reading file: A second dimension and B first dimension missmatch" << std::endl;
        }

    // TODO: Allocate memory for matrices A, B, and C using new or malloc
float* mA = (float*)malloc(sizeof(float) * m * n);
float* mB = (float*)malloc(sizeof(float) * n * p);

        float* mC = (float*)malloc(sizeof(float) * m * p);
    // TODO: Read matrix elements into A and B (row-major order)

        for (uint32_t j = 0; j < m * n; ++j) {
            fA >> mA[j];
        }
    fA.close();
        for (uint32_t j = 0; j < n * p; ++j) {
            fB >> mB[j];
        }
    fB.close();

    // Step 2: Perform matrix multiplication
    // TODO: Call naive_matmul

naive_matmul(mC, mA, mB, m, n, p);




    // Step 3: Write result to result.raw in the same directory as input files
    // TODO: Write dimensions and elements to result.raw
std::ofstream mCout(mw_path);
    if (!mCout.is_open())
    {
        std::cerr << "Opening files failed" << std::endl; //Why is clion so aggressive with completion?
    }
    mCout << m << " " << p << std::endl;

for (int j = 0; j < m; ++j){
    for (int k = 0; k < p; ++k){
        mCout<< std::fixed << std::setprecision(2) << mC[j * p + k];
        if (k != p - 1)
        {
            mCout << " ";
        }
        // I can't figure how to manipulate the ofstream so that the floats with xy.00 are made to write as xy.
        // Maybe it's some inbuild function, but is it really just custom code? I just can't find it
    }
    //if (j != p - 1)
    //{
        mCout << std::endl;

    //}
// for some reason the case 2 kept breaking with this one. Maybe my logic doesn't really match all cases of dimensionality?
}

mCout.close();



    // Step 4: validate the result
    // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw
std::ifstream mT(mt_path);
std::ifstream mW(mw_path);
        float test_v, wri_v;
        float rmT, cmT, rmW, cmW;
        bool suc=true;
        mT >> rmT, mT >>cmT, mW >> rmW, mW >> cmW;
        if (rmT!=rmW || cmT!=cmW)
        {
            std::cerr << "Dimension missmatch: " << i<<"\n result: ("<<cmT<<"//"<<cmW<<") output : ("<<rmT<<"//"<<cmT<<")" << std::endl;
            suc=false;
        }

        while(mT >>test_v && mW >> wri_v)
        {
            if (std::round(test_v) != std::round(wri_v) )
            {
                std::cerr << "Error in test: " << i <<"\n The differing values where C: "<< wri_v << " T:"<< test_v << std::endl;
                break;
                suc=false;
            }
        }
if (suc)
{
    std::cout << "Test case " << i << ", succeeded." << std::endl;
}else
{
    std::cerr << "Test case : " << i << ", failed!" << std::endl;
}

    // Step 5: Clean up memory
    // TODO: Deallocate A, B, and C
        free(mA);
        free(mB);
        free(mC);

    }
    return 0;
}