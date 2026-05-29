#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1024
#define NUM_THREADS 4

int main(int argc, char* argv[]) {
    // argv[1]=schedule_type (static/dynamic/guided), argv[2]=chunk_size
    char *sched = (argc > 1) ? argv[1] : "static";
    int chunk   = (argc > 2) ? atoi(argv[2]) : 64;
    static double A[N][N], B[N][N], C[N][N];
    srand(42);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = (double)rand() / RAND_MAX;
            B[i][j] = (double)rand() / RAND_MAX;
        }
    omp_set_num_threads(NUM_THREADS);
    char env_str[64];
    snprintf(env_str, 64, "%s,%d", sched, chunk);
    setenv("OMP_SCHEDULE", env_str, 1);
    double start = omp_get_wtime();
    #pragma omp parallel for schedule(runtime)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
        }
    double end = omp_get_wtime();
    printf("SCHED=%s CHUNK=%d TIME=%.4f\n", sched, chunk, end - start);
    return 0;
}
