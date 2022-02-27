#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

long modpow(long a, long m, long n){
    if(m%2 == 0){
        return (long)(powl(powl((long double )a, (m-1)/2), 2)*a)%n;
    }
    return (long)(powl((long double)a, m))%n;
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

long modpow_naive(long a, long m, long n){
    long powA = a;
    for(int i=0; i<m; i++){
        powA *= a;
    }
    return powA%n;
}

int is_prime_miller(long p, int k) {
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
    if (t == 0){
        *u = 1;
        *v = 0;
        return s;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t, s % t , &uPrim, &vPrim);
    *u = vPrim;
    *v = uPrim-(s/t)*vPrim;
    return gcd;
}

void generate_key_values(long p, long q, long *n, long *s,long *u){
    *n = p*q;
    long t = (p-1)*(q-1);
    long *v = 0;
    *s = random_prime_number(3,t, 200);
    while(extended_gcd(*s,t,u,v) != 1){
        *s = random_prime_number(3,t,200);
    }
}

long *encrypt(char *chaine, long s, long n){
    int taille_chaine = strlen(chaine);
    long *chaine_encrypt = (long*)malloc(taille_chaine*sizeof(long));
    for(int i = 0; i < taille_chaine; i++){
        chaine_encrypt[i] = modpow((int)chaine[i], s, n);
    }
    return chaine_encrypt;
}

char *decrypt(long *crypted, int size, long u, long n){
    char *chaine_decrypt = (char*)malloc((size+1)*sizeof(char));
    for(int i = 0; i < size; i++){
        chaine_decrypt[i] = (char)modpow(crypted[i], u, n);
    }
    chaine_decrypt[size] = '\0';
    return chaine_decrypt;
}

void print_long_vector(long *result, int size){
    printf("Vector: [");
    for(int i=0; i<size; i++){
        printf("%ld \t", result[i]);
    }
    printf("]\n");
}

int main(){
    //Generation de cle :
    long p = random_prime_number(1, 6, 5000);
    long q = random_prime_number(1, 6, 5000);
    long n, s, u;
    printf("p: %ld q: %ld\n", p, q);
    generate_key_values(p, q, &n, &s, &u);
    printf("cle publique = (%ld, %ld) \n", s, n);
    printf("cle privee = (%ld, %ld) \n", u, n);

    char message[1000] = "Hello";
    int len = strlen(message);

    //Chiffrement :
    long *crypted = encrypt(message, s, n);

    printf("Initial message, %s \n", message);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);

    //Dechiffrement
    char *decoded = decrypt(crypted, len, u, n);
    printf("Decoded: %s\n", decoded); 

    return 0;
}
