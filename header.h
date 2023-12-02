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

    int sommet; //nom sommet destination
    int poids; //poids de l'arrête
    int type; //permet de savoir si c'est une arrête du graphe de précédence (=0) ou une liaison d'exclusion (=1)
    struct arc* arc_suivant;

};
typedef struct arc t_arc;

//structure sommet
typedef struct sommet{

    int valeur; //numéro de l'opération
    int marque;
    int* sommet_adjacent; //pour le graphe d'exclusion
    int* sommet_pre; //pour le graphe de précédence
    float temps; // temps opération
    t_arc *arc;

}t_sommet;

//structure graphe
typedef struct graphe{

    int ordre;
    int taille;
    t_sommet* sommet; //tableau dynamique de sommet

}t_graphe;

typedef struct workstation{
    t_sommet* sommet_in;
    int nb_operation;
    float temps_tot;
}t_workstation;

typedef struct chaine_op{
    t_workstation *workstation;
    int nb_station;
}t_chaine_op;

/* PRECEDENCE */


/*// Structure du nœud dans la pile
struct PileNoeud {
    int value;
    struct PileNoeud* next;
};

// Structure de la pile
struct Noeud {
    struct PileNoeud* top;
};

/*
typedef struct WorkstationPrecedence {
    t_sommet* sommet_in;
    int nb_operation;
    int tempsTotal;
} t_WorkstationPrecedence;

typedef struct ChaineOpPrecedence {
    t_WorkstationPrecedence* workstationprecedence;
    int nb_stationprecedence;
} t_ChaineOpPrecedence;*/


#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_1_7_HEADER_H
