//
// Created by Elisha BAJEMON on 21/11/2023.
//

struct Noeud* CreerNoeud(int operation, int temps_execution) {
    struct Noeud* NouveauNoeud = (struct Noeud*)malloc(sizeof(struct Noeud));
    NouveauNoeud->operation = operation;
    NouveauNoeud->temps_execution = temps_execution;
    NouveauNoeud->suivant = NULL;
    return NouveauNoeud;
}