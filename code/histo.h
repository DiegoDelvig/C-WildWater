#ifndef HISTO_H
#define HISTO_H

#include "avl.h"

typedef struct {
    char id[256];
    double val;
} Tab;


A insererSomme(A avl, char *id, double val);
void traiter_histo(char *mode);

#endif
