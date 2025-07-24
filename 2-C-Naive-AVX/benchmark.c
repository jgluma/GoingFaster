#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 2048

double gettime(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

int main() 
{
    float *a, *b, *c;
    int i, j, k;
    float sum = 0.0;
    double dtime;

    // Allocate aligned memory
    posix_memalign((void **)&a, 32, N * N * sizeof(float));
    posix_memalign((void **)&b, 32, N * N * sizeof(float));
    posix_memalign((void **)&c, 32, N * N * sizeof(float));

    // Initialize input matrices
    for (i = 0; i < N * N; i++) {
        a[i] = b[i] = 3.14f;
        c[i] = 0.0f;
    }

    // Matrix multiplication
    dtime = gettime();

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            float cij = 0.0f;
            for (k = 0; k < N; k++) {
                cij += a[i * N + k] * b[k * N + j];
            }
            c[i * N + j] = cij;
        }
    }

    dtime = gettime() - dtime;

    // Accumulate result
    for (i = 0; i < N * N; i++) {
        sum += c[i];
    }

    printf("Sum for matrix C: %12.4f\n", sum);
    printf("Exec time: %9.5f sec.\n", dtime);

    free(a); free(b); free(c);
    return 0;
}
