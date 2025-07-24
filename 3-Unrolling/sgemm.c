#define OFF(row, col) row *n + col

#ifndef UNROLL
#define UNROLL 8
#endif

const char *sgemm_desc = "sgemm using loop unrolling";

void square_sgemm(int n, float *A, float *B, float *C)
{
  /* For each row i of A */
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j += UNROLL)
    {
      float cij[UNROLL];
      for (int x = 0; x < UNROLL; x++)
        cij[x] = C[OFF(i, j) + x];

      for (int k = 0; k < n; k++)
      {
        for (int x = 0; x < UNROLL; x++)
          cij[x] += A[OFF(i, k)] * B[OFF(k, j) + x];
      }

      for (int x = 0; x < UNROLL; x++)
        C[OFF(i, j) + x] = cij[x];
    }
}
