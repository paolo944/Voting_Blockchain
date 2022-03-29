#include <stdio.h>
#include <time.h>
#include "./LC/fctBiblioLC.h"
#include "./LC/entreeSortieLC.h"
#include "./H/fctBiblioH.h"
#include "./H/entreeSortieH.h"
#include "clock.h"


void test(int num, char *titre, char *auteur, BiblioH *bH, Biblio *bLC, LivreH* (*rechNumH)(BiblioH*, int), LivreH* (*rechTitreH)(BiblioH*, char*), BiblioH* (*rechAuteurH)(BiblioH*, char*), Livre* (*rechNumLC)(Biblio*, int), Livre* (*rechTitreLC)(Biblio*, char*), Biblio* (*rechAuteurLC)(Biblio*, char*)){
	/*Fonction qui teste le temps de recherche de chaque fonction avec les arguments donnée en paramètres
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_numH;
	double temps_cpu_titreH;
	double temps_cpu_auteurH;
	double temps_cpu_numLC;
	double temps_cpu_titreLC;
	double temps_cpu_auteurLC;
	
	temps_initial = clock(); //Recherche Livre Num hachage
	rechNumH(bH, num);
	temps_final = clock();

	temps_cpu_numH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	temps_initial = clock(); //Recherche Livre Titre hachage
	rechTitreH(bH, titre);
	temps_final = clock();

	temps_cpu_titreH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	temps_initial = clock(); //Recherche Livre Auteur hachage
	rechAuteurH(bH, auteur);
	temps_final = clock();

	temps_cpu_auteurH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	temps_initial = clock(); //Recherche Livre Num Liste chainees
	rechNumLC(bLC, num);
	temps_final = clock();

	temps_cpu_numLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	temps_initial = clock(); //Recherche Livre Titre Liste chainees
	rechTitreLC(bLC, titre);
	temps_final = clock();

	temps_cpu_titreLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	temps_initial = clock(); //Recherche Livre Auteur Liste chainees
	rechAuteurLC(bLC, auteur);
	temps_final = clock();

	temps_cpu_auteurLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;

	FILE *fic = fopen("test.txt", "w");

	fprintf(fic, "Hachage: ");
	fputc('\n', fic);

	fprintf(fic, "Temps recherche num H: %.2f \n", temps_cpu_numH);
	fputc('\n', fic);

	fprintf(fic, "Temps recherche titre H: %.2f \n", temps_cpu_titreH);
	fputc('\n', fic);

	fprintf(fic, "Temps recherche auteur H: %.2f \n", temps_cpu_auteurH);
	fputc('\n', fic);

	fprintf(fic, "Liste chainées: ");
	fputc('\n', fic);

	fprintf(fic, "Temps recherche num LC: %.2f \n", temps_cpu_numLC);
	fputc('\n', fic);

	fprintf(fic, "Temps recherche titre LC: %.2f \n", temps_cpu_titreLC);
	fputc('\n', fic);

	fprintf(fic, "Temps recherche auteur LC: %.2f \n", temps_cpu_auteurLC);
	fputc('\n', fic);

	fclose(fic);
}

void numH(BiblioH *bH, LivreH* (*rechNumH)(BiblioH*, int)){
	/*fonction qui teste tous les numéros des livres de bH
	une table de hachage
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_numH;
	FILE *fic = fopen("./donnees/numH.txt", "w");

	LivreH *l;

	for(int i=0; i<bH->m; i++){
		l = bH->T[i];
		while(l){
			temps_initial = clock(); //Recherche Livre Num table de hachage
			rechNumH(bH, l->num);
			temps_final = clock();
			temps_cpu_numH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
			fprintf(fic, "%.5f", temps_cpu_numH);
			fputc('\n', fic);
			l = l->suivant;
		}
	}
	temps_initial = clock(); //Recherche Livre absent
	rechNumH(bH, 100000);
	temps_final = clock();
	temps_cpu_numH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_numH);
	fputc('\n', fic);

	fclose(fic);
}

void titreH(BiblioH *bH, LivreH* (*rechTitreH)(BiblioH*, char*)){
	/*fonction qui teste tous les titres des livres de bH
	une table de hachage
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_titreH;
	FILE *fic = fopen("./donnees/titreH.txt", "w");

	LivreH *l;

	for(int i=0; i<bH->m; i++){
		l = bH->T[i];
		while(l){
			temps_initial = clock(); //Recherche Livre Titre table de hachage
			rechTitreH(bH, l->titre);
			temps_final = clock();
			temps_cpu_titreH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
			fprintf(fic, "%.5f", temps_cpu_titreH);
			fputc('\n', fic);
			l = l->suivant;
		}
	}
	temps_initial = clock(); //Recherche Livre absent
	rechTitreH(bH, "aaaaaa");
	temps_final = clock();
	temps_cpu_titreH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_titreH);
	fputc('\n', fic);

	fclose(fic);
}

void auteurH(BiblioH *bH, BiblioH* (*rechAuteurH)(BiblioH*, char*)){
	/*fonction qui teste tous les auteurs des livres de bH
	une table de hachage
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_auteurH;
	FILE *fic = fopen("./donnees/auteurH.txt", "w");

	LivreH *l;

	for(int i=0; i<bH->m; i++){
		l = bH->T[i];
		while(l){
			temps_initial = clock(); //Recherche Livre auteur table de hachage
			rechAuteurH(bH, l->auteur);
			temps_final = clock();
			temps_cpu_auteurH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
			fprintf(fic, "%.5f", temps_cpu_auteurH);
			fputc('\n', fic);
			l = l->suivant;
		}
	}
	temps_initial = clock(); //Recherche Livre absent
	rechAuteurH(bH, "aaaaaa");
	temps_final = clock();
	temps_cpu_auteurH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_auteurH);
	fputc('\n', fic);

	fclose(fic);
}

void numLC(Biblio *bLC, Livre* (*rechNumLC)(Biblio*, int)){
	/*fonction qui teste tous les numéros des livres de bLC
	une liste chaînée
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_numLC;
	FILE *fic = fopen("./donnees/numLC.txt", "w");

	Livre *l = bLC->L;

	while(l){
		temps_initial = clock(); //Recherche Livre Num liste chaînée
		rechNumLC(bLC, l->num);
		temps_final = clock();
		temps_cpu_numLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%.5f", temps_cpu_numLC);
		fputc('\n', fic);
		l = l->suivant;
	}
	temps_initial = clock(); //Recherche Livre absent
	rechNumLC(bLC, 100000);
	temps_final = clock();
	temps_cpu_numLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_numLC);
	fputc('\n', fic);

	fclose(fic);
}

void titreLC(Biblio *bLC, Livre* (*rechTitreLC)(Biblio*, char*)){
	/*fonction qui teste tous les titres des livres de bLC
	une liste chaînée
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_titreLC;
	FILE *fic = fopen("./donnees/titreLC.txt", "w");

	Livre *l = bLC->L;

	while(l){
		temps_initial = clock(); //Recherche Livre Titre liste chaînée
		rechTitreLC(bLC, l->titre);
		temps_final = clock();
		temps_cpu_titreLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%.5f", temps_cpu_titreLC);
		fputc('\n', fic);
		l = l->suivant;
	}
	temps_initial = clock(); //Recherche Livre absent
	rechTitreLC(bLC, "aaaaaa");
	temps_final = clock();
	temps_cpu_titreLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_titreLC);
	fputc('\n', fic);

	fclose(fic);
}

void auteurLC(Biblio *bLC, Biblio* (*rechAuteurLC)(Biblio*, char*)){
	/*fonction qui teste tous les auteurs des livres de bLC
	une liste chaînée
	et écrit le temps d'executions en seconde dans un fichier
	*/
	clock_t temps_initial;
	clock_t temps_final;

	double temps_cpu_auteurLC;
	FILE *fic = fopen("./donnees/auteurLC.txt", "w");

	Livre *l = bLC->L;

	while(l){
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		rechAuteurLC(bLC, l->auteur);
		temps_final = clock();
		temps_cpu_auteurLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%.5f", temps_cpu_auteurLC);
		fputc('\n', fic);
		l = l->suivant;
	}
	temps_initial = clock(); //Recherche Livre absent
	rechAuteurLC(bLC, "aaaaaa");
	temps_final = clock();
	temps_cpu_auteurLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
	fprintf(fic, "Livre absent %.5f", temps_cpu_auteurLC);
	fputc('\n', fic);

	fclose(fic);
}

void moyenne(char *nomFic){
	/*Fonction qui calcule et affiche la moyenne
	des données d'un fichier données en paramètre
	*/
	float s = 0;
	char buffer[256];
	FILE *fic = fopen(nomFic, "r");
	if(!fic){
		printf("erreur de lecture\n");
	}
	int i = 0;
	float t = 0;
	float absent = 0;
	while(fgets(buffer, 256, fic) != NULL){
		if(sscanf(buffer, "%f\n", &t) == 1){
			s += t;
			i++;
		}
		else if(sscanf(buffer, "Livre absent %f\n", &t) == 1){
			absent = t;
		}
		else{
			printf("Erreur de lecture\n");
		}
	}
	printf("Moyenne de %s: %.8f \n", nomFic, s/i);
	printf("Temps livre absent : %.8f \n",absent);
	fclose(fic);
}

BiblioH *creation_doublonH(BiblioH *b, int m){
	/*Fonction créeant des doublons d'une bibliothèque
	(table de hachage) de taille m en incrémentant de
	1000 le numéro de chaque livre
	*/
    BiblioH *doublon = creer_biblioH(m);
    for(int i=0; i<b->m; i++){
        LivreH *tmp = b->T[i];
        while(tmp){
            inserer_en_teteH(doublon, tmp->num+10000, tmp->titre, tmp->auteur);
            tmp = tmp->suivant;
        }
    }
    return doublon;
}

Biblio *creation_doublonLC(Biblio *b){
	/*Fonction créeant des doublons d'une bibliothèque
	(liste chaînée) en incrémentant de 1000 le numéro 
	de chaque livre
	*/
    Biblio *doublon = creer_biblio();
    Livre *tmp = b->L;
    while(tmp){
        inserer_en_tete(doublon, tmp->num+10000, tmp->titre, tmp->auteur);
        tmp = tmp->suivant;
    }
    return doublon;
}

void rechercheDoublon(int m){
	/*Fonction qui calcule le temps d'execution de doublonLC et doublonH
	en incrémentant la taille de la liste et de la table de hachage de 500
	car sinon le temps de calcul serait trop conséquant
	*/
	Biblio *b1LC = NULL;
	Biblio *b2LC = NULL;
	Biblio *b3LC = NULL;
	Biblio *b4LC = NULL;
	Biblio *b5LC = NULL;
	BiblioH *b1H = NULL;
	BiblioH *b2H = NULL;
	BiblioH *b3H = NULL;
	BiblioH *b4H = NULL;
	BiblioH *b5H = NULL;
	clock_t temps_initial;
	clock_t temps_final;

	FILE *fic = fopen("./donnees/doublon.txt", "w");
	if(fic == NULL){
		printf("Erreur d'ouverture");
		return;
	}

	double temps_cpu_rechercheLC;
	double temps_cpu_rechercheH;

	for(int i=1000; i<20000; i+=500){
		//Tableau et listes de taille allant de 1000 à 10000
		printf("%d\n", i);
		b1LC = charge_n_entrees("GdeBiblio.txt", (i/2)+1);
		b2LC = creation_doublonLC(b1LC);
		fusion_biblio(b1LC, b2LC);
		b1H = charge_n_entreesH("GdeBiblio.txt", (i/2)+1, m);
		b2H = creation_doublonH(b1H, m);
		fusion_biblioH(b1H, b2H);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublon(b1LC);
		temps_final = clock();
		temps_cpu_rechercheLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublonH(b1H, m);
		temps_final = clock();
		temps_cpu_rechercheH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%d %.5f %.5f", i, temps_cpu_rechercheLC, temps_cpu_rechercheH);
		fputc('\n', fic);
	}
	printf("oui\n");
	for(int i=20000; i<30000; i +=500){
		//Tableau et listes de taille allant de 20000 à 30000
		printf("%d\n", i);
		b1LC = charge_n_entrees("GdeBiblio.txt", (i/2)+1);
		b2LC = creation_doublonLC(b1LC);
		b3LC = creation_doublonLC(b2LC);
		fusion_biblio(b1LC, b2LC);
		fusion_biblio(b1LC, b3LC);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublon(b1LC);
		temps_final = clock();
		temps_cpu_rechercheLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		b1H = charge_n_entreesH("GdeBiblio.txt", (i/2)+1, m);
		b2H = creation_doublonH(b1H, m);
		b3H = creation_doublonH(b2H, m);
		fusion_biblioH(b1H, b2H);
		fusion_biblioH(b1H, b3H);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublonH(b1H, m);
		temps_final = clock();
		temps_cpu_rechercheH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%d %.5f %.5f", i, temps_cpu_rechercheLC, temps_cpu_rechercheH);
		fputc('\n', fic);
	}
	printf("oui\n");
	for(int i=30000; i<40000; i +=500){
		//Tableau et listes de taille allant de 30000 à 40000
		printf("%d\n", i);
		b1LC = charge_n_entrees("GdeBiblio.txt", (i/2)+1);
		b2LC = creation_doublonLC(b1LC);
		b3LC = creation_doublonLC(b2LC);
		b4LC = creation_doublonLC(b3LC);
		fusion_biblio(b1LC, b2LC);
		fusion_biblio(b1LC, b3LC);
		fusion_biblio(b1LC, b4LC);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublon(b1LC);
		temps_final = clock();
		temps_cpu_rechercheLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		b1H = charge_n_entreesH("GdeBiblio.txt", (i/2)+1, m);
		b2H = creation_doublonH(b1H, m);
		b3H = creation_doublonH(b2H, m);
		b4H = creation_doublonH(b3H, m);
		fusion_biblioH(b1H, b2H);
		fusion_biblioH(b1H, b3H);
		fusion_biblioH(b1H, b4H);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublonH(b1H, m);
		temps_final = clock();
		fprintf(fic, "%d %.5f %.5f", i, temps_cpu_rechercheLC, temps_cpu_rechercheH);
		fputc('\n', fic);
	}
	printf("oui\n");
	for(int i=40000; i<50000; i +=500){
		//Tableau et listes de taille allant de 40000 à 50000
		printf("%d\n", i);
		b1LC = charge_n_entrees("GdeBiblio.txt", (i/2)+1);
		b2LC = creation_doublonLC(b1LC);
		b3LC = creation_doublonLC(b2LC);
		b4LC = creation_doublonLC(b3LC);
		b5LC = creation_doublonLC(b4LC);
		fusion_biblio(b1LC, b2LC);
		fusion_biblio(b1LC, b3LC);
		fusion_biblio(b1LC, b4LC);
		fusion_biblio(b1LC, b5LC);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublon(b1LC);
		temps_final = clock();
		temps_cpu_rechercheLC = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		b1H = charge_n_entreesH("GdeBiblio.txt", (i/2)+1, m);
		b2H = creation_doublonH(b1H, m);
		b3H = creation_doublonH(b2H, m);
		b4H = creation_doublonH(b3H, m);
		b5H = creation_doublonH(b4H, m);
		fusion_biblioH(b1H, b2H);
		fusion_biblioH(b1H, b3H);
		fusion_biblioH(b1H, b4H);
		fusion_biblioH(b1H, b5H);
		temps_initial = clock(); //Recherche Livre auteur liste chaînée
		recherche_doublonH(b1H, m);
		temps_final = clock();
		temps_cpu_rechercheH = ((double)(temps_final-temps_initial))/CLOCKS_PER_SEC;
		fprintf(fic, "%d %.5f %.5f", i, temps_cpu_rechercheLC, temps_cpu_rechercheH);
		fputc('\n', fic);
	}
	printf("oui\n");
	printf("fait\n");
	liberer_biblio(b1LC);
	liberer_biblioH(b1H);
	fclose(fic);
}
