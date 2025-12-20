#include "avl.h"

float min(float a, float b){
    if(a < b){
        return a;
    }
    return b;
}

float max(float a, float b){
    if(a > b){
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

int existeFg(A a){
    if(a != NULL && a->fg != NULL){
        return 1;
    }
    return -1;
}

int existeFd(A a){
    if(a != NULL && a->fd != NULL){
        return 1;
    }
    return -1;
}

A crea_noeud(float val, char* id){
    A new = malloc(sizeof(Avl));
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

A equilibrerAvl(A a) {
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


void compterAvl(A avl, int *count) {
    if (avl != NULL) {
        compterAvl(avl->fg, count);
        (*count)++;
        compterAvl(avl->fd, count);
    }
}

void suprAvl(A avl) {
    if (avl != NULL) {
        suprAvl(avl->fg);
        suprAvl(avl->fd);
        free(avl);
    }
}

