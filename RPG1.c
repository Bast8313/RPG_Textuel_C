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

// struct Evenement (reponses et impacts)
typedef struct {
    char question[BUF_SIZE];
    char reponses[3][BUF_SIZE];
    int impacts[3];
} Evenement;

// fct sauvegarde
void sauvegarder(Joueur j) {
    FILE *f = fopen("inventaire.save", "w");
    if (f == NULL) {
        printf("Erreur de sauvegarde !\n");
        return;
    }
    
    //ecriture du bloc d données dans le fichier
    fprintf(f, "%d\n", j.pv);
    fprintf(f, "%d\n", j.endurance);
    fprintf(f, "%d\n", j.or);
    fprintf(f, "%d\n", j.evenements);
    fclose(f);
}
// fct chargementt inventaire et lect
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

// fct  affichage stats
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
    fgets(contenu, taille_max, f);
    fclose(f);
}

// readFile = lire_fichier
void readFile_impact(const char* chemin, int* impact) {
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", chemin);
        return;
    }
    fscanf(f, "%d", impact);
    fclose(f);
}
// fonctions pour le changement des réponses et impacts (chemin A REVOIR...)
void charger_evenement
(const char* dossier_questions,
    const char* dossier_reponses,
    const char* dossier_impacts, 
    int numero_evenement, Evenement* evenement)

    {
    char chemin_question[BUF_SIZE];
    char chemin_reponse[BUF_SIZE];
    char chemin_impact[BUF_SIZE];

    // snprintf = sprintf plus l caractère null (de fin)
    snprintf(chemin_question, BUF_SIZE, "quête/%s/question%d.txt", dossier_questions, numero_evenement);

    readFile_texte(chemin_question, evenement->question, BUF_SIZE);

    for (int i = 0; i < 3; i++) {
        snprintf(chemin_reponse, BUF_SIZE, "quête/%s/reponse%d_%d.txt", dossier_reponses, numero_evenement, i + 1);
        readFile_texte(chemin_reponse, evenement->reponses[i], BUF_SIZE);

        snprintf(chemin_impact, BUF_SIZE, "quête/%s/impact%d_%d.txt", dossier_impacts, numero_evenement, i + 1);
        readFile_impact(chemin_impact, &evenement->impacts[i]);
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

    // boucle d'évenement
    while (1) {
        Evenement evenement;
        const char* dossier_questions = "questions";
        const char* dossier_reponses = "réponses";
        const char* dossier_impacts = "impacts";
        int numero_evenement = j.evenements++;

        charger_evenement(dossier_questions, dossier_reponses, dossier_impacts, numero_evenement, &evenement);
        
        printf("Question: %s\n", evenement.question);
        for (int i = 0; i < 3; i++) {
            printf("Réponse %d: %s (Impact: %d)\n", i + 1, evenement.reponses[i], evenement.impacts[i]);
        }

        int choix;
        printf("Entrez votre choix (1, 2 ou 3) : ");
        scanf("%d", &choix);

        if (choix < 1 || choix > 3) {
            printf("Choix invalide, veuillez réessayer.\n");
            continue;
        }

        // applique les impacts de la réponse choisie par le joueur aux statistiques du joueur.
        int impact = (choix == 1) ? evenement.impacts[0] : (choix == 2) ? evenement.impacts[1] : evenement.impacts[2];
        j.endurance += impact;
        j.pv += impact;
        j.or += impact;
        j.evenements++;

        afficher_statistiques(j);
        sauvegarder(j);
    }

    return 0;
}