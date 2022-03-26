#include "headers/cellKey.h"

CellKey *create_cell_key(Key *key){
    //paramètres: 
    //
    //valeur de retour
    CellKey *cell = (CellKey*)malloc(sizeof(CellKey));
    if(!cell){
        return NULL;
    }
    else{
        cell->data = key;
        cell->next = NULL;
    }
    return cell;
}

void ajout_en_tete(CellKey *key, CellKey **liste){
    //paramètres: 
    //
    //valeur de retour
    if(*liste == NULL){
        *liste = key;
        return;
    }
    key->next = *liste;
    *liste = key;
}

CellKey *read_public_keys(char *nomFic){
    //paramètres: 
    //
    //valeur de retour
    FILE *file = fopen(nomFic, "r");
    if(!file){
        return NULL;
    }
    char buffer[256];
    Key *key = NULL;
    CellKey *liste = NULL;
    while(fgets(buffer, 256, file)){
        key = str_to_key(buffer);
        ajout_en_tete(create_cell_key(key), &liste);
    }
    return liste;
}

void print_list_keys(CellKey *LCK){
    //paramètres: 
    //
    //valeur de retour
    CellKey *tmp = LCK;
    while(tmp){
        printf("%s\n", key_to_str(tmp->data));
        tmp = tmp->next;
    }
}

void delete_cell_key(CellKey *c){
    //paramètres: 
    //
    //valeur de retour
    free(c->data);
    free(c);
}

void delete_liste_key(CellKey *c){
    //paramètres: 
    //
    //valeur de retour
    CellKey *tmp = NULL;
    while(c){
        tmp = c->next;
        delete_cell_key(c);
        c = tmp;
    }
    free(c);
}
