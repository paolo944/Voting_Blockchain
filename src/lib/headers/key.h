#include <stdlib.h>
#include "rsa.h"

#ifndef KEY_H
#define KEY_H

typedef struct _key{
    long val;
    long n;
}Key;


void init_key(Key* key, long val, long n);
void init_pair_keys(Key* pkey, Key* skey, long low_size, long up_size);
char* key_to_str(Key* key);
Key* str_to_key(char* str);

#endif //KEY_H