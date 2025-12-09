#!/bin/bash

if [ "$1" == "histo" ] && [ "$2" == "src" ]; then
    TEMP_FILE="tests/temp.dat"
    
    # On extrait tout d'un coup vers le fichier temporaire.
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2 > "$TEMP_FILE"

    # On sépare ensuite les colonnes vers les fichiers finaux
    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt

    # Supprime le fichier temporaire
    rm "$TEMP_FILE"

    # On compile le fichier histo.c puis on l'execute
    make src
    ./c-wildwater

    # On affiche les fichiers créer par le fichier C
    echo " "
    echo "--- 📊 Résultats de l'histogramme (ID USINE | VOLUME TOTAL CAPTÉ) ---"
    # L'option awk pour un affichage bien aligné
    paste -d'|' tests/histo_nom_result.txt tests/histo_quantite_result.txt 
    echo "------------------------------------------------------------------"
    
    
elif [ "$1" == "histo" ] && [ "$2" == "real" ]; then
    TEMP_FILE="tests/temp.dat"
    
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2,3 > "$TEMP_FILE"

    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt
    cut -d';' -f3 "$TEMP_FILE" > tests/histo_fuite.txt

    # Nettoyage
    rm "$TEMP_FILE"

else
    echo "Cette commande n'est pas reconnue."
fi
