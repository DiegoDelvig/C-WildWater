#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "histo.h"

#define FILE_IN_NOM "tests/histo_nom.txt"
#define FILE_IN_QTE "tests/histo_quantite.txt"
#define FILE_IN_FUITE "tests/histo_fuite.txt"
#define FILE_OUT_NOM "tests/histo_nom_result.txt"
#define FILE_OUT_QTE "tests/histo_quantite_result.txt"

typedef struct Avl{
    char id[256];
    float v;
    int eq;
    struct Avl* fg;
    struct Avl* fd;
} Avl;
typedef Avl* A;

typedef struct {
    char id[256];
    double val;
} Tab;

float min(float a, float b){
    if(a<b){
        return a;
    }
    return b;
}

float max(float a, float b){
    if(a>b){
        return a;
    }
    return b;
}

int getHauteur(A a) {
    if (a == NULL) {
        return 0;
    }
    return a->eq;
}

int calculEq(A a) {
    if (a == NULL) {
        return 0;
    }
    return (getHauteur(a->fd) - getHauteur(a->fg));
}

int existefg(A a){
    if(a != NULL && a->fg != NULL){
        return 1;
    }
    return -1;
}

int existefd(A a){
    if(a != NULL && a->fd != NULL){
        return 1;
    }
    return -1;
}

A crea_noeud(float val, char* id){
    A new=malloc(sizeof(Avl));
    strcpy(new->id, id);
    new->v = val;
    new->eq = 0;
    new->fg = NULL;
    new->fd = NULL;
    return new;
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

A inserer(A avl, char *id, double val) {
    if (avl == NULL) {
        return crea_noeud(val, id);
    }

    int cmp = strcmp(id, avl->id);

    if (cmp < 0) {
        avl->fg = inserer(avl->fg, id, val);
    } else if (cmp > 0) {
        avl->fd = inserer(avl->fd, id, val);
    } else {
        avl->v += val;
        return avl;
    }

    return equilibrerAVL(avl);
}

void compterAVL(A avl, int *count) {
    if (avl != NULL) {
        compterAVL(avl->fg, count);
        (*count)++;
        compterAVL(avl->fd, count);
    }
}

void remplirTab(A avl, Tab *tab, int *index) {
    if (avl != NULL) {
        remplirTab(avl->fd, tab, index);
        strcpy(tab[*index].id, avl->id);
        // Conversion aux bonnes unités
        tab[*index].val = avl->v / 1000.0;
        (*index)++;
        remplirTab(avl->fg, tab, index);
    }
}

void suprAVL(A avl) {
    if (avl != NULL) {
        suprAVL(avl->fg);
        suprAVL(avl->fd);
        free(avl);
    }
}

void traiter_histo(char *mode) {
    printf("Traitement histo %s\n", mode);

    FILE *f_nom = fopen(FILE_IN_NOM, "r");
    FILE *f_qte = fopen(FILE_IN_QTE, "r");
    FILE *f_fuite = fopen(FILE_IN_FUITE, "r");

    if (!f_nom || !f_qte) {
        printf("Impossible d'ouvrir les fichiers\n");
        exit(1);
    }

    A avl = NULL;
    char bufferNom[256];
    char bufferQte[256];
    char bufferFuite[256];
    double qte, fuite, res;

    while (fgets(bufferNom, 256, f_nom) && fgets(bufferQte, 256, f_qte) && fgets(bufferFuite, 256, f_fuite)) {
        bufferNom[strcspn(bufferNom, "\n")] = 0;
        qte = atof(bufferQte);
        fuite = atof(bufferFuite);

        if (strcmp(mode, "real") == 0) {
            res = qte * (1.0 - (fuite / 100.0));

            if (res < 0) {
                res = 0;
            }
        } else {
            res = qte;
        }
        avl = inserer(avl, bufferNom, res);
    }
    fclose(f_nom);
    fclose(f_qte);
    fclose(f_fuite);

    int count = 0;
    compterAVL(avl, &count);

    if (count == 0) {
        printf("Erreur\n");
        return;
    }

    Tab *tab = malloc(count * sizeof(Tab));
    int index = 0;
    remplirTab(avl, tab, &index);

    FILE *f_res_nom = fopen(FILE_OUT_NOM, "w");
    FILE *f_res_qte = fopen(FILE_OUT_QTE, "w");

    if (!f_res_nom || !f_res_qte) {
        printf("Erreur dans la creation des fichiers\n");
        free(tab);
        suprAVL(avl);
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(f_res_nom, "%s\n", tab[i].id);
        fprintf(f_res_qte, "%f\n", tab[i].val);
    }

    fclose(f_res_nom);
    fclose(f_res_qte);
    free(tab);
    suprAVL(avl);
}
