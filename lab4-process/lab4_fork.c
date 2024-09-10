#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

#define N 2048

void naive_gemm(int m, int n, int k, 
                double *a, int lda,
                double *b, int ldb,
                double *c, int ldc)
{
    int i, j, p;

    for (i = 0; i < m ; i += 1) 
    {
        for (j = 0; j < n; j += 1) 
        {
            for (p = 0; p < k; p += 1) 
            {
                c[i * n + j] += a[i * k + p] * b[p * n + j];
            }
        }
    }
    
}

void initialize_matrix(double *A, int m, int n)
{
    for (int i = 0; i < m; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            // 生成0.0到1.0之间的随机浮点数
            A[i * N + j] = (double) rand() / RAND_MAX;
        }
    }
}

int main()
{
    // 建立并随机初始化矩阵
    double *A = (double *)malloc(N * N * sizeof(double));
    double *B = (double *)malloc(N * N * sizeof(double));
    double *C = (double *)malloc(N * N * sizeof(double));

    srand(time(NULL));

    initialize_matrix(A, N, N);  
    initialize_matrix(B, N, N);  
    initialize_matrix(C, N, N);

    // 调用fork
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        naive_gemm(N, N, N, A, N, B, N, C, N);
    }
    else {
        // parent goes down this path (original process)
        naive_gemm(N, N, N, A, N, B, N, C, N);
    }

    // 释放内存
    free(A);
    free(B);
    free(C);

    return 0;
}