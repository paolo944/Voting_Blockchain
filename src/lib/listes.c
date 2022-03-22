#include "headers/listes.h"

CellKey *create_cell_key(Key *key){
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
    if(!key){
        return;
    }
    if(liste == NULL){
        liste = &key;
    }
    key->next = *liste;
    liste = &key;
}

CellKey **read_public_keys(char *nomFic){
    FILE *file = fopen(nomFic, "r");
    char buffer[256];
    Key *key;
    CellKey **liste = NULL;
    while(fgets(buffer, 256, file)){
        key = str_to_key(buffer);
        ajout_en_tete(create_cell_key(key), liste);
    }
    return liste;
}

void print_list_keys(CellKey *LCK){
    CellKey *tmp = LCK;
    while(tmp){
        printf("ici\n");
        printf("%s\n", key_to_str(tmp->data));
        printf("%s\n", key_to_str(tmp->next->data));
        tmp = tmp->next;
    }
}

void delete_cell_key(CellKey *c){

}
