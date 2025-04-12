#include <iostream>
#include <fstream>
#include <cstdint>
using namespace std;
void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p
    // Cleaning up the C array
    for(int i=0;i<m;i++)
        for(int j=0;j<p;j++)
            C[i*p+j]=0;
    // Calculation of mult
    for(int i=0;i<m;i++)
        for(int j=0;j<p;j++)
            for(int k=0;k<n;k++)
                C[i*p+j]+=A[i*n+k]*B[k*p+j];
}
bool result_validation(string output_path, string result_path) {
    ifstream fptr_output(output_path),fptr_result(result_path);
    try {
        if(fptr_output.fail())
            throw "output.raw";
        else if(fptr_result.fail())
            throw "result.raw";
    }
    catch(const char* name) {
        cout<<"Failed to read "<<name<<endl;
        exit(-1);
    }
    float temp1,temp2;
    while(!fptr_output.eof()) {
        fptr_output>>temp1;
        fptr_result>>temp2;
        // I used deficit because of precision issue with floating numbers
        if(abs(temp1-temp2)>=0.5) {
            cout<<"Failed comparison: "<<temp1<<" "<<temp2<<endl;
            return false;
        }
    }
    return true;
}
int main() {
    for(int test_number=0;test_number<10;test_number++) {
        cout<<"Test number ("<<test_number<<"):\n";
        // Step 1: Open and read input files
        // TODO: Read dimensions from input0.raw and input1.raw available in the data directory
        string input0_path="..\\data\\"+to_string(test_number)+"\\input0.raw";
        string input1_path="..\\data\\"+to_string(test_number)+"\\input1.raw";
        ifstream fptr_input0(input0_path),fptr_input1(input1_path);
        try {
            if(fptr_input0.fail())
                throw "input0.raw";
            else if(fptr_input1.fail())
                throw "input1.raw";
        }
        catch(const char* name) {
            cout<<"Failed to read "<<name<<endl;
            return -1;
        }
        // TODO: Read dimensions from input0.raw and input1.raw
        int m, n, p;  // A is m x n, B is n x p, C is m x p
        fptr_input0>>m>>n;
        fptr_input1>>n>>p;
        cout<<"Dimensions: "<<m<<" "<<n<<" "<<p<<"\n";
        // TODO: Allocate memory for matrices A, B, and C using new or malloc
        float* A = new float[m*n];
        float* B = new float[n*p];
        float* C = new float[m*p];
        // TODO: Read matrix elements into A and B (row-major order)
        for(int i=0;i<m;i++)
            for(int j=0;j<n;j++)
                fptr_input0>>A[i*n+j];
        for(int i=0;i<n;i++)
            for(int j=0;j<p;j++)
                fptr_input1>>B[i*p+j];
        // Step 2: Perform matrix multiplication
        // TODO: Call naive_matmul
        naive_matmul(C, A, B, m, n, p);
        // Step 3: Write result to result.raw in the same directory as input files
        string result_path="..\\data\\"+to_string(test_number)+"\\result.raw";
        string output_path="..\\data\\"+to_string(test_number)+"\\output.raw";
        ofstream fptr_result(result_path);
        try {
            if(fptr_result.fail())
                throw "result.raw";
        }
        catch(const char* name) {
            cout<<"Failed to open "<<name<<endl;
            return -1;
        }
        // TODO: Write dimensions and elements to result.raw
        fptr_result << m<<" "<<p << endl;
        for(int i=0;i<m;fptr_result<<endl,i++)
            for(int j=0;j<p;j++)
                fptr_result<<C[i*p+j]<<" ";
        // Step 4: validate the result
        // TODO: Implement validation function to check if the result is correct by comparing results.raw and output.raw

        bool result=result_validation(output_path,result_path);


        // Step 5: Clean up memory
        // TODO: Deallocate A, B, and C
        delete[] A;
        delete[] B;
        delete[] C;
        if(result)
            cout<<"Test passed!!!";
        else
            cout<<"Validation failed!!!";
        cout<<endl<<endl;

    }
    return 0;
}