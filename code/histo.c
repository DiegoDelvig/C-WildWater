#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "histo.h"
#include "avl.h"

#define FILE_IN_NOM "tests/histo_nom.txt"
#define FILE_IN_QTE "tests/histo_quantite.txt"
#define FILE_IN_FUITE "tests/histo_fuite.txt"
#define FILE_OUT_NOM "tests/histo_nom_result.txt"
#define FILE_OUT_QTE "tests/histo_quantite_result.txt"

A insererSomme(A avl, char *id, double val) {
    if (avl == NULL) {
        return crea_noeud(val, id);
    }

    int cmp = strcmp(id, avl->id);

    if (cmp < 0) {
        avl->fg = insererSomme(avl->fg, id, val);
    } else if (cmp > 0) {
        avl->fd = insererSomme(avl->fd, id, val);
    } else {
        avl->v += val;
        return avl;
    }

    return equilibrerAvl(avl);
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
        avl = insererSomme(avl, bufferNom, res);
    }
    fclose(f_nom);
    fclose(f_qte);
    fclose(f_fuite);

    int count = 0;
    compterAvl(avl, &count);

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
        suprAvl(avl);
        exit(1);
    }

    for (int i = 0; i < count; i++) {
        fprintf(f_res_nom, "%s\n", tab[i].id);
        fprintf(f_res_qte, "%f\n", tab[i].val);
    }

    fclose(f_res_nom);
    fclose(f_res_qte);
    free(tab);
    suprAvl(avl);
}
