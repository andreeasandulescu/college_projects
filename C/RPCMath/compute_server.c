#include "compute.h"
#include <math.h>

int is_prime(long int nr)	//verificarea primalitatii unui nr
{
	long int i = 0;
	long int sqroot;

	if(nr == 0)
		return 0;
	if (nr > 0 && nr < 4)
		return 1;

	if( nr % 2 == 0 || nr % 3 == 0)
		return 0;

	sqroot = sqrt(nr);
	for(i = 5 ; i <= sqroot; i+= 6)		//verificare cu toate numerele <= sqrt(nr)
	{									//de forma 6k + 1, 6k - 1
		if( nr % i == 0 || nr % (i+2) == 0)
			return 0;
	}
	return 1;
}


int get_operands(long int nr, long int* a, long int*b)
{
	long int m = nr / 2;
	long int p = m;

	if(m % 2 == 0)
	{
		m--;
		p++;
	}

	while(1)
	{
		if( m <= 0 || p == nr)
			break;
		if(is_prime(m) && is_prime(p))		//cautarea este oprita cand se gasesc 2 numere prime
		{
			*a = m;
			*b = p;
			return 0;
		}
		m -= 2;						//pentru a nu mai verifica primalitatea numerelor pare
		p += 2;

	}
	return 1;
}

response * compute_1_svc(long *argp, struct svc_req *rqstp)
{
	static response result;

	if(*argp % 2 == 0)
	{
		long int a, b;
		result.is_prime = -1; 
		get_operands(*argp, &a, &b);
		result.operator1 = a;
		result.operator2 = b;
	}
	else
	{
		result.is_prime = is_prime(*argp);
	}

	return &result;
}
