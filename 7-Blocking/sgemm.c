#include <stdio.h>

#define OFF(row, col) row *n + col

#ifndef BLOCKSIZE
#define BLOCKSIZE 8
#endif

const char *sgemm_desc = "sgemm using cache blocking"; // Aqui puedes dar una pequeña descripcion de tu programa

void do_block(int n, int si, int sj, int sk,
              float *A, float *B, float *C)
{
  for (int i = si; i < si + BLOCKSIZE; i++)
    for (int j = sj; j < sj + BLOCKSIZE; j++)
    {
      register float cij = C[OFF(i, j)]; /* cij = C[i][j] */
      for (int k = sk; k < sk + BLOCKSIZE; k++)
        cij += A[OFF(i, k)] * B[OFF(k, j)]; /* cij += A[i][k]*B[k][j] */
      C[OFF(i, j)] = cij;                   /*C[i][j] = cij*/
    }
}

void square_sgemm(int n, float *A, float *B, float *C)
{
  if (n < BLOCKSIZE)
  {
    printf("matrix size should be larger than blocksize\n");
    return;
  }
  printf("Using cache blocking with block size %d\n", BLOCKSIZE);
  for (int sj = 0; sj < n; sj += BLOCKSIZE)
    for (int si = 0; si < n; si += BLOCKSIZE)
      for (int sk = 0; sk < n; sk += BLOCKSIZE)
        do_block(n, si, sj, sk, A, B, C);
}
