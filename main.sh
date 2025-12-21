#!/bin/bash

start_time=$(date +%s%3N)

DATA_DIR="data/"
TEST_DIR="tests/"
EXECUTABLE="./c-wildwater"
MAKEFILE="Makefile"

get_time() {
    end_time=$(date +%s%3N)
    
    elapsed_time=$((end_time - start_time))
    
    echo "Durée totale : ${elapsed_time} ms"
}

show_help() {
    echo "Usage: $0 <fichier.dat> <commande> [option]"
    echo "  $0 data/data.csv histo (src|real|max)"
    echo "  $0 data/data.csv leaks \"Facility...\""
    
    get_time
    exit 1
}

generate_graphs() {
    local input_file="$1"
    local suffix="$2"
    local ylabel="$3"
    local title_context="$4"

    local sorted_file="${TEST_DIR}sorted_${suffix}.dat"
    local top10_file="${TEST_DIR}top10_${suffix}.dat"
    local min50_file="${TEST_DIR}min50_${suffix}.dat"
    local img_top10="histo_${suffix}_top10.png"
    local img_min50="histo_${suffix}_min50.png"

    # 1. On trie tout en DECROISSANT d'abord (Plus grand en haut)
    sort -t';' -k2,2nr "$input_file" > "$sorted_file"

    # 2. On extrait et on inverse (sort -n) pour que le graph monte vers la droite
    head -n 10 "$sorted_file" | sort -t';' -k2,2n > "$top10_file"
    tail -n 50 "$sorted_file" | sort -t';' -k2,2n > "$min50_file"

    # 3. Gnuplot générique
    gnuplot <<- EOF
        set terminal pngcairo size 1200,800 font "Arial,10"
        set datafile separator ";"
        set key outside top right
        set lmargin 15
        set offset 0,0,0.05,0
        set format y "%s.0f"
        set style data histograms
        set style fill solid 1.0 border -1
        set boxwidth 0.7 relative
        set ylabel "$ylabel"
        set grid y
        set yrange [0:*]

        # Graphique Top 10
        set output '$img_top10'
        set title "Top 10 : $title_context"
        set xlabel "Usines"
        set xtics rotate by -45 scale 0
        plot "$top10_file" using 2:xtic(1) title "$title_context" linecolor rgb "#2E8B57"

        # Graphique Min 50
        set output '$img_min50'
        set title "50 plus petites usines : $title_context"
        set xlabel "Usines"
        set xtics rotate by -45 scale 0
        plot "$min50_file" using 2:xtic(1) title "$title_context" linecolor rgb "#DC143C"
EOF

    echo "Graphiques générés :"
    echo "  -> $img_top10"
    echo "  -> $img_min50"
    
    # Ouverture (optionnel)
    xdg-open "$img_top10" 2>/dev/null
    xdg-open "$img_min50" 2>/dev/null
}

rm -rf "$TEST_DIR"
mkdir -p "$TEST_DIR"

DATA_FILE="$1"
COMMAND="$2"
OPTION="$3"

cat << "EOF"
   ______      _       __  _  __     __  _       __        __           
  / ____/     | |     / / (_)/ /____/ / | |     / /____ _ / /_ ___   _____
 / /     ____ | | /| / / / // // __  /  | | /| / // __ `// __// _ \ / ___/
/ /___  /___/ | |/ |/ / / // // /_/ /   | |/ |/ // /_/ // /_ /  __// /    
\____/        |__/|__/ /_//_/ \__,_/    |__/|__/ \__,_/ \__/ \___//_/
EOF

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

if [ "$COMMAND" == "histo" ]; then
    if [ "$OPTION" != "src" ] && [ "$OPTION" != "real" ] && [ "$OPTION" != "max" ]; then
        show_help
    elif [ "$OPTION" == "max" ]; then       
        ALL_USINES_FILE="${TEST_DIR}usines_all.dat"
        
        # Colonne 1 et 3 '-'
        # A faire Verifier si on peut utiliser sort
        awk -F';' '$1 == "-" && $3 == "-" && $5 == "-" {print $2";"$4}' "$DATA_FILE" > "$ALL_USINES_FILE"
    
        generate_graphs "$ALL_USINES_FILE" "$OPTION" "Capacité (M.m3)" "Capacité max" 

    elif [ "$OPTION" == "src" ] || [ "$OPTION" == "real" ]; then
        TEMP_FILE="${TEST_DIR}temp.dat"
        MIXED_FILE="${TEST_DIR}histo_mixed.dat"
        SORTED_FILE="${TEST_DIR}histo_trie.dat"
        
        TOP10_DATA_FILE="${TEST_DIR}histo_top10.dat"
        MIN50_DATA_FILE="${TEST_DIR}histo_min50.dat"
        
        IMG_TOP10="${TEST_DIR}histo_top10.png"
        IMG_MIN50="${TEST_DIR}histo_min50.png"

        INPUT_C_NOM="${TEST_DIR}histo_nom.txt"
        INPUT_C_QTE="${TEST_DIR}histo_quantite.txt"
        INPUT_C_FUITE="${TEST_DIR}histo_fuite.txt"
        
        OUTPUT_C_NOM="${TEST_DIR}histo_nom_result.txt"
        OUTPUT_C_QTE="${TEST_DIR}histo_quantite_result.txt"

        # Extraction des lignes 'Spring'
        grep "Spring" "$DATA_FILE" | cut -d';' -f3,4,5 > "$TEMP_FILE" 

        # Découpage dans les fichiers
        cut -d';' -f1 "$TEMP_FILE" > "$INPUT_C_NOM"   # ID Usines
        cut -d';' -f2 "$TEMP_FILE" > "$INPUT_C_QTE"   # Volume
        cut -d';' -f3 "$TEMP_FILE" > "$INPUT_C_FUITE" # Fuite

        rm "$TEMP_FILE"

        # Exécution du programme C
        $EXECUTABLE "histo" "$OPTION"

        if [ $? -ne 0 ]; then
            echo "Erreur : Le programme C a échoué."
            exit 1
        fi
        
        paste -d';' "$OUTPUT_C_NOM" "$OUTPUT_C_QTE" > "$MIXED_FILE"

        generate_graphs "$MIXED_FILE" "$OPTION" "Volume (M.m3)" "Volume $OPTION"
    fi

elif [ "$COMMAND" == "leaks" ]; then
    if [ -z "$OPTION" ]; then
        echo "Erreur : Il faut préciser un filtre pour leaks"  
        exit 1
    fi

    LEAK_PARENT="${TEST_DIR}leak_nom_parent.txt"
    LEAK_ENFANT="${TEST_DIR}leak_nom_enfant.txt"
    LEAK_QTE="${TEST_DIR}leak_quantite.txt"
    LEAK_FUITE="${TEST_DIR}leak_fuite.txt"

    TEMP_FILE="${TEST_DIR}temp.dat"

    
    # Ignore les lignes où le Parent ou l'Enfant est vide
    # On remplace les "-" par "0" pour le Volume et la Fuite
    awk -F';' '
        NR > 1 && $2 != "-" && $3 != "-" {
            if ($4 == "-") vol="0"; else vol=$4;
            
            if ($5 == "-") leak="0"; else leak=$5;

            print $2";"$3";"vol";"leak;
        }
    ' "$DATA_FILE" > "$TEMP_FILE"

    cut -d';' -f1 "$TEMP_FILE" > "$LEAK_PARENT"
    cut -d';' -f2 "$TEMP_FILE" > "$LEAK_ENFANT"
    cut -d';' -f3 "$TEMP_FILE" > "$LEAK_QTE"
    cut -d';' -f4 "$TEMP_FILE" > "$LEAK_FUITE"
    
    if [ ! -s "$TEMP_FILE" ]; then
        echo "Aucune donnée correspondante avec $OPTION"
        rm $TEMP_FILE
        exit 1
    fi
    
    rm "$TEMP_FILE"

    $EXECUTABLE "leaks" "$OPTION"

    if [ $? -ne 0 ]; then
        echo "erreur execution du C"
        exit 1
    fi

    HISTORIQUE_FILE="leaks.dat"

    # Fichiers résultats générés par le C
    RES_NOM="${TEST_DIR}leak_nom_result_fuite.txt"
    RES_QTE="${TEST_DIR}leak_quantite_result_fuite.txt"

    if [ -f "$RES_QTE" ] && [ -f "$RES_NOM" ]; then
        VALEUR=$(head -n 1 "$RES_QTE")
        
        if [ "$VALEUR" != "-1" ]; then
             paste -d';' "$RES_NOM" "$RES_QTE" >> "$HISTORIQUE_FILE"
             
             echo "Historique mis à jour : $HISTORIQUE_FILE"
             echo "Resultat : Volume total perdu $VALEUR M.m3"
        else
             echo "Resultat : Usine introuvable (non ajouté à l'historique)"
        fi
    fi
fi

get_time
exit 0
