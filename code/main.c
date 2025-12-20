#include <stdio.h>
#include <string.h>

#include "histo.h"
#include "leaks.h"

int main(int argc, char *argv[]) {
  /*
    argv[0]: ./c-wildwater
    argv[1]: 'histo' ou 'leaks' ('max' fait en shell)
    argv[2]: 'src', 'real' ou ID d'usine
  */

  if (argc < 2) {
    printf("Pas assez d'arguments");
    return 1;
  }

  if (strcmp(argv[1], "histo") == 0) {
    if (argc < 3) {
      printf("Mode histo manquant (src, real ou max)");
      return 1;
    }
    traiter_histo(argv[2]);
  }

  else if (strcmp(argv[1], "leaks") == 0) {
    if (argc < 3) {
      printf("ID d'unsine manquant pour la commande leaks\n");
      return 1;
    }
    traiter_leaks(argv[2]);
  }
  else {
    printf("commande inconnue\n");
    return 1;
  }
  
  return 0;
}
