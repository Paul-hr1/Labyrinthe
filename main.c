#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Cell {
    struct Cell* adjacent_cells[4];
};

enum Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

int main(int argc, char** argv) {
    // Initialisation avec une graine dépendant du temps.
    srand(time(NULL));
    printf("%d", rand() % 100); // Nombre aléatoire entre 0 et 99.
    int ligne, colonne;
    ligne = atoi(argv[1]);
    colonne = atoi(argv[2]);
    printf("Ligne: %d, Colonne: %d\n", ligne, colonne);
    return 0;
}