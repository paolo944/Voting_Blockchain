#include "headers/block.h"
#include <stdlib.h>

void ecrire_block(Block *b, char *nomFic){
    FILE *fic = fopen(nomFic, "w");

    fprintf(fic, "%s", key_to_str(b->author));
    fputc('\n', fic);
    fprintf(fic, "%s", b->hash);
    fputc('\n', fic);
    fprintf(fic, "%s", b->previous_hash);
    fputc('\n', fic);
    fprintf(fic, "%d", b->nonce);
    fputc('\n', fic);
    CellProtected *tmp = b->votes;
    while(tmp){
        fprintf(fic, "%s", protected_to_str(tmp->data));
        fputc('\n', fic);
        tmp = tmp->next;
    }

    fclose(fic);
}

Block *lire_block(char *nomFic){
    FILE *fic = fopen(nomFic, "r");
    Block *b = (Block*)malloc(sizeof(Block));
    char buffer[256];

    char *keyStr = NULL;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%s\n", keyStr) == 1){
        b->author = str_to_key(keyStr);
        free(keyStr);
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    unsigned char *hash = NULL;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%s\n", hash) == 1){
        b->hash = hash;
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    unsigned char *prevHash = NULL;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%s\n", prevHash) == 1){
        b->previous_hash = prevHash;
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    int nonce;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%d\n", &nonce) == 1){
        b->nonce = nonce;
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    CellProtected *liste = NULL;
    char *protectedStr = NULL;
    while(fgets(buffer, 256, fic)){
        if(sscanf(buffer, "%s\n", protectedStr) == 1){
            ajout_en_tete_protected(create_cell_protected(str_to_protected(protectedStr)), &liste);
        }
        else{
            printf("erreur de lecture\n");
            return NULL;
        }
    }
    
    b->votes = liste;

    fclose(fic);
    return b;
}

char *block_to_str(Block *block){
    char *key = key_to_str(block->author);
    unsigned char *previous_hashchar = block->previous_hash;
    int lenNonceChar;
    for(int i =0 ;((block->nonce)/(pow(10,i)))>=1;i++){
        lenNonceChar+=1;
    }
    char *noncechar = malloc(lenNonceChar*(sizeof(char)));
    sprintf(noncechar, "%d", block->nonce);
    CellProtected *tmp = block->votes;
    int lenVotes;
    while(tmp->next==NULL){
        char *tmp2 = protected_to_str(tmp->data);
        lenVotes+=strlen(tmp2);
        tmp=tmp->next;
        free(tmp2);
    }
    int lenTotale=lenVotes+lenNonceChar+strlen(previous_hashchar);
    char *res = malloc(lenTotale+1*(sizeof(char)));

    strcat(res,key);
    
    strcat(res,previous_hashchar);
    
    tmp=block->votes;
    while(tmp->next==NULL){
        char *tmp2 = protected_to_str(tmp->data);
        strcat(res,tmp2);
        tmp=tmp->next;
        free(tmp2);

    }
    
    strcat(res,noncechar);
	return res;
}

unsigned char *miaou256(const char *mess){
	unsigned char *res = SHA256(mess, strlen(mess), 0);
	return res;
}

void compute_proof_of_work(Block* B,int d){
    B->nonce=0;
    unsigned char* testHexa=miaou256(block_to_str(B));
    int i=1;
    while (i){
        int p=0;
        for(int j=0;j<d;j++){
            if (testHexa[j]!=0){
                p-=1;
                B->nonce+=1;
                testHexa=miaou256(block_to_str(B));
                break;
            }
            else{
                p+=1;
            }
		}
        if (p==d){
            i=0;
        }
    }
}
							 
int verify_block(Block* b, int d){
    unsigned char* testHexa=miaou256(block_to_str(b));
    int p=0;
    for(int j=0;j<d;j++){
        if (testHexa[j]!=0){
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