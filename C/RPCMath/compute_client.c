#include "compute.h"

char* yes_no(int x)		//functie utila pt generarea output-ului
{
	if(x == 1)
		return "YES";
	else
		return "NO";
}

int main (int argc, char *argv[])
{

	int i, nr_lines = 0;
	long int nr;
	response *res;
	FILE * fp_in, *fp_out;
	CLIENT *clnt;

	if (argc < 2) {
		printf ("usage: compute_client IP input_file\n");
		exit (1);
	}

	clnt = clnt_create (argv[1], COMPUTE_PROG, COMPUTE_VERS, "tcp");

	if(clnt == NULL) {
		perror("clnt create");
		return -1;
	}

	fp_in = fopen (argv[2], "r");
	fp_out = fopen ("out.txt", "w+");   

	fscanf(fp_in, "%d",&nr_lines);
   
   	for(i = 0 ; i < nr_lines; i++)
   	{
		fscanf(fp_in, "%ld",&nr);

		res = compute_1(&nr, clnt);			//trimite numarul citit din fisier la server
		if (res == (response *) NULL) {
			clnt_perror (clnt, "call failed");
		}

		if(res->is_prime == -1)				//numarul trimis la server este par
			fprintf(fp_out, "YES %ld %ld\n", res->operator1, res->operator2);
		else
			fprintf(fp_out, "%s %ld %d\n", yes_no(res->is_prime), nr, res->is_prime);
   	}

 	fclose(fp_in);
 	fclose(fp_out);
	exit (0);
}
