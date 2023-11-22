//
// Created by lucas on 19/11/2023.
// commit et push uniquement ce qu'on a modifié

#include "header.h"

//création des sommets
void creerSommet (t_graphe *grf, FILE *fichier){

    int S1 = 0, ordre = 0, marque = 1, comptage = 0;
    float S2 = 0;
    grf->sommet = (t_sommet*)malloc((ordre+1)* sizeof(t_sommet));

    while (fscanf(fichier, "%d %f", &S1, &S2)==2){

        for(int i = 0; i<=ordre; i++){
            if(grf->sommet[i].valeur == S1){
                marque = 0;
            }
        }
        if(marque==1){
            ordre++;
            grf->sommet = (t_sommet*)realloc(grf->sommet, (ordre+1) * sizeof(t_sommet));
            grf->sommet[comptage].valeur = S1;
            grf->sommet[comptage].temps = S2;
            grf->sommet[comptage].arc = NULL;
            comptage++;
        }
        marque = 1;
    }
    grf->ordre = ordre;
}

// Initialisation des arrêtes
void creerArrete(t_graphe *grf, FILE *fichier, int type) {

    //on pourrait rajouter la possibilité de choisir entre graphe orienté ou non
    //Ici le code permet de charger un graphe non orienté
    int i = 0, j = 0, l = 0;
    int s1 = 0, s2 = 0;

    while (fscanf(fichier, "%d %d", &s1, &s2)==2){// répétition du nombre d'arc présent

        j=0;
        l=0;
        t_arc *new_arc_s1 = malloc(sizeof(t_arc));
        t_arc *new_arc_s2 = malloc(sizeof(t_arc));

        if (type==0) {
            // Création d'une nouvelle arête de s1 à s2
            new_arc_s1->sommet = s2; // sommet de destination de l'arc
            new_arc_s1->poids = 0; // poids de l'arc
            new_arc_s1->type = 0; //précédence donc type0
            new_arc_s1->arc_suivant = NULL;
        }

        if (type == 1) {
            // Création d'une nouvelle arête de s1 à s2
            new_arc_s1->sommet = s2; // sommet de destination de l'arc
            new_arc_s1->poids = 0; // poids de l'arc
            new_arc_s1->type = 1; //contrainte d'exclusion donc type1
            new_arc_s1->arc_suivant = NULL;
            // Création d'une nouvelle arête de s2 à s1 (pour les graphe non orienté)
            new_arc_s2->sommet = s1;
            new_arc_s2->poids = 0;
            new_arc_s2->type = 1; //contrainte d'exclusion donc type1
            new_arc_s2->arc_suivant = NULL;
        }

        //recherche de l'indice du sommet s1
        while (s1 != grf->sommet[j].valeur){
            j++;
        }
        if (type == 1){
            while (s2 != grf->sommet[j].valeur){
                l++;
            }
        }

        if (type==0) {
            //On ajoute l'arête de S1 à S2 dans le sommet S1
            if (grf->sommet[j].arc == NULL) { // s'il n'y a pas d'arc
                grf->sommet[j].arc = new_arc_s1;
            } else { // sinon on va jusqu'au dernier arc
                t_arc *arc_temporaire_s1 = grf->sommet[j].arc; // créer un arc temporaire permet de ne pas écraser les arcs déjà présent
                while (arc_temporaire_s1->arc_suivant != NULL) {
                    arc_temporaire_s1 = arc_temporaire_s1->arc_suivant;
                }
                arc_temporaire_s1->arc_suivant = new_arc_s1;
            }
        }
        if (type==1) {
            //On ajoute l'arête de S1 à S2 dans le sommet S1
            if (grf->sommet[j].arc == NULL) { // s'il n'y a pas d'arc
                grf->sommet[j].arc = new_arc_s1;
            } else { // sinon on va jusqu'au dernier arc
                t_arc *arc_temporaire_s1 = grf->sommet[j].arc; // créer un arc temporaire permet de ne pas écraser les arcs déjà présent
                while (arc_temporaire_s1->arc_suivant != NULL) {
                    arc_temporaire_s1 = arc_temporaire_s1->arc_suivant;
                }
                arc_temporaire_s1->arc_suivant = new_arc_s1;
            }
            //On ajoute l'arête de S2 à S1 dans le sommet S2
            if (grf->sommet[l].arc == NULL) { // s'il n'y a pas d'arc
                grf->sommet[l].arc = new_arc_s2;
            } else { // sinon on va jusqu'au dernier arc
                t_arc *arc_temporaire_s2 = grf->sommet[l].arc;
                while (arc_temporaire_s2->arc_suivant != NULL) {
                    arc_temporaire_s2 = arc_temporaire_s2->arc_suivant;
                }
                arc_temporaire_s2->arc_suivant = new_arc_s2;
            }
        }
        if (type==0){
            i++;
        }
    }
    if (type == 0) {
        grf->taille = i;
    }
}

void init_graphe (char *nomfichier_op, char *nomfichier_pre, t_graphe *grf){

    FILE *fichier_op = fopen(nomfichier_op,"r");
    FILE *fichier_pre = fopen(nomfichier_pre, "r");
    if(!fichier_op){
        printf("Erreur de lecture du fichier_op\n");
        exit(-1);
    }
    if(!fichier_pre){
        printf("Erreur de lecture du fichier_pre\n");
        exit(-1);
    }

    //création et initialisation des sommets
    fseek(fichier_op, 0, SEEK_SET); // on retourne au début du fichier
    creerSommet(grf, fichier_op);

    //création et initialisation des arrêtes
    fseek(fichier_pre, 0, SEEK_SET); // on retourne au début du fichier
    creerArrete(grf, fichier_pre, 0);
}

void init_exclusion (char *nomfichier_exclu, t_graphe *grf){
    FILE *fichier_exclu = fopen(nomfichier_exclu,"r");
    if(!fichier_exclu){
        printf("Erreur de lecture du fichier_exclu\n");
        exit(-1);
    }
    //création et initialisation des sommets
    fseek(fichier_exclu, 0, SEEK_SET); // on retourne au début du fichier
    creerArrete(grf, fichier_exclu, 1);
}

void afficher_graphe(t_graphe grf) {
    printf("Ordre : %d \n", grf.ordre);
    printf("Sommets : \n");
    for (int i = 0; i < grf.ordre; i++) {
        printf("Nom: %d  Valeur: %f\n", grf.sommet[i].valeur,grf.sommet[i].temps);
    }
    printf("\n");
    printf("Taille : %d \n", grf.taille);
    printf("Arretes : \n");
    for (int i = 0; i < grf.ordre; i++) {
        t_arc *arc_actuel = grf.sommet[i].arc;

        while (arc_actuel != NULL) {
            if (arc_actuel->type==0) {
                printf("%d %d %d\n", grf.sommet[i].valeur, arc_actuel->sommet, arc_actuel->poids);
                arc_actuel = arc_actuel->arc_suivant;
            }
        }
    }
}
void afficher_temps_cycle(char *nom_fichier,int *temps) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier==NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
    }
    int result = fscanf(fichier, "%d", temps);
    if (result == 1) {
        fclose(fichier);
        // Afficher le temps d'un cycle
        printf("Contenu lu depuis %s : temps de cycle : %d secondes\n", nom_fichier, *temps);
    } else if (result == 0) {
        fclose(fichier);
        printf("Le contenu du fichier %s n'est pas un nombre entier\n", nom_fichier);
    } else {
        fclose(fichier);
        printf("Erreur lors de la lecture du fichier %s\n", nom_fichier);
        exit(-1);
    }
}

int main(){
    t_graphe precedence; // création du graphe 1
    int temps_cycle;

    //récupération du nom du fichier
    char recip_nom[50];
    char fic_precedence[50];
    char fic_temps_cycle[50];
    char fic_operation[50];
    char fic_exclusion[50];
    strcpy(fic_precedence, "../precedence.txt");
    strcpy(fic_operation, "../operations.txt");
    strcpy(fic_temps_cycle, "../temps_cycle.txt");
    strcpy(fic_exclusion, "../exclusions.txt");

    //Pour que l'utilisateur puisse choisir le fichier qu'il souhaite
    /*char *nom;
    printf("Rentrez le nom du fichier a lire :\n");
    gets(recip_nom);
    nom = malloc(strlen(recip_nom)+1 * sizeof(char *));
    strcpy(nom, recip_nom);
    fflush(NULL);*/

    // Afficher le temps de cycle
    afficher_temps_cycle(fic_temps_cycle, &temps_cycle);
    // Initialisation du graphe
    init_graphe(fic_operation, fic_precedence, &precedence);
    //chargement des exclusions
    init_exclusion(fic_exclusion, &precedence);

    // Affichage precedence
    afficher_graphe(precedence);

    //free(nom);


}