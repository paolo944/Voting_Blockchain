#include "protected.h"
#include "cellProtected.h"
#include "block.h"
#include "cellTree.h"

void submit_vote(Protected* p){
    CellProtected* vote=malloc(sizeof(CellProtected));
    vote->data=p;
    vote->next=NULL;
    Block* bloc = malloc(sizeof(Block));
    bloc->author=p->pKey;
    bloc->previous_hash=NULL;
    bloc->hash=miaou256(protected_to_str("123"));
    bloc->votes=vote;

}