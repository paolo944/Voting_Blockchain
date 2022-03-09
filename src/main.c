#include <stdio.h>
#include <stdlib.h>
#include "lib/headers/encryption.h"
#include "lib/headers/rsa.h"
#include "lib/headers/key.h"
#include "lib/headers/signature.h"
#include "lib/headers/protected.h"

void print_long_vector(long *result, int size){
    printf ("Vector: [");
    for(int i=0; i < size; i++){
        printf("%lx \t" , result[i]);
    }
    printf ("]\n");
}

int present(int *tab, int len, int x){
    for(int i=0; i<len; i++){
        if(tab[i] == x) return 1;
    }
    return 0;
}

void swap(int *t, int i, int j){
    int tmp = t[i];
    t[i] = t[j];
    t[j] = tmp;
}

int tri_rapide_separation(int *t, int deb, int fin){
    int i, sep = deb+1;
    for(i=deb+1; i<=fin; i++){
        if(t[i] < t[deb]){
            if(i != sep) swap(t, i, sep);
            sep++;
        }
    }
    if(sep != deb+1){
        swap(t, deb, sep-1);
    }
    return sep-1;
}

void tri_rapide(int *t, int deb, int fin){
    if(deb<fin){
        int milieu = tri_rapide_separation(t, deb, fin);
        tri_rapide(t, deb, milieu);
        tri_rapide(t, milieu+1, fin);
    }
}

void afficher_tab(int *tab, int len){
    for(int i=0; i<len; i++){
        printf("tab[%d] = %d\n", i, tab[i]);
    }
}

Signature *vote(Key *sKey, Key *candidat){
    Signature *signature = sign(key_to_str(candidat), sKey);
    return signature;
}

void generate_random_data(int nv, int nc){
	FILE *file = fopen("keys.txt", "w+");
    FILE *fileC = fopen("candidates.txt", "w+");
    FILE *fileV = fopen("declarations.txt", "w");
	if(!file){
		printf("Erreur pendant l'ouverture du fichier\n");
	}
    if(!fileC){
		printf("Erreur pendant l'ouverture du fichier\n");
	}
	else{
		Key *pKey = malloc(sizeof(Key));
    	Key *sKey = malloc(sizeof(Key));
		for(int i=0; i<nv; i++){
			Key *pKey = malloc(sizeof(Key));
    		Key *sKey = malloc(sizeof(Key));
    		init_pair_keys(pKey, sKey, 3, 7);
    		fprintf(file, "%s , %s", key_to_str(pKey), key_to_str(sKey));
    		fputc('\n', file);
		}
		free(pKey);
		free(sKey);
		int ligne[nc];
        int x;
        int j = 0;
        char buffer[256];
        char key1[256];
        char key2[256];
        Key *sKeyV;
        Key *pKeyV;
        Key *pKeyC;
        Signature *signature;
		for(int i=0; i<nc; i++){
            do{
                x = rand()%nv;
            }
            while(present(ligne, i+1, x));
			ligne[i] = x;
		}
        tri_rapide(ligne, 0, nc);
        rewind(file);
        for(int i=1; i<=nv; i++){
            if(j == nc){
                break;
            }
            if(fgets(buffer, 256, file)){
                if(i == ligne[j]){
                    fprintf(fileC, "%s", buffer);
                    j++;
                }
            }
            else{
                printf("erreur de lecture\n");
            }
        }
        rewind(file);
        for(int i=0; i<nv; i++){
            rewind(fileC);
            x = rand()%nc;
            if(sscanf(buffer, "%s , %s", key1, key2) == 2){
                pKeyV = str_to_key(key1);
                sKeyV = str_to_key(key2);
            }
            else{
                printf("erreur de formatage\n");
            }
            for(j=0; j<x; j++){
                fgets(buffer, 256, fileC);
            }
            if(sscanf(buffer, "%s , %s", key1, key2) == 2){
                pKeyC = str_to_key(key1);
            }
            else{
                printf("erreur de formatage\n");
            }
            signature = vote(sKeyV, pKeyC);
            fprintf(fileV, "%s", signature_to_str(signature));
            fputc('\n', fileV);
        }
	}
	fclose(file);
    fclose(fileC);
    fclose(fileV);
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

    //Testing Init Keys
    Key *pKey = malloc(sizeof(Key));
    Key *sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey, 3, 7);
    printf("pKey: %lx, %lx \n", pKey->val, pKey->n);
    printf("sKey: %lx, %lx \n", sKey->val, sKey->n);

    //Testing Key Serialization
    char *chaine = key_to_str(pKey);
    printf("key_to_str: %s \n", chaine);
    Key *k = str_to_key(chaine);
    printf("str_to_key: %lx, %lx \n", k->val, k->n);
	free(chaine);

    //Testing signature
    //Cadidate keys
    Key *pKeyC = malloc(sizeof(Key));
    Key *sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC, 7, 9);
    //Declaration:
    char *mess = key_to_str(pKeyC);
    printf("%s vote pour %s\n", key_to_str(pKey), mess);
    Signature *sgn = sign(mess, sKey);
    printf("signature: ");
    print_long_vector(sgn->tab, sgn->taille);
    chaine = signature_to_str(sgn);
    printf("signature_to_str: %s \n", chaine);
    sgn = str_to_signature(chaine);
    printf("str_to_signature: ");
    print_long_vector(sgn->tab, sgn->taille);

    //Testing protected:
    Protected *pr = init_protected(pKey, mess, sgn);
    //Verification:
    if(verify(pr)){
        printf("Signature valide\n");
    }
    else{
        printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr);
    printf("protected_to_str: %s\n", chaine);
    pr = str_to_protected(chaine);
    printf("str_to_protected: %s %s %s\n", key_to_str(pr->pKey), pr->mess, signature_to_str(pr->sign));
    free(pKey);
    free(sKey);
    free(pKeyC);
    free(sKeyC);
	free(chaine);
	free(k);
	free(mess);
	free(pr->pKey);
	free(pr->sign->tab);
	free(pr->sign);
	free(pr->mess);
	free(pr);
	generate_random_data(100, 10);
    return 0;
}
