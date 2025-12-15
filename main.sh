#!/bin/bash

if [ "$1" == "histo" ] && [ "$2" == "src" ]; then
    TEMP_FILE="tests/temp.dat"
    
    # On extrait tout d'un coup vers le fichier temporaire.
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2 > "$TEMP_FILE"

    # On sépare ensuite les colonnes vers les fichiers finaux
    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt
    echo "0" > tests/histo_fuite.txt

    rm "$TEMP_FILE"

    # On compile le fichier histo.c puis on l'execute
    make src
    ./c-wildwater

    #GNU Plot: Nom des fichiers -> histo_nom_result.txt - histo_quantite_result.txt
    RES_NOM="tests/histo_nom_result.txt"
    RES_QTE="tests/histo_quantite_result.txt"
    DATA_PLOT="tests/donnees_histo.dat"

    if [[ -f "$RES_NOM" && -f "$RES_QTE" ]]; then
        paste "$RES_NOM" "$RES_QTE" > "$DATA_PLOT"
    else
        echo "Erreur : Les fichiers résultats du C sont introuvables."
        exit 1
    fi

    echo "Génération du graphique..."
    gnuplot <<- EOF
        # 1. Configuration de la sortie (Image PNG)
        # 'pngcairo' donne une meilleure qualité que 'png' standard
        set terminal png size 800,600
        set output 'tests/histogramme.png'

        # 2. Titres et Axes
        set title "Statistiques : Quantité par Nom"
        set xlabel "Noms"
        set ylabel "Quantité"
        
        # Ajoute une grille horizontale pour faciliter la lecture des hauteurs
        set grid y

        # 3. Style des barres
        set style data histograms
        set style fill solid 1.0 border -1  # Remplissage solide, bordure noire fine
        set boxwidth 0.7                    # Largeur des barres (0.7 = 70% de l'espace)

        # 4. Gestion des étiquettes (Noms) sur l'axe X
        # On les tourne à -45 degrés pour éviter qu'elles ne se chevauchent si elles sont longues
        set xtics rotate by -45 scale 0

        # 5. Dessin du graphique
        # "$DATA_PLOT" : Le fichier temporaire créé par 'paste'
        # using 2:xtic(1) : Utilise la col 2 pour la hauteur, et le texte de la col 1 pour l'axe X
        plot "$DATA_PLOT" using 2:xtic(1) title "Quantité" linecolor rgb "#4682B4"    
EOF

    # Nettoyage du fichier temporaire de données fusionnées (optionnel)
    rm "$DATA_PLOT"

    echo "Terminé !"
    echo "Le graphique a été sauvegardé sous : histogramme.png"
    xdg-open tests/histogramme.png
elif [ "$1" == "histo" ] && [ "$2" == "real" ]; then
    TEMP_FILE="tests/temp.dat"
    
    grep "Spring" data/c-wildwater_v0.dat | cut -d';' -f3- | cut -d'#' -f2 | cut -d';' -f1,2,3 > "$TEMP_FILE"

    cut -d';' -f1 "$TEMP_FILE" > tests/histo_nom.txt
    cut -d';' -f2 "$TEMP_FILE" > tests/histo_quantite.txt
    cut -d';' -f3 "$TEMP_FILE" > tests/histo_fuite.txt

    rm "$TEMP_FILE"

    # On compile le fichier histo.c puis on l'execute
    make src
    ./c-wildwater

    #GNU Plot: Nom des fichiers -> histo_nom_result.txt - histo_quantite_result.txt
    RES_NOM="tests/histo_nom_result.txt"
    RES_QTE="tests/histo_quantite_result.txt"
    DATA_PLOT="tests/donnees_histo.dat"

    if [[ -f "$RES_NOM" && -f "$RES_QTE" ]]; then
        paste "$RES_NOM" "$RES_QTE" > "$DATA_PLOT"
    else
        echo "Erreur : Les fichiers résultats du C sont introuvables."
        exit 1
    fi

    echo "Génération du graphique..."
    gnuplot <<- EOF
        # 1. Configuration de la sortie (Image PNG)
        # 'pngcairo' donne une meilleure qualité que 'png' standard
        set terminal png size 800,600
        set output 'tests/histogramme.png'

        # 2. Titres et Axes
        set title "Statistiques : Quantité par Nom"
        set xlabel "Noms"
        set ylabel "Quantité"
        
        # Ajoute une grille horizontale pour faciliter la lecture des hauteurs
        set grid y

        # 3. Style des barres
        set style data histograms
        set style fill solid 1.0 border -1  # Remplissage solide, bordure noire fine
        set boxwidth 0.7                    # Largeur des barres (0.7 = 70% de l'espace)

        # 4. Gestion des étiquettes (Noms) sur l'axe X
        # On les tourne à -45 degrés pour éviter qu'elles ne se chevauchent si elles sont longues
        set xtics rotate by -45 scale 0

        # 5. Dessin du graphique
        # "$DATA_PLOT" : Le fichier temporaire créé par 'paste'
        # using 2:xtic(1) : Utilise la col 2 pour la hauteur, et le texte de la col 1 pour l'axe X
        plot "$DATA_PLOT" using 2:xtic(1) title "Quantité" linecolor rgb "#4682B4"    
EOF

    # Nettoyage du fichier temporaire de données fusionnées (optionnel)
    rm "$DATA_PLOT"

    echo "Terminé !"
    echo "Le graphique a été sauvegardé sous : histogramme.png"
    xdg-open tests/histogramme.png

elif [ "$1" == "histo" ] && [ "$2" == "max" ]; then
    FILE_OUT="tests/vol_max.txt"

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
