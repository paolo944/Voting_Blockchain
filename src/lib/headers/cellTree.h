#include "block.h"

#ifndef CELLTREE_H
#define CELLTREE_H

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

CellTree* create_node(Block* b);
int update_height(CellTree* father,CellTree* child);
void add_child(CellTree* father, CellTree* child);
void print_tree(CellTree* arbre);
void delete_node(CellTree* node);
void delete_node2(CellTree* node);
void delete_tree(CellTree* arbre);
void delete_tree2(CellTree* arbre);
CellTree* highest_child(CellTree* cell);
CellTree* last_node(CellTree* tree);
CellProtected *longest_node_votes(CellTree *cell);

#endif //CELLTREE_H