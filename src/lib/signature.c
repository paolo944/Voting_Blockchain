#include "headers/signature.h"

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