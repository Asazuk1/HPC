#include <cblas.h>

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
    /*
    Naive implementation which regards result C as
    A(m * k) multiplies B(k * n) 
    */
    int i, j, p;

    for (i = 0; i < m ; i += 1) {
        for (j = 0; j < n; j += 1) {
            for (p = 0; p < k; p += 1) {
                /* c[i * n + j] *= beta; */
                c[i * n + j] += a[i * k + p] * b[p * n + j];
            }
        }
    }
}