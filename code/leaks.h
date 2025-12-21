#ifndef LEAKS_H
#define LEAKS_H

#include "avl.h"

Ab ab_crea_noeud(float val, char* id, float fuite, int cas);
A avl_crea_noeud(float val, char* id, Ab parent, float fuite, int cas);
A insertionAvl(A a, char* id, float val, float fuite, int *h, Ab parent, int cas, A* retour);
void supp_fixe(A a);
void write_leak_res(char* id, float somme_leaks, FILE* nom, FILE* value);
void somme_leak(Ab ab, float volume, float* total);
void traiter_leaks(char *idUsine);

#endif