/*
 * Tema 2 ASC
 * 2018 Spring
 * Catalin Olaru / Vlad Spoiala
 */
#include <cblas.h>
#include "utils.h"

/* 
 * Add your BLAS implementation here
 */
double* my_solver(int N, double *A) {
	double* C, var = 1;
	C = (double *) calloc(2 * N * N, sizeof(double));
	cblas_zsyrk(CblasRowMajor, CblasUpper, CblasNoTrans, N, N, &var, A, N, &var,(void *) C, N);
	printf("BLAS SOLVER\n");
	return C;
}
