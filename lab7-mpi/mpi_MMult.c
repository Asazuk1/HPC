#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mpi.h"

// 矩阵A, B, C规模均为 N * N
#define N 2

// naive版本的矩阵乘法
void naive_MMult(int m, int n, int k,
                 double *a, double *b, double *c)
{
    int i, j, p;

    for (i = 0; i < m; i += 1) 
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

// 打印规模为m * n的矩阵a 
void print_matrix(double *a, int m, int n)
{
    for (int i = 0; i < m; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            printf("%.7f\t", a[i * n + j]);
        }
        printf("\n");
    }
}

// 将矩阵b复制给矩阵a, 作为a的初始值
void init_matrix(double *a, double b[], int m, int n)
{
    for (int i = 0; i < m; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            a[i * n + j] = b[i * n + j];
        }
    }
}

// 将m * n的矩阵a全部初始化为val
void init_matrix_all_same(double *a, int m, int n, double val)
{
    for (int i = 0; i < m; i += 1)
    {
        for (int j = 0; j < n; j += 1)
        {
            a[i * n + j] = val;
        }
    }
}


int main(int argc, char *argv[])
{
    int proc_num, proc_id;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_num);

    // 分块方式: 将C横切为N / proc_num(进程数)块
    int proc_row = N / proc_num;
    double *A = NULL, *B = NULL, *C = NULL;
    // A_local和C_local为每个线程各自存储的结果
    double *A_local = (double *)malloc(proc_row * N * sizeof(double));
    double *C_local = (double *)malloc(proc_row * N * sizeof(double));
    
    if (proc_id == 0) {
        /* 初始化矩阵 A, B, C */
        A = (double *)malloc(N * N * sizeof(double));
        B = (double *)malloc(N * N * sizeof(double));
        C = (double *)malloc(N * N * sizeof(double));

        double A_init[N * N] = {
            -1.000000e+00,
            -9.167380e-01,
            -9.980292e-01,
            -6.467147e-01
        };

        double B_init[N * N] = {
            -2.707955e-01,
            -8.154047e-01,
            -8.173388e-01,
            -2.556555e-02
        };

        init_matrix(A, A_init, N, N);
        init_matrix(B, B_init, N, N);
        init_matrix_all_same(C, N, N, 0.0);

        printf("矩阵A初始:\n");
        print_matrix(A, N, N);
        printf("矩阵B初始:\n");
        print_matrix(B, N, N);
        printf("矩阵C初始:\n");
        print_matrix(C, N, N);

        // 主进程A_local即自身A
        for (int i = 0; i < proc_row * N; i += 1)
        {
            A_local[i] = A[i];
        }

        /* 将初始化的数据发送给其他进程 */
        for (int i = 1; i < proc_num; i += 1)
        {
            MPI_Send(A + i * proc_row * N, proc_row * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(B, N * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        /* 接受矩阵A, B, C */
		MPI_Recv(A_local, proc_row * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        B = (double *)malloc(N * N * sizeof(double));
        MPI_Recv(B, N * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    /* 各进程分块计算 */
    init_matrix_all_same(C_local, proc_row, N, 0.0);
    naive_MMult(proc_row, N, N, A_local, B, C_local);

    if (proc_id == 0) {
        /* 接收各子进程的计算结果，进行汇总 */
        // 主进程接受自身结果
        for (int i = 0; i < proc_row * N; i += 1)
        {    
            C[i] = C_local[i];
        }
        // 主进程接受各子进程结果
        for (int i = 1; i < proc_num; i += 1)
        {
            MPI_Recv(C + i * proc_row * N, proc_row * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
        }
        // 打印结果
        printf("矩阵C结果:\n");
        print_matrix(C, N, N);
    } else {
        /* 将计算结果发送给主进程 */
        MPI_Send(C_local, proc_row * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    // 回收内存
    free(A_local);
    free(B);
    free(C_local);
    if (proc_id == 0) {
        free(A);
        free(C);
    }

    MPI_Finalize();
	
    return 0;
}