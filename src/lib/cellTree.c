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

CellTree* racineCellTree(CellTree* node){
    CellTree* tmp=node;
    while(tmp->father){
        tmp=tmp->father;
    }
    return tmp;
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
        CellTree* tmp=racineCellTree(arbre);
        printf("Hauteur:%d Valeur hachée:%s \n",tmp->height,tmp->block->hash);
        if(tmp->firstChild){
            CellTree* tmp2=tmp->firstChild;
            print_tree(tmp2);
            while (tmp2->nextBro){ 
                tmp2=tmp2->nextBro;
                print_tree(tmp2);
            }
            tmp=tmp->nextBro;
        }
    }
}

void delete_node(CellTree* node){
    if (node->father){
        CellTree* tmp= node->father;
        if(node->nextBro){
            if(tmp->firstChild==node){
                tmp->firstChild==node->nextBro;
            }
            else{
                CellTree* tmp2=node->father->firstChild;
                while(tmp2->nextBro!=node){
                    tmp2=tmp2->nextBro;
                }
                tmp2->nextBro=node->nextBro;
            }
        }
        else{
            if(tmp->firstChild=node){
                if(node->firstChild){
                    tmp->firstChild=node->firstChild;
                }
                else{tmp->firstChild=NULL;}
            }
            else{
                tmp->firstChild->nextBro=NULL;
                tmp->firstChild->nextBro=node->firstChild;
                
            }
        }
    }
    delete_block(node->block);
    free(node);
}

void delete_tree(CellTree* arbre){
    CellTree* tmp=racineCellTree(arbre);
    while(tmp->firstChild){
        tmp=tmp->firstChild;
        delete_node(tmp->firstChild);
    }
    delete_node(tmp);
}



CellTree* highest_child(CellTree* cell){
    CellTree* res;
    CellTree* tmp;
    if(cell->firstChild){
        res=cell->firstChild;
        tmp=res;
        while(tmp->nextBro){
            tmp=tmp->nextBro;
            if(res->height<tmp->nextBro){
                res=tmp;
            }
        return res;
        }
    }
    return NULL;  //jsp
}

CellTree* last_node(CellTree* arbre){
    CellTree* tmp=racinceCellTree(arbre);
    while (tmp->firstChild){
        tmp=highest_child(tmp);
    }
    return tmp;
}