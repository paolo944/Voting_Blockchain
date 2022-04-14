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
unsigned char *miaou256(const char *mess);
void compute_proof_of_work(Block* B,int d);
int verify_block(Block* b, int d);




#endif //BLOCK_H