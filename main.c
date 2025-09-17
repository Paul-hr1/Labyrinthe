#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct Cell {
    struct Cell* adjacent_cells[4];
};

struct Grid{
    struct Cell** cells;
    int colonne;
    int ligne;  
};

enum Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

struct Position {
    int x;
    int y;
};

struct Tab {
    struct Position* positions;
    int taille;
    int capacite;
};

struct Tab* tabinit(int taille) {
    struct Tab* t = malloc(sizeof(struct Tab));
    t->positions = malloc(taille * sizeof(struct Position));
    t->taille = taille;
    t->capacite = taille;
    return t;
}

struct Tab* pushtab(struct Tab* tab, struct Position element) {
    if (tab->taille >= tab->capacite) {
        tab->capacite *= 2;
        tab->positions = realloc(tab->positions, tab->capacite * sizeof(struct Position));
    }
    tab->positions[tab->taille] = element;
    tab->taille++;
    return tab;
}

struct Tab* poptab(struct Tab* tab) {
    if (tab->taille == 0) {
        return tab;
    }
    tab->taille--;
    return tab;
}


struct Cell cell_initialize() {
    struct Cell cell;
    for (int i = 0; i < 4; i++) {
        cell.adjacent_cells[i] = NULL;
    }
    return cell;
}

struct Grid grid_initialize(int ligne, int colonne) {
    struct Grid grid;
    grid.ligne = ligne;
    grid.colonne = colonne;
    grid.cells = malloc(sizeof(struct Cell*) * ligne);
    for (int i = 0; i < ligne; i++) {
        grid.cells[i] = malloc(sizeof(struct Cell) * colonne);
        for (int j = 0; j < colonne; j++) {
            grid.cells[i][j] = cell_initialize();
        }
    }
    return grid;
}

void grid_print(struct Grid grid){
    for (int i = 0; i < grid.ligne; i++) {
        for (int j = 0; j < grid.colonne; j++) {
            printf("+---");
        }
        printf("+\n");

        for (int j = 0; j < grid.colonne; j++) {
            printf("|   ");
        }

    printf("|\n");
    
    }

    for (int j = 0; j < grid.colonne; j++) {
        printf("+---");
    }
    printf("+\n");
}

void chemin(int ligne, int colonne){
    struct Tab* tab = tabinit(ligne*colonne);
    tab->positions[0].x = rand() % ligne;
    tab->positions[0].y = rand() % colonne;
    pushtab(tab, tab->positions[0]);
    while(tab->taille <= )
}

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("%d \n", rand() % 100 ); // Nombre aléatoire entre 0 et 99.
    int ligne, colonne;
    ligne = atoi(argv[1]);
    colonne = atoi(argv[2]);
    printf("Ligne: %d, Colonne: %d\n", ligne, colonne);
    struct Grid grid = grid_initialize(ligne, colonne);
    grid_print(grid);
    chemin(ligne, colonne);
    return 0;
}