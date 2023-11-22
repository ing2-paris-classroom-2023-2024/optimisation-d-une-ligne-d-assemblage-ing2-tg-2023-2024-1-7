//
// Created by lucas on 19/11/2023.
// commit et push uniquement ce qu'on a modifié

#include "header.h"

//création des sommets
void creerSommet (t_graphe *grf, FILE *fichier){

    int S1 = 0, ordre = 0, marque = 1, comptage = 0;
    grf->sommet = (t_sommet*)malloc((ordre+1)* sizeof(t_sommet));

    while (fscanf(fichier, "%d", &S1)==1){

        for(int i = 0; i<=ordre; i++){
            if(grf->sommet[i].valeur == S1){
                marque = 0;
            }
        }
        if(marque==1){
            ordre++;
            grf->sommet = (t_sommet*)realloc(grf->sommet, (ordre+1) * sizeof(t_sommet));
            grf->sommet[comptage].valeur = S1;
            grf->sommet[comptage].arc = NULL;
            comptage++;
        }
        marque = 1;
    }
    grf->ordre = ordre;
}

// Initialisation des arrêtes
void creerArrete(t_graphe *grf, FILE *fichier) {

    //on pourrait rajouter la possibilité de choisir entre graphe orienté ou non
    //Ici le code permet de charger un graphe non orienté
    int i = 0, j = 0;
    int s1 = 0, s2 = 0;

    while (fscanf(fichier, "%d %d", &s1, &s2)==2){// répétition du nombre d'arc présent

        j=0;
        // Création d'une nouvelle arête de s1 à s2
        t_arc *new_arc_s1 = malloc(sizeof(t_arc));
        new_arc_s1->sommet = s2; // sommet de destination de l'arc
        new_arc_s1->poids = 0; // poids de l'arc
        new_arc_s1->arc_suivant = NULL;

        // Création d'une nouvelle arête de s2 à s1 (pour les graphe non orienté)
        /*t_arc *new_arc_s2 = malloc(sizeof(t_arc));
        new_arc_s2->sommet = s1;
        new_arc_s2->poids = p;
        new_arc_s2->arc_suivant = NULL;*/
        for (int j = 0; s1 != grf->sommet[j].valeur; j++ ){

        }

        //recherche de l'indice du sommet s1
        while (s1 != grf->sommet[j].valeur){
            j++;
        }

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

        i++;

        /* Uniquement pour les graphe non orienté
        //On ajoute l'arête de S2 à S1 dans le sommet S2
        if (grf->sommet[s2].arc == NULL) { // s'il n'y a pas d'arc
            grf->sommet[s2].arc = new_arc_s2;
        } else { // sinon on va jusqu'au dernier arc
            t_arc *arc_temporaire_s2 = grf->sommet[s2].arc;
            while (arc_temporaire_s2->arc_suivant != NULL) {
                arc_temporaire_s2 = arc_temporaire_s2->arc_suivant;
            }
            arc_temporaire_s2->arc_suivant = new_arc_s2;
        }
        */
    }
    grf->taille=i;
}

void init_graphe (char *nomfichier, t_graphe *grf)
{
    FILE *fichier = fopen(nomfichier,"r");
    if(!fichier){
        printf("Erreur de lecture du fichier1\n");
        exit(-1);
    }

    //création et initialisation des sommets
    fseek(fichier, 0, SEEK_SET); // on retourne au début du fichier
    creerSommet(grf, fichier);

    //création et initialisation des arrêtes
    fseek(fichier, 0, SEEK_SET); // on retourne au début du fichier
    creerArrete(grf, fichier);
}

void afficher_graphe(t_graphe grf) {
    printf("Ordre : %d \n", grf.ordre);
    printf("Sommets : \n");
    for (int i = 0; i < grf.ordre; i++) {
        printf("%d ", grf.sommet[i].valeur);
        
    }

    printf("\n");
    printf("Taille : %d \n", grf.taille);
    printf("Arretes : \n");
    for (int i = 0; i < grf.ordre; i++) {
        t_arc *arc_actuel = grf.sommet[i].arc;

        while (arc_actuel != NULL) {
            printf("%d %d %d\n", grf.sommet[i].valeur, arc_actuel->sommet, arc_actuel->poids);
            arc_actuel = arc_actuel->arc_suivant;
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
        exit(-1);
    } else if (result == 0) {
        fclose(fichier);
        printf("Le contenu du fichier %s n'est pas un nombre entier\n", nom_fichier);
        exit(-1);
    } else {
        fclose(fichier);
        printf("Erreur lors de la lecture du fichier %s\n", nom_fichier);
        exit(-1);
    }
}

int main(){
    t_graphe precedence; // création du graphe 1

    //récupération du nom du fichier
    char recip_nom[50];
    char *nom;
    printf("Rentrez le nom du fichier a lire :\n");
    gets(recip_nom);
    nom = malloc(strlen(recip_nom)+1 * sizeof(char *));
    strcpy(nom, recip_nom);
    fflush(NULL);
    int temps;
    if (strcmp(nom, "../temps_cycle.txt") == 0) {
        // Afficher le temps de cycle
        afficher_temps_cycle(nom, &temps);
    } else {
        // Initialisation du graphe
        init_graphe(nom, &precedence);

        // Affichage precedence
        afficher_graphe(precedence);
    }

    free(nom);


}