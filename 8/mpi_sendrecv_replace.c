#include <stdio.h>
#include <stdlib.h>
#include "../Vector/vector.h"
#include <time.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    int ProcNum, ProcRank, RecvRank;
    MPI_Init(&argc, &argv);
    double start = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int isRoot = ProcRank == 0;
    int size_comm = atoi(argv[1]);
    int num_repeat = atoi(argv[2]);

    vector_t vec = init_vector(size_comm);
    vec.size = size_comm;

    for (int i = 0; i < size_comm; ++i) {
        vec.array[i] = rand() % 1000;
    }
    for (int k = 0; k < num_repeat; ++k) {
        MPI_Sendrecv_replace(vec.array, size_comm, MPI_INT, !ProcRank, k, !ProcRank, k, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    double end = MPI_Wtime();
    MPI_Finalize();
    
    free(vec.array);

    if (isRoot){
        printf("%f\n", end - start);
    }
    return 0;
}