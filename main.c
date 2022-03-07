#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"
#include "rsa.h"

typedef struct _key{
    long val;
    long n;
}Key;

void init_key(Key* key, long val, long n){
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key* pkey, Key* skey, long low_size, long up_size){
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
    init_key(pkey, s, n);
    init_key(skey, u, n);
}

char* key_to_str(Key* key){
    char *key_char = (char*)malloc(256*sizeof(char));
    if(!key_char){
        printf("Erreur pendant l'allocation\n");
        return NULL;
    }
    if(sprintf(key_char, "(%lx,%lx)", key->val, key->n) < 0){
        printf("Erreur de formatage\n");
        return NULL;
    }
    return key_char;
}

Key* str_to_key(char* str){
    long val;
    long n;
    if(sscanf(str, "(%lx,%lx)", &val, &n) == 2){
        Key *key = (Key*)malloc(sizeof(Key));
        if(!key){
            printf("Erreur pendant l'allocation\n");
            return NULL;
        }
        key->val = val;
        key->n = n;
    }
    else{
        printf("Erreur de formatage\n");
        return NULL;
    }
}

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
    char message[1000] = "la grosse bite de Philippe";
    int len = strlen(message);

    //Chiffrement :
    long *crypted = encrypt(message, s, n);

    printf("Initial message, %s \n", message);
    printf("Encoded representation : \n");
    print_long_vector(crypted, len);
    
    //Dechiffrement
    char *decoded = decrypt(crypted, len, u, n);
    printf("Decoded: %s\n", decoded); 
    free(crypted);
    free(decoded);

    return 0;
}
