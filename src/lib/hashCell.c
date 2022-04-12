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
    h->key = malloc(sizeof(Key));
	init_key(h->key, key->val, key->n);
    h->val = 0;
    return h;
}

int hash_function(Key *key, int size){
    //paramètres: clé de type Key et la taille de la table
    //
    //valeur de retour: la valeur de hachage de key->val
	if(!key){
		printf("Key non initialisé\n");
		return -1;	
	}
	float nbo = (sqrtf(5)-1)/2; //on utilise le nombre d'or diminué de 1
    return ((int) floor(size * (key->val * nbo - floor(key->val * nbo))));
	//return (int)(size*((key->val)*((sqrt(5)-1)/2)-(int)((key->val)*((sqrt(5)-1)/2))));
}

int find_position(HashTable *t, Key *key){
    //paramètres: une table de hashage t clé de type Key
    //renvoie l'indice de la clé dans la table sinon l'indice ou elle devrait se trouver
    //valeur de retour: l'indice de la clé dans la table
	if(!key || !t){
		printf("erreur\n");
		return -1;
	}
    int cle = hash_function(key, t->size);
	if(!t->tab){
		printf("tableau non initialisé\n");
		return 0;	
	}
    if(t->tab[cle] != NULL){
        if(t->tab[cle]->key){
            if(t->tab[cle]->key->val == key->val && t->tab[cle]->key->n == key->n){
                return cle;
            }
            else{
                int i = 1;
				int k = (cle+i)%t->size;
                while(t->tab[k] && i%t->size){
                    if(t->tab[cle]->key->val == key->val && t->tab[cle]->key->n == key->n){
                        return k;
                    }
                    i++;
					k = (cle+i)%t->size;
                }
                if(i%t->size == 0) return cle;
                return k;
            }
        }
    }
	return cle;
}

HashTable *create_hashtable(CellKey *keys, int size){
    //paramètres: une liste chaînée de clé et la taille de la liste
    //
    //valeur de retour: la table de hachage crée
    HashTable *h = (HashTable*)calloc(1, sizeof(HashTable)); //Allocation de la mémoire
    if(!h){
        printf("erreur d'allocation\n");
        return NULL;
	}
	
    h->tab = (HashCell**)calloc(size, sizeof(HashCell*)); //Allocation de la mémoire
	if(!h->tab){
		printf("Erreur d'allocation\n");
		free(h);
		return NULL;
	}
	
	h->size = size;
	
	if (len_cellkey(keys) > size)
    {
        puts("Attention, le tableau de hashage n'est pas assez grand pour toutes les clés.");
    }

    CellKey *tmp = keys;
	int i = 0;
	
    while(tmp){
		i = find_position(h, tmp->data);
		if(!(h->tab[i])){
        	h->tab[i] = create_hashcell(tmp->data);
		}
		else{
			printf("le tableau est trop petit\n");
		}
		tmp = tmp->next;
    }
    return h;
}

void delete_hashtable(HashTable* t){
    //paramètres: une table de hachage t
    //
    //valeur de retour: aucune
	if(!t){
		printf("hash table inexistante \n");
		return;
	}
    for(int i=0; i<t->size; i++){
		if(t->tab[i]){
			if(t->tab[i]->key){
				free(t->tab[i]->key);	
			}
			free(t->tab[i]);
		}
    }
    free(t->tab);
    free(t);
}

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeV, int sizeC){
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
    Key *winner = (Key*)malloc(sizeof(Key));
	if(!winner){
		printf("erreur d'allocation\n");
		return NULL;
	}
	init_key(winner, hc->tab[find_position(hc, max->key)]->key->val, hc->tab[find_position(hc, max->key)]->key->n) ;
	delete_hashtable(hc);
	delete_hashtable(hv);
    return winner;
}
