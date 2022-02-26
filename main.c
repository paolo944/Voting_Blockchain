#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

long random_prime_number (int low_size, int up_size, int k){
    for (int i=pow(2.0,low_size);i<pow(2.0,up_size+1)-1;i++){
        if (is_prime_miller((long)i,k)){
            return i;
        }
    }
}

long extended_gcd(long s, long t, long *u, long *v){
    if (t==0){
        *u=1;
        *v=0;
        return s;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t, s % t , &uPrim, &vPrim);
    *u =vPrim;
    *v= uPrim-(s-t)*vPrim;
    return gcd;
}

void generate_key_values(long p, long q, long* n, long *s,long *u){
    *n=p*q;
    double t= (p-1)*(q-1);
    long *v;
    *s= random_prime_number(&n,t,200);
    while(extended_gcd(s,t,*u,*v)!= s){
        *s= random_prime_number(&n,t,200);
    }
}

int is_prime_naive(long p, int k){
    if(p == 2){
        return 1;
    }
    if(!(p & 1) || p <= 1){ //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while(!(d & 1)){//tant que d n'est pas impair
        d = d/2;
        b = b+1;
    }
    // On genere k valeurs pour a, et on testesi c'est un temoin :
    long a;
    int i;
    for(i = 0; i < k; i++){
        a = ranglong(2, p-1);
        if(witness(a, b, d, p)){
            return 0;
        }
    }
    return 1;
}

int is_prime_naive(long p){
    for(int i=3; i<p; i++){
        if(p%i == 0){
            return 0;
        }
    }
    return 1;
}

long modpow(long a, long m, long n){
    if(m%2 == 0){
        return (long)(powl(powl((long double )a, (m-1)/2), 2)*a)%n;
    }
    return (long)(powl((long double)a, m))%n;
}

long modpow_naive(long a, long m, long n){
    long powA = a;
    for(int i=0; i<m; i++){
        powA *= a;
    }
    return powA%n;
}

int main(){
    FILE *fic = fopen("courbesNaive.txt", "w");
    if(fic == NULL){
        printf("erruer d'ouverture");
    }
    clock_t temps_initial1;
    clock_t temps_final1;
    clock_t temps_initial2;
    clock_t temps_final2;
    double temps_cpu1;
    double temps_cpu2;

    for(long i=1000; i<8000; i++){
        temps_initial1 = clock();
        modpow_naive(2, i, 4);
        temps_final1 = clock();
        temps_cpu1 = (double)(temps_final1-temps_initial1)/CLOCKS_PER_SEC;
        temps_initial2 = clock();
        modpow(2, i, 4);
        temps_final2 = clock();
        temps_cpu2 = (double)(temps_final1-temps_initial1)/CLOCKS_PER_SEC;
        fprintf(fic, "%ld %.10f %.10f\n", i, temps_cpu1, temps_cpu2);
    }

    return 0;
}
