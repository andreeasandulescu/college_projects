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


double* my_solver(int N, double *A) {
	double *B;
	register double *ptrAIK, *ptrAJK, *ptrBI, *ptrAI, *ptrAJ, *ptr_B_real, *ptr_AIK_fin, *ptr_AJ_fin;
	int i, incr;

	incr = 2* N;
	B = (double*) malloc(incr * N * sizeof(double));
	DIE(B == NULL, "Error in calloc");

	ptrAI = A;									//A[i]
	ptrBI = B;									//B[i]
	ptr_AJ_fin = A + N * incr;					//used to check if last element of A is reached

	for(i = 0 ; i < N; i++)
	{
		ptr_B_real = ptrBI;						//B[i][j]
		ptr_AIK_fin = ptrAI + incr;				//used to check if last element from the Kth row of A is reached
		for(ptrAJ = ptrAI; ptrAJ != ptr_AJ_fin; ptrAJ += incr)				//A[j]
		{
			for(ptrAIK = ptrAI, ptrAJK = ptrAJ ; ptrAIK != ptr_AIK_fin;  ptrAIK += 2, ptrAJK += 2 )			//AIK = A[i][k]  AJK = A[j][k]
			{
				*ptr_B_real += (*ptrAIK) * (*ptrAJK) - *(ptrAIK + 1) * *(ptrAJK + 1);					//real
				*(ptr_B_real + 1) += (*ptrAIK) * *(ptrAJK + 1) + (*ptrAJK) * *(ptrAIK + 1);				//img
			}
			ptr_B_real += 2;
		}
		ptrAI += incr;			//skip to the next row
		ptrBI += incr + 2;		//skip to the next row, but add 2 in order to replace j * 2 (from 0 to j), the rest of j* 2 is changed in ptr_B_real;	
	}
	printf("OPT SOLVER\n");
	return B;
}
