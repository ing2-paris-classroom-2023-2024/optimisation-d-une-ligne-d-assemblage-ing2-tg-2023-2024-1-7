//
// Created by lucas on 19/11/2023.
// commit et push uniquement ce qu'on a modifié

#include "header.h"

/**********************************************************************************************************************
 * Création des sommets
 *********************************************************************************************************************/

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
            grf->sommet[comptage].marque = 0;
            grf->sommet[comptage].arc = NULL;
            comptage++;
        }
        marque = 1;
    }
    grf->ordre = ordre;
}

/**********************************************************************************************************************
 * Création des arêtes de type 0 et 1
 *********************************************************************************************************************/

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
            while (s2 != grf->sommet[l].valeur){
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

/**********************************************************************************************************************
 * Chargement fichier de précédence et opérations
 *********************************************************************************************************************/

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

/**********************************************************************************************************************
 * Chargement fichier d'exclusion
 *********************************************************************************************************************/

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

/**********************************************************************************************************************
 * Affichage du graphe
 *********************************************************************************************************************/

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
            else {
                arc_actuel = arc_actuel->arc_suivant;
            }
        }
    }
    printf("\n");
    printf("Exclusion : \n");
    for (int i = 0; i < grf.ordre; i++) {
        t_arc *arc_actuel = grf.sommet[i].arc;

        while (arc_actuel != NULL) {
            if (arc_actuel->type==1) {
                printf("%d %d %d\n", grf.sommet[i].valeur, arc_actuel->sommet, arc_actuel->poids);
                arc_actuel = arc_actuel->arc_suivant;
            }
            else {
                arc_actuel = arc_actuel->arc_suivant;
            }
        }
    }
}
/**********************************************************************************************************************
 * Chargement temps de cycle
 *********************************************************************************************************************/

void afficher_temps_cycle(char *nom_fichier,float *temps) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier==NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
    }
    int result = fscanf(fichier, "%f", temps);
    if (result == 1) {
        fclose(fichier);
        // Afficher le temps d'un cycle
        printf("Contenu lu depuis %s : temps de cycle : %f \n", nom_fichier, *temps);
    } else {
        fclose(fichier);
        printf("Erreur lors de la lecture du fichier %s\n", nom_fichier);
        exit(-1);
    }
}

/**********************************************************************************************************************
 * Réponse à la contrainte d'exclusion
 *********************************************************************************************************************/

//recherche des adjacences

void adjacence (t_graphe *grf){
    int j;
    for (int i=0; i<grf->ordre; i++){
        j=0;
        grf->sommet[i].sommet_adjacent =(int*)malloc(j*sizeof(int));
        t_arc* arc_actuel =  grf->sommet[i].arc;

        while (arc_actuel!=NULL){
            if(arc_actuel->type==1){
                grf->sommet[i].sommet_adjacent = (int*)realloc(grf->sommet[i].sommet_adjacent, (j+1) * sizeof(int));
                grf->sommet[i].sommet_adjacent[j] = arc_actuel->sommet;
                j++;
            }
            arc_actuel = arc_actuel->arc_suivant;
        }
        grf->sommet[i].sommet_adjacent = (int*)realloc(grf->sommet[i].sommet_adjacent, (j+1) * sizeof(int));
        grf->sommet[i].sommet_adjacent[j] = -1;
    }
}

//répartition des workstation

void assign_station (t_graphe grf, t_chaine_op *stat) {

    //Init val = 0
    stat->nb_station = 0;

    int comptage_som = 0;

    //Si ordre > 0 alors il y a au moins une station
    if (grf.ordre != 0) {
        stat->nb_station = stat->nb_station + 1;
    }
    if (stat->nb_station > 0) {
        //on ajoute le premier sommet//
        comptage_som++;
        stat->workstation = (t_workstation *) malloc(stat->nb_station * sizeof(t_workstation)); //allocation dynamique 1er station
        stat->workstation[0].sommet_in = (t_sommet *) malloc(comptage_som * sizeof(t_sommet)); //allocation dynamique 1er sommet
        stat->workstation[0].sommet_in[comptage_som - 1] = grf.sommet[0]; //Ajoute le 1er sommet
        stat->workstation[0].nb_operation = comptage_som; // save nb sommet
        //                          //

        printf("nb_station %d", stat->nb_station);
        for (int i = 1; i < grf.ordre; i++) { // tableau de sommet
            int k=0;
            int ajout = 1;
            for (int j = 0; grf.sommet[i].sommet_adjacent[j] != -1; j++) { //tableau d'exclusion
                    for (int l = 0; l < stat->workstation[k].nb_operation; l++) {
                        // si égalité alors exclusion donc changement de station
                        if (grf.sommet[i].sommet_adjacent[j] == stat->workstation[k].sommet_in[l].valeur) {
                            k++;
                            //création nouvelle station
                            if(k+1>stat->nb_station){
                                stat->nb_station = k+1;
                                stat->workstation = (t_workstation*)realloc(stat->workstation, stat->nb_station * sizeof(t_workstation));
                                stat->workstation[k].nb_operation = 1;
                                stat->workstation[k].sommet_in = (t_sommet *) malloc(stat->workstation[k].nb_operation * sizeof(t_sommet)); //allocation dynamique 1er sommet
                                stat->workstation[k].sommet_in[0] = grf.sommet[i]; //Ajoute le 1er sommet
                                ajout = 0;
                            }
                            else {
                                j=-1;
                                break;
                            }
                        }
                    }
            }
            if (ajout == 1){
                stat->workstation[k].nb_operation++;
                stat->workstation[k].sommet_in = (t_sommet *)realloc(stat->workstation[k].sommet_in, stat->workstation[k].nb_operation * sizeof(t_sommet));
                stat->workstation[k].sommet_in[stat->workstation[k].nb_operation-1] = grf.sommet[i];
            }
        }
    }
}

void affiche_workstation (t_chaine_op stat){
    printf("\n\n\n");
    printf("Il y a %d station\n", stat.nb_station);
    for (int i = 0; i < stat.nb_station; i++){
        printf("Dans la workstation numero %d : \n", i);
        printf("    Il y a %d operation\n", stat.workstation[i].nb_operation);
        printf("    Le temps total est de %f\n", stat.workstation[i].temps_tot);
        printf("    Il y a les operations : ");
        for (int j = 0; j < stat.workstation[i].nb_operation; j++){
            printf("%d ", stat.workstation[i].sommet_in[j].valeur);
        }
        printf("\n");
    }
}

void affichage_adjacence (t_graphe grf){
    for (int i=0; i<grf.ordre; i++){
        printf("Le sommet %d : ", grf.sommet[i].valeur);
        for (int j=0; grf.sommet[i].sommet_adjacent[j] != -1; j++){
            printf("%2d ", grf.sommet[i].sommet_adjacent[j]);
        }
        printf("\n");
    }
}

/**********************************************************************************************************************
 * Réponse à la contrainte de précédence
 *********************************************************************************************************************/

void init_sommet_pre(t_graphe *grf){
    for (int i=0; i<grf->ordre; i++){
        grf->sommet[i].sommet_pre = (int*)malloc(1*sizeof (int));
        grf->sommet[i].sommet_pre[0] = -1;
    }
}

//Recherche des précédence pour les mettres dans des tableau
void init_precedence(t_graphe *grf) {

    init_sommet_pre(grf);

    //parcours des sommets
    for (int i = 0; i < grf->ordre; i++) {
        // Parcours des arcs pour chaque sommet
        t_arc *arc_temp = grf->sommet[i].arc;
        while (arc_temp != NULL) {
            if (arc_temp->type == 0) { // Si c'est une arrête du graphe de précédence
                //recherche d'indice
                int k = 0;
                int j = 0;
                //recherche de l'indice du sommet
                while (grf->sommet[j].valeur != arc_temp->sommet){
                    j++;
                    if(j==grf->ordre){
                        j--;
                        break;
                    }
                }
                while(grf->sommet[j].sommet_pre[k] != -1){
                    k++;
                }
                grf->sommet[j].sommet_pre = (int*) realloc(grf->sommet[j].sommet_pre, (k+2)*sizeof(int));
                grf->sommet[j].sommet_pre[k] = grf->sommet[i].valeur;
                grf->sommet[j].sommet_pre[k+1] = -1;
            }
            arc_temp = arc_temp->arc_suivant;
        }
    }
}

//init station
void init_station (t_chaine_op *stat) {
    stat->nb_station = 1;
    stat->workstation = (t_workstation*) malloc(1*sizeof (t_workstation));
    stat->workstation[0].nb_operation = 0;
    stat->workstation[0].temps_tot = 0;
    stat->workstation[0].sommet_in = (t_sommet*) malloc(1*sizeof (t_sommet));
    stat->workstation[0].sommet_in[0].valeur = -2;
}


//Calcul des workstation
void precedence (t_graphe grf, t_chaine_op *stat, float temps_cycle){

    init_station(stat);

    //Parcours de tout les sommets
    for (int i = 0; i < grf.ordre; i++) {

        //vérification précédence
        if (grf.sommet[i].sommet_pre[0] == -1) { //s'il n'y a pas de précédence
            int j = 0;
            int k = 0;
            //Si le temps de cycle est trop grand on va à la station d'après
            while (stat->workstation[k].temps_tot + grf.sommet[i].temps > temps_cycle) {
                k++;
                //si il n'y a pas assez de station
                if (k > stat->nb_station - 1) {
                    stat->workstation = (t_workstation *) realloc(stat->workstation, (k + 1) * sizeof(t_workstation));
                    stat->workstation[k].nb_operation = 0;
                    stat->workstation[k].temps_tot = 0;
                    stat->workstation[k].sommet_in = (t_sommet *) malloc(1 * sizeof(t_sommet));
                    stat->workstation[k].sommet_in[0].valeur = -2;
                    stat->nb_station = stat->nb_station + 1; //nouvelle station
                }
            }

            //ajout du temps de cycle
            stat->workstation[k].temps_tot = stat->workstation[k].temps_tot + grf.sommet[i].temps;
            //ajout de l'opération au bon indice
            while (stat->workstation[k].sommet_in[j].valeur != -2) {
                j++;
            }
            stat->workstation[k].nb_operation = stat->workstation[k].nb_operation + 1;
            stat->workstation[k].sommet_in = (t_sommet *) realloc(stat->workstation[k].sommet_in,(j + 2) * sizeof(t_sommet));
            stat->workstation[k].sommet_in[j] = grf.sommet[i];
            stat->workstation[k].sommet_in[j + 1].valeur = -2;
            grf.sommet[i].marque = 1;
        }
    }

    int fini = 0;
    while(fini != 1) {
        //parcours de la liste des sommet
        int indice_som = 0;
        for (int i = 0; i < grf.ordre; i++) {
            //ajout de variable save
            int indice_stat = 0;
            int a_pre = 0;
            int verif = 0;
            int nb_pre = 0;
            if (grf.sommet[i].marque == 0) { //si le sommet actuel n'a pas encore été marqué
                //recherche du nombre de précédence
                for (int k = 0; grf.sommet[i].sommet_pre[k] != -1; k++){
                    nb_pre = k+1;
                }
                int prerequis = 0;
                //parcours liste sommet pré
                for (int j = 0; grf.sommet[i].sommet_pre[j] != -1; j++) {
                    //parcours liste workstation
                    for (int l = 0; l < stat->nb_station; l++) {
                        //parcours liste des sommets présent dans la workstation
                        for (int k = 0; k < stat->workstation[l].nb_operation; k++) {
                            //s'il y a une précédence
                            if (stat->workstation[l].sommet_in[k].valeur == grf.sommet[i].sommet_pre[j]) {
                                prerequis++;
                                verif=1; //confirmation de précédence
                                indice_som = i; //sauvegarde de l'indice du sommet
                                if (indice_stat < l) {
                                    indice_stat = l; //sauvegarde de l'indice de la workstation
                                }
                            }
                        }
                    }
                }
                if (prerequis == nb_pre){
                    a_pre = 1; //confirmation de précédence
                }
            }
            //S'il y a eu une précédence de trouver
            if (a_pre == 1) {
                int j = 0;
                //vérification temps de cycle
                while (stat->workstation[indice_stat + 1].temps_tot + grf.sommet[indice_som].temps > temps_cycle) {
                    indice_stat++;
                    //si il n'y a pas assez de station
                    if (indice_stat + 1 > stat->nb_station - 1) {
                        stat->workstation = (t_workstation *) realloc(stat->workstation, (indice_stat + 1 + 1) * sizeof(t_workstation));
                        stat->workstation[indice_stat + 1].nb_operation = 0;
                        stat->workstation[indice_stat + 1].temps_tot = 0;
                        stat->workstation[indice_stat + 1].sommet_in = (t_sommet *) malloc(1 * sizeof(t_sommet));
                        stat->workstation[indice_stat + 1].sommet_in[0].valeur = -2;
                        stat->nb_station = stat->nb_station + 1; //nouvelle station
                    }
                }
                //vérification quantité de station
                if (indice_stat + 2 > stat->nb_station) {
                    stat->workstation = (t_workstation *) realloc(stat->workstation,(indice_stat + 2) * sizeof(t_workstation));
                    stat->workstation[indice_stat + 1].nb_operation = 0;
                    stat->workstation[indice_stat + 1].temps_tot = 0;
                    stat->workstation[indice_stat + 1].sommet_in = (t_sommet *) malloc(1 * sizeof(t_sommet));
                    stat->workstation[indice_stat + 1].sommet_in[0].valeur = -2;
                    stat->nb_station = stat->nb_station + 1; //nouvelle station
                }
                //recherche dernier sommet station
                while (stat->workstation[indice_stat + 1].sommet_in[j].valeur != -2) {
                    j++;
                }
                //ajout du sommet trouver
                stat->workstation[indice_stat + 1].temps_tot = stat->workstation[indice_stat + 1].temps_tot + grf.sommet[indice_som].temps;
                stat->workstation[indice_stat + 1].nb_operation = stat->workstation[indice_stat + 1].nb_operation + 1;
                stat->workstation[indice_stat + 1].sommet_in = (t_sommet *) realloc(stat->workstation[indice_stat + 1].sommet_in, (j + 2) * sizeof(t_sommet));
                stat->workstation[indice_stat + 1].sommet_in[j] = grf.sommet[indice_som];
                stat->workstation[indice_stat + 1].sommet_in[j + 1].valeur = -2;
                grf.sommet[indice_som].marque = 1;
            }
        }
        int test = 0;
        for (int i = 0; i<grf.ordre; i++){
            if (grf.sommet[i].marque == 0){
                test = 1;
            }
        }
        if (test == 1){
            fini = 0;
        }
        else{
            fini = 1;
        }
    }
}

void afficherPrecedence(t_graphe grf) {
    printf("Tableau de precedence :\n");
    for (int i = 0; i < grf.ordre; i++) {
        int j = 0;
        printf("Sommet %d : ", grf.sommet[i].valeur);
        while (grf.sommet[i].sommet_pre[j] != -1){
            printf("%d ", grf.sommet[i].sommet_pre[j]);
            j++;
        }
        printf("\n");
    }
}


int main(){

    t_graphe graphe; // création du graphe 1
    t_chaine_op station_op_exclu; //création d'une chaine de production avec exclusion
    t_chaine_op station_op_pre; // création d'une chaine de production avec précédence et temps de cycle
    float temps_cycle;
    int nb_stat;

    //récupération du nom du fichier
    //char recip_nom[50];
    char fic_precedence[25];
    char fic_temps_cycle[25];
    char fic_operation[25];
    char fic_exclusion[25];
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

    /*******************************************************************************************************************
     *
     * Ne pas décommenter !! Fonction essentiel au fonctionnement
     *
     *******************************************************************************************************************/
    // Initialisation du graphe
    init_graphe(fic_operation, fic_precedence, &graphe);
    //chargement des exclusions
    init_exclusion(fic_exclusion, &graphe);
    //adjacence
    adjacence(&graphe);
    //Calcul des workstation
    assign_station(graphe, &station_op_exclu);
    /*******************************************************************************************************************
     *******************************************************************************************************************/


    ///FONCTION D'AFFICHAGE D'INFO//////////////////////////////

    //Afficher le temps de cycle et init le temps de cycle
    afficher_temps_cycle(fic_temps_cycle, &temps_cycle);

    // Affichage graphe
    //afficher_graphe(graphe);

    //Affichage adjacence
    //affichage_adjacence(graphe);

    //Affichage des station
    affiche_workstation(station_op_exclu);
    ///////////////////////////////////////////////////////////

// Calcul de la précédence
    init_precedence(&graphe);

// Affichage de la précédence
    //afficherPrecedence(graphe);

    //Calcul workstation
    precedence(graphe, &station_op_pre, temps_cycle);

    //Affichage des station
    affiche_workstation(station_op_pre);
}