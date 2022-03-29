#include "headers/hashCell.h"

HashCell *create_hashcell(Key *key){
    HashCell *h = (HashCell*)malloc(sizeof(HashCell)); //Allocation de la mémoire
    if(!h){
        printf("erreur d'allocation\n");
        return NULL;
    }
    h->key = key;
    h->val = 0;
    return h;
}

int hash_function(Key *key, int size){
	return (int)(size*((key->val)*((sqrt(5)-1)/2)-(int)((key->val)*((sqrt(5)-1)/2))));
}

int find_position(HashTable *t, Key *key){
    int cle = hash_function(key, t->size);
    if(t->tab[cle]->key == key){
        return cle;
    }
    else{
        int i = 1;
        while(t->tab[(cle+i)%t->size] && i%t->size != 0){
            if(t->tab[(cle+i)%t->size]->key == key){
                return (cle+i)%t->size;
            }
        }
        return (cle+i)%t->size;
    }
}

HashTable *create_hashtable(CellKey *keys, int size){
    HashTable *h = (HashTable*)malloc(sizeof(HashTable)); //Allocation de la mémoire
    if(!h){
        printf("erreur d'allocation\n");
        return NULL;
    }
    h->tab = (HashCell**)malloc(sizeof(HashCell)*size); //Allocation de la mémoire
    if(!(h->tab)){
        printf("erreur d'allocation\n");
        free(h);
        return NULL;
    }
    h->size = size;
    CellKey *tmp = keys;
    while(tmp){
        h->tab[hash_function(tmp->data, size)] = tmp->data;
        tmp = tmp->next;
    }
    return h;
}

void delete_hashtable(HashTable* t){
    for(int i=0; i<t->size; i++){
        free(t->tab[i]->key);
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    HashTable *hc = create_hashtable(candidates, sizeC);
    HashTable *hv = create_hashtable(voters, sizeV);
    
}