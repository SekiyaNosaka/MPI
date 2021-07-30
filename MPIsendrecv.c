/* 
First send & receive program
by R. Hayashi, 2017

Compile command example: mpicc simpleMPIhello.c
You can add "-o (executable file name)" option.
Execution command example: mpirun -np X ./a.out
or
Another execution command example: mpirun -np X ./(executable file name)

X: number of nodes

バケツリレー通信
*/

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char **argv)
{
  int myrank; // 自ノード番号を入れる変数
  int size; // 使用するノード数を入れる変数
  int sendmsg; // 
  int recvmsg;
  int rnode; // 右隣のノード番号を入れる変数
  int lnode; // 左隣のノード番号を入れる変数

  MPI_Status status; // MPIの状態を入れる構造体

  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank ); // 自ノード番号を調べて変数に入れる
  MPI_Comm_size( MPI_COMM_WORLD, &size ); // 使用ノード数

  if(myrank==0) printf("Number. of process = %d.\n",size);

  MPI_Barrier(MPI_COMM_WORLD); // コミュニケータを指定

  rnode=(myrank+1)%size;
  lnode=(myrank-1+size)%size;

  printf("I am %d, right node=%d, left node=%d.\n",myrank, rnode,lnode);

  srand(myrank);
  sendmsg=rand();

  MPI_Sendrecv(
	       &sendmsg,1,MPI_INT,rnode,0,
	       &recvmsg,1,MPI_INT,lnode,0,
	       MPI_COMM_WORLD, &status
	       ); // バッファの先頭アドレス, メッセージサイズ, データタイプ, 宛先プロセスのアドレス, メッセージタグ

  MPI_Barrier(MPI_COMM_WORLD);

  printf("I am %d, I sent %d to node %d, I received %d from node %d.\n", 
                                    myrank, sendmsg,rnode,recvmsg,lnode);

  MPI_Finalize();
}
