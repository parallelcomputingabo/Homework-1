#include <iostream>
#include <fstream>
#include <filesystem>
#include <format>

using namespace std;
namespace fs = filesystem;

void naive_matmul(float *C, float *A, float *B, int m, int n, int p)
{
    // A is m x n, B is n x p, C is m x p
    for (int i = 0; i < m; i++) // Rows of A
    {          
        for (int j = 0; j < p; j++) // Columns of B
        {    
            // Initialize C with 0
            C[i * p + j] = 0;

            for (int k = 0; k < n; k++) // Shared dimension
            {  
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

void obtain_file(fs::path& fileName, char* fileFolderNumber)
{
    // Build the data directory path with file number subfolder
    fs::path dataFolder = fs::path(SOURCE_DIR) / "data" / fileFolderNumber;
   
    // Create full path
    fileName = dataFolder / fileName;

    cout << "Obtained full file path: " << fileName << endl;
}

int read_matrix_dimensions(fs::path path, int& rows, int& cols)
{
    ifstream file(path);
    if (file.is_open()) {
        file >> rows >> cols;
        file.close();
    }
    else {
        return -1;
    }

    return 0;
}

int read_matrix_elements(fs::path path, float* matrix, int rows, int cols)
{
    ifstream file(path);
    if (file.is_open()) {

        int dummy1, dummy2;
        file >> dummy1 >> dummy2;  // Skip the dimensions
        
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                file >> matrix[i * cols + j];
            }
        }

        file.close();
    }
    else {
        return -1;
    }

    return 0;
}

int write_matrix_result(fs::path fileName, char* fileNumber, float* matrix, int rows, int cols)
{
    // Build the data directory path with file number subfolder
    fs::path dataFolder = fs::path(SOURCE_DIR) / "data" / fileNumber;

    // Create full paths to input1.txt and input2.txt
    fs::path resultPath = dataFolder / fileName;

    cout << "Writing output to: " << resultPath << endl;

    ofstream file(resultPath);
    if (file.is_open()) {
        // Write rows and columns
        file << rows << " " << cols << "\n";

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                file << matrix[i * cols + j] << " ";
            }
            file << "\n"; // Newline after each row
        }

        file.close();
    }
    else {
        return -1;
    }

    return 0;
}

bool compare_matrices(float* A, float* B, int cols, int rows)
{
    bool same = true;
    float a, b;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Using a comparison with rounding because of precision error for floating point numbers
            a = A[i * cols + j];
            b = B[i * cols + j];
            a = std::round(a * 100.0f) / 100.0f;
            b = std::round(b * 100.0f) / 100.0f;
            if (a != b)
            {
                cout << format("Difference at row = {} col = {} element1 = {} element2 = {}", i, j, a, b) << endl;
                same = false;
                break;
            }
        }

        if (same == false)
            break;
    }

    return same;
}

int main(int argc, char *argv[])
{
    // Step 1: Open and read input files based on the argument the user provides

    // User didn't provide only one additional argument
    if (argc != 2)
    {
        cerr << "Provide one command line argument (a number between 0 and 9, both inclusive)" << endl;
        cerr << "You provided: " << argc - 1 << " arguments" << endl;
        return 1;
    }

    // The provided argument is not a number between 0 and 9, both inclusive
    try
    {
        int argument = stoi(argv[1]);
        if (argument < 0 || argument > 9)
        {
            cerr << "Provide one command line argument (a number between 0 and 9, both inclusive)" << endl;
            cerr << "You provided: " << argv[1] << endl;
            return 1;
        }
    }
    catch (const exception &e)
    {
        cerr << "Error: Invalid number format: " << argv[1] << endl;
        return 1;
    }

    char* fileFolderNumber = argv[1];

    // Obtain the correct input files
    fs::path path0 = "input0.raw";
    fs::path path1 = "input1.raw";

    obtain_file(path0, fileFolderNumber);
    obtain_file(path1, fileFolderNumber);

    // Read dimensions from obtained files
    int m, n, p; // A is m x n, B is n x p, C is m x p

    int result = read_matrix_dimensions(path0, m, n);
    if (result == -1)
    {
        cerr << "Failed to open file: " << path0 << endl;
        return 1;
    }

    result = read_matrix_dimensions(path1, n, p);
    if (result == -1)
    {
        cerr << "Failed to open file: " << path1 << endl;
        return 1;
    }

    cout << format("Dimensions are: m {} n {} p {}", m, n, p) << endl;

    // Allocate memory for matrices A, B, and C using linear approach
    float* A = new float[m * n];
    float* B = new float[n * p];
    float* C = new float[m * p];

    // Read matrix elements into A and B (row-major order)
    result = read_matrix_elements(path0, A, m, n);
    if (result == -1)
    {
        cerr << "Failed to open file: " << path0 << endl;
        return 1;
    }

    result = read_matrix_elements(path1, B, n, p);
    if (result == -1)
    {
        cerr << "Failed to open file: " << path1 << endl;
        return 1;
    }

    // Step 2: Perform matrix multiplication
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    fs::path resultFileName = "result.raw";
    write_matrix_result(resultFileName, fileFolderNumber, C, m, p);
    if (result == -1)
    {
        cerr << "Failed to write to file: " << resultFileName << endl;
        return 1;
    }

    // Step 4: Validate the result

    // Read back the saved matrix
    obtain_file(resultFileName, fileFolderNumber);
    result = read_matrix_elements(resultFileName, C, m, p);
    if (result == -1)
    {
        cerr << "Failed to open file: " << resultFileName << endl;
        return 1;
    }

    // Define a temorary matrix for comparison
    float* tmpMatrix = new float [m * p];
    fs::path comparisonFileName = "output.raw";
    obtain_file(comparisonFileName, fileFolderNumber);
    result = read_matrix_elements(comparisonFileName, tmpMatrix, m, p);
    if (result == -1)
    {
        cerr << "Failed to open file: " << comparisonFileName << endl;
        return 1;
    }

    // Compare
    if (compare_matrices(C, tmpMatrix, m, p))
        cout << "Matrices are the same" << endl;
    else
        cout << "Matrices are NOT the same" << endl;

    // Step 5: Clean up memory
    delete[] tmpMatrix;
    delete[] C;
    delete[] B;
    delete[] A;

    return 0;
}