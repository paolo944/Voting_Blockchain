#include <stdio.h>
#include <stdlib.h>
#include "lib/headers/encryption.h"
#include "lib/headers/rsa.h"
#include "lib/headers/key.h"
#include "lib/headers/signature.h"
#include "lib/headers/protected.h"
#include "lib/headers/cellKey.h"
#include "lib/headers/cellProtected.h"

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

/*
void interclasser(int *t, int p, int m, int g){
    int tmp[p-g+1]; 
    int i, j, k;
    for(i=p; i<=g; i++) tmp[i]=t[i];

    i = p; j=m+1; k=p;

    for(k=p; k<=g; k++){
        if(i==m+1)
            t[k]=tmp[j++];
        else if(j==g+1)
            t[k]=tmp[i++];
        else if(tmp[i]<tmp[j])
            t[k]=tmp[i++];
        else
            t[k]=tmp[j++];
    }
}


void tri_fusion(int *t, int deb, int fin){
    //paramètres: tab[int], int i(indice de tableau), int j(indice du tableau)
    //trie le tableau t
    //Aucune valeur de retour.
    if(deb<fin){
        int milieu = (deb+fin)/2;
        tri_fusion(t, deb, milieu);
        tri_fusion(t, milieu+1, fin);
        interclasser(t, deb, milieu, fin);
    }
}
*/

void merge(int arr[], int p, int q, int r) {

  // Create L ← A[p..q] and M ← A[q+1..r]
  int n1 = q - p + 1;
  int n2 = r - q;

  int L[n1], M[n2];
  remplir(L, n1);
  remplir(M, n2);

  for (int i = 0; i < n1; i++)
    L[i] = arr[p + i];
  for (int j = 0; j < n2; j++)
    M[j] = arr[q + 1 + j];

  // Maintain current index of sub-arrays and main array
  int i, j, k;
  i = 0;
  j = 0;
  k = p;

  // Until we reach either end of either L or M, pick larger among
  // elements L and M and place them in the correct position at A[p..r]
  while (i < n1 && j < n2) {
    if (L[i] <= M[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = M[j];
      j++;
    }
    k++;
  }

  // When we run out of elements in either L or M,
  // pick up the remaining elements and put in A[p..r]
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = M[j];
    j++;
    k++;
  }
}

// Divide the array into two subarrays, sort them and merge them
void mergeSort(int arr[], int l, int r) {
  if (l < r) {

    // m is the point where the array is divided into two subarrays
    int m = l + (r - l) / 2;

    mergeSort(arr, l, m);
    mergeSort(arr, m + 1, r);

    // Merge the sorted subarrays
    merge(arr, l, m, r);
  }
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
	FILE *file = fopen("keys.txt", "w+"); //Ouverture du fichier key.txt
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
		Key *pKey = malloc(sizeof(Key)); //Allocation de la mémoire (libéré)
        if(!pKey){ //vérification de l'allocation
            printf("Erreur d'allocation\n");
            return;
        }
    	Key *sKey = malloc(sizeof(Key)); //Allocation de la mémoire (libéré)
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
        mergeSort(ligne, 0, nc); //tri du tableau des candidats
        afficher_tab(ligne, nc);
        rewind(file); //remonter dans le fichier contenant les votants
        int j = 0; //variable d'incrémentation dans la boucle des candidats
        for(int i=1; i<=nv; i++){ //boucle qui remplit le fichier des candidats
            if(j == nc){ //si le nombre de candidats trouvé est atteint, la boucle est arrêtée
                break;
            }
            if(fgets(buffer, 256, file)){ //sinon lecture de chaque ligne dans le fichier contenant les votants et vérification de la lecture
                //printf("i=%d, j=%d\n", i, ligne[j]);
                if(i == ligne[j]){ //si la ligne actuelle est celle d'un candidat
                    printf("i=%d\n", i);
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
            for(j=0; j<x; j++){ //lecture pour retrouver la ligne contenant le candidat pour lequelle le votant va voter
                if(!fgets(buffer, 256, fileC)){ //si erreur de lecture
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
}*/
int main(void){
    srand(time(NULL));
    //début du main fourni
    //Testing Init Keys
    Key *pKey = malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!pKey){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        return 1;
    }
    Key *sKey = malloc(sizeof(Key)); //allocation de la mémoire (libérée)
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
    //free(k);
	free(chaine);
    //Testing signature
    //Cadidate keys
    Key *pKeyC = malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!pKeyC){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        //free(pKeyC);
        return 1;
    }
    Key *sKeyC = malloc(sizeof(Key)); //allocation de la mémoire (libérée)
    if(!sKeyC){ //vérification de l'allocation
        printf("Erreur d'allocation\n");
        //free(sKeyC);
        return 1;
    }
    init_pair_keys(pKeyC, sKeyC, 7, 9);
    //Declaration:
    char *mess = key_to_str(pKeyC); //allocation de la mémoire (libérée)
    char *pKeyStr = key_to_str(pKey); //allocation de la mémoire (libérée)
    printf("%s vote pour %s\n", pKeyStr, mess);
    free(pKeyStr);
    Signature *sgn = sign(mess, sKey); //allocation de la mémoire (libérée)
    //free(mess);
    printf("signature: ");
    print_long_vector(sgn->tab, sgn->taille);
    chaine = signature_to_str(sgn); //allocation de la mémoire (libérée)
    delete_signature(sgn);
    printf("signature_to_str: %s \n", chaine);
    sgn = str_to_signature(chaine);  //allocation de la mémoire (libérée)
    free(chaine);
    printf("str_to_signature: ");
    print_long_vector(sgn->tab, sgn->taille);
    //free(pKeyC);
    //free(sKeyC);
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
    //free(keyStr);
    free(signStr);
    delete_protected(pr);
    //free(sKey);
    //delete_signature(sgn);
    //fin du main fourni

	generate_random_data(100, 10); //génération des donées aléatoire
    CellKey *liste = read_public_keys("keys.txt"); //allocation de la mémoire d'une liste contenant les clés des votants (libérée)
    print_list_keys(liste); //affichage de la liste
    CellProtected *liste2 = read_protected("declarations.txt"); ////allocation de la mémoire d'une liste contenant les déclarations de vote (libérée)
    afficher_cell_protected(liste2); //affichage de la liste
    verification_fraude(&liste2); //vérification de fraudes
    delete_liste_key(liste); //libération de la mémoire
    delete_liste_protected(liste2); //libération de la mémoire
    printf("sizeof sign: %ld\n", sizeof(Signature));
    return 0;
}
