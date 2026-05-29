#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int N = (argc > 1) ? atoi(argv[1]) : 512;
    int T = (argc > 2) ? atoi(argv[2]) : 4;
    omp_set_num_threads(T);
    double *A = malloc(N*N*sizeof(double));
    double *B = malloc(N*N*sizeof(double));
    double *C = malloc(N*N*sizeof(double));
    if (!A || !B || !C) { printf("Alokasi gagal\n"); return 1; }
    srand(42);
    for (int i = 0; i < N*N; i++) {
        A[i] = (double)rand()/RAND_MAX;
        B[i] = (double)rand()/RAND_MAX; }
    double start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i*N+k] * B[k*N+j];
            C[i*N+j] = sum;
        }
    printf("N=%d T=%d TIME=%.4f\n", N, T, omp_get_wtime()-start);
    free(A); free(B); free(C);
    return 0;
}
