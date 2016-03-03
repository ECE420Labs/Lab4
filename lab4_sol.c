#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab4_IO.h"

int *n;

typedef struct {
    int Di[1500];
    int size_Di;
    int li;
    double ri;
} node;

void main(void) {
    FILE* ip;
    int i,j;

    if ((ip = fopen("data_input","r")) == NULL){
            printf("error opening the input data.\n");
    }
    fscanf(ip, "%d\n\n", n);

    node *A;
    A = malloc(*n * sizeof(node));
    for (i = 0; i < *n; i++) {
        A[i].li = 0;
        A[i].size_Di = 0;
        A[i].ri = 1 / *n;
    }

    int src, dest;
    for (i = 0; i < *n; ++i){
        fscanf(ip, "%d %d\t", src, dest);
        A[dest].Di[A[dest].size_Di] = src;
        (A[dest].size_Di)++;
        (A[src].li)++;

        fscanf(ip, "\n");
    }

    for (i = 0; i < *n; ++i){
            printf("%d %d %d %f", A[i].Di[(A[i].size_Di)-1], A[i].size_Di, A[i].li, A[i].ri);

    }
}
