/* 
First reduction program
by R. Hayashi, 2017

Compile command example: mpicc simpleMPIhello.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)

X: number of nodes

ノード間の集計を行う
*/

#include "mpi.h"
#include "stdio.h"


int main(int argc, char** argv)
{
  int myrank;
  int ranksum;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  MPI_Reduce(
      &myrank, 
      &ranksum,
      1,
      MPI_INT,
      MPI_SUM,
      0,
      MPI_COMM_WORLD
  );

  if(myrank==0) printf("Summation of node No. = %d.\n",ranksum);

  MPI_Finalize();
}
