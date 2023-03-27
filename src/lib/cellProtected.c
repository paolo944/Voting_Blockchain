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
    fclose(fic);
    return liste;
}

void afficher_cell_protected(CellProtected *liste){
    //paramètres: pointeur vers une liste de CellProtected
    //affiche toutes les cellules de la liste
    //valeur de retour: aucune
    CellProtected *tmp = liste; //tampon pour manipuler la liste
    char *chaine;
	while(tmp){ //tant que toute la liste n'est pas parcourue entièrement
        chaine = protected_to_str(tmp->data);
        printf("%s\n", chaine); //affichage de la cellule actuelle
        free(chaine);
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
    CellProtected* prec = *liste;
    
    if (!prec->data || !verify(prec->data))
    {
        *liste = (*liste)->next;
        delete_cell_protected(prec);
    }
    
    prec = *liste;
    CellProtected* temp = (*liste)->next;

    while (temp)
    {
        if (!temp->data || !verify(temp->data))
        {
            if (!temp->next)
            {
                delete_cell_protected(temp);
                prec->next = NULL;
            }
            else
            {
                prec->next = temp->next;
                delete_cell_protected(temp);
            }
        }
        prec = prec->next;
        temp = temp->next;
    }
}

void fusion(CellProtected **l1, CellProtected **l2){
    //paramètres: deux listes
    //fusion des deux listes
    //valeur de retour: aucune
	if(*l1 == NULL){
		*l1 = *l2;
		*l2 = NULL;
	}
	if(*l2 == NULL){
		return;	
	}
	
	CellProtected *tmp = *l1;
	
	while(tmp->next != NULL){
		tmp = tmp->next;	
	}
	tmp->next = *l2;
	*l2 = NULL;
}
