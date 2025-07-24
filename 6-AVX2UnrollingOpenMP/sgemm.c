#include <omp.h>
#include <immintrin.h>

#define OFF(row, col) row *n + col

#ifndef UNROLL
#define UNROLL 4
#endif

/*  to specify the number of threads:
With an environment variable
         export OMP_NUM_THREADS=4
With a function provided by OpenMP
         omp_set_num_threads(NTHREADS)
*/

#ifndef NTHREADS
#define NTHREADS 2 /* Try different number of threads like 1,2,4,8,16 */
#endif

const char *sgemm_desc = "sgemm using multiple threads (OpenMP) and unrolling+AVX2";

void square_sgemm(int n, float *A, float *B, float *C)
{
  omp_set_num_threads(NTHREADS);
#pragma omp parallel for

  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; j += UNROLL * 8) /* procesamos 8 floats (256 bits) */
    {
      __m256 cij[UNROLL];

      for (int x = 0; x < UNROLL; x++)
        cij[x] = _mm256_loadu_ps(&C[OFF(i, j + x * 8)]); /* cij[x] = C[i][j+x..j+x+7] */

      for (int k = 0; k < n; ++k)
      {
        __m256 aik = _mm256_set1_ps(A[OFF(i, k)]); // A[i][k] broadcast

        for (int x = 0; x < UNROLL; x++)
        {
          __m256 bkj = _mm256_loadu_ps(&B[OFF(k, j + x * 8)]); // B[k][j..j+7]
          cij[x] = _mm256_fmadd_ps(aik, bkj, cij[x]);          // cij += A[i][k] * B[k]

          _mm256_storeu_ps(&C[OFF(i, j + x * 8)], cij[x]); // Guardamos en C[i][j..j+7]
        }
      }
    }
  }
}
