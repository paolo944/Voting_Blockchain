#include "headers/encryption.h"

long *encrypt(char *chaine, long s, long n){
    //paramètres: 
    //
    //valeur de retour
    long *chaine_encrypt = (long*)malloc(strlen(chaine)*sizeof(long));
    if(!chaine_encrypt){
        printf("Erreur d'allocation\n");
        return NULL;
    }  
    int i = 0;
    while(chaine[i]){
        chaine_encrypt[i] = (long)modpow((long)chaine[i], s, n);
        i++;
    }
    return chaine_encrypt;
}

char *decrypt(long *crypted, int size, long u, long n){
    //paramètres: 
    //
    //valeur de retour
    char *chaine_decrypt = (char*)malloc((size+1)*sizeof(char));
    if(!chaine_decrypt){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    int i=0;
    while(i<size){
        chaine_decrypt[i] = (char)modpow(crypted[i], u, n);
        i++;
    }
    chaine_decrypt[i] = '\0';
    return chaine_decrypt;
}