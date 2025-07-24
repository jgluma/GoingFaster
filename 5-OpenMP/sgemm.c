#include <omp.h>

#define OFF(row, col) row *n + col

/*  to specify the number of threads:
With an environment variable:        export OMP_NUM_THREADS=4
With a function provided by OpenMP:   omp_set_num_threads(NTHREADS)
*/

#ifndef NTHREADS
#define NTHREADS 2 /* Try different number of threads like 1,2,4,8,16 */
#endif

const char *sgemm_desc = "A naive C version";

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in row-major format.
 * On exit, A and B maintain their input values. */
void square_sgemm(int n, float *A, float *B, float *C)
{

  omp_set_num_threads(NTHREADS);

#pragma omp parallel for
  /* For each row i of C */
  for (int i = 0; i < n; ++i)
    /* For each column j of C */
    for (int j = 0; j < n; ++j)
    {
      /* Compute C(i,j) */
      register float cij = C[OFF(i, j)]; /* cij = C[i][j] */
      for (int k = 0; k < n; k++)
        cij += A[OFF(i, k)] * B[OFF(k, j)]; /* cij += A[i][k]*B[k][j] */
      C[OFF(i, j)] = cij;                   /*C[i][j] = cij*/
    }
}
