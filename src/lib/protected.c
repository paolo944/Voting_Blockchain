#include "headers/protected.h"

Protected *init_protected(Key *pKey, char *mess, Signature *sign){
    //paramètres: une clé publique pKey, un message et une signature de type Signature
    //intialise une déclaration de type Protected
    //valeur de retour: la déclaration initialisé
    Protected *prot = (Protected*)malloc(sizeof(Protected)); //allocation de mémoire
    if(!prot){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(prot);
        return NULL;
    }
    prot->pKey = pKey;
    prot->mess = mess;
    prot->sign = sign;
    return prot;
}

int verify(Protected *pr){
    //paramètres: une déclaration pr de type Protected
    //vérification de la signature avec la clé publique contenu dans la déclaration et le message oiriginale
    //valeur de retour: un boolean
    char *decoded = decrypt(pr->sign->tab, pr->sign->taille, pr->pKey->val, pr->pKey->n); //allocation de mémoire (libérée) et decryptage du message
    if(strcmp(decoded, pr->mess) == 0){ //compraison de la chaine originale et de celle decrypté pour vérifier la signature
        free(decoded); //libération de la chaine
        return 1;
    }
    else{
        free(decoded); //liberation de la chaine
        return 0;
    }
}

char *protected_to_str(Protected *pr){
    //paramètres: une déclaration pr de type protected
    //la déclaration est transformé en une chaine de caractère contenant les mêmes informations
    //valeur de retour: la chaine de caractère contenant la déclaration
    char *chaine = (char*)malloc(256*sizeof(char)); //allocation de mémoire
    if(!chaine){ //vérification de l'allocation
        printf("Erreur pendant l'allocation\n");
        free(chaine);
        return NULL;
    }
    char *pKeyStr = key_to_str(pr->pKey); //allocation de mémoire (libérée) et transformation de la clé sous forme string
    char *prStr = signature_to_str(pr->sign); //allocation de mémoire (libérée) et transformation de la signature sous forme string
    sprintf(chaine, "%s , %s , %s", pKeyStr, pr->mess, prStr); //recopie des informations dans la chaine
    free(pKeyStr); //libérations des chaines
    free(prStr); //libérations des chaines
    return chaine;
}

Protected *str_to_protected(char *chaine){
    //paramètres: une chaine de caractère contenant la déclaration
    //transformer la chaine de caractère en protected
    //valeur de retour: la déclaration de type protected
    char *pKey_str = (char*)malloc(256*sizeof(char)); //allocation de mémoire
    if(!pKey_str){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(pKey_str);
        return NULL;
    }
    char *mess = (char*)malloc(256*sizeof(char)); //allocation de mémoire
    if(!mess){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(mess);
        return NULL;
    }
    char *sign_str = (char*)malloc(256*sizeof(char)); //allocation de mémoire
    if(!sign_str){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(sign_str);
        return NULL;
    }
    if(sscanf(chaine, "%s , %s , %s", pKey_str, mess, sign_str) != 3){ //formatage et sa vérification
        printf("Erreur pendant le formatage\n");
        return NULL;
    }
    Key *pKey = str_to_key(pKey_str); //allocation de mémoire
    free(pKey_str);
    Signature *sign = str_to_signature(sign_str); //allocation de mémoire
	free(sign_str);
    return init_protected(pKey, mess, sign); //allocation de mémoire et initialisation de la déclaration
}

void delete_protected(Protected *p){
    //paramètres: une déclaration pr de type Protected
    //supprime la déclaration
    //valeur de retour: aucune
    free(p->pKey);
    free(p->mess);
    free(p->sign->tab);
    free(p->sign);
    free(p);
}
