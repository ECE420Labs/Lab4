#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "Lab4_IO.h"

typedef struct {
    int Di[1500]; // Set of nodes with an outgoing link to this node
    int size_Di;  // Number of nodes with outgoing link to this node
    int li;       // Number of outgoing links from this node
} node;

int calculate(double *r, node *A);
double rel_err(double *r, double *t, int size);

int n; // number of nodes

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

void main(void) {
    FILE *ip;
    int i,j;
    int src, dest;  // For reading in node data from file
    node *A;        // node data struct
    double *R;      // result
    double start, end; // for time

    if ((ip = fopen("data_input","r")) == NULL){
        printf("error opening the input data.\n");
    }
    fscanf(ip, "%d\n", &n);

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
        fscanf(ip, "%d\t%d\n", &src, &dest);
        A[dest].Di[A[dest].size_Di] = src;
        (A[dest].size_Di)++;
        (A[src].li)++;
    }

    fclose(ip);

    GET_TIME(start);
    calculate(R, A);
    GET_TIME(end);

    printf("%f", end-start);

    for (i = 0; i < n; ++i){
        //printf("%f ", R[i]);
        //printf("%d: %d %d %d \n", i, A[i].Di[(A[i].size_Di)-1], A[i].size_Di, A[i].li);
        /*for (j = 0; j < A[i].size_Di; ++j) {
            printf("%d ", A[i].Di[j]);
        }
        printf("\n");*/
    }
    printf("\n");


    Lab4_saveoutput(R, n, end-start);

    free(A); free(R);
}

int calculate(double *r, node *A) {
    int i, j;
    double *r_pre;
    r_pre = malloc(n * sizeof(double));
    double damp_const;
    damp_const = (1.0 - DAMPING_FACTOR) / n;

    do {
        vec_cp(r, r_pre, n);
        for ( i = 0; i < n; ++i){
            r[i] = 0.0;
            for ( j = 0; j < A[i].size_Di; ++j) {
                r[i] += r_pre[A[i].Di[j]] / A[A[i].Di[j]].li;
            }
            r[i] *= DAMPING_FACTOR;
            r[i] += damp_const;
        }
    } while(rel_err(r, r_pre, n) >= EPSILON);

    free(r_pre);

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
