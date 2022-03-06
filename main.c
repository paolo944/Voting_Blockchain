#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"
#include "rsa.h"

void print_long_vector(long *result, int size){
    printf("Vector: [");
    for(int i=0; i<size; i++){
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}

int main(){
    srand(time(NULL));

    //Generation de cle :
    long p = random_prime_number(15, 16, 5000);
    long q = random_prime_number(15, 16, 5000);
    while(p==q){
        q = random_prime_number(3,7, 5000);
    }
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u);
    if (u<0){
        long t = (p-1)*(q-1);
        u = u+t;
    }
    printf("cle publique = (%lx , %lx ) \n",s,n);
    printf("cle privee = (%lx , %lx) \n",u,n);
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
