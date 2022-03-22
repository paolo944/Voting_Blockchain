#include <string.h>
#include "encryption.h"
#include "signature.h"

#ifndef PROTECTED_H
#define PROTECTED_H

typedef struct _protected{
    Key *pKey;
    char *mess;
    Signature *sign;
}Protected;

Protected *init_protected(Key *pKey, char *mess, Signature *sign);
int verify(Protected *pr);
char *protected_to_str(Protected *pr);
Protected *str_to_protected(char *chaine);
void delete_protected(Protected *p);

#endif //PROTECTED_H