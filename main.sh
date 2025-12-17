#!/bin/bash

start_time=$(date +%s%3N)

DATA_DIR="data/"
TEST_DIR="tests/"
EXECUTABLE="./c-wildwater"
MAKEFILE="Makefile"

get_time() {
    end_time=$(date +%s.%3N)
    elapsed_time=$(echo "scale=3; $end_time - $start_time" | bc)
    echo "Temps écoulé : $elapsed_time ms"
}

show_help() {
    echo "Usage: $0 <fichier.dat> <commande> [option]"
    echo "  $0 data/data.csv histo (src|real|max)"
    echo "  $0 data/data.csv leaks \"Facility...\""
    
    get_time
    exit 1
}

rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

DATA_FILE="$1"
COMMAND="$2"
OPTION="$3"

if [ -z "$DATA_FILE" ] || [ -z "$COMMAND" ]; then
    show_help
fi

if [ ! -f "$DATA_FILE" ]; then
    echo "'$DATA_FILE' inexistant"
    show_help
fi

if [ $COMMAND != "histo" ] && [ $COMMAND != "leaks" ]; then
    show_help
fi

if [ -f "$MAKEFILE" ]; then
    echo "Compilation en cours"
    make
    if [ $? -ne 0 ]; then
        echo "Erreur de compilation"
        exit 1
    fi
else
    echo "Makefile introuvable"
    exit 1
fi

if [ $COMMAND == "histo" ]; then
    if [ $OPTION != "src" ] && [ $OPTION != "real" ] && [ $OPTION != "max" ]; then
        show_help
    elif [ $OPTION == "max" ]; then       
        ALL_USINES_FILE="usines_all.dat"
        VOL_MAX_FILE="vol_max.dat"
        VOL_MIN_FILE="vol_min.dat"
        
        # Colonne 1 et 3 '-'
        # A faire Verifier si on peut utiliser sort
        awk -F';' '$1 == "-" && $3 == "-" && $5 == "-" {print $2";"$4}' "$DATA_FILE" > "$TEST_DIR/$ALL_USINES_FILE"

        # Graph des 10 plus grandes
        sort -t';' -k2nr "$TEST_DIR/$ALL_USINES_FILE" | head -n 10 > "$TEST_DIR/$VOL_MAX_FILE"
        gnuplot <<- EOF
            set terminal pngcairo size 800,600
            set output 'tests/histo_max_top10.png'
            set title "Top 10 : Plus Grandes Usines"
            set xlabel "Usines"
            set ylabel "Capacité (m3)"
            set grid y
            set style data histograms
            set style fill solid 1.0 border -1
            set boxwidth 0.7
            set xtics rotate by -45 scale 0 font ",8"
            set datafile separator ";"
            plot "$TEST_DIR/$VOL_MAX_FILE" using 2:xtic(1) title "Capacité Max" linecolor rgb "#228B22"
EOF
        echo "Graphique généré : tests/histo_max_top10.png"

        # Graph des 50 plus petites
        sort -t';' -k2n "$TEST_DIR/$ALL_USINES_FILE" | head -n 50 > "$TEST_DIR/$VOL_MIN_FILE"
        gnuplot <<- EOF
            set terminal pngcairo size 1200,600
            set output 'tests/histo_max_min50.png'
            set title "Top 50 : Plus Petites Usines"
            set xlabel "Usines"
            set ylabel "Capacité (m3)"
            set grid y
            set style data histograms
            set style fill solid 1.0 border -1
            set boxwidth 0.7
            # Police plus petite car il y a 50 barres
            set xtics rotate by -45 scale 0 font ",6"
            set datafile separator ";"
            plot "$TEST_DIR/$VOL_MIN_FILE" using 2:xtic(1) title "Capacité Min" linecolor rgb "#B22222"
EOF
        echo "Graphique généré : tests/histo_max_min50.png"

        # On supprime les fichiers temporaires
        rm "$TEST_DIR/$ALL_USINES_FILE" "$TEST_DIR/$VOL_MAX_FILE" "$TEST_DIR/$VOL_MIN_FILE"

        # Ouverture des images
        xdg-open tests/histo_max_top10.png 2>/dev/null
        xdg-open tests/histo_max_min50.png 2>/dev/null
    
    elif [ $OPTION == "src" ] || [ $OPTION == "real" ]; then
        TEMP_FILE="$TEST_DIR/temp.dat"

        # Les lignes qui contiennent 'Spring'
        grep "Spring" "$DATA_FILE" | cut -d';' -f3,4,5 > "$TEMP_FILE" 

        cut -d';' -f1 "$TEMP_FILE" > "$TEST_DIR/histo_nom.txt" # ID Usines
        cut -d';' -f2 "$TEMP_FILE" > "$TEST_DIR/histo_quantite.txt" # Volume
        cut -d';' -f3 "$TEMP_FILE" > "$TEST_DIR/histo_fuite.txt" # Fuite

        rm "$TEMP_FILE"

        # Execution du programme C
        $EXECUTABLE "histo" "$OPTION"

        if [ $? -ne 0 ]; then
            echo "Erreur : Le programme C a échoué."
            exit 1
        fi

        # Graph
        paste -d';' "tests/histo_nom_result.txt" "tests/histo_quantite_result.txt" > "tests/histo_mixed.dat"

        # 2. Configuration et exécution de Gnuplot
        gnuplot <<- EOF
            set terminal pngcairo size 1200,700
            set output 'tests/histo_result.png'
            
            set title "Histogramme des volumes ($OPTION)"
            set xlabel "Usines"
            set ylabel "Volume (m3)"
            
            set grid y
            set style data histograms
            set style fill solid 1.0 border -1
            
            # Ajustement auto des barres
            set boxwidth 0.7 relative
            
            # Rotation des noms d'usines pour qu'ils soient lisibles
            set xtics rotate by -45 scale 0 font ",8"
            
            # Important : dire à Gnuplot que le séparateur est ';'
            set datafile separator ";"
            
            # On plotte le fichier fusionné
            # using 2:xtic(1) -> Utilise la col 2 pour la hauteur, et la col 1 pour les noms en bas
            plot "tests/histo_mixed.dat" using 2:xtic(1) title "Volume" linecolor rgb "#4682B4"
EOF

        echo "Graphique généré : tests/histo_result.png"
        
        # Nettoyage du fichier fusionné temporaire
        rm "tests/histo_merged.dat"

        # Ouverture automatique de l'image (optionnel)
        xdg-open tests/histo_result.png 2>/dev/null
    fi
elif [ $COMMAND == "leaks" ]; then
    if [ -z "$OPTION" ]; then
        echo "Erreur : Il faut préciser un filtre pour leaks"  
        exit 1
    fi

    TEMP_FILE="$TEST_DIR/temp.dat"

    # on filtre avec l'option (ID d'usine) passé dans l'execution du script
    grep "$OPTION" "$DATA_FILE" | cut -d';' -f3,4,5 > "$TEMP_FILE"

    if [ ! -s "$TEMP_FILE" ]; then
        echo "Aucune donnée correspondante avec $OPTION"
        rm $TEMP_FILE
        exit 1
    fi

    cut -d';' -f1 "$TEMP_FILE" > "$TEST_DIR/histo_nom.txt"
    cut -d';' -f2 "$TEMP_FILE" > "$TEST_DIR/histo_quantite.txt"
    
    awk -F';' '{
        valeur = $3;

        if (length(valeur) == 0) {
            print "0";
        } 
        else {
            print valeur;
        }
    }' "$TEMP_FILE" > "$TEST_DIR/histo_fuite.txt"

    rm $TEMP_FILE

    $EXECUTABLE "leaks"
    if [ $? -ne 0 ]; then
        echo "erreur execution du C"
        exit 1
    fi

    # Finir leaks.c

fi

get_time
exit 0
