#include "lib/headers/hashCell.h"
#include "lib/headers/vote.h"


void test3 (void (*fct)(Block*, int), Block* b, int min, int max, int pas)
{
    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;
    FILE* fd = fopen("compute_pof3.txt", "w");
    int i = 0;                                      //compteur de tour
    while (min < max)
    {
        temps_initial = clock();
        fct(b, min);                                //lance compute_proof_of_work  
        temps_final = clock();
        temps_cpu = ((double) (temps_final - temps_initial)) / CLOCKS_PER_SEC;

        printf("Test n°%d\n", ++i);                 //printf en stdout le numéro du test
        fprintf(fd, "%d %f\n", min, temps_cpu);     //fprintf dans fd le temps que ça a pris
        min += pas;                                 //on incrémente de pas
    }
    
    //si notre incrémentation dépasse le max (ou est égale à max mais on sort de la boucle), on effectue un dernier test à la valeur max
    temps_initial = clock();
    fct(b, max);
    temps_final = clock();
    temps_cpu = ((double) (temps_final - temps_initial)) / CLOCKS_PER_SEC;
    
    printf("Test n°%d\n", ++i);
    fprintf(fd, "%d %f\n", max, temps_cpu);
    fclose(fd);
}

void print_long_vector(long *result, int size){ //Fonction fourni par le sujet
    printf ("Vector: [");
    for(int i=0; i < size; i++){
        printf("%lx \t" , result[i]);
    }
    printf ("]\n");
}

int present(int *tab, int len, int x){
    //paramètres: tab[int], int len, int x(élément à rechercher)
    //recherche si x est présent dans un tableau non trié
    //Retourne 1 si est présent et 0 sinon.
    for(int i=0; i<len; i++){
        if(tab[i] == x){
			return 1;
		}
    }
    return 0;
}

void remplir(int *tab, int len){
	for(int i=0; i<len; i++){
		tab[i] = -1;
	}
}

static int intCompare(const void *p1, const void *p2){
    int int_a = *((int*)p1);
    int int_b = *((int*)p2);

    if(int_a == int_b) return 0;
    else if(int_a < int_b) return -1;
    else return 1;
}

void afficher_tab(int *tab, int len){
    //fonction d'affichage du tableau tab de taile len
    //Aucune valeur de retour
    for(int i=0; i<len; i++){
        printf("tab[%d] = %d\n", i, tab[i]);
    }
}

Signature *vote(Key *sKey, Key *candidat){
    //paramètres: clé privé du votant et clé public du cadidat
    //Signe la clé public du candidat avec la clé privée du votant
    //Retourne la signature
    char *s = key_to_str(candidat); //Allocation de la mémoire (libéré)
    Signature *signature = sign(s, sKey);
    free(s); //Libération de la mémoire alloué
    return signature;
}

void generate_random_data(int nv, int nc){
    //Paramètres: nombre de votant voulu et nombre de candidat parmi les votants voulu
    //Genère un fichier contenant les votants, un fichier contenant les candidats
    //Et un dernier fichier contenant les déclarations de vote
    //Aucune valeur de retour
	FILE *file = fopen("votants.txt", "w+"); //Ouverture du fichier key.txt
    FILE *fileC = fopen("candidates.txt", "w+"); //Ouverture du fichier candidats.txt
    FILE *fileV = fopen("declarations.txt", "w"); //Ouverture du fichier declarations.txt
	if(!file){ //vérification de l'ouverture
		printf("Erreur pendant l'ouverture du fichier\n");
        return;
	}
    if(!fileC){ //vérification de l'ouverture
		printf("Erreur pendant l'ouverture du fichier\n");
        return;
    }
    if(!fileV){ //vérification de l'ouverture
		printf("Erreur pendant l'ouverture du fichier\n");
        return;
    }
	else{ //Si toutes les ouvertures se sont bien déroulées
		Key *pKey = (Key*)malloc(sizeof(Key)); //Allocation de la mémoire (libéré)
        if(!pKey){ //vérification de l'allocation
            printf("Erreur d'allocation\n");
            return;
        }
    	Key *sKey = (Key*)malloc(sizeof(Key)); //Allocation de la mémoire (libéré)
        if(!sKey){ //vérification de l'allocation
            printf("Erreur d'allocation\n");
            return;
        }
        //Si toutes les allocations se passent bien
		for(int i=0; i<nv; i++){ //boucle générant les clés des votant
    		init_pair_keys(pKey, sKey, 3, 7);
            char *pKeyStr = key_to_str(pKey); //allocation de la mémoire à chaque tour de boucle (libéré)
            char*sKeyStr = key_to_str(sKey); //allocation de la mémoire à chaque tour de boucle (libéré)
    		fprintf(file, "%s , %s", pKeyStr, sKeyStr); //écriture dans le fichier
    		fputc('\n', file); //saut de ligne dans le fichier
            free(pKeyStr); //Libération de la mémoire
            free(sKeyStr); //Libération de la mémoire
		}
		free(pKey); //Libération de la mémoire
		free(sKey); //Libération de la mémoire
		int ligne[nc]; //tableau de statique contenant les lignes des candidats dans le fichier des votants
		remplir(ligne, nc);
        int x; //variable qui contiendra la nombre aléatoire
        char buffer[256]; //buffer pour le lecture
        char key1[256]; //chaîne de caractère qui contiendra la première clé
        char key2[256]; //chaîne de caractère qui contiendra la première clé
        Key *sKeyV; //pointeur vers la clé privé du votant
        Key *pKeyV; //pointeur vers la clé public du votant
        Key *pKeyC; //pointeur vers la clé public du candidat
        Signature *signature; //pointeur vers la signature
		for(int i=0; i<nc; i++){ //boucle dans laquelle les candidats seront choisis parmi les votants
            do{
                x = rand()%nv; //choix aléatoire d'un candidat
            }           
            while(present(ligne, i+1, x)); //vérification si ce candidat a déjà été choisi
			ligne[i] = x; //si il n'est pas présent, l'ajouter au tableau
        }
        qsort(ligne, nc, sizeof(int), intCompare); //tri du tableau des candidats
        //afficher_tab(ligne, nc);
        rewind(file); //remonter dans le fichier contenant les votants
        int j = 0; //variable d'incrémentation dans la boucle des candidats
        for(int i=0; i<nv; i++){ //boucle qui remplit le fichier des candidats
            if(j == nc){ //si le nombre de candidats trouvé est atteint, la boucle est arrêtée
                break;
            }
            if(fgets(buffer, 256, file)){ //sinon lecture de chaque ligne dans le fichier contenant les votants et vérification de la lecture
                //printf("i=%d, j=%d\n", i, ligne[j]);
                if(i == ligne[j]){ //si la ligne actuelle est celle d'un candidat
                    fprintf(fileC, "%s", buffer); //on recopie dans le fichier des candidats la ligne actuelle
                    j++; //incrémentation de j
                }
            }
            else{ //si la lecture s'est mal passée
                printf("erreur de lecture b\n");
            }
        }
        rewind(file); //remonter dans le fichier contenant les votants
        Protected *pr; //pointeur vers la déclaration de vote de type Protected
        for(int i=0; i<nv; i++){ //boucle pour la déclaration de vote de chaque votant
            rewind(fileC); //remonter dans le fichier contenant les candidats
            x = rand()%nc; //choix du candidats pour le votant actuelle
            if(fgets(buffer, 256, file)){ //lecture de la ligne dans le fichier de votant
                if(sscanf(buffer, "%s , %s", key1, key2) == 2){ //vérification du formatage
                    pKeyV = str_to_key(key1); //allocation de la mémoire à chaque tour de boucle pour la clé publique du votant (libéré)
                    sKeyV = str_to_key(key2); //allocation de la mémoire à chaque tour de boucle pour la clé privée du votant (libéré)
                }
                else{ //si erreur du formatage
                    printf("erreur de formatage\n");
                    return;
                }
            }
            else{ //si il y a une erreur de lecture
                printf("erreur de lecture a\n");
                return;
            }
            for(j=0; j<x-1; j++){ //lecture pour retrouver la ligne contenant le candidat pour lequelle le votant va voter
                if(!fgets(buffer, 256, fileC)){ //si erreur de lecture
                    printf("buffer: %s", buffer);
                    printf("erreur de lecture e\n");
                    return;
                }
            }
            if(sscanf(buffer, "%s , %s", key1, key2) == 2){ //formatage de la ligne du candidat souhaité
            	pKeyC = str_to_key(key1); //allocation de la mémoire à chaque tour de boucle (libéré)       
            }
            else{ //si erreur de formatage
	            printf("erreur de formatage\n");
            }
            signature = vote(sKeyV, pKeyC); //allocation de la mémoire à chaque tour de boucle pour la signature du vote (libéré)
            pr = init_protected(pKeyV, key_to_str(pKeyC), signature); //allocation de la mémoire à chaque tour de boucle conenant la déclaration de vote (libéré)
            char *chaine = protected_to_str(pr);
            fprintf(fileV, "%s", chaine); //écriture dans le fichier declaration.txt la declaration de vote du votant actuelle
            fputc('\n', fileV); //saut de ligne dans le fichier
            free(chaine);
            free(sKeyV); //libération de la mémoire
            free(pKeyC);
            delete_protected(pr); //libération de la mémoire
        }
	}
	fclose(file); //fermeture du fichier
    fclose(fileC); //fermeture du fichier
    fclose(fileV); //fermeture du fichier
}

/*int main ()
{
    srand(time(NULL));

    //Generation de cle :
    long p = random_prime_number(3, 7, 5000);
    long q = random_prime_number (3, 7, 5000);
    while(p == q) {
        q = random_prime_number (3, 7, 5000);
    }
    long n , s , u ;
    generate_key_values (p, q, &n, &s, &u);
    //Pour avoir des cles positives :
    if(u < 0){
        long t = (p - 1) * ( q - 1);
        u = u + t; //on aura toujours s*u mod t = 1
    }

    //Afichage des cles en hexadecimal
    printf("cle publique = (%lx, %lx) \n", s, n);
    printf("cle privee = (%lx, %lx) \n", u , n);

    //Chiffrement:
    char mess[10] = "Hello";
    int len = strlen(mess);
    long * crypted = encrypt(mess, s, n);

    printf("Initial message: %s \n", mess);
    printf("Encoded representation: \n");
    print_long_vector(crypted, len);

    //Dechiffrement
    char * decoded = decrypt (crypted, len, u, n);
    printf ("Decoded : %s \n", decoded) ;

    return 0;
}

int main(){
	FILE *fic = fopen("courbes_random.txt", "w");
	if(fic == NULL){
		printf("erreur d'ouverture");
	}

    clock_t temps_initial;
    clock_t temps_final;
    double temps_cpu;

    for(int i=10; i<800; i+=50){
        temps_initial = clock();
        generate_random_data(1000, i);
        temps_final = clock();
        temps_cpu = (double)(temps_final-temps_initial)/CLOCKS_PER_SEC;
        fprintf(fic, "%d %.10f\n", i, temps_cpu);
    }
	return 0;
}

int main(void){
    srand(time(NULL));
    //début du main fourni
    //Testing Init Keys
    Key *pKey = (Key*)malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!pKey){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        return 1;
    }
    Key *sKey = (Key*)malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!sKey){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        //free(sKey);
        return 1;
    }
    init_pair_keys(pKey, sKey, 3, 7);
    printf("pKey: %lx, %lx \n", pKey->val, pKey->n);
    printf("sKey: %lx, %lx \n", sKey->val, sKey->n);

    //Testing Key Serialization
    char *chaine = key_to_str(pKey); //allocation de la mémoire (libéré)
    printf("key_to_str: %s \n", chaine);
    Key *k = str_to_key(chaine); //allocation de la mémoire (libérée)
    printf("str_to_key: %lx, %lx \n", k->val, k->n);
    free(k);
	free(chaine);
    //Testing signature
    //Cadidate keys
    Key *pKeyC = (Key*)malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!pKeyC){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        return 1;
    }
    Key *sKeyC = (Key*)malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!sKeyC){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        return 1;
    }
    init_pair_keys(pKeyC, sKeyC, 7, 9);
    //Declaration:
    char *mess = key_to_str(pKeyC); //allocation de la mémoire (libérée)
    char *pKeyStr = key_to_str(pKey); //allocation de la mémoire (libérée)
    printf("%s vote pour %s\n", pKeyStr, mess);
    free(pKeyStr);
    Signature *sgn = sign(mess, sKey); //allocation de la mémoire (libérée)
    printf("signature: ");
    print_long_vector(sgn->tab, sgn->taille);
    chaine = signature_to_str(sgn); //allocation de la mémoire (libérée)
    delete_signature(sgn);
    printf("signature_to_str: %s \n", chaine);
    sgn = str_to_signature(chaine);  //allocation de la mémoire (libérée)
    free(chaine);
    printf("str_to_signature: ");
    print_long_vector(sgn->tab, sgn->taille);
    //Testing protected:
    Protected *pr = init_protected(pKey, mess, sgn); //allocation de la mémoire (libérée)
    //Verification:
    if(verify(pr)){
        printf("Signature valide\n");
    }
    else{
        printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr); //allocation de la mémoire (libérée)
    delete_protected(pr);
    printf("protected_to_str: %s\n", chaine);
    pr = str_to_protected(chaine); //allocation de la mémoire (libérée)
    free(chaine);
    char *keyStr = key_to_str(pr->pKey); //allocation de la mémoire (libérée)
    char *signStr = signature_to_str(pr->sign); //allocation de la mémoire (libérée)
    printf("str_to_protected: %s %s %s\n", keyStr, pr->mess, signStr);
    free(keyStr);
    free(signStr);
    delete_protected(pr);
    free(sKeyC);
    free(pKeyC);
    free(sKey);
    //delete_signature(sgn);
    //fin du main fourni

	generate_random_data(10, 2); //génération des donées aléatoire
    CellKey *liste = read_public_keys("keys.txt"); //allocation de la mémoire d'une liste contenant les clés des votants (libérée)
    print_list_keys(liste); //affichage de la liste
    CellProtected *liste2 = read_protected("declarations.txt"); ////allocation de la mémoire d'une liste contenant les déclarations de vote (libérée)
	printf("On ajoute une fraude\n");
	Key* ktest = (Key*)malloc(sizeof(Key));
    ktest->val = 123456;
    ktest->n = 456789;
    Signature* signtest = sign("prout", ktest);
	char *fraude = strdup("coucou la fraude");
    Protected* prtest = init_protected(ktest, fraude, signtest);
    CellProtected* cptest = create_cell_protected(prtest);
	ajout_en_tete_protected(cptest, &liste2);
	afficher_cell_protected(liste2); //affichage de la liste
    verification_fraude(&liste2); //vérification de fraudes
	printf("\e[0;35m-----\e[0;31mAprès vérification des fraudes\e[0;35m-----\033[0m\n");
	afficher_cell_protected(liste2); //affichage de la liste
    delete_liste_key(liste); //libération de la mémoire
    delete_liste_protected(liste2); //libération de la mémoire
    return 0;
}
*/

/*
int main(){
    srand(time(NULL));
	generate_random_data(100, 10); //création d'éléction aléatoire
    CellKey *votants = read_public_keys("keys.txt"); //liste chaînée des votants
    CellProtected *declarations = read_protected("declarations.txt"); //liste chaînée des déclarations de vote
    CellKey *candidats = read_public_keys("candidates.txt"); //liste chaînée des candidats
    Key *winner = compute_winner(declarations, candidats, votants, 10, 100s);
	char *keyStr = key_to_str(winner);
    printf("And the winner is: %s\n", keyStr);
	free(keyStr);
    delete_liste_key(votants); //libération de la mémoire
    delete_liste_protected(declarations); //libération de la mémoire
    delete_liste_key(candidats); //libération de la mémoire
	free(winner);
	Block *b = lire_block("block.txt");
	printf("\nmiaou\n");
	char *block_str = block_to_str(b);
	unsigned char *s = miaou256(block_str);
	free(block_str);
	for(int i=0; i<SHA256_DIGEST_LENGTH; i++){
		printf("%02x", s[i]);	
	}
	printf("\nmiaou fin\n");
	compute_proof_of_work(b, 4);
	printf("%d\n", b->nonce);
	for(int i=0; i<SHA256_DIGEST_LENGTH; i++){
		printf("%02x", b->hash[i]);	
	}
	putchar('\n');
	printf("verify block: %d\n", verify_block(b,4));
	ecrire_block(b, "uiui.txt");
	delete_block(b);

    return 0;
}
*/
int main(){
	srand(time(NULL));
	printf("\t\t\e[0;35m----------\e[0;31mMAIN PARTIE 5 EXO 9\e[0;35m----------\033[0m\n");
	
	generate_random_data(1000, 5); //Creations des votes
	CellProtected *declarations = read_protected("declarations.txt"); //Déclarations de vote
	CellKey *votants = read_public_keys("votants.txt"); //Clé des votants
	CellKey *candidats = read_public_keys("candidates.txt"); //Clé des candidats
	
	CellProtected *tmp = declarations; //tampon pour sumbit vote
	
	int i = 1;
	
	Key *author_pub = (Key*)malloc(sizeof(Key)); //clé publique de l'auteur
	Key *author_priv = (Key*)malloc(sizeof(Key)); //clé privée de l'auteur
	init_pair_keys(author_pub, author_priv, 10, 16); //Création d'une pair de clé pour l'auteur
	char nom[12]; //nom du fichier
	
	Block *b = lire_block("block.txt"); //lecture du block
	
	//test3(compute_proof_of_work, b, 0, 5, 1);
	
    compute_proof_of_work(b, 4)

	CellTree *tree = create_node(b); //création de l'arbre
	
	ecrire_block(b, "uiui.txt"); //écriture du block lue avec le bon nonce
	
	while(tmp){
		if(i%10 == 0){
			create_block(tree, author_pub, 1); //Creation du block
			sprintf(nom, "block%d", (i/10)); //creation du nom du block
			add_block(1, nom); //On ajoute le block
		}
		submit_vote(tmp->data); //sumbit du vote
		i++;
		tmp = tmp->next;
	}
	printf("creations blocks termine\n");
	delete_tree2(tree);
	
	CellTree *arbre = read_tree(); //Lecture de l'arbre
	print_tree(arbre); //Affichage de l'arbre
	
	
	Key *winner = compute_winner_BT(arbre, candidats, votants, 10, 1010); //Calcul du gagnant
	
	char *winner_str = key_to_str(winner);
	
	printf("Le gagnant est %s\n", winner_str); //Affichage du gagnant
	
	
	free(author_priv);
	free(author_pub);
	free(winner);
	free(winner_str);
	delete_tree2(arbre);
	delete_liste_key(votants); //libération de la mémoire
    delete_liste_protected(declarations); //libération de la mémoire
    delete_liste_key(candidats); //libération de la mémoire
	
	return 0;
}
