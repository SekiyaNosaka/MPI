/* 
Various communication program
by R. Hayashi, 2017

Compile command example: mpicc progname.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)
X: number of nodes

データ回収
各ノードで個別に生成したデータをノード０に集める

*/

#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"


int main(int argc, char **argv)
{
  int n=3;/* number of data */
  int maxp=64;/* maximum number of process */
  int myrank;/* number of each node */
  int size;/* The number of process */
  int sendmsg[n];/* Array for send */
  int recvmsg[n*maxp];/* Array for receive */
  int i,ierr;
  MPI_Status status;

  /* MPI initiation and important setting */
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
  MPI_Comm_size( MPI_COMM_WORLD, &size );

  if(maxp<size){
    if(myrank==0) printf("No. of process exceeds maximum of %d. Execution will stop.\n",maxp);
    MPI_Abort(MPI_COMM_WORLD,ierr);
    // MPI_Finalize();
  }

  if(myrank==0){
      printf("No. of process = %d.\n",size);
      printf("No. of data = %d.\n",n);
  }

  /* Data setting */
  MPI_Barrier(MPI_COMM_WORLD);
  for(i=0;i<n;i++){
    sendmsg[i]=n*myrank+i;
    printf("Node:%d, %d th data= %d.\n",myrank, i, sendmsg[i]);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  /* Communication */
  MPI_Gather(
	     //&sendmsg[0],n,MPI_INT,
	     &sendmsg,n,MPI_INT,
	     //&recvmsg[0],n,MPI_INT,
	     &recvmsg,n,MPI_INT,
	     0,MPI_COMM_WORLD
  );

  if(myrank==0){
        printf("Node 0 have the data of:\n");
	for(i=0;i<n*size;i++) printf("%d\n",recvmsg[i]);
  }

  MPI_Finalize();
}
