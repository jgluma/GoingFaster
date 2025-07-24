#include <stdlib.h> // For: exit, drand48, malloc, free, NULL, EXIT_FAILURE
#include <stdio.h>  // For: perror
#include <string.h> // For: memset
#include <limits.h> // For: INT_MAX

#ifdef GETTIMEOFDAY
#include <sys/time.h> // For struct timeval, gettimeofday
#else
#include <time.h> // For struct timespec, clock_gettime, CLOCK_MONOTONIC
#endif

// Performance counters
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>

#define NUM_EVENTS 6

static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
  return syscall(__NR_perf_event_open, hw_event, pid, cpu,
                 group_fd, flags);
}

struct perf_event_attr pe[NUM_EVENTS];
int fds[NUM_EVENTS];
uint64_t values[NUM_EVENTS];
int configs[NUM_EVENTS] = {
    PERF_COUNT_HW_INSTRUCTIONS,
    PERF_COUNT_HW_CPU_CYCLES,
    PERF_COUNT_HW_CACHE_REFERENCES,
    PERF_COUNT_HW_CACHE_MISSES,
    PERF_COUNT_HW_BRANCH_INSTRUCTIONS,
    PERF_COUNT_HW_BRANCH_MISSES};

/* Your function must have the following signature: */
extern const char *sgemm_desc;
extern void square_sgemm(int, float *, float *, float *);

/* Auxiliary functions */

double wall_time()
{
#ifdef GETTIMEOFDAY
  struct timeval t;
  gettimeofday(&t, NULL);
  return 1. * t.tv_sec + 1.e-6 * t.tv_usec;
#else
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return 1. * t.tv_sec + 1.e-9 * t.tv_nsec;
#endif
}

void die(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

/* This routine performs a reference sgemm operation to check correctness
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in row-major format.
 * On exit, A and B maintain their input values. */
void ref_square_sgemm(int n, float *A, float *B, float *C)
{
  /* For each row i of A */
  for (int i = 0; i < n; ++i)
    /* For each column j of B */
    for (int j = 0; j < n; ++j)
    {
      /* Compute C(i,j) */
      float cij = C[i * n + j]; /* cij = C[i][j] */
      for (int k = 0; k < n; k++)
        cij += A[k + i * n] * B[j + k * n]; /* cij += A[i][k]*B[k][j] */
      C[i * n + j] = cij;                   /*C[i][j] = cij*/
    }
}

void check_correctness(int n, float *A, float *B, float *C, float *Cref)
{
  float dif = 0, max_error = 0.00001;
  ref_square_sgemm(n, A, B, Cref);
  for (int x = 0; x < n * n; x++)
    dif += abs(C[x] - Cref[x]);
  if (dif > max_error)
    printf("Bad multiplication routine, absolute error: %f\n", dif);
  else
    printf("Multiplication routine works correctly\n");
}

void fill(float *p, int n)
{
  for (int i = 0; i < n; ++i)
    p[i] = ((float)rand() / (float)RAND_MAX) * 2 - 1; // Uniformly distributed over [-1, 1]
}

void flush_cache()
{
  int bigsize = 10000000;
  float *clr = NULL;
  clr = (float *)calloc(bigsize, sizeof(float));
  if (clr == NULL)
    die("failed to allocate buffer");
  fill(clr, bigsize);
}

void perf_start()
{
  for (int i = 0; i < NUM_EVENTS; ++i)
  {
    memset(&pe[i], 0, sizeof(struct perf_event_attr));
    pe[i].type = PERF_TYPE_HARDWARE;
    pe[i].size = sizeof(struct perf_event_attr);
    pe[i].config = configs[i];
    pe[i].disabled = 1;
    pe[i].exclude_kernel = 1;
    pe[i].exclude_hv = 1;

    fds[i] = perf_event_open(&pe[i], 0, -1, (i == 0 ? -1 : fds[0]), 0);
    if (fds[i] == -1)
    {
      perror("perf_event_open");
      exit(EXIT_FAILURE);
    }
  }

  ioctl(fds[0], PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
  ioctl(fds[0], PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
}

void end_perf()
{
  ioctl(fds[0], PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

  for (int i = 0; i < NUM_EVENTS; ++i)
  {
    read(fds[i], &values[i], sizeof(uint64_t));
    close(fds[i]);
  }

  const char *names[NUM_EVENTS] = {
      "Instructions",
      "CPU Cycles",
      "Cache References",
      "Cache Misses",
      "Branch Instructions",
      "Branch Misses"};

  printf("\n[PERF RESULTS]\n");
  for (int i = 0; i < NUM_EVENTS; ++i)
  {
    printf("%s: %lu\n", names[i], values[i]);
  }
}

/* The benchmarking program */
void print_usage(const char *program_name)
{
  printf("Usage: %s [options] [size]\n", program_name);
  printf("Options:\n");
  printf("  -c          Check correctness of the multiplication\n");
  printf("  -p          Enable performance counters\n");
  printf("  -h, --help  Show this help message\n");
  printf("  size        Matrix size (positive integer, default: 512)\n");
  printf("\nExamples:\n");
  printf("  %s 1024      # Run with 1024x1024 matrices\n", program_name);
  printf("  %s -c 256    # Check correctness with 256x256 matrices\n", program_name);
  printf("  %s -p -c 512 # Run with performance counters and correctness check\n", program_name);
}

int main(int argc, char **argv)
{
  double seconds = 0;
  int n = 512; // Default matrix size
  int check_correctness_flag = 0;
  int performance_counters = 0;

  // Parse all command line arguments
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-c") == 0)
    {
      check_correctness_flag = 1;
    }
    else if (strcmp(argv[i], "-p") == 0)
    {
      performance_counters = 1;
    }
    else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
    {
      print_usage(argv[0]);
      return 0;
    }
    else if (argv[i][0] != '-')
    {
      // This should be the matrix size
      char *endptr;
      long parsed_n = strtol(argv[i], &endptr, 10);

      // Check if the entire string was consumed and the value is valid
      if (*endptr != '\0' || parsed_n <= 0 || parsed_n > INT_MAX)
      {
        fprintf(stderr, "Error: Invalid matrix size '%s'. Must be a positive integer.\n", argv[i]);
        print_usage(argv[0]);
        return EXIT_FAILURE;
      }
      n = (int)parsed_n;
    }
    else
    {
      fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
      print_usage(argv[0]);
      return EXIT_FAILURE;
    }
  }

  printf("Description:\t%s on matrices of size %d x %d\n", sgemm_desc, n, n);
  if (check_correctness_flag)
    printf("Correctness check: ENABLED\n");
  if (performance_counters)
    printf("Performance counters: ENABLED\n");
  printf("\n");

  /* allocate memory */
  float *buf = NULL;
  buf = (float *)malloc(4 * n * n * sizeof(float));
  if (buf == NULL)
    die("failed to allocate matrices");

  /* set pointers */
  float *A = buf + 0;
  float *B = A + n * n;
  float *C = B + n * n;
  float *Cref = C + n * n;

  srand(time(NULL)); /* getting a new seed*/
  fill(A, n * n);
  fill(B, n * n);
  fill(C, n * n);
  memcpy(Cref, C, n * n * sizeof(float));

  seconds = wall_time();
  if (performance_counters)
    perf_start();
  square_sgemm(n, A, B, C);
  if (performance_counters)
    end_perf();
  seconds = wall_time() - seconds;

  printf("Time %.4f s\n", seconds);

  // Check correctness if requested
  if (check_correctness_flag)
  {
    check_correctness(n, A, B, C, Cref);
  }

  free(buf);

  return 0;
}
