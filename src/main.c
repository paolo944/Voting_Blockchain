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

void generate_random_data(int nv, int nc){
	FILE *file = fopen("keys.txt", "w");
	if(!file){
		printf("Erreur pendant l'ouverture du fichier\n");
	}
	else{
		Key *pKey = malloc(sizeof(Key));
    	Key *sKey = malloc(sizeof(Key));
		for(int i=0; i<nv; i++){
			Key *pKey = malloc(sizeof(Key));
    		Key *sKey = malloc(sizeof(Key));
    		init_pair_keys(pKey, sKey, 3, 7);
    		fprintf(file, "%s,%s", key_to_str(pKey), key_to_str(sKey));
    		fputc('\n', file);
		}
		free(pKey);
		free(sKey);
		int ligne[nc];
		for(int i=0; i<nc; i++){
			ligne[i] = rand()%nc;
		}
	}
	fclose(file);
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
	generate_random_data(100, 0);
    return 0;
}
