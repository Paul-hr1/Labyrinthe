#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

struct Cell {
    struct Cell* adjacent_cells[4];
    bool is_visited;
    struct cell* from;
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

struct Queue {
    struct Position* positions;
    int debut;
    int fin;
    int capacite;
};

struct Queue* queueinit(int taille) {
    struct Queue* q = malloc(sizeof(struct Queue));
    q->positions = malloc(taille * sizeof(struct Position));
    q->debut = 0;
    q->fin = 0;
    q->capacite = taille;
    return q;
}

void pushQueue(struct Queue* queue, struct Position element) {
    queue->positions[queue->fin] = element;
    queue->fin = (queue->fin + 1);
}

struct Position popQueue(struct Queue* queue){
    if (queue->debut == queue->fin) {
        return queue->positions[0];
    }
    return queue->positions[queue->debut++];
}


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
    cell.is_visited = false;
    cell.from = NULL;
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
            if(i==0){
                printf("+---");
                continue;
            }
            if (grid.cells[i][j].adjacent_cells[NORTH] != NULL)
            {
                printf("+   ");
            } 
            else
            {
                printf("+---");
            }
        }
        printf("+\n");

        for (int j = 0; j < grid.colonne; j++) {
            if(j==0){
                printf("|   ");
                continue;
            }
            if (grid.cells[i][j].adjacent_cells[WEST] != NULL)
            {
                printf("    ");
            }
            else
            {
                printf("|   ");
            }

        }

    printf("|\n");
    
    }

    for (int j = 0; j < grid.colonne; j++) {
        printf("+---");
    }
    printf("+\n");
}

void chemin(struct Grid grid,int ligne, int colonne){
    struct Tab* tab = tabinit((ligne)*(colonne));
    struct Position start = { rand() % ligne, rand() % colonne};
    pushtab(tab, start);
    grid.cells[start.x][start.y].is_visited = true;
    while(tab->taille > 0 ){
        struct Position current = tab->positions[tab->taille - 1];
        struct Position next;
        if (((current.y - 1 < 0) || (grid.cells[current.x][current.y - 1].is_visited == true)) &&
            ((current.y + 1 >= colonne) || (grid.cells[current.x][current.y + 1].is_visited == true)) &&
            ((current.x - 1 < 0) || (grid.cells[current.x - 1][current.y].is_visited == true)) &&
            ((current.x + 1 >= ligne) || (grid.cells[current.x + 1][current.y].is_visited == true))){
            poptab(tab);
            continue;
        }
        int random = rand() % 4;
        
        switch (random)
        {
        case 0: //NORTH
            next.x = current.x -1;
            next.y = current.y;
            if(next.x < 0 || grid.cells[next.x][next.y].is_visited){
                continue;
            }
            grid.cells[current.x][current.y].adjacent_cells[NORTH] = &grid.cells[next.x][next.y];
            grid.cells[next.x][next.y].adjacent_cells[SOUTH] = &grid.cells[current.x][current.y];
            break;
        case 1: //SOUTH
            next.x = current.x +1;
            next.y = current.y ;
            if(next.x >= ligne || grid.cells[next.x][next.y].is_visited){
                continue;
            }
            grid.cells[current.x][current.y].adjacent_cells[SOUTH] = &grid.cells[next.x][next.y];
            grid.cells[next.x][next.y].adjacent_cells[NORTH] = &grid.cells[current.x][current.y];
            break;
        case 2: //WEST
            next.x = current.x ;
            next.y = current.y -1;
            if(next.y < 0 || grid.cells[next.x][next.y].is_visited){
                continue;
            }
            grid.cells[current.x][current.y].adjacent_cells[WEST] = &grid.cells[next.x][next.y];
            grid.cells[next.x][next.y].adjacent_cells[EAST] = &grid.cells[current.x][current.y];
            break;
        case 3: //EAST
            next.x = current.x ;
            next.y = current.y +1;
            if(next.y >= colonne || grid.cells[next.x][next.y].is_visited){
                continue;
            }
            grid.cells[current.x][current.y].adjacent_cells[EAST] = &grid.cells[next.x][next.y];
            grid.cells[next.x][next.y].adjacent_cells[WEST] = &grid.cells[current.x][current.y];
            break;
        default:
            break;
        }

        current = next;
        pushtab(tab,current);
        grid.cells[current.x][current.y].is_visited = true; 
    }
}


void resolution(struct Grid grid){
    struct Queue* queue = queueinit((grid.ligne)*(grid.colonne));
    struct Position start = { grid.ligne -1, 0};
    struct Position end = { 0, grid.colonne -1};
    while(queue->debut != queue->fin){
        struct Position current = popQueue(queue);
        int i = current.x;
        int j = current.y;
        if(i == end.x && j == end.y){
            printf("Chemin trouvé !\n");
            return;
        }
        if(grid.cells[i][j].adjacent_cells[NORTH] != NULL){
            struct Position next = {i-1, j};
            pushQueue(queue, next);
            grid.cells[i-1][j].from = &grid.cells[i][j];
        }
        if(grid.cells[i][j].adjacent_cells[SOUTH] != NULL){
            struct Position next = {i+1, j};
            pushQueue(queue, next);
            grid.cells[i+1][j].from = &grid.cells[i][j];
        }
        if(grid.cells[i][j].adjacent_cells[WEST] != NULL){
            struct Position next = {i, j-1};
            pushQueue(queue, next);
            grid.cells[i][j-1].from = &grid.cells[i][j];
        }
        if(grid.cells[i][j].adjacent_cells[EAST] != NULL){
            struct Position next = {i, j+1};
            pushQueue(queue, next);
            grid.cells[i][j+1].from = &grid.cells[i][j];
        }
        }
    }

int main(int argc, char** argv) { 
    srand(time(NULL));
    // printf("%d \n", rand() % 100 ); // Nombre aléatoire entre 0 et 99.
    int ligne, colonne;
    ligne = atoi(argv[1]);
    colonne = atoi(argv[2]);
    // printf("Ligne: %d, Colonne: %d\n", ligne, colonne);
    struct Grid grid = grid_initialize(ligne, colonne);
    chemin(grid,ligne, colonne);
    resolution(grid);
    grid_print(grid);
    return 0;
}