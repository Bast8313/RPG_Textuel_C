#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int pv;
    int endurance;
    int or;
    int evenements;
} Personnage;

void clear_screen() {
    system("clear");
}

void sauvegarder(Personnage p) {
    FILE *f = fopen("inventaire.save", "w");
    if (f == NULL) {
        printf("Erreur de sauvegarde !\n");
        return;
    }
    
    fprintf(f, "%d\n", p.pv);
    fprintf(f, "%d\n", p.endurance);
    fprintf(f, "%d\n", p.or);
    fprintf(f, "%d\n", p.evenements);
    fclose(f);
}

void charger(Personnage *p) {
    FILE *f = fopen("inventaire.save", "r");
    if (f == NULL) {
        printf("Aucun fichier de sauvegarde trouvé, création d'un nouveau personnage...\n");
        return;
    }
    
    fscanf(f, "%d", &p->pv);
    fscanf(f, "%d", &p->endurance);
    fscanf(f, "%d", &p->or);
    fscanf(f, "%d", &p->evenements);
    fclose(f);
}

void afficher_statistiques(Personnage p) {
    clear_screen();
    printf("|----------------------------------|\n");
    printf("|-------------RPG Texutel----------|\n");
    printf(" PV                      %d\n", p.pv);
    printf(" ENDurance               %d\n", p.endurance);
    printf(" Pièce(s) Or             %d\n", p.or);
    printf(" EVEN(T)ement rencontrés  %d\n", p.evenements);
    printf("|----------------------------------|\n");
}

void evenement(Personnage *p) {
    int choix;
    afficher_statistiques(*p);
    printf("Il y a devant vous un ours gigantesque ! Que faites-vous ?\n");
    printf("1. Je le mord au visage ! Arrrg ! (-10 ENDurance)\n");
    printf("2. Je m'enfuis... (-3 ENDurance)\n");
    printf("3. Je cherche une arme (-1 ENDurance)\n");
    printf("4. Quitter\n");

    printf("Tapez votre réponse : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            p->endurance -= 10;
            p->pv -= 199;
            p->evenements++;
            printf("Vous sautez au visage de l'ours et lui mordez l'oeil ! Mais malheureusement, il lève une patte fortement et vous déchire en deux...\n");
            printf("GAME OVER\n");
            break;
        case 2:
            p->endurance -= 3;
            p->evenements++;
            printf("Vous fuyez rapidement, l'ours ne vous poursuit pas.\n");
            break;
        case 3:
            p->endurance -= 1;
            p->evenements++;
            printf("Vous trouvez une petite épée, vous pouvez maintenant vous défendre.\n");
            break;
        case 4:
            printf("Vous avez choisi de quitter le jeu.\n");
            exit(0);
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
    }

    if (p->pv <= 0) {
        printf("GAME OVER\n");
        exit(0);
    }
    if (p->or >= 1000) {
        printf("Vous avez atteint 1000 pièces d'or ! Vous avez gagné !\n");
        exit(0);
    }
}

int main() {
    Personnage joueur;
    int choix;

    // Charger ou créer un personnage
    charger(&joueur);

    printf("Bienvenue dans AvAlon Quest !\n");
    joueur.pv = 18;
    joueur.endurance = 99;
    joueur.or = 0;
    joueur.evenements = 0;

    // Sauvegarde au début
    sauvegarder(joueur);

    while (1) {
        evenement(&joueur);
        // Sauvegarder l'état du joueur après chaque événement
        sauvegarder(joueur);
    }

    return 0;
}


