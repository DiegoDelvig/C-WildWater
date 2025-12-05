#!/bin/bash

if [ "$1" == "histo" ] && [ "$2" == "src" ]; then
    TEMP_FILE="tests/temp.dat"

    # On extrait tout d'un coup vers le fichier temporaire
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2 > "$TEMP_FILE"

    # On sépare ensuite les colonnes vers les fichiers finaux
    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt

    # Nettoyage
    rm "$TEMP_FILE"
elif [ "$1" == "histo" ] && [ "$2" == "real" ]; then
    TEMP_FILE="tests/temp.dat"

    # On extrait tout d'un coup vers le fichier temporaire
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2,3 > "$TEMP_FILE"

    # On sépare ensuite les colonnes vers les fichiers finaux
    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt
    cut -d';' -f3 "$TEMP_FILE" > tests/histo_fuite.txt

    # Nettoyage
    rm "$TEMP_FILE"
else
    echo "Cette commande n'est pas reconnue."
fi
