#include "headers/rsa.h"

long modpow_naive(long a, long m, long n){
    //paramètres: une long a, un long m et un long n
    //Version naive de modpow et sans optimisation
    //valeur de retour: renvoie a puissance m modulo n
    long powA = a;
    for(int i=0; i<m; i++){
        powA *= a;
    }
    return powA%n;
}

long modpow(long a, long m, long n){
    //paramètres: une long a, un long m et un long n
    //version optimisé de modpow en utilisant un algorithme dichotomique
    //valeur de retour: renvoie a puissance m modulo n
    long b = 0;
    if(m == 0){
        return 1;
    }
    else if(m%2 == 0){
        b = modpow(a, m/2, n);
        return (b*b)%n;
    }
    else{
        b = modpow(a, (int)(m/2), n);
        return (a*b*b)%n;
    }
}

int witness(long a, long b, long d, long p){ //fonction fourni
    long x = modpow(a, d, p);
    if(x == 1){
        return 0;
    }
    for(long i = 0; i<b ; i++){
        if(x == p-1){
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}

long rand_long(long low, long up){ //fonction fourni
    return rand() % (up-low + 1)+low;
}

int is_prime_naive(long p){
    //paramètres: une long p
    //fonction naive qui retrouve si un entier est premier ou non
    //valeur de retour: 1 si l'entier est premier et 0 sinon
    for(int i=3; i<p; i++){
        if(p%i == 0){
            return 0;
        }
    }
    return 1;
}

int is_prime_miller(long p, int k){ //fonction fourni
    if (p == 2){
        return 1;
    }
    if (!(p & 1) || p<= 1){ //on verifie que p est impair et different de 1
        return 0;   
    }
	//on determine b et d
    long b=0;
    long d =p-1;
    while(!(d & 1)){ //tant que d n'est pas impair
        d=d/2;
        b=b+1;
    }
	//On genere k valeurs pour a, et on teste si c'est un temoin :
    long a;
    int i;
    for(i=0; i<k; i++){
        a= rand_long(2, p-1);
        if(witness(a,b,d,p)){
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k){
    //paramètres: 3 entiers
    //valeur de retour: renvoie un entier premeir au hasard
    if (low_size > up_size){
        printf("Probleme de borne : borne inferieure > borne superieure.\nInversion des bornes.\n");
        int temp = low_size;
        low_size = up_size;
        up_size = temp;
    }
    long s;
    do{
        s = rand_long(pow(2, low_size), pow(2, up_size+1)-1);
    }
    while(is_prime_miller(s, k) != 1);
	return s;
}

long extended_gcd(long s, long t, long *u, long *v){ //fonction fourni
    if (s == 0){
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim, vPrim;
    long gcd = extended_gcd(t%s, s , &uPrim, &vPrim);
    *u = vPrim-(t/s)*uPrim;
    *v = uPrim;
    return gcd;
}

void generate_key_values(long p, long q, long *n, long *s,long *u){
    //paramètres: 2 long et 3 pointeurs vers des long
    //initialise une paire de clé (s, n) et (u, n) avec p et q des nombres premiers
    //valeur de retour: aucune, intilalise les valeurs des pointeurs
    *n = p*q;
    long t = (p-1)*(q-1);
    long v = 0;
    long gcd;
    do{
        *s = rand_long(0, t);
        gcd = extended_gcd(*s, t, u, &v);
    }
    while((gcd != 1));
}
