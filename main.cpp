#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void naive_matmul(double* C, double* A, double* B, uint32_t m, uint32_t n, uint32_t p) {
    // A = First matrix, in the form of one-dimensional/row-major list
    // B = Second Matrix, in the form of one-dimensional/row-major list
    // C = A x B, in the form of one-dimensional/row-major list
    //
    // m = number of rows in matrix A
    // n = number of columns in matrix A
    // p = number of columns in matrix B

    // TODO: Implement naive matrix multiplication C = A x B
    // A is m x n, B is n x p, C is m x p

    // Just to keep track of what the assignment is saying...
    uint32_t N = p;

    /*
        Loop through A and B and perform multiplication for each row element in A with column element of B (m x p).
        Loop is multidimensional and covers elements as if they were part of matrices.

        The result C points to somewhere in the memory, where the calculations
    */
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < N; j++) {

            C[i * N + j] = 0; 
            for (int k = 0; k < n; k++) {
                C[i * N + j] += A[i * n + k] * B[k * N + j];
            }
        }
    }
}

// Define a matrix in the context of this program.
// - A matrix has a dimension X x Y
// - Matrix data is stored in one-dimensional row-major list somewhere in memory.
struct Matrix {
    int x, y;
    double* dataRowMajorOrder;
};

// Extract data from a file, and build a row-major matrix accordingly to struct Matrix
Matrix* getMatrix(string filePath) {
    // Read file
    ifstream file(filePath);

    if (!file) {
        Matrix* result = new Matrix;
        return result;
    }

    // Max size of a line (size can be anything if available in memory. 100000 in this case
    char line[100000];

    // Dimensions
    int x = 0;
    int y = 0;

    // iteration/list element
    int i = 0;

    // Matrix data, placed somewhere in memory
    double* collection = nullptr;

    while (file.getline(line, 100000)) {
        // Loop through the file, line by line.
        // A line is a character list consisting of 100000 slots
        
        // Split line by blank space into token
        char* token = strtok(line, " ");
        while (token != nullptr) {
            // Iterate over one token at a time
            
            // First line in each input file represents dimensions X Y 
            if (x == 0) {
                // If X has not been set, then pick first token from first line,
                // Assuming this is the first line and this is the first token
                x = atoi(token);
            }
            else if (y == 0) {
                // If Y has not been set, then pick second token from first line,
                // Assuming this is the first line and this is the second token iteration
                y = atoi(token);

                // Now, set the total slots (x * y) in memory 
                collection = new double[x * y]; 
            }
            else {
                // Insert token into collection
                // Assuming this is the second line and onwards
                collection[i++] = atof(token);
            }

            token = strtok(nullptr, " ");
        }
    }

    file.close();

    // Define the Matrix
    Matrix* result = new Matrix;

    // Matrix dimensions
    result->x = x;
    result->y = y;

    // Data collection of tokens from second line and onwards
    result->dataRowMajorOrder = collection;

    return result;
}

// Compare two files by inserting their contents in separate strings
// - Remove blank spaces in each string
// - Compare the strings.
// Return true if comparison passes. And false otherwise.
bool compareFiles(string originPath, string targetPath) {
    ifstream originalFile(originPath);
    ifstream targetFile(targetPath);

    if(!originalFile || !targetFile) {
        return false;
    }

    string originalFileContents;
    string targetFileContents;

    while (getline(originalFile, originalFileContents) && getline(targetFile, targetFileContents)) {
        string originalFileLetters = "";
        string targetFileLetters = "";

        for (char character : targetFileContents) {
            if (character != ' ') {
                targetFileLetters += character;
            }
        }

        for (char character : originalFileContents) {
            if (character != ' ') {
                originalFileLetters += character;
            }
        }

        if (targetFileLetters != originalFileLetters) {
            targetFile.close();
            originalFile.close();
            return false;
        }
    }

    targetFile.close();
    originalFile.close();
    return true;
}

int main(int argc, char* argv[]) {
    string num = FOLDER_NUM;

    // Create a matrix from input files
    Matrix* AMatrix = getMatrix("C:\\Users\\Priva\\Documents\\DI-studies\\parallell-programming\\assignment1\\assignment1\\Homework-1\\data\\" + num + "\\input0.raw");
    Matrix* BMatrix = getMatrix("C:\\Users\\Priva\\Documents\\DI-studies\\parallell-programming\\assignment1\\assignment1\\Homework-1\\data\\" + num + "\\input1.raw");
    
    if (!AMatrix->x || !AMatrix->y || !BMatrix->x || !BMatrix->y) {
        // One or more dimensions for either Matrix A or B is missing
        // End the program and delete all memories.
        delete[] AMatrix->dataRowMajorOrder;
        delete[] BMatrix->dataRowMajorOrder;
        delete AMatrix;
        delete BMatrix;
        return 0;
    }
    
    int m, n, p;  // A is m x n, B is n x p, C is m x p

    // m = number of rows in matrix A
    // n = number of columns in matrix A
    // p = number of columns in matrix B
    m = AMatrix->x;
    n = AMatrix->y;
    p = BMatrix->y;

    // Total size of the new matrix
    int size = m * p;
    double* C = new double[size];

    // Perform matrix multiplication. The result is stored in C after function ends.
    naive_matmul(C, AMatrix->dataRowMajorOrder, BMatrix->dataRowMajorOrder, m, n, p);

    // Open/create result.raw, where dimensions and C is inserted
    ofstream resultFile("C:\\Users\\Priva\\Documents\\DI-studies\\parallell-programming\\assignment1\\assignment1\\Homework-1\\data\\" + num + "\\result.raw");
    
    // m = number of rows in matrix A
    // p = number of columns in matrix B
    // m x p = dimension of C
    // Write the dimensions to file.
    resultFile << m << " " << p << "\n";

    int i = 0;

    // Loop through and insert each data element from matrix C to the results file
    while (i < size) {
        bool isInteger = fabs(C[i] - round(C[i])) < 1e-6;

        if (i % p == p - 1 && i + 1 != size) {
            // If this is the last character in a line, then insert linebreaks
            // Some integers in the output file has a dot (e.g. 247.). This code makes sure to satisfy that in results.raw
            if (isInteger) {
                resultFile << C[i] << "." << "\n";
            }
            else {
                resultFile << C[i] << "\n";
            }
        }
        else {
            if (isInteger) {
                resultFile << C[i] << "." << " ";
            }
            else {
                resultFile << C[i] << " ";
            }
        }
        i++;
    }
    resultFile.close();

    // Compare two files (output.raw and result.raw)
    bool compareResults = compareFiles("C:\\Users\\Priva\\Documents\\DI-studies\\parallell-programming\\assignment1\\assignment1\\Homework-1\\data\\" + num + "\\output.raw", "C:\\Users\\Priva\\Documents\\DI-studies\\parallell-programming\\assignment1\\assignment1\\Homework-1\\data\\" + num + "\\result.raw");

    if (compareResults) {
        cout << "Data folder " << FOLDER_NUM << " PASSES" << endl;

    }
    else {
        cout << "Data folder " << FOLDER_NUM << " DOES NOT PASS" << endl;
    }

    // Delete all memories and end program.
    delete[] AMatrix->dataRowMajorOrder;
    delete[] BMatrix->dataRowMajorOrder;
    delete[] C;
    delete AMatrix;
    delete BMatrix;

    return 0;
}