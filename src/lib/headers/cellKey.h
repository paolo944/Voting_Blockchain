#include <stdlib.h>
#include <stdio.h>
#include "key.h"

#ifndef CELLKEY_H
#define CELLKEY_H

typedef struct cellKey{
    Key *data;
    struct cellKey *next;
}CellKey;

CellKey *create_cell_key(Key *key);
void ajout_en_tete(CellKey *key, CellKey **liste);
CellKey *read_public_keys(char *nomFic);
void print_list_keys(CellKey* LCK);
void delete_cell_key(CellKey *c);
void delete_liste_key(CellKey *c);

#endif //CELLKEY_H