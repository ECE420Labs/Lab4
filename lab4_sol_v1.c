#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "timer.h"
#include "Lab4_IO.h"

typedef struct {
    int Di[1500]; // Set of nodes with an outgoing link to this node
    int size_Di;  // Number of nodes with outgoing link to this node
    int li;       // Number of outgoing links from this node
} node;

int calculate(double *r, node *A);
double rel_err(double *r, double *t, int size);
int backup_vec(double *r, double *t, int size);

int n; // number of nodes

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

int main(void) {
    FILE *ip;
    int i;
    int src, dest;  // For reading in node data from file
    node *A;        // node data struct
    double *R;      // result

    if ((ip = fopen("data_input","r")) == NULL){
        printf("error opening the input data.\n");
    }
    if (!fscanf(ip, "%d\n", &n)) {
        printf("%s\n", "Read error");
    }

    A = malloc(n * sizeof(node));
    for (i = 0; i < n; i++) {
        A[i].li = 0;
        A[i].size_Di = 0;
    }

    R = malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        R[i] = 1.0 / n;
    }

    while(!feof(ip)){
        if (!fscanf(ip, "%d\t%d\n", &src, &dest)) {
            printf("%s\n", "Read error");
        }
        A[dest].Di[A[dest].size_Di] = src;
        (A[dest].size_Di)++;
        (A[src].li)++;
    }

    fclose(ip);

    calculate(R, A);

    free(A); free(R);

    return 0;
}

int calculate(double *r, node *A) {
    int i, j;
    double *r_pre;
    double damp_const;
    damp_const = (1.0 - DAMPING_FACTOR) / n;
    int my_rank, comm_sz, local_n;
    double start = 0, end = 0; // for time

    //r_pre = malloc(n * sizeof(double));

    MPI_Init(NULL, NULL);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    r_pre = malloc(n * sizeof(double));

    local_n = n / comm_sz;

    double still_err = 1;
    double *local_r = malloc(local_n * sizeof(double));

    if (my_rank == 0) {
        GET_TIME(start);
    }
    while (still_err > EPSILON) {
        backup_vec(r, r_pre, n);
        for ( i = local_n * my_rank; i < local_n * (my_rank + 1); i++) {
            local_r[i - local_n * my_rank] = 0.0;
            for ( j = 0; j < A[i].size_Di; ++j) {
                local_r[i - local_n * my_rank] += r_pre[A[i].Di[j]] / A[A[i].Di[j]].li;
            }
            local_r[i - local_n * my_rank] *= DAMPING_FACTOR;
            local_r[i - local_n * my_rank] += damp_const;
        }
        MPI_Allgather(local_r, local_n, MPI_DOUBLE, r, local_n, MPI_DOUBLE, MPI_COMM_WORLD);
        still_err = rel_err(r, r_pre, n);
    }
    if (my_rank == 0) {
        GET_TIME(end);
        printf("%f\n", end-start);
        Lab4_saveoutput(r, n, end-start);
    }
    free(r_pre);

    MPI_Finalize();

    return 0;
}

double rel_err(double *r, double *t, int size){
    int i;
    double norm_diff = 0, norm_vec = 0;
    for (i = 0; i < size; ++i){
        norm_diff += (r[i] - t[i]) * (r[i] - t[i]);
        norm_vec += t[i] * t[i];
    }
    return sqrt(norm_diff)/sqrt(norm_vec);
}

int backup_vec(double *r, double *t, int size){
    int i;
    for (i = 0; i < size; ++i) t[i] = r[i];
    return 0;
}
