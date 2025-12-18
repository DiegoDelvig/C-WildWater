#include "Biblio_lin.h"
typedef struct Arbre{
    char id[20];
    float v;
    float fuite;
    struct Arbre* son;
    struct Arbre* bro;
}Arbre;  
typedef Arbre* Ab;
typedef struct Avl{
    char id[20];
    int eq;
    struct Avl* fg;
    struct Avl* fd;
    Ab ab;
}Avl;  
typedef Avl* A;
float min(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}
float max(int a, int b){
    if(a>b){
        return a;
    }
    return b;
}
Ab ab_crea_noeud(float val, char* id, float fuite, int cas){
    Ab new_ab=malloc(sizeof(Arbre));
    strcpy(new_ab->id, id);
    if(cas==1){
        new_ab->v=val;
        new_ab->bro=NULL;
        new_ab->son=NULL;
        return new_ab;
    }
    new_ab->v=val;
    new_ab->fuite=fuite;
    new_ab->bro=NULL;
    new_ab->son=NULL;
    return new_ab;
}
A avl_crea_noeud(float val, char* id, Ab parent, float fuite, int cas){
    A new_avl=malloc(sizeof(Avl));
    new_avl->eq=0;
    strcpy(new_avl->id, id);
    new_avl->ab=ab_crea_noeud(val, id, fuite, cas);
    new_avl->fg=NULL;
    new_avl->fd=NULL;
    if(cas==2){
        new_avl->ab->bro=parent->son;
        parent->son=new_avl->ab;
    }
    return new_avl;
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
A insertionAVL(A a, char* id, float val, float fuite, int *h, Ab parent, int cas, A* retour){
    if(a == NULL){
        *h = 1;
        A new=avl_crea_noeud(val, id, parent, fuite, cas);
        if(retour!=NULL){
            *retour=new;
        }       
        return new;
    }
    int cmp = strcmp(id, a->id);
    if(cmp == 0) { 
        *h = 0;
        if(retour != NULL){
            *retour = a; 
        }
        return a;
    }
    else if(cmp < 0){
        a->fg = insertionAVL(a->fg, id, val, fuite, h, parent, cas, retour);
        *h = -*h;
    } 
    else{
        a->fd = insertionAVL(a->fd, id, val, fuite, h, parent, cas, retour);
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
void supp_jspfixe(A a){
    if(a!=NULL){
        supp_jspfixe(a->fg);
        supp_jspfixe(a->fd);
        free(a->ab);
        free(a);
    }
}
A researche(A a, char* id){
    if(a==NULL){
        return NULL;
    }
    if(strcmp(a->id,id)==0){
        return a;
    }
    if(strcmp(a->id,id)>0){
        return researche(a->fg, id);
    }
    else{
        return researche(a->fd, id);
    }
}
void write(char* id, float somme_leaks, FILE* nom, FILE* value){
    if(id!=NULL){
        fprintf(nom, "%s\n", id); 
        fprintf(value, "%f\n", somme_leaks);
    }
    
}
void somme_leak(Ab ab, float volume, float* total){
    if(ab==NULL){
        return;
    }
    int i=0;
    float perte=(volume*(ab->fuite))/100;
    *total+=perte;
    float reste=volume-perte;
    Ab p=ab->son;
    while(p!=NULL){
        i++;
        p=p->bro;
    }
    p=ab->son;
    if(i==0){
        return;
    }
    while(p!=NULL){
        somme_leak(p, reste/i, total);
        p=p->bro;
    }
    return;
}
int main(int argc, char* argv[]){
    FILE *nom_parent = fopen("tests/leak_nom_parent.txt", "r");
    FILE *nom_enfant = fopen("tests/leak_nom_enfant.txt", "r");
    FILE *val = fopen("tests/leak_quantite.txt", "r");
    FILE *fuite = fopen("tests/leak_fuite.txt", "r");
    FILE *result_nom = fopen("tests/leak_nom_result_fuite.txt", "w");
    FILE *result_val = fopen("tests/leak_quantite_result_fuite.txt", "w");
    if(nom_parent == NULL || val == NULL || nom_enfant == NULL || fuite == NULL){
        printf("Erreur d'ouverture des fichiers.\n");
        return 1;
    }
    if(argc<2){
        printf("\nerreur d'argument");
        fclose(nom_parent);
        fclose(nom_enfant);
        fclose(val);
        fclose(fuite);
        fclose(result_nom);
        fclose(result_val);
        return 1;
    }
    float nb;
    char id_parent[20];
    char id_enfant[20];
    A avl = NULL;
    A pparent=NULL;
    int h = 0;
    float fuites;
    int i=0;
    while(fscanf(val, "%f", &nb) == 1 && fscanf(fuite, "%f", &fuites) == 1){
        fgets(id_parent, 19, nom_parent);
        id_parent[strcspn(id_parent, "\n")] = 0; 
        id_parent[strcspn(id_parent, "\r")] = 0; 
        fgets(id_enfant, 19, nom_enfant);
        id_enfant[strcspn(id_enfant, "\n")] = 0; 
        id_enfant[strcspn(id_enfant, "\r")] = 0; 
        avl=insertionAVL(avl, id_parent, nb, fuites, &h, NULL, 1, &pparent);
        if(pparent != NULL){
            avl = insertionAVL(avl, id_enfant, nb, fuites, &h, pparent->ab, 2, NULL);
        }
    }   
    char* id=argv[1];
    A usine=researche(avl, id);
    if(usine!=NULL){
        float somme=0;
        somme_leak(usine->ab, usine->ab->v, &somme);
        write(id, somme, result_nom, result_val);
    }
    else{
        supp_jspfixe(avl);
        fclose(nom_parent);
        fclose(nom_enfant);
        fclose(val);
        fclose(fuite);
        fclose(result_nom);
        fclose(result_val);
        return 1;
    }
    supp_jspfixe(avl);
    fclose(nom_parent);
    fclose(nom_enfant);
    fclose(val);
    fclose(fuite);
    fclose(result_nom);
    fclose(result_val);
    return 0;
}
