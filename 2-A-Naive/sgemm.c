#define OFF(row, col) row *n + col
const char* sgemm_desc = "A naive C version";

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are matrices stored in row-major format.
 * On exit, A and B maintain their input values. */    
void square_sgemm (int n, float* A, float* B, float* C)
{
  /* For each row i of C */
  for (int i = 0; i < n; ++i)
    /* For each column j of C */
    for (int j = 0; j < n; ++j) 
     {
     for( int k = 0; k < n; k++ )
	     C[OFF(i,j)] += A[OFF(i,k)] * B[OFF(k,j)]; /* C[i][j] += A[i][k]*B[k][j] */

     }
 }
