#include "headers/protected.h"

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
    sprintf(chaine, "%s , %s , %s", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sign));
    return chaine;
}

Protected *str_to_protected(char *chaine){
    char *pKey_str = (char*)malloc(256*sizeof(char));
    char *mess = (char*)malloc(256*sizeof(char));
    char *sign_str = (char*)malloc(256*sizeof(char));
    if(sscanf(chaine, "%s , %s , %s", pKey_str, mess, sign_str) != 3){
		printf("%s\n", pKey_str);
        printf("Erreur pendant le formatage\n");
        return NULL;
    }
    Key *pKey = str_to_key(pKey_str);
    Signature *sign = str_to_signature(sign_str);
    return init_protected(pKey, mess, sign);
}