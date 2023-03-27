#include "headers/cellKey.h"

CellKey *create_cell_key(Key *key){
    //paramètres: clé de type Key
    //crée la cellule de la liste chainée qui contient la clé donnée en paramètre
    //valeur de retour: la cellule crée
    CellKey *cell = (CellKey*)malloc(sizeof(CellKey)); //allocation de mémoire
    if(!cell){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(cell);
        return NULL;
    }
    else{
        cell->data = key;
        cell->next = NULL;
    }
    return cell;
}

void ajout_en_tete(CellKey *key, CellKey **liste){
    //paramètres: une cellule de type CellKey et une liste de CellKey
    //ajoute la cellule Key dans la liste
    //valeur de retour: aucune
    if(*liste == NULL){
        *liste = key;
        return;
    }
    key->next = *liste;
    *liste = key;
}

CellKey *read_public_keys(char *nomFic){
    //paramètres: une chaine de caractère contenant le nom fichier à lire
    //fonction qui lit le fichier nomFic et retourne la liste contenant les clés du fichier
    //valeur de retour: la liste des clés lus de type CellKey
    FILE *file = fopen(nomFic, "r"); //ouverture du fichier
    if(!file){ //vérification de l'ouverture du fichier
        return NULL;
    }
    char buffer[256]; //buffer
    Key *key = NULL; //pointeur vers les clés qui seront lues
    CellKey *liste = NULL; //liste qui sera retourné
    while(fgets(buffer, 256, file)){ //lecture de toutes 
        key = str_to_key(buffer); //allocation de mémoire
        ajout_en_tete(create_cell_key(key), &liste); //allocation de mémoire
    }
    fclose(file); //fermeture du fichier
    return liste;
}

void print_list_keys(CellKey *LCK){
    //paramètres: pointeur vers cellule LCK de type de CellKey
    //affichage de la liste LCK
    //valeur de retour: void
    CellKey *tmp = LCK; //tampon pour manipuler la liste
    char *dataStr = NULL;
    while(tmp){ //tant que n'est pas entièrement parcourue
        dataStr = key_to_str(tmp->data); //allocation de la mémoire (libérée)
        printf("%s\n", dataStr); //affichage d'une cellule
        tmp = tmp->next;
        free(dataStr); //libération de la mémoire
    }
}

void delete_cell_key(CellKey *c){
    //paramètres: pointeur vers la cellule à supprimer de type de CellKey
    //supprime la cellule
    //valeur de retour: aucune
    free(c->data);
    free(c);
}

void delete_liste_key(CellKey *c){
    //paramètres: pointeur vers la liste à supprimer de type de CellKey
    //supprime la liste et toutes les cellules de c
    //valeur de retour: aucune
    CellKey *tmp = NULL; //tampon
    while(c){ //tant que la liste n'est pas vide
        tmp = c->next;
        delete_cell_key(c);
        c = tmp;
    }
    free(c);
}

int len_cellkey(CellKey *c){
	int i = 0;
	CellKey *tmp = c;
	while(tmp){
		i++;
		tmp = tmp->next;
	}
	return i;
}
