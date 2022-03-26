#include "headers/encryption.h"

long *encrypt(char *chaine, long s, long n){
    //paramètres: la chaine à encrypter et deux long (n et s)
    //la chaine est encrypté avec s et n
    //valeur de retour: un tableau de type long de la chaine encrypté
    long *chaine_encrypt = (long*)malloc(strlen(chaine)*sizeof(long)); //allocation de mémoire
    if(!chaine_encrypt){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(chaine_encrypt);
        return NULL;
    }  
    int i = 0; //variable d'incrémentation
    while(chaine[i]){
        chaine_encrypt[i] = (long)modpow((long)chaine[i], s, n); //encryptage de tout les caractères de la chaine
        i++;
    }
    return chaine_encrypt;
}

char *decrypt(long *crypted, int size, long u, long n){
    //paramètres:la chaine à décrypter de type tab[long] et deux long (u et n)
    //la chaine est décrypté avec u et n
    //valeur de retour: chaine de caractère décrypté
    char *chaine_decrypt = (char*)malloc((size+1)*sizeof(char)); //allocation de mémoire
    if(!chaine_decrypt){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(chaine_decrypt);
        return NULL;
    }
    int i = 0; //variable d'incrémentation
    while(i<size){
        chaine_decrypt[i] = (char)modpow(crypted[i], u, n); //décryptage de tout les caractères de la chaine
        i++;
    }
    chaine_decrypt[i] = '\0'; //ajout du caractère de fin
    return chaine_decrypt;
}