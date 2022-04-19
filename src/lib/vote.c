#include "vote.h"


void submit_vote(Protected* p){
    FILE *F=foppen("Pending_votes.txt","a");
    char* pstr=protected_to_str(p);
    fprintf(F,"%s",pstr);
    fputc(F,'\n');
    fclose(F);
    free(pstr);    
}
/*
CellProtected* vote=create_cell_protected(p);
    Block* bloc = malloc(sizeof(Block));
    bloc->author=p->pKey;
    bloc->previous_hash=NULL;
    bloc->nonce=0;
    bloc->hash=miaou256(protected_to_str("123"));//123 pour que ça ne passe pas le premier test
    bloc->votes=vote;                           //compute_proof_of_work mettra tout seul la bonne valeur hash
    compute_proof_of_work(bloc,5);
    ecrire_block(bloc,"Pending_votes.txt");
*/

void create_block(CellTree* tree, Key* author, int d) {
    FILE *fic = fopen("Pending_vote.txt", "r");
    char buffer[256];
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
    Block* Pending_block=malloc(sizeof(Block));
    remove("Pending_block.txt");
    Pending_block->author=author;
    Pending_block->votes=liste;
    compute_proof_of_work(Pending_block,d);
}
