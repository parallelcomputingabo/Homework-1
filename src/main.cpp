#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <cmath>

void matrix_multiply(const float* left_matrix, const float* right_matrix, float* result_matrix, 
                    int left_rows, int left_cols, int right_cols) {
    for (int row = 0; row < left_rows; ++row) {
        for (int col = 0; col < right_cols; ++col) {
            float sum = 0.0f;
            for (int k = 0; k < left_cols; ++k) {
                sum = std::fma(left_matrix[row * left_cols + k], 
                             right_matrix[k * right_cols + col], 
                             sum);
            }
            result_matrix[row * right_cols + col] = sum;
        }
    }
}

float* read_matrix_from_file(const std::string& filename, int& rows, int& cols) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return nullptr;
    }

    std::string dimension_line;
    std::getline(input_file, dimension_line);
    std::stringstream dimension_stream(dimension_line);
    dimension_stream >> rows >> cols;

    float* matrix_data = new float[rows * cols];
    for (int i = 0; i < rows * cols; ++i) {
        input_file >> matrix_data[i];
    }

    input_file.close();
    return matrix_data;
}

void write_matrix_to_file(const std::string& filename, const float* matrix_data, 
                         int rows, int cols) {
    std::ofstream output_file(filename);
    if (!output_file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    output_file << rows << " " << cols << "\n";
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float val = matrix_data[row * cols + col];
            
            // Check if value is a whole number
            if (val == std::floor(val)) {
                output_file << static_cast<int>(val) << "."; // Format as "XXX."
            } else {
                // For decimal numbers, use precision 2 but remove trailing zero
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << val;
                std::string s = ss.str();
                s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                if (s.back() == '.') s += "0"; // Ensure trailing .0
                output_file << s;
            }

            if (col < cols - 1) {
                output_file << " ";
            }
        }
        output_file << "\n";
    }

    output_file.close();
}

bool compare_float_lines(const std::string& line1, const std::string& line2) {
    std::istringstream iss1(line1), iss2(line2);
    float val1, val2;
    
    while (iss1 >> val1 && iss2 >> val2) {
        if (std::abs(val1 - val2) > 0.001f) {
            return false;
        }
    }
    
    // Check if both streams are exhausted
    return iss1.eof() && iss2.eof();
}

bool compare_files(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1), f2(file2);
    if (!f1 || !f2) {
        std::cerr << "Error opening files for comparison" << std::endl;
        return false;
    }

    std::string line1, line2;
    while (std::getline(f1, line1) && std::getline(f2, line2)) {
        if (line1 != line2 && !compare_float_lines(line1, line2)) {
            std::cerr << "Mismatch found:\n"
                      << "Expected: " << line2 << "\n"
                      << "Got:      " << line1 << std::endl;
            return false;
        }
    }

    return f1.eof() && f2.eof();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <test_case_number>" << std::endl;
        return 1;
    }

    std::filesystem::path executable_path = std::filesystem::absolute(argv[0]);
    std::string data_directory = executable_path.parent_path().string() + 
                               "/data/" + 
                               std::to_string(std::stoi(argv[1])) + "/";

    int left_matrix_rows, left_matrix_cols;
    int right_matrix_rows, right_matrix_cols;
    
    float* left_matrix = read_matrix_from_file(data_directory + "input0.raw", 
                                             left_matrix_rows, left_matrix_cols);
    float* right_matrix = read_matrix_from_file(data_directory + "input1.raw", 
                                              right_matrix_rows, right_matrix_cols);

    if (left_matrix_cols != right_matrix_rows) {
        std::cerr << "Error: Matrix dimensions incompatible for multiplication" << std::endl;
        delete[] left_matrix;
        delete[] right_matrix;
        return 1;
    }

    float* product_matrix = new float[left_matrix_rows * right_matrix_cols];
    matrix_multiply(left_matrix, right_matrix, product_matrix, 
                   left_matrix_rows, left_matrix_cols, right_matrix_cols);
    
    std::string result_path = data_directory + "result.raw";
    write_matrix_to_file(result_path, 
                        product_matrix, 
                        left_matrix_rows, 
                        right_matrix_cols);

    // Validation step
    std::string reference_path = data_directory + "output.raw";
    if (compare_files(result_path, reference_path)) {
        std::cout << "Validation SUCCESS: result.raw matches output.raw" << std::endl;
    } else {
        std::cout << "Validation FAILED: result.raw differs from output.raw" << std::endl;
    }

    delete[] left_matrix;
    delete[] right_matrix;
    delete[] product_matrix;
    return 0;
}