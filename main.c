#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_NOT(expr, msg) \
    do                        \
    {                         \
        if (expr)             \
        {                     \
            perror(msg);      \
            exit(1);          \
        }                     \
    } while (0)

void naive_matmul(float *C, float *A, float *B, uint32_t m, uint32_t n, uint32_t p)
{
    // A is m x n, B is n x p, C is m x p
    for (uint32_t i = 0; i < m; i++)
    {
        for (uint32_t j = 0; j < p; j++)
        {
            float sum = 0;
            for (uint32_t k = 0; k < n; k++)
            {
                sum += A[i * n + k] * B[k * p + j];
            }
            C[i * p + j] = sum;
        }
    }
}

int compare_results(char *path_a, char *path_b)
{
    FILE *a;
    FILE *b;

    a = fopen(path_a, "r");
    ASSERT_NOT(a == NULL, "Error reading file a for comparision");
    b = fopen(path_b, "r");
    ASSERT_NOT(b == NULL, "Error reading file b for comparision");

    int len_a, len_b;
    fseek(a, 0, SEEK_END);
    len_a = ftell(a);

    fseek(b, 0, SEEK_END);
    len_b = ftell(b);

    if (len_a != len_b)
        return 0;

    fseek(a, 0, SEEK_SET);
    fseek(b, 0, SEEK_SET);

    char ca, cb;
    for (int i = 0; i < len_a; i++)
    {
        ca = fgetc(a);
        cb = fgetc(b);
        if (ca != cb)
            return 0;
    }

    return 1;
}

/**
 * Formats a floating point number into `buf` to two-decimal precision and no trailing zeroes.
 */
void format_properly(float f, char *buf)
{
    int len = snprintf(buf, 16, "%.2f", f);
    char *ptr = buf + len - 1;
    while (*ptr == '0' && ptr >= buf)
    {
        *ptr-- = '\x00';
    }
}

int main(int argc, char **argv)
{
    char case_num;
    char input0_path[18]; // path will be 17 characters + null
    char input1_path[18];
    char result_path[18];
    char output_path[18];

    FILE *input0;
    FILE *input1;
    FILE *result;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s case_num\n", argv[0]);
        exit(2);
    }

    case_num = argv[1][0];
    if (strlen(argv[1]) != 1 || case_num < '0' || case_num > '9')
    {
        fprintf(stderr, "case_num must be a single digit in the range 0-9\n");
        exit(2);
    }

    snprintf(input0_path, sizeof(input0_path), "data/%c/input0.raw", case_num);
    snprintf(input1_path, sizeof(input1_path), "data/%c/input1.raw", case_num);
    snprintf(result_path, sizeof(result_path), "data/%c/result.raw", case_num);
    snprintf(output_path, sizeof(output_path), "data/%c/output.raw", case_num);

    // Step 1: Open and read input files
    input0 = fopen(input0_path, "r");
    ASSERT_NOT(input0 == NULL, "Error opening input0.raw");
    input1 = fopen(input1_path, "r");
    ASSERT_NOT(input1 == NULL, "Error opening input1.raw");

    int m, n, p; // A is m x n, B is n x p, C is m x p
    fscanf(input0, "%d %d", &m, &n);
    fscanf(input1, "%d %d", &n, &p);

    float *A = malloc(m * n * sizeof(float));
    ASSERT_NOT(A == NULL, "Error allocating memory for matrix A");
    float *B = malloc(n * p * sizeof(float));
    ASSERT_NOT(B == NULL, "Error allocating memory for matrix B");
    float *C = malloc(m * p * sizeof(float));
    ASSERT_NOT(C == NULL, "Error allocating memory for matrix C");

    float f;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(input0, "%f", &f);
            A[i * n + j] = f;
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < p; j++)
        {
            fscanf(input1, "%f", &f);
            B[i * p + j] = f;
        }
    }

    fclose(input0);
    fclose(input1);

    // Step 2: Perform matrix multiplication
    naive_matmul(C, A, B, m, n, p);

    // Step 3: Write result to result.raw in the same directory as input files
    result = fopen(result_path, "w+");
    ASSERT_NOT(result == NULL, "Error opening result.raw");

    fprintf(result, "%d %d\n", m, p);
    char buf[16] = {0};
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            // could do this by just seeking at the end if trailing newline was allowed
            if (j > 0)
                fputc(' ', result);
            format_properly(C[i * p + j], buf);
            fprintf(result, "%s", buf);
        }
        // for whatever reason the output.raw file does not have a newline at end of file
        if (i < m - 1)
            fputc('\n', result);
    }
    fclose(result);

    // Step 4: validate the result
    int return_val = 0;
    if (compare_results(result_path, output_path) == 0)
    {
        printf("%s and %s do not match :(\n", result_path, output_path);
        return_val = 1;
    }
    else
    {
        printf("%s and %s match\n", result_path, output_path);
    }

    // Step 5: Clean up memory
    free(A);
    free(B);
    free(C);

    return return_val;
}
