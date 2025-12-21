#include "Biblio_lin.h"

#include "leaks.h"
#include "avl.h"


Ab ab_crea_noeud(float val, char* id, float fuite, int cas){
    Ab new_ab = malloc(sizeof(Arbre));
    strcpy(new_ab->id, id);

    if(cas == 1){
        new_ab->v = val;
        new_ab->fuite = 0.0;
        new_ab->bro = NULL;
        new_ab->son = NULL;
        return new_ab;
    }

    new_ab->v = val;
    new_ab->fuite = fuite;
    new_ab->bro = NULL;
    new_ab->son = NULL;
    return new_ab;
}

A avl_crea_noeud(float val, char* id, Ab parent, float fuite, int cas){
    A new_avl = malloc(sizeof(Avl));
    new_avl->eq = 0;
    strcpy(new_avl->id, id);
    new_avl->ab = ab_crea_noeud(val, id, fuite, cas);
    new_avl->fg = NULL;
    new_avl->fd = NULL;

    if(cas == 2){
        new_avl->ab->bro = parent->son;
        parent->son = new_avl->ab;
    }

    return new_avl;
}

A insertionAvl(A a, char* id, float val, float fuite, int *h, Ab parent, int cas, A* retour){
    if(a == NULL){
        *h = 1;
        A new = avl_crea_noeud(val, id, parent, fuite, cas);
        if(retour != NULL){
            *retour = new;
        }       
        return new;
    }

    int cmp = strcmp(id, a->id);

    if(cmp == 0) { 
        *h = 0;
        if(retour != NULL) {
            *retour = a; 
        }
        return a;
    }
    else if(cmp < 0){
        a->fg = insertionAvl(a->fg, id, val, fuite, h, parent, cas, retour);
        *h = -*h;
    } else {
        a->fd = insertionAvl(a->fd, id, val, fuite, h, parent, cas, retour);
    } 
    if(*h != 0){
        a->eq = a->eq + *h;
        if (a->eq == 0) *h = 0;
        else *h = 1;

        if (a->eq < -1 || a->eq > 1) {
            a = equilibrerAvl(a);
            if (a->eq == 0) *h = 0;
            else *h = 1; 
        }
    }
    return a;
}

void supp_fixe(A a){
    if(a != NULL){
        supp_fixe(a->fg);
        supp_fixe(a->fd);
        free(a->ab);
        free(a);
    }
}

void write_leak_res(char* id, float somme_leaks, FILE* nom, FILE* value){
    if(id != NULL){
        fprintf(nom, "%s\n", id); 
        fprintf(value, "%f\n", somme_leaks);
    }
}

void somme_leak(Ab ab, float volume, float* total){
    if(ab == NULL){
        return;
    }

    int i = 0;
    float perte = (volume * (ab->fuite)) / 100;
    *total += perte;
    float reste = volume - perte;
    Ab p = ab->son;
    while(p != NULL){
        i++;
        p = p->bro;
    }

    p = ab->son;
    if(i == 0){
        return;
    }

    while(p!=NULL){
        somme_leak(p, reste / i, total);
        p = p->bro;
    }

    return;
}

void traiter_leaks(char *idUsine) {
    // Fichier d'entrée
    FILE *nom_parent = fopen("tests/leak_nom_parent.txt", "r");
    FILE *nom_enfant = fopen("tests/leak_nom_enfant.txt", "r");
    FILE *val = fopen("tests/leak_quantite.txt", "r");
    FILE *fuite = fopen("tests/leak_fuite.txt", "r");

    // Fichier de sortie
    FILE *result_nom = fopen("tests/leak_nom_result_fuite.txt", "w");
    FILE *result_val = fopen("tests/leak_quantite_result_fuite.txt", "w");

    if(nom_parent == NULL || nom_enfant == NULL || val == NULL || fuite == NULL) {
        printf("Erreur d'ouverture des fichiers.\n");
        return;
    }

    float nb;
    char id_parent[256];
    char id_enfant[256];
    A avl = NULL;
    A pparent = NULL;
    int h = 0;
    float fuites;
    
    while(fscanf(val, "%f", &nb) == 1 && fscanf(fuite, "%f", &fuites) == 1){
        fgets(id_parent, 256, nom_parent);
        id_parent[strcspn(id_parent, "\n")] = 0; 
        id_parent[strcspn(id_parent, "\r")] = 0; 

        fgets(id_enfant, 256, nom_enfant);
        id_enfant[strcspn(id_enfant, "\n")] = 0; 
        id_enfant[strcspn(id_enfant, "\r")] = 0; 
        
        avl=insertionAvl(avl, id_parent, nb, fuites, &h, NULL, 1, &pparent);

        if(pparent != NULL){
            avl = insertionAvl(avl, id_enfant, nb, fuites, &h, pparent->ab, 2, NULL);
        }
    }

    A usine = researche(avl, idUsine);
    
    if(usine != NULL){
        float somme = 0;
        somme_leak(usine->ab, usine->ab->v, &somme);
        write_leak_res(idUsine, somme / 1000.0, result_nom, result_val);
    } else {
        printf("Usine %s introuvable dans les données filtrées\n", idUsine);
        fprintf(result_nom, "%s\n", idUsine);
        fprintf(result_val, "-1\n");
    }

    supp_fixe(avl);
    fclose(nom_parent); fclose(nom_enfant);
    fclose(val); fclose(fuite);
    fclose(result_nom); fclose(result_val);
}
