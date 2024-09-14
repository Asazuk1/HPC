#include<stdio.h>
#include<unistd.h>
#include<omp.h>

void MY_MMult(int m, int n, int k, 
              double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    int i, j, p;

    #pragma omp parallel for 
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