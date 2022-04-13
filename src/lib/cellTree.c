#include "cellTree.h"

CellTree* create_node(Block* b){
    CellTree* Arbre = malloc(sizeof(CellTree));
    if(Arbre==NULL){
        printf("Erreur d'allocation de l'arbre");
    }
    Arbre->block=b;
    Arbre->height=0;
    Arbre->firstChild=NULL;
    Arbre->father=NULL;
    Arbre->nextBro=NULL;
}

int update_height(CellTree* father,CellTree* child){
    if((child->height+1)>father->height){
        father->height=(child->height)+1;
        return 1;
    }
    else{
        return 0;
    }
}

void add_child(CellTree* father, CellTree* child){
    father->firstChild->nextBro=child;
    if(update_height(father,child)){
        CellTree* tmp=father;
        CellTree* tmp2=child;
        while(tmp2){
            update_height(tmp,tmp2);
            tmp2=tmp;
            tmp=tmp->father;
        }
    }
}

void print_tree(CellTree* arbre){
    if (arbre){
        CellTree* tmp=arbre;
        if (arbre->father){
            while(tmp->father){
                tmp=tmp->father;
            }
        }
        printf("Hauteur:%d Valeur hachée:%s \n",tmp->height,tmp->block->hash);
        if(tmp->firstChild){
            print_tree(tmp);
            while (tmp->nextBro){ 
                tmp=tmp->nextBro;
                print_tree(tmp);
        }
    }
}

void delete_node(CellTree* node){
    if (node->father){
        CellTree* tmp= node->father;
        if(tmp->nextBro){
            if(tmp->firstChild==node){
                tmp->firstChild==tmp->nextBro
            }
            else{
                CellTree* tmp2=node->father->firstChild;
                while(tmp2->nextBro!=tmp){
                    tmp2=tmp2->nextBro;
                }
                tmp2->nextBro=tmp->nextBro;
            }
        }
        else{
            if(tmp->firstChild){
                tmp->father->firstChild=tmp->firstChild;
            }
            else{tmp->father->firstChild=NULL;}
        }
    }
    delete_block(tmp->block);
    free(tmp);
}