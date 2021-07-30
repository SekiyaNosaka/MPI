/* 
First send & receive program
by R. Hayashi, 2017

Compile command example: mpicc simpleMPIhello.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)

Comparing MPI_Bcast and bucket relay PtoP communication 

X: number of nodes

ファイル I/O
*/

#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"


int main(argc, argv)
int argc;
char **argv;
{
  int n=3;/* number of data */
  int myrank;/* number of each node */
  int size;/* The number of process */
  FILE *fp;
  char filename[10];
  int i;
  MPI_Status status;

  /* MPI initiation and important setting */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if(myrank==0){
    printf("No. of process = %d.\n",size);
  }

  sprintf(filename,"fname%d",myrank);
  fp=fopen(filename,"w");

  if(fp==NULL){
    printf("Node %d could NOT open file. \n",myrank);
  }
  else{
    printf("Node %d open file. \n",myrank);
  }

  /* file output */
  fprintf(fp,"Number of data = %5d\n",n);
  for(i=0;i<n;i++) {
    fprintf(fp,"%5d\n",n*myrank+i);
  }

  fclose(fp);
  MPI_Finalize();

}
