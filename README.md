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

- le programme parcourt l’ensemble du réseau en aval,
- calcule le volume total d’eau perdu,
- retourne le résultat en millions de m³ par an.
  
Si l’identifiant de l’usine n’existe pas, la valeur -1 est retournée.

Structure de notre dépot :

- code : programmes c
- data : fichier fourni pour tester notre programme
- tests : fichiers crées pour pouvoir tester notre code
- main.sh : 
- makefile
- Projet_C-WildWater_preIng2_2025_2026_v1.1.pdf

Éxecution du programme :

- Compilation :
  

- Commande histogrammes :

    ./myscript.sh c-wildwater_v3.dat histo <critere> (max, real ou src)

- Commande leaks : 

    ./myscript.sh c-wildwater_v3.dat leaks "<identifiant_usine>"


  
