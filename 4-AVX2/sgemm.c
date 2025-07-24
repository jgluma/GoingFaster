#include <immintrin.h>
#define OFF(row, col) row *n + col

const char *sgemm_desc = "sgemm with C intrinsics to generate the AVX2 subword-parallel instructions for the x86";

void square_sgemm(int n, float *A, float *B, float *C)
{
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; j += 8) /* procesamos 8 floats (256 bits) */
    {
      __m256 cij = _mm256_load_ps(&C[OFF(i, j)]); /* c0 = C[i][j..j+7] */
      for (int k = 0; k < n; ++k)
      {
        __m256 aik = _mm256_set1_ps(A[OFF(i, k)]);  // A[i][k] broadcast
        __m256 bkj = _mm256_load_ps(&B[OFF(k, j)]); // B[k][j..j+7]
        cij = _mm256_fmadd_ps(aik, bkj, cij);       // cij += A[i][k] * B[k][j..j+7]
      }
      _mm256_store_ps(&C[OFF(i, j)], cij); // Guardamos en C[i][j..j+7]
    }
  }
}
