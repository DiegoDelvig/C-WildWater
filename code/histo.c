#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "histo.h"

#define FILE_IN_NAME "tests/histo_nom.txt"
#define FILE_IN_QTY "tests/histo_quantite.txt"
#define FILE_IN_LEAK "tests/histo_fuite.txt"
#define FILE_OUT_NOM "tests/histo_nom_result.txt"
#define FILE_OUT_QTY "tests/histo_quantite_result.txt"

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
} Arr;

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

A insert(A avl, char *id, double val) {
    if (avl == NULL) {
        return crea_noeud(val, id);
    }

    int cmp = strcmp(id, avl->id);

    if (cmp < 0) {
        avl->fg = insert(avl->fg, id, val);
    } else if (cmp > 0) {
        avl->fd = insert(avl->fd, id, val);
    } else {
        avl->v += val;
        return avl;
    }

    return equilibrerAVL(avl);
}

void countAVL(A avl, int *count) {
    if (avl != NULL) {
        countAVL(avl->fg, count);
        (*count)++;
        countAVL(avl->fd, count);
    }
}

void fillArray(A avl, Arr *arr, int *index) {
    if (avl != NULL) {
        fillArray(avl->fg, arr, index);
        strcpy(arr[*index].id, avl->id);
        arr[*index].val = avl->v;
        (*index)++;
        fillArray(avl->fd, arr, index);
    }
}

void freeAvl(A avl) {
    if (avl != NULL) {
        freeAvl(avl->fg);
        freeAvl(avl->fd);
        free(avl);
    }
}

void traiter_histo(char *mode) {
    printf("Traitement hist %s\n", mode);

    FILE *f_name = fopen(FILE_IN_NAME, "r");
    FILE *f_qty = fopen(FILE_IN_QTY, "r");
    FILE *f_leak = fopen(FILE_IN_LEAK, "r");

    if (!f_name || !f_qty) {
        printf("Impossible d'ouvrir les fichiers\n");
        exit(1);
    }

    A avl = NULL;
    char bufferName[256];
    char bufferQty[256];
    char bufferLeak[256];
    double qte, leak, res;

    while (fgets(bufferName, 256, f_name) && fgets(bufferQty, 256, f_qty)) {
        bufferName[strcspn(bufferName, "\n")] = 0;
        qte = atof(bufferQty);
        leak = atof(bufferLeak);

        if (strcmp(mode, "real") == 0) {
            res = qte * (1.0 - (leak / 100.0));

            if (res < 0) {
                res = 0;
            }
        } else {
            res = qte;
        }
        avl = insert(avl, bufferName, res);
    }
    fclose(f_name);
    fclose(f_qty);
    fclose(f_leak);

    int count = 0;
    countAVL(avl, &count);

    if (count == 0) {
        printf("Erreur\n");
        return;
    }

    Arr *arr = malloc(count * sizeof(Arr));
    int index = 0;
    fillArray(avl, arr, &index);

    FILE *f_res_name = fopen(FILE_OUT_NOM, "w");
    FILE *f_res_qty = fopen(FILE_OUT_QTY, "w");

    if (!f_res_name || !f_res_qty) {
        printf("Erreur dans la creation des fichiers\n");
        free(arr);
        freeAvl(avl);
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(f_res_name, "%s\n", arr[i].id);
        fprintf(f_res_qty, "%f\n", arr[i].val);
    }

    fclose(f_res_name);
    fclose(f_res_qty);
    free(arr);
    freeAvl(avl);
}
