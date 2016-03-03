/*
Header for the Lab 4 IO functions
Need "-lm" tag when include this header
*/
#ifndef LAB4_H_INCLUDE
#define LAB4_H_INCLUDE

//===========
// Mandatory included functions
int Lab4_saveoutput(double* R, int nodecount, double Time);

//===========
// Supporting structures and reference functions for serialtester
// math functions
//double rel_error(double *r, double *t, int size); // the relative difference between vec r and vec t with respect to t, i.e., it returns norm(r-t)/norm(t)

#endif
