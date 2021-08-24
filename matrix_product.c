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

void calcProduct(double *arr1, double *arr2, double *arr3)
{
    int i, j, k;
    
    for(i=0;i<X;i++){
        for(j=0;j<Z;j++){
            for(k=0;k<Y;k++){
                arr3[i*Z+j] += arr1[i*Y+k]*arr2[k*Z+j];
            }
        }
    }
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
    printf("\n");
}

double calcFlops(double t)
{
    double flops;
    flops = (X*Y*Z)*2.0/t;

    return flops;
}

int main(int argc, char** argv)
{
    double start;
    double t_1;
    double flops;

    double *m_a = (double *)malloc(sizeof(double)*X*Y);
    double *m_b = (double *)malloc(sizeof(double)*Y*Z);
    double *m_c = (double *)malloc(sizeof(double)*X*Z);

    initArray(m_a, m_b, m_c);

    start = MPI_Wtime();

    calcProduct(m_a, m_b, m_c);

    t_1 = MPI_Wtime()-start;

    flops = calcFlops(t_1);

    display(m_a, m_b, m_c);
    printf("sequence_version\n");
    printf("TIME = %lf\n", t_1);
    printf("FLOPS = %lf\n", flops);
    printf("MFLOPS = %lf\n", flops*1.0e-6);

    free(m_a);
    free(m_b);
    free(m_c);

    return 0;
}
