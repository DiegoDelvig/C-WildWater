#!/bin/bash

if [ "$1" == "histo" ] && [ "$2" == "src" ]; then
    TEMP_FILE="tests/temp.dat"
    
    # On extrait tout d'un coup vers le fichier temporaire.
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2 > "$TEMP_FILE"

    # On sépare ensuite les colonnes vers les fichiers finaux
    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt

    rm "$TEMP_FILE"

    # On compile le fichier histo.c puis on l'execute
    make src
    ./c-wildwater

    #GNU Plot 
    
elif [ "$1" == "histo" ] && [ "$2" == "real" ]; then
    TEMP_FILE="tests/temp.dat"
    
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2,3 > "$TEMP_FILE"

    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt
    cut -d';' -f3 "$TEMP_FILE" > tests/histo_fuite.txt

    rm "$TEMP_FILE"

    # Gnu plot

elif [ "$1" == "histo" ] && [ "$2" == "max" ]; then
    FILE_OUT="tests/vol_max.dat"

    DATA=$(awk -F';' '$1 == "-" && $3 == "-" && $5 == "-"' data/c-wildwater_v0.dat | cut -d';' -f2,4 | tr '[:lower:]' '[:upper:]')
    
    TOP=$(echo "$DATA" | sort -t';' -k2nr | head -n 3)
    BOTTOM=$(echo "$DATA" | sort -t';' -k2n | head -n 3)
    echo "identifier;max volume (k.m³.year⁻¹)"
    echo "$TOP"
    
    echo ""
    echo "identifier;max volume (k.m³.year⁻¹)"
    echo "$BOTTOM"        
    
else
    echo "Cette commande n'est pas reconnue."
fi
