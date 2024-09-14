#include <assert.h>
#include <pthread.h>
#include <math.h>

// 设置cpu核数,均为2的幂次
#define CPU_CORES 8

// 定义结构体args_gemm_t用于传入矩阵乘法的参数
// lda, ldb, ldc暂时无用
typedef struct 
{
    int m_start;
    int m_end;
    int n_start;
    int n_end;
    int m;
    int n;
    int k;
    double *a;
    // int lda;
    double *b;
    // int ldb;
    double *c;
    // int ldc;
} args_gemm_t;

// 线程执行函数
void *naive_gemm(void *arg)
{
    // 获取参数
    args_gemm_t *args = (args_gemm_t *) arg;
    int m_start = args->m_start, m_end = args->m_end,
        n_start = args->n_start, n_end = args->n_end,
        m = args->m, n = args->n, k = args->k;
        // lda = args->lda, ldb = args->ldb, ldc = args->ldc,
    double *a = args->a, *b = args->b, *c = args->c;

    int i, j, p;

    // 计算c(m_start, n_start)到c(m_end, n_end)的结果
    for (i = m_start; i < m_end; i += 1) 
    {
        for (j = n_start; j < n_end; j += 1) 
        {
            for (p = 0; p < k; p += 1) 
            {
                // c(i, j) = a(i, p) * b(p, j)
                c[i * n + j] += a[i * k + p] * b[p * n + j];
            }
        }
    }
    return NULL;
}

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    /*
     * 确定分块参数
     * 示例:
     * 对CPU_CORES = 8
     * block_row = 4, block_col = 2
     * 表示将矩阵横切为4块, 竖切为2块, 共八块, 对应cpu核数
     */
    pthread_t p[CPU_CORES];
    int block_col = (int)sqrt(CPU_CORES);  
    int block_row = CPU_CORES / block_col;  

    int m_gap = m / block_row;
    int n_gap = n / block_col;
    int m_start, m_end,
        n_start, n_end;

    int rc;
    int i = 0;

    // 创建线程
    for (m_start = 0, m_end = m_gap;
         m_start < m;
         m_start += m_gap, m_end += m_gap)
    {
        for (n_start = 0, n_end = n_gap;
             n_start < n;
             n_start += n_gap, n_end += n_gap)
        {
            args_gemm_t args = {m_start, m_end, n_start, n_end, m, n, k, a, b, c};
            rc = pthread_create(&p[i++], NULL, naive_gemm, &args); assert (rc == 0);
        }
    }

    // 运行线程并在结束后回收资源
    for (i = 0; i < CPU_CORES; i++)
    { 
        rc = pthread_join(p[i], NULL); assert (rc == 0);
    }
}
