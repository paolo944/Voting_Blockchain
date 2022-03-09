#include "headers/key.h"

void init_key(Key* key, long val, long n){
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key* pkey, Key* skey, long low_size, long up_size){
    long p = random_prime_number(low_size, up_size, 5000);
    long q = random_prime_number(low_size, up_size, 5000);
    while(p==q){
        q = random_prime_number(low_size, up_size, 5000);
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
        return key;
    }
    else{
        printf("Erreur de formatage\n");
        return NULL;
    }
}
