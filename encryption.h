#include <string.h>
#include "rsa.h"


#ifndef ENCRYPTION_H
#define ENCRYPTION_H

char *decrypt(long *crypted, int size, long u, long n);
long *encrypt(char *chaine, long s, long n);

#endif //ENCRYPTION_H