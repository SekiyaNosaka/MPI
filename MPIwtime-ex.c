/* 
time measure program
by R. Hayashi, 2017

Compile command example: mpicc progname.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)
X: number of nodes

時間計測
*/

#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"


int main(argc, argv)
int argc;
char **argv;
{
  int n=3;/* number of data */
  double start,time;
  int myrank;/* number of each node */
  int size;/* The number of process */
  int sendmsg[n];/* Array for send */
  int recvmsg[n];/* Array for receive */
  int rnode;/* right node */
  int lnode;/* left node */
  int i;
  MPI_Status status;

  /* MPI initiation and important setting */
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if(myrank==0){
        printf("No. of process = %d.\n",size);
        printf("No. of data = %d.\n",n);
        printf("Time tick = %16.6E seconds.\n",MPI_Wtick());
  }

  rnode=(myrank+1)%size;
  lnode=(myrank-1+size)%size;

  printf("I am %d, right node=%d, left node=%d.\n",myrank, rnode,lnode);

  /* Data setting */
  for(i=0;i<n;i++) {
    sendmsg[i]=n*myrank+i;
    printf("Node:%d, %d th data= %d.\n",myrank, i, sendmsg[i]);
  }

  /* Communication */
  MPI_Barrier(MPI_COMM_WORLD);
  start=MPI_Wtime();

  MPI_Sendrecv(
	       &sendmsg[0],n,MPI_INT,rnode,0,
	       &recvmsg[0],n,MPI_INT,lnode,0,
	       MPI_COMM_WORLD, &status
	       );
  time=MPI_Wtime()-start;

  MPI_Barrier(MPI_COMM_WORLD);

  printf("Node %d used time = %f seconds.\n",myrank,time);

  MPI_Finalize();
}
