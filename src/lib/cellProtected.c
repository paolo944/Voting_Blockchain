#include "headers/cellProtected.h"

CellProtected *create_cell_protected(Protected *pr){
    //paramètres: 
    //
    //valeur de retour
    CellProtected *c = (CellProtected*)malloc(sizeof(CellProtected));
    if(!c){
        printf("Erreur d'allocation\n");
        return NULL;
    }
    c->data = pr;
    c->next = NULL;
    return c;
}

void ajout_en_tete_protected(CellProtected *pr, CellProtected **liste){
    //paramètres: 
    //
    //valeur de retour
    if(*liste == NULL){
        *liste = pr;
        return;
    }
    pr->next = *liste;
    *liste = pr;
}

CellProtected *read_protected(char *nomfic){
    //paramètres: 
    //
    //valeur de retour
    FILE *fic = fopen(nomfic, "r");
    if(!fic){
        return NULL;
    }
    char buffer[256];
    Protected *pr = NULL;
    CellProtected *liste = NULL;
    while(fgets(buffer, 256, fic)){
        pr = str_to_protected(buffer);
        ajout_en_tete_protected(create_cell_protected(pr), &liste);
    }
    return liste;
}

void afficher_cell_protected(CellProtected *liste){
    //paramètres: 
    //
    //valeur de retour
    CellProtected *tmp = liste;
    while(tmp){
        printf("%s\n", protected_to_str(tmp->data));
        tmp = tmp->next;
    }
}

void delete_cell_protected(CellProtected *c){
    //paramètres: 
    //
    //valeur de retour
    delete_protected(c->data);
    free(c);
}

void delete_liste_protected(CellProtected *c){
    //paramètres: 
    //
    //valeur de retour
    CellProtected *tmp = NULL;
    while(c){
        tmp = c->next;
        delete_cell_protected(c);
        c = tmp;
    }
    free(c);
}

void verification_fraude(CellProtected **liste){
    //paramètres: 
    //
    //valeur de retour
    CellProtected *c = *liste;
    CellProtected *tmp = NULL;
    while(c->next){
        if(!verify(c->next->data)){
            tmp = c->next;
            c->next = c->next->next;
            delete_cell_protected(tmp);
        }
        c = c->next;
    }
    if(!verify((*liste)->data)){
        tmp = *liste;
        liste = &((*liste)->next);
        delete_cell_protected(tmp);
    }
}
