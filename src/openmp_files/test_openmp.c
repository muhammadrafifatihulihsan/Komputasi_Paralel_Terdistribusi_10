#include <stdio.h>
#include <omp.h>

int main() {
    printf("Total threads tersedia: %d\n", omp_get_max_threads());
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        printf("Hello dari thread %d\n", id);
    }
    return 0;
}
