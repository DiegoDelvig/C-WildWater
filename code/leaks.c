typedef struct Troncon {
	double pourcentage;  
	struct Noeud_Acteur *acteur_aval;
	struct Troncon_Enfant *suivant;
} Troncon;

typedef struct arbre {
	char *id_arbre;            
	char *id_parent;     
  
	double volume;      
	double volume_perdu;
    
	struct arbre *parent;
	Troncon *troncons_aval;
	int enfants;              
} Arbre;

typedef struct avl {
	char *id_arbre;          
	Arbre *adresse_a;  
    
	int eq;                  
	struct avl *fg;
	struct avl *fd;
} Avl;

Avl* creer_avl(const char *id_arbre, Arbre *adresse_a) {
	Avl *nv_noeud =malloc(sizeof(Avl));
	if (nv_noeud == NULL) {
        	return NULL;
    	}
    
    	nv_noeud->id_arbre = strdup(id_arbre);
    	if (nv_noeud->id_arbre == NULL) {
        	free(nv_noeud);
        	return NULL;
    	}

    	nv_noeud->adresse_a = adresse_a;
    	nv_noeud->fg = NULL;
    	nv_noeud->fd = NULL;
    	nv_noeud->eq = 0;
    
    	return nv_noeud;
}

Arbre* rechercher_avl(Avl *racine, const char *id_arbre) {
    if (racine == NULL) {
        return NULL;
    }

    int cmp = strcmp(id_arbre, racine->id_arbre);

    if (cmp < 0) {
        return rechercher_avl(racine->fg, id_arbre);
    } else if (cmp > 0) {
        return rechercher_avl(racine->fd, id_arbre);
    } else {
        return racine->adresse_a;
    }
}

int max(int a, int b){
	if(a>b){
		return a;
	}
	return b;
}
int min(int a, int b){
	if(a<b){
		return a;
	}
	return b;
}

Avl* rotationDroite(Avl* a) {
    Avl* pivot;
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

Avl* rotationGauche(Avl* a) {
    Avl* pivot;
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
Avl* doubleRotationGauche(Avl* a) {
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

Avl* doubleRotationDroite(Avl* a) {
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

Avl* equilibrerAVL(Avl* a) {
    if (a->eq >= 2) {
    	if(a->fd!=NULL){
        	if (a->fd->eq >= 0){
            		return rotationGauche(a);
            	}
        	else{
            		return doubleRotationGauche(a);
            	}
    	} 
    }
    else if (a->eq <= -2) {
    	if(a->fg!=NULL){
        	if (a->fg->eq <= 0){
            		return rotationDroite(a);
            	}
        	else{
            		return doubleRotationDroite(a);
            	}
    	}
    }
    return a;
}


Arbre* creer_arbre(const char *id, const char *id_usine) {
    	if (id == NULL || id_usine == NULL) {
    		return NULL;
    	}
    	Arbre *nv =malloc(sizeof(Arbre));
    	if (nv == NULL) {
        	return NULL;
    	}
    	nv->id_arbre = strdup(id);
    	nv->id_parent = strdup(id_usine);
    
    	if (nv->id_arbre == NULL || nv->id_parent == NULL) {
       		free(nv->id_arbre);
        	free(nv->id_parent);
        	free(nv);
        	return NULL;
    	}

    	nv->volume= 0.0;
    	nv->volume_perdu = 0.0;
  
    	nv->parent = NULL;
    	nv->troncons_aval = NULL;
    	nv->enfants = 0;
    
    	return nv;
}
