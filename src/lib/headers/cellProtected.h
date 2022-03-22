#include "protected.h"

#ifndef CELLPROTECTED_H
#define CELLPROTECTED_H

typedef struct cellProtected{
    Protected *data;
    struct cellProtected *next;
} CellProtected;

CellProtected *create_cell_protected(Protected *pr);
void ajout_en_tete_protected(CellProtected *pr, CellProtected **liste);
CellProtected *read_protected(char *nomfic);
void afficher_cell_protected(CellProtected *liste);
void delete_cell_protected(CellProtected *c);
void delete_liste_protected(CellProtected *c);

#endif //PROTECTED_H