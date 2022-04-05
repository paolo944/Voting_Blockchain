#include "cellProtected.h"

#ifndef BLOCK_H
#define BLOCK_H

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

void ecrire_block(Block *b, char *nomFic);
Block *lire_block(char *nomFic);
char *block_to_str(Block *block);


#endif //BLOCK_H