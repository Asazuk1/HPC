#include <assert.h>
#include <pthread.h>

// 设置cpu核数
#define CPU_CORES 8

// 定义结构体args_gemm_t用于传入矩阵乘法的参数
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
    int lda;
    double *b;
    int ldb;
    double *c;
    int ldc;
} args_gemm_t;

// 线程执行函数
void *naive_gemm(void *arg)
{
    // 获取参数
    args_gemm_t *args = (args_gemm_t *) arg;
    int m_start = args->m_start, m_end = args->m_end,
        n_start = args->n_start, n_end = args->n_end,
        m = args->m, n = args->n,
        lda = args->lda, ldb = args->ldb, ldc = args->ldc,
        k = args->k;
    double *a = args->a, *b = args->b, *c = args->c;

    int i, j, p;

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
    // 设置线程
    pthread_t p[CPU_CORES];
    int ret;
    int m_gap = m / 4;
    int n_gap = n / 2;
    int m_start, m_end,
        n_start, n_end;
    int i = 0;

    for (m_start = 0, m_end = m_gap - 1;
         m_start < m;
         m_start += m_gap, n_end += n_gap)
    {
        // TODO: 确定分块矩阵参数
        for (n_start = 0, n_end = n_gap - 1;
             n_start < n;
             n_start += n_gap, n_end += n_gap)
        {
            args_gemm_t args = {m_start, m_end, n_start, n_end, m, n, k, a, lda, b, ldb, c, ldc};
            ret = pthread_create(&p[i++], NULL, naive_gemm, (void *)&args); assert (ret == 0);
        }
    }

    for (i = 0; i < CPU_CORES; i++)
    {
        ret = pthread_join(p[i], NULL); assert (ret == 0);
    }
}
