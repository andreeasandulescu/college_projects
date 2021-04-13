/*
 * is_prime -> 1 pt numar impar, prim
 *			   0 pt numar impar, neprim
 * 			   -1 pt numar par
 */


struct response{
    long int operator1;
    long int operator2;
    int is_prime;
};

program COMPUTE_PROG{
    version COMPUTE_VERS{
        response compute(long int) = 1;
    } = 1;
} = 123456789;