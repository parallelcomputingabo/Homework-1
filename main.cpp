// Carl Nordenbrink
// Parallell programming 2025 Assignment 1

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <Windows.h>
#include <string>

void multTestFolder(const std::string &);
bool fileExists(const std::string& path);
bool validateResult(const std::string&);
std::string getExecutablePath();
std::string getParentPath();

std::string getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}


void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
     
    // for-loop to initialise result matrix C
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            C[i * p + j] = 0.0f;
        }
    }

    // This nestled for-loop iterates first over rows of A, then columns of B
    // and finally columns of A and rows of B.
    for (uint32_t i = 0; i < m; ++i) {
        for (uint32_t j = 0; j < p; ++j) {
            for (uint32_t k = 0; k < n; ++k) {
                C[i * p + j] += A[i * n + k] * B[k * p + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {

    std::string path = "C:\\Users\\norde\\source\\repos\\Homework-1\\data\\0\\input0.raw";
    if (fileExists(path)) {
        std::cout << "File exists: " << path << std::endl;
    }
    else {
        std::cout << "File does not exist: " << path << std::endl;
    }

    int caseNumber = std::atoi(argv[1]);

	//std::string folder_name[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
 //   for (int i = 0; i < 9; i++) multTestFolder(folder_name[i]);

    multTestFolder(std::to_string(caseNumber));

    return 0;
}

void multTestFolder(const std::string& folder_name)
{
    //std::string root_path = "C:\\Users\\norde\\source\\repos\\Homework-1\\data\\";

    //std::string root_path = "data/";
    std::string parentPath = getParentPath();
    std::cout << "Parent Path: " << parentPath << std::endl;

    std::string root_path = parentPath + "\\data\\"; // Construct path relative to the parent directory
    std::cout << "Root Path: " << root_path << std::endl;


    std::ifstream ifs;

	std::string input_dir_1 = root_path + folder_name + "/input0.raw";
	std::string input_dir_2 = root_path + folder_name + "/input1.raw";
	std::string result_dir = root_path + folder_name + "/result.raw";

    
    std::ifstream input0(input_dir_1);
    std::ifstream input1(input_dir_2);
    if (!input0.is_open() || !input1.is_open()) {
        std::cerr << "Error opening input files." << std::endl;
        return;
    }

    int m, n;
    input0 >> m;
    input0 >> n;

    float* A = (float*)malloc(m * n * sizeof(float));
    if (!A) {
        std::cerr << "Memory allocation failed for matrix A." << std::endl;
        return;
    }



    for (int i = 0; i < m * n; ++i) input0 >> A[i];
    input0.close();

    int n_check, p;
    input1 >> n_check;
    input1 >> p;

    if (n != n_check) {
        std::cerr << "Matrix dimensions do not match for multiplication." << std::endl;
        free(A);
        return;
    }

    float* B = (float*)malloc(n * p * sizeof(float));
    if (!B) {
        std::cerr << "Memory allocation failed for matrix B." << std::endl;
        free(A);
        return;
    }

    for (int i = 0; i < n * p; ++i) input1 >> B[i];
    input1.close();

    float* C = (float*)malloc(m * p * sizeof(float));
    if (!C) {
        std::cerr << "Memory allocation failed for matrix C." << std::endl;
        free(A);
        free(B);
        return;
    }

    naive_matmul(C, A, B, m, n, p);

    std::ofstream result(result_dir.c_str());
    if (!result.is_open()) {
        std::cerr << "Error opening result file." << std::endl;
        free(A);
        free(B);
        free(C);
        return;
    }

    result << m << " " << p << std::endl;
	result << std::fixed << std::setprecision(2);
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < p; ++j) {
			result << C[i * p + j] << " ";
		}
		result << std::endl;
	}


    result.close();

    free(A);
    free(B);
    free(C);


    if (validateResult(root_path + folder_name)) std::cout << folder_name << ": Correct transfer" << std::endl;
    else std::cout << folder_name << ": Incorrect transfer" << std::endl;
}


bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.is_open();
}

#include <sstream> // For std::istringstream

bool validateResult(const std::string& path) {
    std::string result_file = path + "\\result.raw";
    std::string output_file = path + "\\output.raw";

    std::ifstream result(result_file);
    std::ifstream output(output_file);

    if (!result.is_open() || !output.is_open()) {
        std::cerr << "Error opening result or output file in path: " << path << std::endl;
        return false;
    }

    std::string result_line, output_line;
    int line_number = 1; // To track which line has a mismatch

    while (std::getline(result, result_line) && std::getline(output, output_line)) {
        std::istringstream result_stream(result_line);
        std::istringstream output_stream(output_line);

        float result_value, output_value;
        int value_index = 1; // To track which value in the line has a mismatch

        while (result_stream >> result_value && output_stream >> output_value) {
            // Compare the float values with a small tolerance to account for floating-point precision issues
            if (std::abs(result_value - output_value) > 1e-5) {
                std::cerr << "Mismatch found at line " << line_number << ", value " << value_index << ":\n"
                    << "Result: " << result_value << ", Output: " << output_value << std::endl;
                result.close();
                output.close();
                return false;
            }
            value_index++;
        }

        // Check if one line has more values than the other
        if ((result_stream >> result_value) || (output_stream >> output_value)) {
            std::cerr << "Mismatch in number of values at line " << line_number << "." << std::endl;
            result.close();
            output.close();
            return false;
        }

        line_number++;
    }

    // Check if one file has more lines than the other
    if (result.good() || output.good()) {
        std::cerr << "Mismatch in number of lines between files." << std::endl;
        result.close();
        output.close();
        return false;
    }

    result.close();
    output.close();
    return true;
}

std::string getParentPath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string executablePath(buffer);
    std::string::size_type pos = executablePath.find_last_of("\\/");
    return executablePath.substr(0, pos).substr(0, executablePath.substr(0, pos).find_last_of("\\/"));
}
