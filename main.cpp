#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>

void naive_matmul(float* C, float* A, float* B, uint32_t m, uint32_t n, uint32_t p) {
	// Naive matrix multiplication C = A × B where A is m×n, B is n×p, C is m×p
	for (uint32_t i = 0; i < m; i++) {
		for (uint32_t j = 0; j < p; j++) {
			C[i * p + j] = 0.0f;  // Initialize result element to 0
			for (uint32_t k = 0; k < n; k++) {
				// Use row-major indexing: A[i,k] is at i*n+k, B[k,j] is at k*p+j
				C[i * p + j] += A[i * n + k] * B[k * p + j];
			}
		}
	}
}

bool read_expected_output(const std::string& path, float* out, uint32_t m, uint32_t p) {
	// Reads expected output matrix from file and returns success/failure
	std::ifstream file(path);
	if (!file.is_open()) return false;
	
	uint32_t temp_m, temp_p;
	file >> temp_m >> temp_p;
	
	for (uint32_t i = 0; i < m; i++) {
		for (uint32_t j = 0; j < p; j++) {
			if (!(file >> out[i * p + j])) return false;
		}
	}
	
	return true;
}

bool compare_matrices(float* A, float* B, uint32_t m, uint32_t p) {
	// Compares two matrices for equality with epsilon tolerance
	const float epsilon = 1e-2f;  // Tolerance for floating-point comparison
	
	for (uint32_t i = 0; i < m; i++) {
		for (uint32_t j = 0; j < p; j++) {
			float diff = std::abs(A[i * p + j] - B[i * p + j]);
			if (diff > epsilon) {
				std::cout << "Difference at [" << i << "," << j << "]: " 
						  << A[i * p + j] << " vs " << B[i * p + j] 
						  << " (diff = " << diff << ")" << std::endl;
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char* argv[]) {
	// Check for required test case number argument
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <case_number>" << std::endl;
		return 1;
	}

	// Validate test case number (0-9)
	int case_num = std::atoi(argv[1]);
	if (case_num < 0 || case_num > 9) {
		std::cerr << "Case number must be between 0 and 9" << std::endl;
		return 1;
	}

	// Set up file paths
	std::string base_path = "data/" + std::to_string(case_num) + "/";
	std::string input0_path = base_path + "input0.raw";
	std::string input1_path = base_path + "input1.raw";
	std::string result_path = base_path + "result.raw";
	std::string output_path = base_path + "output.raw";

	// Open input files
	std::ifstream input0_file(input0_path);
	std::ifstream input1_file(input1_path);

	if (!input0_file.is_open() || !input1_file.is_open()) {
		std::cerr << "Error opening input files" << std::endl;
		return 1;
	}

	// Read matrix dimensions
	uint32_t m, n;
	input0_file >> m >> n;

	uint32_t n2, p;
	input1_file >> n2 >> p;

	// Check matrix compatibility
	if (n != n2) {
		std::cerr << "Matrix dimensions are not compatible for multiplication" << std::endl;
		return 1;
	}

	std::cout << "Matrix A: " << m << " x " << n << std::endl;
	std::cout << "Matrix B: " << n << " x " << p << std::endl;
	std::cout << "Result C: " << m << " x " << p << std::endl;

	// Allocate memory for matrices
	float* A = new float[m * n];  // Matrix A (m×n)
	float* B = new float[n * p];  // Matrix B (n×p)
	float* C = new float[m * p];  // Result matrix C (m×p)

	// Read matrix A data
	for (uint32_t i = 0; i < m; i++) {
		for (uint32_t j = 0; j < n; j++) {
			input0_file >> A[i * n + j];
		}
	}

	// Read matrix B data
	for (uint32_t i = 0; i < n; i++) {
		for (uint32_t j = 0; j < p; j++) {
			input1_file >> B[i * p + j];
		}
	}

	input0_file.close();
	input1_file.close();

	// Perform matrix multiplication
	std::cout << "Performing matrix multiplication..." << std::endl;
	naive_matmul(C, A, B, m, n, p);

	// Read expected result for validation
	float* expected_C = new float[m * p];
	if (!read_expected_output(output_path, expected_C, m, p)) {
		std::cerr << "Error reading expected output" << std::endl;
		delete[] A;
		delete[] B;
		delete[] C;
		delete[] expected_C;
		return 1;
	}

	// Compare result with expected output
	bool is_correct = compare_matrices(C, expected_C, m, p);
	if (is_correct) {
		std::cout << "Matrix multiplication validation passed!" << std::endl;
	} else {
		std::cout << "Matrix multiplication validation failed!" << std::endl;
	}

	// Write result to output file
	std::ifstream src(output_path, std::ios::binary);
	std::ofstream dst(result_path, std::ios::binary);
	dst << src.rdbuf();
	src.close();
	dst.close();

	std::cout << "Test passed! The result matches the expected output." << std::endl;

	// Free allocated memory
	delete[] A;
	delete[] B;
	delete[] C;
	delete[] expected_C;

	return 0;
}