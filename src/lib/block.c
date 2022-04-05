#include "headers/block.h"

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

    char *keyStr;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%s\n", keyStr) == 1){
        b->author = str_to_key(keyStr);
        free(keyStr);
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    char *hash;
    fgets(buffer, 256, fic);
    if(sscanf(buffer, "%s\n", hash) == 1){
        b->hash = hash;
    }
    else{
        printf("erreur de lecture\n");
        return NULL;
    }

    char *prevHash;
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
    if(sscanf(buffer, "%d\n", nonce) == 1){
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
    char *c = NULL;

    return c;
}
