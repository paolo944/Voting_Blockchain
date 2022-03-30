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
    if(t->tab[cle]->key != NULL){
        if(t->tab[cle]->key == key){
            return cle;
        }
        else{
            int i = 1;
            while(t->tab[(cle+i)%t->size] && i%t->size != 0){
                if(t->tab[(cle+i)%t->size]->key == key){
                    return (cle+i)%t->size;
                }
                i++;
            }
            if(i%t->size == 0) return -1;
            return (cle+i)%t->size;
        }
    }
    else{
        int i = 1;
        while(t->tab[(cle+i)%t->size] && i%t->size != 0){
            if(t->tab[(cle+i)%t->size]->key == key){
                return (cle+i)%t->size;
            }
            i++;
        }
        if(i%t->size == 0) return -1;
        return (cle+i)%t->size;
    }
}

HashTable *create_hashtable(CellKey *keys, int size){
    HashTable *h = (HashTable*)malloc(sizeof(HashTable)); //Allocation de la mémoire
    if(!h){
        printf("erreur d'allocation\n");
        return NULL;
    }
    h->tab = (HashCell**)malloc(sizeof(HashCell*)*size); //Allocation de la mémoire
    if(!(h->tab)){
        printf("erreur d'allocation\n");
        free(h);
        return NULL;
    }
    h->size = size;
    CellKey *tmp = keys;
    while(tmp){
        h->tab[find_position(h, tmp->data)]->key = tmp->data;
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
    verification_fraude(&decl);
    HashTable *hc = create_hashtable(candidates, sizeC);
    HashTable *hv = create_hashtable(voters, sizeV);
    CellProtected *tmp = decl;
    int cle = 0;
    Key *pKeyC;
    while(tmp){
        cle = find_position(hv, tmp->data->pKey);
        if(hv->tab[cle]){
            if(hv->tab[cle]->val == 0){
                pKeyC = str_to_key(tmp->data->mess);
                hv->tab[cle]->val = 1;
                hc->tab[find_position(hc, pKeyC)]->val++;
                free(pKeyC);
            }
        }
        tmp = tmp->next;
    }
    HashCell *vainqeur = hc->tab[0];
    for(int i=1; i<hc->size; i++){
        if(vainqeur->val < hc->tab[i]->val){
            vainqeur = hc->tab[i];
        }
    }
    return vainqeur->key;
}