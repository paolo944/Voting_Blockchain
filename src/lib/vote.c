#include "headers/vote.h"


void submit_vote(Protected* p){
	//paramètres: un protected
    //la fonction ajoute le vote dans pending vote
    //valeur de retour: aucune
    FILE *F = fopen("Pending_votes.txt","a");
	if(!F){
		printf("erreur pendant l'ouverture\n");
		return;
	}
    char* pstr = protected_to_str(p);
    fprintf(F,"%s",pstr);
    fputc('\n', F);
    free(pstr);
	fclose(F);
}


void create_block(CellTree* tree, Key* author, int d) {
	//paramètres: une arbre, un auteur et le nombre de 0 voulu
    //crée un block qui est lu dans pending votes et le met dans pending block
    //valeur de retour: aucune
    Block* Pending_block=malloc(sizeof(Block));
    Pending_block->votes = read_protected("Pending_votes.txt");

    remove("Pending_votes.txt");
	Key *author2 = (Key*)malloc(sizeof(Key));
	init_key(author2, author->val, author->n);
    Pending_block->author = author2;
    Pending_block->previous_hash=(unsigned char*)strdup((char*)last_node(tree)->block->hash);

    Pending_block->hash=NULL;
    compute_proof_of_work(Pending_block,d);
	add_child(last_node(tree), create_node(Pending_block));
    ecrire_block(Pending_block,"Pending_block.txt");
}

void add_block(int d, char* name){
	//paramètres: le nombre de 0 voulu et le nom du fichier
    //ajoute le block qui est dans pending block dans le repertoire blockchain et le verifie avant
    //valeur de retour: aucune
    Block *b = lire_block("Pending_block.txt");
	
	if(verify_block(b,d)){ //verifie que b ait bien le bon nombre de 0
		char str[256];
    	strcpy(str, "./Blockchain/");
		strcat(str, name);
		strcat(str, ".txt");
		ecrire_block(b, str);
    }
	delete_block2(b);
    remove("Pending_block.txt");
}

CellTree* read_tree(){
	//paramètres: aucune
    //lit l'arbre dans le repertoire blockchain et lit tous les block qui sont dedans
    //valeur de retour: l'arbre lu
	int cpt = 0;
	
	DIR *rep = opendir("./Blockchain/");
	char fichier[30];
	struct dirent* dir;
	while ((dir=readdir(rep))){ //compter le nombres de fichiers (blocks) présents
    	if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
			cpt++;
		}
	}
	
	rewinddir(rep);
	
    CellTree** T = calloc(cpt, sizeof(CellTree*));
	if(!T){
		printf("erreur d'allocation\n");
		return NULL;
	}
    
    if(rep!=NULL){
		int i = 0;
        while ((dir=readdir(rep))){
    		if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
				strcpy(fichier, "./Blockchain/");
				strcat(fichier, dir->d_name);
				Block *b = lire_block(fichier);
                T[i]=create_node(b);
                i+=1;
            }
        }
        closedir(rep);
    }
	else{
		printf("repertoire non existant ou non accessible\n");
		return NULL;
	}
    
	int f = 0;
	for(int i = 0; i < cpt; i++){
		for(int k = 0; k < cpt; k++){
			if(T[k]->block->previous_hash != NULL){
            	if(strcmp((const char*)T[k]->block->previous_hash, (const char*)T[i]->block->hash) == 0){
                	add_child(T[i],T[k]);
					printf("%d\n", f++);
            	}
			}
        }
    }
	
	CellTree *tmp = NULL;
	
	f = 0;
	
  	for(int j = 0; j<cpt; j++){
        if(T[j]->father==NULL){
        	tmp = T[j];
			printf("%d\n", f++);
        }
	}
	
	free(T);
	return tmp;
}

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	//paramètres: un arbre, liste de candidats, liste de votants, nombre de candidants, nombre votants
    //renvoie le gagnant
    //valeur de retour: la clé du gagnant
    CellProtected* ListeVote=longest_node_votes(tree); //liste totale des votes
	Key *winner = compute_winner(ListeVote,candidates,voters,sizeC,sizeV);
	delete_liste_protected(ListeVote);
    return winner;
}