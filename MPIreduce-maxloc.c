/* 
First reduction program
by R. Hayashi, 2017

Compile command example: mpicc simpleMPIhello.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)

X: number of nodes
*/

#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"


int main(int argc, char** argv)
{
  int myrank;
  struct{
    int val;
    int rank;
  } sendval,rankmaxloc;
  MPI_Status status;

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  srand(100-myrank);
  sendval.val=rand();
  sendval.rank=myrank;

  printf("Node %d has value = %d.\n",sendval.rank,sendval.val);

  MPI_Reduce(
      &sendval, 
      &rankmaxloc,
      1,
      MPI_2INT,
      MPI_MAXLOC,
      0,
      MPI_COMM_WORLD
  );

  if(myrank==0)printf("Max value = %d on %d.\n",rankmaxloc.val,rankmaxloc.rank);

  MPI_Finalize();
}
