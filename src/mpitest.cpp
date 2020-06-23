#include "mpitest.h"

#include <mpi.h>

void HelloMPI(int argc, char* argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Finalize();
}
