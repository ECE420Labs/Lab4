#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "Lab4_IO.h"

#define EPSILON 0.00001
#define DAMPING_FACTOR 0.85

typedef struct {
    int Di[1500]; // Set of nodes with an outgoing link to this node
    int size_Di;  // Number of nodes with outgoing link to this node
    int li;       // Number of outgoing links from this node
} node;

double* calculate(double *r, node *A);
int n; // number of nodes


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

    R = malloc(n * sizeof(double));
    for (i = 0; i < n; i++) {
        R[i] = 1.0 / n;
    }

    A = malloc(n * sizeof(node));
    for (i = 0; i < n; i++) {
        A[i].li = 0;
        A[i].size_Di = 0;
    }

    for (i = 0; i < n; ++i){
        fscanf(ip, "%d\t%d\n", &src, &dest);
        A[dest].Di[A[dest].size_Di] = src;
        (A[dest].size_Di)++;
        (A[src].li)++;
    }

    GET_TIME(start);
    R = calculate(R, A);
    GET_TIME(end);

    for (i = 0; i < n; ++i){
        //printf("%f ", R[i]);
        printf("%d %d %d \n", A[i].Di[(A[i].size_Di)-1], A[i].size_Di, A[i].li);
    }
    printf("\n");


    Lab4_saveoutput(R, n, end-start);

    free(A); free(R);
}

double* calculate(double *r, node *A) {
    int i, j;
    double *r_pre;
    r_pre = malloc(n * sizeof(double));
    double damp_const;
    damp_const = (1.0 - DAMPING_FACTOR) / n;

    do {
        // Save previous values
        for (i = 0; i < n; i++) {
            r_pre[i] = r[i];
        }
        // Calculate new
        for ( i = 0; i < n; i++){
            r[i] = 0;
            for ( j = 0; j < A[i].size_Di; ++j) {
                r[i] += r_pre[A[i].Di[j]] / A[A[i].Di[j]].li;
            }
            r[i] *= DAMPING_FACTOR;
            r[i] += damp_const;
        }
    } while(rel_error(r, r_pre, n) >= EPSILON);

    free(r_pre);

    return r;
}
