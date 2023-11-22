//
// Created by lucas on 19/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_1_7_HEADER_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_1_7_HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//liste chainé d'arc
struct arc{

    int sommet;
    int poids;
    struct arc* arc_suivant;

};
typedef struct arc t_arc;

//structure sommet
typedef struct sommet{

    int valeur; //numéro de l'opération
    float temps; // temps opération
    t_arc *arc;

}t_sommet;

//structure graphe
typedef struct graphe{

    int ordre;
    int taille;
    t_sommet* sommet; //tableau dynamique de sommet

}t_graphe;

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_1_7_HEADER_H
