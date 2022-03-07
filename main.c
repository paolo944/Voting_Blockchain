#include <stdio.h>
#include <stdlib.h>
#include "encryption.h"
#include "rsa.h"

typedef struct _key{
    long val;
    long n;
}Key;

typedef struct _signature{
    long *tab;
    int taille;
}Signature;

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
        return key;
    }
    else{
        printf("Erreur de formatage\n");
        return NULL;
    }
}

Signature *init_signature(long *content, int size){
    Signature *sign = (Signature*)malloc(sizeof(Signature));
    if(!sign){
        printf("Erreur pendant l'allocation\n");
        return NULL;
    }
    sign->tab = (long*)malloc(sizeof(long));
    if(!sign->tab){
        printf("Erreur pendant l'allocation\n");
        return NULL;
    }
    sign->tab = content;
    sign->taille = size;
    return sign;
}

Signature* sign(char* mess, Key* sKey){
    int taille = strlen(mess);
    long *content = (long*)malloc(taille*sizeof(long));
    if(!content){
        printf("Erreur pendant l'allocation\n");
        return NULL;
    }
    content = encrypt(mess, sKey->val, sKey->n);
    Signature *sign = init_signature(content, taille);
    return sign;
}

char *signature_to_str(Signature * sgn){
    char * result = malloc (10*sgn->taille*sizeof(char));
    result[0]= '#' ;
    int pos = 1;
    char buffer[156];
    for(int i=0; i<sgn->taille; i++){
        sprintf(buffer ,"%lx" , sgn->tab[i]);
        for(int j=0; j<strlen(buffer) ; j++){
            result[pos] = buffer[j];
            pos = pos +1;
        }
        result[pos] = '#' ;
        pos = pos+1;
    }
    result[pos] = '\0' ;
    result = realloc(result , (pos+1)*sizeof(char)) ;
    return result;
}

Signature *str_to_signature(char *str){
    int len = strlen(str);
    long *content = (long*)malloc(sizeof(long)*len) ;
    int num = 0;
    char buffer [256];
    int pos = 0;
    for(int i=0; i<len ; i++){
        if (str[i] != '#'){
            buffer [pos] = str[i];
            pos = pos+1;
        }else{
            if(pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer , "%lx" , &(content[num])) ;
                num = num + 1;
                pos = 0;
            }
        }
    }
    content = realloc(content , num*sizeof(long));
    return init_signature(content , num);
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
