C-WildWater — Distribution d’Eau

Analyse de données d’un système de distribution d’eau potable.

Projet en C et Shell.



Description :

Ce projet permet d’analyser un fichier de données décrivant un réseau de distribution d’eau potable.

Il combine un script Shell (point de départ de l'utilisateur) et des programmes en C pour effectuer des traitements efficaces sur de grandes quantités de données.

Les résultats sont donnés sous forme de fichiers de données (CSV) et de graphiques (PNG).


Fonctionnalités : 

1. Histogrammes des usines de traitement
   
Le programme peut générer des histogrammes selon un critère unique à la fois :

- max : capacité maximale de traitement des usines
- src : volume total capté par les sources alimentant chaque usine
- real : volume réellement traité après prise en compte des fuites

2. Calcul des pertes d’eau (fuites)
   
À partir d’un identifiant d’usine :

- le programme parcourt l’ensemble du réseau en aval, calcule le volume total d’eau perdu et retourne le résultat en millions de m³ par an.
- Il génère ou modifie le fichier de données leaks.dat. Si le fichier existe déjà, une nouvelle ligne est ajoutée à la fin.
  
  Exemple de ligne : Facility Complex #RH400057F; 12.5
  
  Si l’identifiant de l’usine n’existe pas, Facility Complex #INEXISTANT; -1 est affiché sur le terminal.

Structure de notre dépot :

- code : programmes c et points h
- data : fichier fourni pour tester notre programme
- tests : fichiers crées pour pouvoir tester notre code
- main.sh : script Shell principal permettant de lancer les traitements et générer les graphiques.
- makefile : compilation du programme C
- Projet_C-WildWater_preIng2_2025_2026_v1.1.pdf : énoncé du projet 
- Rapport de projet

Éxecution du programme :

1. Rendre le fichier bash exécutable :

     chmod +x main.sh

2. Execution des différentes commandes :
   
- Commande histogrammes :

    ./main.sh data/c-wildwater_v0.dat histo <critere> (max, real ou src)

- Commande leaks : 

    ./main.sh data/c-wildwater_v0.dat leaks "identifiant_usine"
  
    Exemple : ./main.sh data/c-wildwater_v0.dat leaks "Plant #JA200000I"


  
