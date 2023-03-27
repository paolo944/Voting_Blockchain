#include "headers/key.h"

void init_key(Key* key, long val, long n){
    //paramètres: une pointeur vers une clé alloué et la valeur de la clé et n
    //initialise key avec val et n
    //valeur de retour; aucune
    key->val = val;
    key->n = n;
}

void init_pair_keys(Key* pkey, Key* skey, long low_size, long up_size){
    //paramètres: deux clés alloué, une borne min et une borne max
    //initialise une paire de clé privée et publique et une borne min et une borne max
    //valeur de retour: aucune
    long p = random_prime_number(low_size, up_size, 5000); //nombrte premier aléatoire avec 5000 tests de miller
    long q = random_prime_number(low_size, up_size, 5000); //nombrte premier aléatoire avec 5000 tests de miller
    while(p==q){ //boucle si p est le même nombre que q
        q = random_prime_number(low_size, up_size, 5000); //nombrte premier aléatoire avec 5000 tests de miller
    }
    long n, s, u;
    generate_key_values(p, q, &n, &s, &u); //génération des valeurs des clés
    if (u<0){
        long t = (p-1)*(q-1);
        u = u+t;
    }
    init_key(pkey, s, n); //initialisation de la clé publique
    init_key(skey, u, n); //initialisation de la clé privée
}

char* key_to_str(Key* key){
    //paramètres: une clé key de type Key
    //retourne la clé sour forme de chaîne de caractères
    //valeur de retour: chaine de caractères
    char *key_char = (char*)malloc(256*sizeof(char)); //allocation de mémoire
    if(!key_char){ //vérification de l'allocation
        printf("Erreur pendant l'allocation\n");
        free(key_char);
        return NULL;
    }
    if(sprintf(key_char, "(%lx,%lx)", key->val, key->n) < 0){ //écriture des valeurs de clé dans key_char et vérification du formatage
        printf("Erreur de formatage\n");
        free(key_char);
        return NULL;
    }
    return key_char;
}

Key* str_to_key(char* str){
    //paramètres: une chaine de caractère contenant une clé
    //retourne la clé contenu dans la chaine
    //valeur de retour: la clé de type Key
    long val;
    long n;
    if(sscanf(str, "(%lx,%lx)", &val, &n) == 2){
        //free(str);
        Key *key = (Key*)malloc(sizeof(Key)); //allocation de mémoire
        if(!key){ //vérification de l'allocation
            printf("Erreur pendant l'allocation\n");
            return NULL;
        }
        key->val = val;
        key->n = n;
        return key;
    }
    else{
        printf("Erreur de formatage clé sale chien1\n");
        return NULL;
    }
}
