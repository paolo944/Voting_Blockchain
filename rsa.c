#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include "rsa.h"

long modpow_naive(long a, long m, long n){
    long powA = a;
    for(int i=0; i<m; i++){
        powA *= a;
    }
    return powA%n;
}

long modpow(long a, long m, long n){
    long temp = 0;
    if(m == 0){
        return 1;
    }
    if(m%2 == 0){
        temp = modpow(a, m/2, n);
        return temp*a%n;
    }
    else{
        temp = modpow(a, m-1, n);
        return temp*a%n;
    }
}

int witness(long a, long b, long d, long p){
    long x = modpow(a, d, p);
    if(x == 1){
        return 0;
    }
    for(long i = 0; i<b ; i++){
        if(x == p-1){
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){
    return rand() % (up-low + 1)+low;
}

int is_prime_naive(long p){
    for(int i=3; i<p; i++){
        if(p%i == 0){
            return 0;
        }
    }
    return 1;
}

int is_prime_miller(long p, int k){
    if (p == 1){
        return 1;
    }
    if (!(p & 1) || p<= 1){
        return 0;   
    }

    long b=0;
    long d =p-1;
    while(!(d & 1)){
        d=d/2;
        b=b+1;
    }

    long a;
    int i;
    for(i=0;i<k;i++){
        a= rand_long(2, p-1);
        if(witness(a,b,d,p)){
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k){
    srand(time(NULL));
    long s;
    do{
        s = rand_long(pow(2, low_size), pow(2, up_size+1)-1);
        //printf("s: %ld\n", s);
    }
    while(!is_prime_miller(s, k));
    return s;
}

long extended_gcd(long s, long t, long *u, long *v){
    if (s == 0){
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s , &uPrim, &vPrim);
    *u = vPrim-(t/s)*uPrim;
    *v = uPrim;
}

void generate_key_values(long p, long q, long *n, long *s,long *u){
    *n = p*q;
    long t = (p-1)*(q-1);
    long v = 0;
    long gcd;
    do{
        *s = rand_long(1, t);
        gcd = extended_gcd(*s, t, u, &v);
    }
    while((gcd != 1) && ((*u) < 0));
}
