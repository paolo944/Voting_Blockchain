#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"
#include "rsa.h"

void print_long_vector(long *result, int size){
    printf("Vector: [");
    for(int i=0; i<size; i++){
        printf("%ld \t", result[i]);
    }
    printf("]\n");
}

int main(){
    //Generation de cle :
    long p = random_prime_number(15, 16, 5000);
    long q = random_prime_number(15, 16, 5000);
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
