// #include "Biblio_lin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Avl{
    char id[20];
    int v;
    int eq;
    struct Avl* fg;
    struct Avl* fd;
}Avl;
typedef Avl* A;

int researche(A a, char* id){
    A p=a;
    if(a==NULL){
        return -1;
    }
    if(strcmp(a->id,id)==0){
        return 1;
    }
    if(strcmp(a->id,id)>0){
        return researche(a->fd, id);
    }
    else{
        return researche(a->fg, id);
    }
}

int min(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}

int max(int a, int b){
    if(a>b){
        return a;
    }
    return b;
}

A crea_noeud(int val, char* id){
    A new=malloc(sizeof(Avl));
    new->v=val;
    new->eq=0;
    for(int i=0;i<20;i++){
        new->id[i]=id[i];
    }
    new->fg=NULL;
    new->fd=NULL;
    return new;
}

int existefg(A a){
    if(a!=NULL && a->fg!=NULL){
        return 1;
    }
    return -1;
}

int existefd(A a){
    if(a!=NULL && a->fd!=NULL){
        return 1;
    }
    return -1;
}

A rotationDroite(A a) {
    A pivot;
    int eq_a, eq_p;
    pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));    
    return pivot;
}

A rotationGauche(A a) {
    A pivot;
    int eq_a, eq_p;
    pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    eq_a = a->eq;
    eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));    
    return pivot;
}

A doubleRotationGauche(A a) {
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

A doubleRotationDroite(A a) {
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

A equilibrerAVL(A a) {
    if (a->eq >= 2) {
        if (a->fd->eq >= 0)
            return rotationGauche(a);
        else
            return doubleRotationGauche(a);
    } 
    else if (a->eq <= -2) {
        if (a->fg->eq <= 0)
            return rotationDroite(a);
        else
            return doubleRotationDroite(a);
    }
    return a;
}

A insertionAVL(A a, char* id, int val, int *h) {
    if(a == NULL){
        *h = 1;
        return crea_noeud(val, id);
    }

    int cmp = strcmp(id, a->id);
    if(cmp < 0){
        a->fg = insertionAVL(a->fg, id, val, h);
        *h = -*h;
    } 
    else if (cmp > 0){
        a->fd = insertionAVL(a->fd, id, val, h);
    } 
    else{
        a->v += val; 
        *h = 0;
        return a;
    }
    if(*h != 0){
        a->eq = a->eq + *h;
        if (a->eq == 0) *h = 0;
        else *h = 1;

        if (a->eq < -1 || a->eq > 1) {
            a = equilibrerAVL(a);
            if (a->eq == 0) *h = 0;
            else *h = 1; 
        }
    }
    return a;
}

void afficherInfixe(A a) {
    if (a != NULL) {
        afficherInfixe(a->fg);
        printf("ID: %s (Val: %d, Eq: %d)\n", a->id, a->v, a->eq);
        afficherInfixe(a->fd);
    }
}

void write_Infixe(A a, FILE* name, FILE* val){
    if (a != NULL) {
        write_Infixe(a->fg, name, val);
        fprintf(name, "%s\n", a->id); 
        fprintf(val, "%d\n", a->v);
        write_Infixe(a->fd, name, val);
    }
}

int main(){
    FILE *nom = fopen("tests/histo_nom.txt", "r");
    FILE *val = fopen("tests/histo_quantite.txt", "r");
    FILE *result_nom = fopen("tests/histo_nom_result.txt", "w");
    FILE *result_val = fopen("tests/histo_quantite_result.txt", "w");

    if(nom == NULL || val == NULL){
        printf("Erreur d'ouverture des fichiers.\n");
        return 1;
    }

    int nb;
    char id[20];
    A avl = NULL;
    int h = 0;

    while (fscanf(val, "%d", &nb) == 1) {
        fgets(id, 19, nom);
        id[strcspn(id, "\n")] = 0; 
        id[strcspn(id, "\r")] = 0; 
        avl = insertionAVL(avl, id, nb, &h);
    }   

    // afficherInfixe(avl);
    write_Infixe(avl, result_nom, result_val);
    fclose(nom);
    fclose(val);
    return 0;
}
