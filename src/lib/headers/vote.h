#include "block.h"
#include "cellKey.h"
#include "cellTree.h"
#include "hashCell.h"
#include <dirent.h>
#include <stdio.h>

#ifndef VOTE_H
#define VOTE_H


void submit_vote(Protected* p);
void create_block(CellTree* tree, Key* author, int d);
void add_block(int d, char* name);
Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);
CellTree* read_tree();

#endif //VOTE_H