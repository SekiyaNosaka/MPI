// author: nosaka

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define X 100
#define Y 100
#define Z 100


void initArray(double *arr1, double *arr2, double *arr3)
{
    int i, j;
    
    // rotation matrix
    for(i=0;i<X;i++){
        for(j=0;j<Y;j++){
            arr1[i*Y+j] = (double)rand()/RAND_MAX;
        }
    }

    // coordinate vector
    for(i=0;i<Y;i++){
        for(j=0;j<Z;j++){
            arr2[i*Z+j] = (double)(rand()%10+1);
        }
    }

    // result matrix
    for(i=0;i<X;i++){
        for(j=0;j<Z;j++){
            arr3[i*Z+j] = 0.0;
       }
    }
}

// self made(not used)
void calcProduct(double *arr1, double *arr2, double *arr3, int rank)
{
    int i, j, k;
    
    for(i=rank;i<rank+1;i++){
        for(j=0;j<Z;j++){
            for(k=0;k<Y;k++){
                //arr3[i*Z+j]+=arr1[i*Y+k]*arr2[k*Z+j];
                arr3[j] += arr1[i*Y+k]*arr2[k*Z+j];
            }
        }
    }
}

// 参考 https://www.j-focus.or.jp/old_site/activities/20090616pic/mpi_jirei.pdf
void matMul(int row, int mid, int col,
            double *arr1, double *arr2, double *arr3,
            double *arr1_2, double *arr3_2, int rank, int size)
{
    int i, j, k;
    int rows = row/size;

    MPI_Scatter(arr1,   rows*mid, MPI_DOUBLE,
                arr1_2, rows*mid, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    MPI_Bcast(arr2, mid*col, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for(i=0;i<rows;i++){
        for(j=0;j<col;j++){
            arr3_2[i*col+j] = 0.0;
            for(k=0;k<mid;k++){
                arr3_2[i*col+j] += arr1_2[i*mid+k]*arr2[k*col+j];
            }
        }
    }
    
    MPI_Gather(arr3_2, rows*col, MPI_DOUBLE,
               arr3,   rows*col, MPI_DOUBLE,
               0, MPI_COMM_WORLD);
}

void display(double *arr1, double *arr2, double *arr3)
{
    int i, j;

    printf("rotation matrix\n");
    for(i=0;i<X;i++){
        for(j=0;j<Y;j++){
            printf("%lf ", arr1[i*Y+j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("coordinate vector\n");
    for(i=0;i<Y;i++){
        for(j=0;j<Z;j++){
            printf("%lf ", arr2[i*Z+j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("result matrix\n");
    for(i=0;i<X;i++){
        for(j=0;j<Z;j++){
            printf("%lf ", arr3[i*Z+j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

double calcFlops(double t, int size)
{
    double flops;
    flops = (double)(X/size)*Y*Z*2.0/t;
    return flops;
}

int main(int argc, char** argv)
{
    int myrank;
    int size;

    double start;
    double t_1, t_1_reduce;
    double flops;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *m_a = (double *)malloc(sizeof(double)*X*Y);
    double *m_b = (double *)malloc(sizeof(double)*Y*Z);
    double *m_c = (double *)malloc(sizeof(double)*X*Z);

    double *m_a_2 = (double *)malloc(sizeof(double)*(X/size)*Y);
    double *m_c_2 = (double *)malloc(sizeof(double)*(X/size)*Z);

    initArray(m_a, m_b, m_c);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();
    
    matMul(X, Y, Z,
           m_a, m_b, m_c,
           m_a_2, m_c_2, myrank, size);

    MPI_Barrier(MPI_COMM_WORLD);
    t_1 = MPI_Wtime()-start;

    MPI_Reduce(&t_1, &t_1_reduce, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    //MPI_Reduce(&t_1, &t_1_reduce, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(myrank==0){
        display(m_a, m_b, m_c);

        flops = calcFlops(t_1_reduce, size);
        printf("parallel_version\n");
        printf("TIME = %lf [s]\n", t_1_reduce);
        printf("FLOPS = %lf\n", flops);
        printf("MFLOPS = %lf\n", flops*1.0e-6);
    }

    free(m_a);
    free(m_b);
    free(m_c);
    free(m_a_2);
    free(m_c_2);

    MPI_Finalize();

    return 0;
}
