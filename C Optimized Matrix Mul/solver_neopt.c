/*
 * Tema 2 ASC
 * 2018 Spring
 * Catalin Olaru / Vlad Spoiala
 */
#include "utils.h"

#define RANGE 1000
#define DIE(assertion, call_description)				\
	do {											 	\
		if (assertion) {								\
			fprintf(stderr, "(%s, %d): ",				\
					__FILE__, __LINE__);				\
			perror(call_description);					\
			exit(EXIT_FAILURE);							\
		}												\
	} while(0)

void mul_elems(double a1, double a2, double b1, double b2, double *real, double *img)
{
	double real_aux, img_aux;

	real_aux = *real;
	img_aux = *img;

	real_aux += a1 * a2 - b1 * b2;
	*real = real_aux;
	img_aux += a1 * b2 + a2 * b1;
	*img = img_aux;
}

double* my_solver(int N, double *A) {
	double *B, *ptrB, *ptrA, a1, a2, b1, b2;
	int i, j, k;

	B = (double*) calloc(2 * N * N, sizeof(double));
	DIE(B == NULL, "Error in calloc");

	for(i = 0 ; i < N; i++)
	{
		for(j = i; j < N; j++)
		{
			for(k = 0; k < N; k++)
			{
				ptrB =  B + (i * N * 2) + j * 2;			//	B[i][j]

				ptrA = A + (i * N * 2) + k * 2;				//A[i][k]
				a1 = *ptrA;									//real
				b1 = *(ptrA + 1);							//img

				ptrA = A + (j * N * 2) + k * 2;				//A[j][k]
				a2 = *ptrA;
				b2 = *(ptrA + 1);
				mul_elems(a1, a2, b1, b2, ptrB, ptrB+1);
			}
		}
	}
	printf("NEOPT SOLVER\n");
	return B;
}
