#include <stdlib.h>
#include <math.h>
#include <time.h>

#ifndef RSA_H
#define RSA_H

long modpow_naive(long a, long m, long n);
long modpow(long a, long m, long n);
int witness(long a, long b, long d, long p);
long rand_long(long low, long up);
int is_prime_naive(long p);
int is_prime_miller(long p, int k);
long random_prime_number(int low_size, int up_size, int k);
long extended_gcd(long s, long t, long *u, long *v);
void generate_key_values(long p, long q, long *n, long *s,long *u);

#endif //RSA_H