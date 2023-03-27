#include "headers/cellTree.h"

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
	return Arbre;
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
    if(father->firstChild){
        CellTree* tmp=father->firstChild;
        while(tmp->nextBro){
            tmp=tmp->nextBro;
        }
        tmp->nextBro=child;
    }
    else{
        father->firstChild=child;
    }
	child->father = father;
    if(update_height(father,child)){
        CellTree* tmp=father;
        CellTree* tmp2=child;
        while(tmp2){
            update_height(tmp,tmp2);
			if(tmp->father){
            	tmp2=tmp;
            	tmp=tmp->father;
			}
			else{
				tmp2 = NULL;	
			}
		}
    }
}

void print_tree(CellTree* arbre){
    if(arbre){
    	printf("Hauteur:%d Valeur hachée:%s \n",arbre->height,arbre->block->hash);
        print_tree(arbre->firstChild);
		print_tree(arbre->nextBro);
	}
}

void delete_node(CellTree* node){
    if(node != NULL){
    	delete_block(node->block);
    	free(node);
		node = NULL;
	}
}

void delete_node2(CellTree* node){
    if(node){
    	delete_block2(node->block);
		node->father = NULL;
  		node->firstChild = NULL;
  		node->nextBro = NULL;
    	free(node);
	}
}

void delete_tree(CellTree* arbre){
    if(arbre != NULL){
		if(arbre->nextBro != NULL){
        	delete_tree(arbre->nextBro);
		}
		if(arbre->firstChild != NULL){
			delete_tree(arbre->firstChild);
		}
		delete_node(arbre);
		arbre = NULL;
    }
}

void delete_tree2(CellTree* arbre){
    if(arbre != NULL){
		if(arbre->nextBro != NULL){
        	delete_tree2(arbre->nextBro);
		}
		if(arbre->firstChild != NULL){
			delete_tree2(arbre->firstChild);
		}
		delete_node2(arbre);
		arbre = NULL;
    }
}



CellTree* highest_child(CellTree* cell){
	if(cell == NULL || cell->firstChild == NULL){
		return NULL;		
	}
	
    CellTree* res=cell->firstChild;;
    CellTree* tmp=cell->nextBro;
	
    while(tmp){
        if(res->height<tmp->height){
			res=tmp;
        }
		tmp=tmp->nextBro;
    }
	return res;
}

CellTree* last_node(CellTree* arbre){
	if(!arbre){
		return NULL;	
	}
	if(arbre->firstChild == NULL){
		return arbre;	
	}
	
    CellTree* tmp=arbre;
    while(tmp->firstChild != NULL){
        tmp=highest_child(tmp);
    }
    return tmp;
}

CellProtected *longest_node_votes(CellTree *cell){
	if(!cell){
		printf("arbre vide\n");
		return NULL;
	}

	CellProtected *liste = NULL;
	fusion(&liste, &(cell->block->votes));
	
	CellTree *tmp = highest_child(cell);
	
	while(tmp){
		fusion(&liste, &(tmp->block->votes));
		tmp = highest_child(tmp);
	}
	return liste;
}
