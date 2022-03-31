#include "headers/hashCell.h"

HashCell *create_hashcell(Key *key){
    //paramètres: clé de type Key
    //
    //valeur de retour: la cellule crée
    HashCell *h = (HashCell*)malloc(sizeof(HashCell)); //Allocation de la mémoire
    if(!h){ //vérification de l'allocation
        printf("erreur d'allocation\n");
        return NULL;
    }
    h->key = key;
    h->val = 0;
    return h;
}

int hash_function(Key *key, int size){
    //paramètres: clé de type Key et la taille de la table
    //
    //valeur de retour: la valeur de hachage de key->val
	return (int)(size*((key->val)*((sqrt(5)-1)/2)-(int)((key->val)*((sqrt(5)-1)/2))));
}

int find_position(HashTable *t, Key *key){
    //paramètres: une table de hashage t clé de type Key
    //
    //valeur de retour: l'indice 
    int cle = hash_function(key, t->size);
    if(t->tab[cle] != NULL){
        if(t->tab[cle]->key){
            if(t->tab[cle]->key->val == key->val && t->tab[cle]->key->n == key->n){
                return cle;
            }
            else{
                int i = 1;
                while(t->tab[(cle+i)%t->size] && i%t->size != 0){
                    if(t->tab[cle]->key->val == key->val && t->tab[cle]->key->n == key->n){
                        return (cle+i)%t->size;
                    }
                    i++;
                }
                if(i%t->size == 0) return -1;
                return (cle+i)%t->size;
            }
        }
    }
    return cle;
}

HashTable *create_hashtable(CellKey *keys, int size){
    //paramètres: une liste chaînée de clé et la taille de la liste
    //
    //valeur de retour: la table de hachage crée
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
    for(int i=0; i<h->size; i++){
        h->tab[i] = NULL;
    }
    while(tmp){
        h->tab[find_position(h, tmp->data)] = create_hashcell(tmp->data);
        tmp = tmp->next;
    }
    return h;
}

void delete_hashtable(HashTable* t){
    //paramètres: une table de hachage t
    //
    //valeur de retour: aucune
    for(int i=0; i<t->size; i++){
        free(t->tab[i]->key);
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    //paramètres: 
    //
    //valeur de retour: la clé du gagnat
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
                if(hc->tab[find_position(hc, pKeyC)]){
                    hc->tab[find_position(hc, pKeyC)]->val++;
                }
                free(pKeyC);
            }
        }
        tmp = tmp->next;
    }
    int i=0;
    HashCell *max = hc->tab[i];
    while(!max){
        i++;
        max = hc->tab[i];
    }
    for(int i=1; i<hc->size; i++){
        if(hc->tab[i]){
            if(max->val < hc->tab[i]->val){
                max = hc->tab[i];
            }
        }
    }
    Key *winner = hc->tab[find_position(hc, max->key)]->key;
    return winner;
}