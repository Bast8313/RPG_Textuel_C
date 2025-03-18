#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 255

// struct joueur
typedef struct {
    int pv;
    int endurance;
    int or;
    int evenements;
} Joueur;

// fonction sauvegarder qui prend en entrée un joueur j et ouvre un fichier appelé "inventaire.save" en mode écriture
void sauvegarder(Joueur j) {
    FILE *f = fopen("inventaire.save", "w");
    if (f == NULL) {
        printf("Erreur de sauvegarde !\n");
        return;
    }
    
    //ecriture du bloc de données dans le fichier
    fprintf(f, "%d\n", j.pv);
    fprintf(f, "%d\n", j.endurance);
    fprintf(f, "%d\n", j.or);
    fprintf(f, "%d\n", j.evenements);
    fclose(f);
}

// fct chargement inventaire et lecture
void charger(Joueur *j) {
    FILE *f = fopen("inventaire.save", "r");
    if (f == NULL) {
        printf("Aucun fichier de sauvegarde trouvé, création d'un nouveau personnage...\n");
        // Initialisation par défaut
        j->pv = 100;
        j->endurance = 100;
        j->or = 0;
        j->evenements = 0;
        return;
    }
    fscanf(f, "%d", &j->pv);
    fscanf(f, "%d", &j->endurance);
    fscanf(f, "%d", &j->or);
    fscanf(f, "%d", &j->evenements);
    fclose(f);
}

// fct affichage stats
void afficher_statistiques(Joueur j) {
    printf("|----------------------------------|\n");
    printf("|----------AvALoN QuEsT !!!--------|\n\n");
    printf(" PV                      %d\n", j.pv);
    printf(" Endurance               %d\n", j.endurance);
    printf(" Pièce(s) Or             %d\n", j.or);
    printf(" Evènements rencontrés  %d\n", j.evenements);
    printf("|----------------------------------|\n");
}

// readFile = lire_fichier !!!!!!!! "chemin"  A RECODER !!!!!
void readFile_texte(const char* chemin, char* contenu, int taille_max) {
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", chemin);
        return;
    }
    // si le fichier est ouvert correctement, lit une ligne du fichier dans 'contenu'
    fgets(contenu, taille_max, f);
    fclose(f);
}

// readFile = lire_fichier

// fonction readFile_impact prend en entrée un chemin de fichier et un pointeur vers un entier impact
void readFile_impact(const char* chemin, int* impact) {
    FILE *f = fopen(chemin, "r");
   
   // si le fichier s'ouvre correctement,
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", chemin);
        return;
    }
    fscanf(f, "%d", impact);// lis un entier et stocke dans impact
    fclose(f);
}

// fct pour charger les questions
void charger_question
(const char* dossier_questions, // prend en entrée un dosier de questions, un numéro d'évenement et un buffer pour la question
    int numero_evenement, char* question) 
    {
        char chemin_question[BUF_SIZE];
        snprintf(chemin_question, BUF_SIZE, "quête/%s/question%d.txt", dossier_questions, numero_evenement);
        readFile_question(chemin_question, question, BUF_SIZE);
}

// fct pour charger les réponses
void charger_reponses(const char* dossier_reponses, 
    int numero_evenement, 
    char reponses[BUF_SIZE]) {
    char chemin_reponse[BUF_SIZE];
    for (int i = 0; i < 3; i++) {
        snprintf(chemin_reponse, BUF_SIZE, "quête/%s/reponse%d_%d.txt", dossier_reponses, numero_evenement, i + 1);
        readFile_texte(chemin_reponse, reponses[i], BUF_SIZE);
    }
}

// fct pour charger les impacts
void charger_impacts(const char* dossier_impacts, 
    int numero_evenement, int impacts) {
    char chemin_impact[BUF_SIZE];
    for (int i = 0; i < 3; i++) {
        snprintf(chemin_impact, BUF_SIZE, "quête/%s/impact%d_%d.txt", dossier_impacts, numero_evenement, i + 1);
        readFile_impact(chemin_impact, &impacts);
    }
}

int main() {
    printf("|--------------AvALoN-QuEST-------------|\n|---------------------------------------|\n\n");

    // Lecture du fichier d'introduction
    char intro[BUF_SIZE];
    readFile_texte("quête/intro.txt", intro, BUF_SIZE);
    printf("%s\n", intro);
    
    Joueur j;
    charger(&j);
    afficher_statistiques(j);

    // boucle principale du jeu
    while (1) {
        char question[BUF_SIZE];
        char reponses[BUF_SIZE];
        int impacts;
        const char* dossier_questions = "questions";
        const char* dossier_reponses = "réponses";
        const char* dossier_impacts = "impacts";
        int numero_evenement = j.evenements++;

        // chargement et affichage des questions, réponses et impacts à partir des fichiers
        charger_question(dossier_questions, numero_evenement, question);
        charger_reponses(dossier_reponses, numero_evenement, reponses);
        charger_impacts(dossier_impacts, numero_evenement, impacts);
        
        printf("Question: %s\n", question);
        for (int i = 0; i < 3; i++) {
            printf("Réponse %d: %d (Impact: %d)\n", i + 1, reponses[i], impacts);
        }

        int choix;
        printf("Entrez votre choix (1, 2 ou 3) : ");
        scanf("%d", &choix);

        if (choix < 1 || choix > 3) {
            printf("Choix invalide, veuillez réessayer.\n");
            continue;
        }

        // applique les impacts de la réponse choisie par le joueur aux statistiques du joueur.
        int impact = (choix == 1)// si choix egal 1 impact[0]est assigné à 'impact'sinon ':' passe à l'autre condition
         ? impacts : (choix == 2) 
         ? impacts : impacts;
        j.endurance += impact; // augmente l'endurance du joueur en fonction de l'impact choisi
        j.pv += impact; // augmente PV du joueur en fonction de l'impact
        j.or += impact;                       
        j.evenements++; // +1 évenement 

        // affichage et sauvegarde des nouvelles statistiques du joueur
        afficher_statistiques(j);
        sauvegarder(j);
    }

    return 0;
}