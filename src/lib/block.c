#include "headers/block.h"
#include <stdlib.h>

void ecrire_block(Block *b, char *nomFic){
	//paramètres: un block et le nom du fichier
    //ecrit le nom donné en paramètre dans un fichier dont le nom est celui donnée en paramètre
    //valeur de retour: aucune
    FILE *fic = fopen(nomFic, "w"); //overture du fichier
	if(!fic){
		printf("erreur d'ouverture\n");
		return;
	}
	
	char *keyStr = key_to_str(b->author);
    fprintf(fic, "%s", keyStr);
    fputc('\n', fic);
	free(keyStr);
	
    fprintf(fic, "%s", (char*)b->hash);
    fputc('\n', fic);
    
	
	fprintf(fic, "%s", (b->previous_hash) ? (char*)b->previous_hash : "NULL");
    fputc('\n', fic);
    
	
	fprintf(fic, "%d", b->nonce);
    fputc('\n', fic);
    
	
	CellProtected *tmp = b->votes;
	char *protectedStr;
    while(tmp){
		protectedStr = protected_to_str(tmp->data);
        fprintf(fic, "%s", protectedStr);
        fputc('\n', fic);
        tmp = tmp->next;
		free(protectedStr);
    }

    fclose(fic);
}

Block *lire_block(char *nomFic){
	//paramètres: le nom du fichier(chaine de caractère)
    //lit le bloc contenu dans ce fichier
    //valeur de retour: le bloc lu
    FILE *fic = fopen(nomFic, "r");
	if(!fic){
		printf("erreur d'ouverture\n");
		return NULL;
	}
    Block *b = (Block*)malloc(sizeof(Block));
	if(!b){
		printf("erreur d'allocation\n");
		return NULL;
	}
	
    char buffer[256];
    
	
	if(!fgets(buffer, 256, fic)){
		printf("erreur de lecture1\n");
		return NULL;
	}
	char keyStr[256];
    if(sscanf(buffer, "%s\n", keyStr) == 1){
        b->author = str_to_key(keyStr);
    }
    else{
        printf("erreur de lecture2\n");
        return NULL;
    }
    
	
	unsigned char len[256];
    if(!fgets(buffer, 256, fic)){
		printf("erreur de lecture3\n");
		return NULL;
	}
    if(sscanf(buffer, "%s\n", len) == 1){
		b->hash = (unsigned char*)strdup((char*)(len));
    }
    else{
        printf("erreur de lecture4\n");
        return NULL;
    }
    
	
	unsigned char prevLen[256];
	if(!fgets(buffer, 256, fic)){
		printf("erreur de lecture5\n");
		return NULL;
	}
    if(sscanf(buffer, "%s\n", prevLen) == 1){
        if(strcmp((char*)prevLen, "NULL") == 0){
			b->previous_hash = NULL;	
		}
		else{
			b->previous_hash = (unsigned char*)strdup((char*)(prevLen));
		}
	}
    else{
        printf("erreur de lecture6\n");
        return NULL;
    }
	
    
	int nonce;
    if(!fgets(buffer, 256, fic)){
		printf("erreur de lecture7\n");
		return NULL;
	}
    if(sscanf(buffer, "%d\n", &nonce) == 1){
        b->nonce = nonce;
    }
    else{
        printf("erreur de lecture8\n");
        return NULL;
    }
    CellProtected *liste = NULL;
    while(fgets(buffer, 256, fic)){
		Protected *p = str_to_protected(buffer);
        ajout_en_tete_protected(create_cell_protected(p), &liste);
    }
    b->votes = liste;
    fclose(fic);
    return b;
}

char *block_to_str(Block *block){
	//paramètres: un bloc
    //retourne le bloc sous forme de chaine de caractère
    //valeur de retour: la chaine du bloc
    char *res = (char*)calloc(20000, (sizeof(char)));
	if(!res){
		printf("erreur d'allocation\n");
		return NULL;
	}
	char *key = key_to_str(block->author);
	
    strcat(res, key);
	free(key);
    
    strcat(res, (char*)(block->previous_hash));
	
    CellProtected *tmp = block->votes;
    while(tmp){
        char *tmp2 = protected_to_str(tmp->data);
        strcat(res, tmp2);
        tmp=tmp->next;
        free(tmp2);
    }
    char *noncechar = (char*)calloc(256, (sizeof(char)));
	if(!noncechar){
		printf("erreur d'allocation\n");
		free(res);
		return NULL;
	}
    sprintf(noncechar, "%d", block->nonce);
    strcat(res, noncechar);
	free(noncechar);
	return res;
}

unsigned char *miaou256(const char *mess){
	return SHA256((unsigned char*)(mess), strlen(mess), 0);
}

unsigned char *hexa_miaou256(char *s) {
	//paramètres: une chaine de caractère
    //retourne la valeur hashé de la chaine de caractere sous forme hexadicimal
    //valeur de retour: 
  unsigned char *d = SHA256((const unsigned char*)s, strlen(s), 0);
  unsigned char tmp[8];

  unsigned char *str = (unsigned char *) malloc(256 * sizeof(unsigned char));

  if (str == NULL) {
    printf("erreur d'allocation\n");
	return;
  }

  str[0] = '\0';


  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf((char*)tmp, "%02x", d[i]);
    strcat((char*)str, (char*)tmp);
  }

  return str;
}

void compute_proof_of_work(Block* B, int d){
	//paramètres: un block et le nombre de 0 voulu
    //calcule le nonce qu'il faut pour avoir au moins d 0 au début du hash du block
    //valeur de retour: aucune
    B->nonce=0;
    unsigned char* testHexa=NULL;
	char *block_str;
    int p=0;
	block_str = block_to_str(B);
	testHexa = hexa_miaou256(block_str);
	free(block_str);
    while(p<d){
    	if(testHexa[p] == '0'){
			p++;
        }
        else{ //si le block n'est pas valide
        	p = 0;
			free(testHexa);
			B->nonce++;
			block_str = block_to_str(B);
			testHexa=hexa_miaou256(block_str);
			free(block_str);
        }
    }
	B->hash = testHexa; //modification du hash
}
							 
int verify_block(Block* b, int d){
	//paramètres: un block et le nombre de 0 voulu
    //verifie que le hash du block contient au moins d 0
    //valeur de retour: 0 si le block est invalide et 1 sinon
    int p=0;
    for(int j=0;j<d;j++){
        if (b->hash[j]!='0'){
            p-=1;
        }
        else{
            p+=1;
        }
	}
    if (p==d){
        return 1;
    }
    else{
        return 0;
    }
}

void delete_block(Block *b){
	//paramètres: un block
    //supprimer tout le block sauf l'auteur et le declarations(protected) mais supprime bien la cellule
    //valeur de retour: aucune
	if(b->hash){
		free(b->hash);
		b->hash = NULL;
	}
	if(b->previous_hash){
		free(b->previous_hash);
		b->previous_hash = NULL;
	}
	CellProtected *tmp = b->votes;
	CellProtected *tmp2;
	while(tmp){
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	b->votes = NULL;
	free(b);
	b = NULL;
}


void delete_block2(Block *b){
	//paramètres: un block
    //supprimer tout le block
    //valeur de retour: aucune
	if(b){
		if(b->author){
			free(b->author);
			b->author=NULL;
		}
		if(b->hash){
			free(b->hash);
			b->hash = NULL;
		}
		if(b->previous_hash != NULL){
			free(b->previous_hash);
			b->previous_hash = NULL;
		}
		if(b->votes){
			delete_liste_protected(b->votes);
			b->votes = NULL;
		}
		free(b);
		b = NULL;
	}
}	