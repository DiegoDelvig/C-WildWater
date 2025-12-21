#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Arbre {
  char id[256];
  float v;
  float fuite;
  struct Arbre *son;
  struct Arbre *bro;
} Arbre;
typedef Arbre *Ab;

typedef struct Avl {
  char id[256];
  int eq;
  struct Avl *fg;
  struct Avl *fd;
  float v; // leaks.c ne l'utilise pas
  Ab ab; // histo.c ne l'utilise pas
} Avl;
typedef Avl *A;

// Prototypes
float min(float a, float b);
float max(float a, float b);
int getHauteur(A a); 
int calculEq(A a);
int existeFg(A a);
int existeFd(A a);
A crea_noeud(float val, char *id);
A rotationDroite(A a);
A rotationGauche(A a);
A doubleRotationDroite(A a);
A doubleRotationGauche(A a);
A equilibrerAvl(A a);
void compterAvl(A avl, int *count);
void suprAvl(A avl);
A researche(A a, char* id);

#endif
