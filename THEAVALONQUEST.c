#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 255

typedef struct {
    int pv;
    int endurance;
    int or;
    int evenements;
} Joueur;

typedef struct {
    char question[BUF_SIZE];
    char reponses[3][BUF_SIZE];
    int impacts[3];
} Evenement;

void sauvegarder(Joueur j) {
    FILE *f = fopen("inventaire.save", "w");
    if (f == NULL) {
        printf("Erreur de sauvegarde !\n");
        return;
    }
    
    fprintf(f, "%d\n", j.pv);
    fprintf(f, "%d\n", j.endurance);
    fprintf(f, "%d\n", j.or);
    fprintf(f, "%d\n", j.evenements);
    fclose(f);
}

void charger(Joueur *j) {
    FILE *f = fopen("inventaire.save", "r");
    if (f == NULL) {
        printf("Aucun fichier de sauvegarde trouvé, création d'un nouveau personnage...\n");
        return;
    }
    
    fscanf(f, "%d", &j->pv);
    fscanf(f, "%d", &j->endurance);
    fscanf(f, "%d", &j->or);
    fscanf(f, "%d", &j->evenements);
    fclose(f);
}

void afficher_stat(Joueur j) {
    printf("|----------------------------------|\n");
    printf("|-------------AvALoN QuEsT----------|\n");
    printf(" PV                      %d\n", j.pv);
    printf(" ENDurance               %d\n", j.endurance);
    printf(" Pièce(s) Or             %d\n", j.or);
    printf(" Evènements rencontrés  %d\n", j.evenements);
    printf("|----------------------------------|\n");
}

void lire_fichier_texte(const char* chemin, char* contenu, int taille_max) {
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", chemin);
        return;
    }
    fgets(contenu, taille_max, f);
    fclose(f);
}

// fct pour charger les events
void charger_evenement(const char* chemin_question,
    const char* chemins_reponses[],
    const char* chemins_impacts[],
     Evenement* evenements) {
    lire_fichier_texte(chemin_question, evenements->question, BUF_SIZE);
    
    for (int i = 0; i < 3; i++) {
        lire_fichier_texte(chemins_reponses[i], evenements->reponses[i], BUF_SIZE);
        
        FILE *f = fopen(chemins_impacts[i], "r");
        if (f == NULL) {
            printf("Erreur : impossible d'ouvrir le fichier %s.\n", chemins_impacts[i]);
            return;
        }
        fscanf(f, "%d", &evenements->impacts[i]);
        fclose(f);
    }
}

int main() {
    printf("|--------------AvALoN-QuEST-------------|\n|---------------------------------------|\n\n");
    
    // fonctions pour charger et afficher les stats
    Joueur j;
    charger(&j);
    afficher_stat(j);

    //
    Evenement evenement;
    const char* chemin_question = "questions/question1.txt";
    const char* chemins_reponses[] = {"reponses/reponse1_1.txt", "reponses/reponse1_2.txt", "reponses/reponse1_3.txt"};
    const char* chemins_impacts[] = {"impacts/impact1_1.txt", "impacts/impact1_2.txt", "impacts/impact1_3.txt"};
    
    charger_evenement(chemin_question, chemins_reponses, chemins_impacts, &evenement);
    
    printf("Question: %s\n", evenement.question);
    for (int i = 0; i < 3; i++) {
        printf("Réponse %d: %s (Impact: %d)\n", i+1, evenement.reponses[i], evenement.impacts[i]);
    }

    return 0;
}