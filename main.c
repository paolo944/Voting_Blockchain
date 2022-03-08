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

typedef struct _protected{
    Key *pKey;
    char *mess;
    Signature *sign;
}Protected;

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
    return init_signature(encrypt(mess, sKey->val, sKey->n), strlen(mess));
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

Protected *init_protected(Key *pKey, char *mess, Signature *sign){
    Protected *prot = (Protected*)malloc(sizeof(Protected));
    prot->pKey = pKey;
    prot->mess = mess;
    prot->sign = sign;
    return prot;
}

int verify(Protected *pr){
    char *decoded = decrypt(pr->sign->tab, pr->sign->taille, pr->pKey->val, pr->pKey->n);
    return (strcmp(decoded, pr->mess) == 0);
}

char *protected_to_str(Protected *pr){
    char *chaine = malloc(256*sizeof(char));
    if(!chaine){
        printf("Erreur pendant l'allocation\n");
        return NULL;
    }
    if(sprintf(chaine, "%s, %s, %s", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sign)) != 3){
        printf("Problème pendant le formatage\n");
        return NULL;
    }
    return chaine;
}

Protected *str_to_protected(char *chaine){
    char *pKey_str = (char*)malloc(256*sizeof(char));
    char *mess = (char*)malloc(256*sizeof(char));
    char *sign_str = (char*)malloc(256*sizeof(char));
    if(sscanf(chaine, "%s, %s, %s", pKey_str, mess, sign_str) != 3){
        printf("Erreur pendant le formatage\n");
        return NULL;
    }
    Key *pKey = str_to_key(pKey_str);
    Signature *sign = str_to_signature(sign_str);
    return init_protected(pKey, mess, sign);
}

void print_long_vector(long *result, int size){
    printf ("Vector: [");
    for(int i=0; i < size; i++){
        printf("%lx \t" , result[i]);
    }
    printf ("]\n");
}

int main ()
{
    srand(time(NULL));

    //Generation de cle :
    long p = random_prime_number(3, 7, 5000);
    long q = random_prime_number (3, 7, 5000);
    while(p == q) {
        q = random_prime_number (3, 7, 5000);
    }
    long n , s , u ;
    generate_key_values (p, q, &n, &s, &u);
    //Pour avoir des cles positives :
    if(u < 0){
        long t = (p - 1) * ( q - 1);
        u = u + t; //on aura toujours s*u mod t = 1
    }

    //Afichage des cles en hexadecimal
    printf("cle publique = (%lx, %lx) \n", s, n);
    printf("cle privee = (%lx, %lx) \n", u , n);

    //Chiffrement:
    char mess[10] = "Hello";
    int len = strlen(mess);
    long * crypted = encrypt(mess, s, n);

    printf("Initial message: %s \n", mess);
    printf("Encoded representation: \n");
    print_long_vector(crypted, len);

    //Dechiffrement
    char * decoded = decrypt (crypted, len, u, n);
    printf ("Decoded : %s \n", decoded) ;

    return 0;
}