# Parallel Programming

## Åbo Akademi University, Information Technology Department

### Instructor: Alireza Olama

## Homework Assignment 1: Naive Matrix Multiplication in C++

**Due Date:** **22/04/2025**

**Points:** **100**

### Alexis Run (Linux/Mac)

Give user the permission to execute the `run_all.sh` script.

```bash
chmod +x run_all.sh
```

Then execute the script to run all test cases automatically.:

```bash
./run_all.sh
```

### Assignment Overview

Welcome to the first homework assignment of the `Parallel Programming` course! This is a warm-up exercise designed to
get you comfortable with low-level memory management and basic matrix operations in `C++`. You will implement a naive
matrix multiplication function (`naive_matmul`) that computes `C = A * B` using C-style arrays and manual memory
management. This assignment focuses on `file I/O`, `memory management`, and understanding `matrix storage in memory`,
setting the stage for parallelization topics later in the course.

**Note:** This assignment does _not_ involve parallel programming. You will run your implementation locally on your
machine.

---

### Technical Requirements

#### Matrix Multiplication

- Implement the `naive_matmul` function to perform matrix multiplication `C = A * B` using a triple nested
  loop (naive method).
- Input matrices \( A \) and \( B \) will be provided in two text files: `input0.raw` (for \( A \)) and `input1.raw` (
  for \( B \)).
- The resulting matrix \( C \) must be written to a text file named `result.raw`.
- The first row of `input0.raw` and `input1.raw` contains matrix dimensions (number of rows and columns).

#### Memory Management

- Use only C-style arrays with manual memory allocation (`malloc` or `new`) and deallocation (`free` or `delete`).
- Do _not_ use smart pointers, STL containers (e.g., `std::vector`), or other dynamic memory libraries.

#### Matrix Storage

- Store all matrices as 1D arrays in **row-major order**.
- **Row-Major vs. Column-Major Explanation:**
  - In row-major order, elements of a matrix are stored row-by-row in memory. For a matrix \( `A` \) with
    dimensions \(
    `M * N` \), the element \( `A[i][j]` \) is accessed at index \( `i * N + j` \) in a 1D array.
  - In contrast, column-major order stores elements column-by-column (used in languages like Fortran). Since C++ uses
    row-major order by default, we adopt it here.
  - For more details, refer
    to: [Row- and Column-Major Order](https://en.wikipedia.org/wiki/Row-_and_column-major_order).

#### Validation

- A reference solution file (`output.raw`) is provided.
- Compare your `result.raw` with `output.raw` to validate correctness. The test passes if they match
  exactly.

#### Build Instructions

- Use **CMake** to configure and build your project. A sample `CMakeLists.txt` is provided below.
- **Windows Users:**
  - Option 1: Use CLion with CMake (recommended). Load the project and build/run.
  - Option 2: Use Visual Studio with CMake support. Open the folder and let VS configure it.
  - Alternatively, install MinGW and run `cmake -G "MinGW Makefiles"` followed by `make`.
- **Linux/Mac Users:**
  - Run `cmake .` to generate a Makefile, then `make` to build the executable.

---

The input files for this assignment are located in the `data` folder, which contains 10 test cases organized into
subfolders named `0` through `9`. Each subfolder includes an `input0.raw` file (matrix \( A \)), an `input1.raw` file (
matrix \( B \)), and a reference output file `output.raw` (matrix \( `C = A * B` \)). Your executable should accept a
case number (an integer from `0` to `9`) as a command-line argument, use it to locate the corresponding folder in
`data`, and perform matrix multiplication using the input files in that folder. The resulting matrix should be written
to `result.raw` within the same subfolder. This setup allows automated testing across all 10 cases.

---

### Submission Requirements

1. **Fork this Repository**

2. **Clone the Fork**

   ```bash
   git clone https://github.com/parallelcomputingabo/Homework-1.git
   cd Homework-1
   ```

3. **Create a New Branch**

   ```bash
   git checkout -b student-name
   ```

4. **Implement Your Solution**

5. **Commit and Push**

   ```bash
   git add .
   git commit -m "student-name: Implemented naive matrix multiplication"
   ```

   ```
   git push origin student-name
   ```

6. **Submit a Pull Request (PR)**
   - Go to your forked repository on GitHub.
   - Click on the "Pull Requests" tab.
   - Click on "New Pull Request".
   - Select the base repository and branch (e.g., `main`) and compare it with your branch.
   - Add a title and description, then click "Create Pull Request".

---

### Grading (100 Points Total)

| **Subtask**                                  | **Points** |
| -------------------------------------------- | ---------- |
| File reading and memory allocation           | 20         |
| Matrix multiplication correctness            | 30         |
| Correct use of row-major indexing            | 15         |
| File writing and deallocation                | 15         |
| Code clarity and commenting                  | 10         |
| Successful test comparison with `output.raw` | 10         |
| **Total**                                    | **100**    |

---

Good luck, and have fun coding!
