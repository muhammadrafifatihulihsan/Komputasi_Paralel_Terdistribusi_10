#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000
#define CACHE_LINE_SIZE 65
#define NUM_THREADS 16

// Teknik 1: Dengan False Sharing
double with_false_sharing() {
    int array[NUM_THREADS];
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    { int id = omp_get_thread_num();
      for (int i = 0; i < N; i++) array[id]++; }
    return omp_get_wtime() - start;
}

// Teknik 2: Padding untuk menghindari false sharing
typedef struct { int value; char pad[CACHE_LINE_SIZE - sizeof(int)]; } PaddedInt;
double with_padding() {
    PaddedInt array[NUM_THREADS];
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    { int id = omp_get_thread_num();
      for (int i = 0; i < N; i++) array[id].value++; }
    return omp_get_wtime() - start;
}

// Teknik 3: Private variable (reduction)
double with_private_var() {
    int results[NUM_THREADS];
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    { int local = 0; int id = omp_get_thread_num();
      for (int i = 0; i < N; i++) local++;
      results[id] = local; }
    return omp_get_wtime() - start;
}

int main() {
    double t1 = with_false_sharing();
    double t2 = with_padding();
    double t3 = with_private_var();
    printf("FS_TIME=%.4f\n", t1);
    printf("PAD_TIME=%.4f\n", t2);
    printf("PRIV_TIME=%.4f\n", t3);
    return 0;
}
