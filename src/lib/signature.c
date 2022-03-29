#include "headers/signature.h"

Signature *init_signature(long *content, int size){
    //paramètres: un pointeur vers un message encrypté et sa taille
    //intialise une signature avec le contenu encrypté et sa taille
    //valeur de retour: une signature de type Signature
    Signature *sign = (Signature*)malloc(sizeof(Signature)); //allocation de mémoire
    if(!sign){ //vérification de l'allocation
        printf("Erreur pendant l'allocation\n");
        free(sign);
        return NULL;
    }
    sign->tab = content;
    sign->taille = size;
    return sign;
}

Signature* sign(char* mess, Key* sKey){
    //paramètres: un message et une clé privée
    //encryptage du message grâce à la clé privée donnée en paramètres
    //valeur de retour: une signautre initialisé
    return init_signature(encrypt(mess, sKey->val, sKey->n), strlen(mess)); //allocation de mémoire
}

char *signature_to_str(Signature * sgn){ //fonction fourni
    char * result = (char*)malloc(10*sgn->taille*sizeof(char)); //allocation de mémoire
    if(!result){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(result);
        return NULL;
    }
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
    result = realloc(result , (pos+1)*sizeof(char)); //allocation de mémoire
    return result;
}

Signature *str_to_signature(char *str){ //fonction fourni
    int len = strlen(str);
    long *content = (long*)malloc(sizeof(long)*len); //allocation de mémoire
    if(!content){
        printf("Erreur d'allocation\n");
        free(content);
        return NULL;
    }
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
    content = realloc(content , num*sizeof(long)); //allocation de mémoire
    return init_signature(content , num); //allocation de mémoire
}

void delete_signature(Signature *sign){
    //paramètres: une signature sign de type Signature
    //supprimer la signature et libération de la mémoire
    //valeur de retour: aucune
    free(sign->tab);
    free(sign);
}
