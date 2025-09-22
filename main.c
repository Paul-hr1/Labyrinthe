#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

struct Cell {
    struct Cell* adjacent_cells[4];
    bool is_visited;
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
    EAST,
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
    t->taille = 0;
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

void chemin(struct Grid grid,int ligne, int colonne){
    struct Tab* tab = tabinit(ligne*colonne);
    struct Position start = { rand() % ligne, rand() % colonne};
    pushtab(tab, start);
    grid.cells[start.x][start.y].is_visited = true;
    while(tab->taille > 0 ){;
        struct Position current = tab->positions[tab->taille - 1];
        struct Position next;
        printf("PAUL\n");
        if (current.y - 1 < 0 || grid.cells[current.x][current.y - 1].is_visited == true &&
            current.y + 1 >= colonne || grid.cells[current.x][current.y + 1].is_visited == true &&
            current.x - 1 < 0 || grid.cells[current.x - 1][current.y].is_visited == true &&
            current.x + 1 >= ligne || grid.cells[current.x + 1][current.y].is_visited == true) {
            printf("pablo: %d, picasso: %d \n",current.x,current.y);
            poptab(tab);
            continue;
        }
        printf("pablo: %d, escobar: %d \n",current.x,current.y);
        int random = rand() % 4;
        
        switch (random)
        {
        case 0: //WEST
            next.x = current.x - 1;
            next.y = current.y;
            break;
        case 1: //NORTH
            next.x = current.x;
            next.y = current.y - 1;
            break;
        case 2: //SOUTH
            next.x = current.x;
            next.y = current.y + 1;
            break;
        case 3: //EAST
            next.x = current.x + 1;
            next.y = current.y;
            break;
        default:
            break;
        }

        if(next.x < 0 || next.x >= ligne || next.y < 0 || next.y >= colonne){
            continue;
        }
        if(grid.cells[next.x][next.y].is_visited == false){
            current = next;
            pushtab(tab,current);
            grid.cells[current.x][current.y].is_visited = true;
            printf("ligne: %d, colone: %d\n",current.x,current.y);
        }  
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    printf("%d \n", rand() % 100 ); // Nombre aléatoire entre 0 et 99.
    int ligne, colonne;
    ligne = atoi(argv[1]);
    colonne = atoi(argv[2]);
    printf("Ligne: %d, Colonne: %d\n", ligne, colonne);
    struct Grid grid = grid_initialize(ligne, colonne);
    chemin(grid,ligne, colonne);
    // grid_print(grid);
    return 0;
}