#include "headers/cellProtected.h"

CellProtected *create_cell_protected(Protected *pr){
    //paramètres: pointeur pr de type Protected
    //création d'une cellule contenant pr de type CellProtected
    //valeur de retour: la cellule crée de type CellProtected
    CellProtected *c = (CellProtected*)malloc(sizeof(CellProtected)); //allocation de mémoire
    if(!c){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        free(c);
        return NULL;
    }
    c->data = pr;
    c->next = NULL;
    return c;
}

void ajout_en_tete_protected(CellProtected *pr, CellProtected **liste){
    //paramètres: pointeur vers cellule de type CellProtected et un pointeur vers une liste de type CellProtected
    //ajout en tête de pr dans liste
    //valeur de retour: aucune
    if(*liste == NULL){ //vérification que la liste n'est pas vide
        *liste = pr; //premier élément de la liste est pr
        return;
    }
    pr->next = *liste; //ajout de pr en tête de liste
    *liste = pr; //liste pointe vers pr
}

CellProtected *read_protected(char *nomfic){
    //paramètres: chaine de caractère contenant le nom du fichier à ouvrir
    //lecture du fichier et création d'une liste avec toutes les déclarations contenu dans le fichier
    //valeur de retour: la liste crée
    FILE *fic = fopen(nomfic, "r"); //ouverture du fichier
    if(!fic){ //vérification de l'ouverture
        return NULL;
    }
    char buffer[256]; //buffer
    Protected *pr = NULL; //pointeur
    CellProtected *liste = NULL; //la liste crée qui sera retournée
    while(fgets(buffer, 256, fic)){ //lecture de chaque ligne
        pr = str_to_protected(buffer); //allocation de mémoire
        ajout_en_tete_protected(create_cell_protected(pr), &liste); //allocation de mémoire et ajout de la cellule dans la liste
    }
    return liste;
}

void afficher_cell_protected(CellProtected *liste){
    //paramètres: pointeur vers une liste de CellProtected
    //affiche toutes les cellules de la liste
    //valeur de retour: aucune
    CellProtected *tmp = liste; //tampon pour manipuler la liste
    while(tmp){ //tant que toute la liste n'est pas parcourue entièrement
        printf("%s\n", protected_to_str(tmp->data)); //affichage de la cellule actuelle
        tmp = tmp->next;
    }
}

void delete_cell_protected(CellProtected *c){
    //paramètres: pointeur vers une cellule c de type CellProtected 
    //supprime la cellule c
    //valeur de retour: aucune
    delete_protected(c->data);
    free(c);
}

void delete_liste_protected(CellProtected *c){
    //paramètres: pointeur vers une cellule c de type CellProtected
    //supprime toutes les cellules de la liste
    //valeur de retour: aucune
    CellProtected *tmp = NULL; //tampon
    while(c){ //tant que la liste n'est pas
        tmp = c->next;
        delete_cell_protected(c); //libération de la cellule
        c = tmp;
    }
    free(c);
}

void verification_fraude(CellProtected **liste){
    //paramètres: pointeur vers une pointeur vers liste de type CellProtected
    //vérification de toutes signatures contenu dans la liste
    //valeur de retour: aucune
    CellProtected *c = *liste; //pointeur vers le premier élément de la liste
    CellProtected *tmp = NULL; //tampon
    while(c->next){ //tant que toute la liste n'est pas parcourue
        if(!verify(c->next->data)){ //si la signature n'est pas celle du votant
            tmp = c->next;
            c->next = c->next->next; //le parcourscontinue
            delete_cell_protected(tmp); //supprimer la cellule qui n'est pas correcte
        }
        c = c->next;
    }
    if(!verify((*liste)->data)){ //vérification de la première case de la liste
        tmp = *liste;
        liste = &((*liste)->next);
        delete_cell_protected(tmp); //supprmier la case
    }
}
