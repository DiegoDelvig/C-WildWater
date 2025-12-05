#include <stdio.h>
#include<stdlib.h>
#include<math.h>


typedef struct arbre{
	char id[20];
	int v;
	int eq;
	struct arbre* fd;
	struct arbre* fg;
}Avl;


Avl* creerarbre(char id){
	Avl* nouveau=malloc(sizeof(Avl));
	if(nouveau==NULL){
		exit(1);
	}
	nouveau->id=id;
	nouveau->eq=0;
	nouveau->v=;
	nouveau->fg=NULL;
	nouveau->fd=NULL;
	return nouveau;
}

int existefilsgauche(Avl* a){
	if(a->fg!=NULL){
		return 1;
	}
	return 0;
}


int existefilsdroit(Avl* a){
	if(a->fd!=NULL){
		return 1;
	}
	return 0;
}


void ajouterfilsgauche(Avl* a, int e){
	if(a!=NULL){
		if(existefilsgauche(a)==0){
			a->fg=creerarbre(e);
			}
	}
}

void ajouterfilsdroit(Avl* a, int e){
	if(a!=NULL){
		if(existefilsdroit(a)==0){
			a->fd=creerarbre(e);
			}
	}
}


int min2(int a, int b){
	if(a<b){
		return a;
	}
	return b;
	
}

int max2(int a, int b){
	if(a>b){
		return a;
	}
	return b;
}

Avl* insertAVL(Avl* a, int e, int* h){
	if(a==NULL){
		*h=1;
		return creerarbre(e);
	}
	else if(e<a->a){
		a->fg=insertAVL(a->fg,e,h);
		*h=-*h;
	}
	else if(e>a->a){
		a->fd=insertAVL(a->fd,e,h);
	}
	*h=0;
	if(*h!=0){
		a->eq=a->eq+*h;
		if(a->eq==0){
			*h=0;
		}
		else{
			*h=1;
		}
	}
	return a;
}

int recherche(Avl* a, char id){
	if(a==NULL){
		return 0;
	}
	else if(a->id==id){
		a->volume+=
	}
	else if(a->element>e){
		return recherche(a->fg,e);
	}
	else{
		return recherche(a->fd,e);
	}
}


Avl* suppMin(Avl* a, int* h, int* pe){
	if(a->fg==NULL){
		*pe=a->element;
		*h=-1;
		Avl* tmp=a;
		a=a->fd;
		free(tmp);
		return a;
	}
	else{
		a->fg=suppMin(a->fg,h,pe);
		*h=-*h;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+*h;
		if(a->equilibre==0){
			*h=-1;
		}
		else{
			*h=0;
		}
	}
	return a;
	
}

Avl* suppression(Avl* a, int e, int* h){

	if(a==NULL){
		*h=0;
		return a;
	}
	else if(e>a->element){
		a->fd=suppression(a->fd,e);
	}
	else if(e<a->element){
		a->fg=suppression(a->fg,e);
		*h=-*h;
	}
	else if(existefilsdroit(a)==0){
		a->fd=suppMin(a->fd,h,&a->element);
	
	}
	else{
		Avl* tmp=a;
		a=a->fg;
		free(tmp);
		*h=-1;
		return a;
	}
	if(*h!=0){
		a->equilibre=a->equilibre+*h;
		if(a->equilibre==0){
			*h=-1;
		}
		else{
			*h=0;
		}
	}
	return a;
}


Avl* rotation_gauche(Avl* a){
	Arbre* pivot=NULL;
	int eq_a=0;
	int eq_b=0;
	pivot=a->fd;
	a->fd=pivot->fg;
	pivot->fg=a;
	eq_a=a->eq;
	eq_b=pivot->eq;
	a->eq=eq_a-max2(eq_b,0)-1;
	pivot->eq=min2(min2(eq_a-2,eq_a+eq_b-2), eq_b-1 );//min(min(a,b),c)
	a=pivot;
	return a;
}

Avl* rotation_droite(Avl* a){
	Avl* pivot=NULL;
	int eq_a=0;
	int eq_b=0;
	pivot=a->fg;
	a->fg=pivot->fd;
	pivot->fd=a;
	eq_a=a->eq;
	eq_b=pivot->eq;
	a->eq=eq_a-min2(eq_b,0)+1;
	pivot->eq=min2(min2(eq_a+2,eq_a+eq_b+2, eq_b+1 ));
	a=pivot;
	return a;
}


int main(){
	Avl* a1=creerarbre(1);
	ajouterfilsdroit(a1,2);
	ajouterfilsdroit(a1->fd,3);
	a1->eq=2;
	a1->fd->eq=1;
	a1->fd->fd->eq=1;
	printf("arbre avant rotation gauche")
	return 0;
}
