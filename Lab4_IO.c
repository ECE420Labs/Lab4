#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lab4_IO.h"

int Lab4_saveoutput(double *R, int nodecount, double Time){
/*
    Save the data to file for Lab 4 

    -----
    Input:
    int *R         pointer to the result array 
    int nodecount  number of nodes 
    double Time    measured calculation time
    
    -----
    Output:
    data_output the stored data

    -----
    Example:
    lab4_saveoutput(R, nodecount, Time);
*/    
    FILE* op;
    int i;

    if ((op = fopen("data_output","w")) == NULL) {
        printf("Error opening the input file.\n");
        return 1;
    }
    fprintf(op, "%d\n%f\n", nodecount, Time);
    for (i = 0; i < nodecount; ++i )
        fprintf(op, "%0.10f\n", R[i]);
    fclose(op);
    return 0;
}

// double rel_error(double *r, double *t, int size){
//     int i;
//     double norm_diff = 0, norm_vec = 0;
//     for (i = 0; i < size; ++i){
//         norm_diff += (r[i] - t[i]) * (r[i] - t[i]);
//         norm_vec += t[i] * t[i];
//     }
//     return sqrt(norm_diff)/sqrt(norm_vec);
// }
