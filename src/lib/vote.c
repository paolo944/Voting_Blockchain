#include "protected.h"
#include "cellProtected.h"
#include "block.h"
#include "cellTree.h"


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