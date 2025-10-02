#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

struct Cell { //une case du labyrinthe
    struct Cell* adjacent_cells[4];
    bool is_visited; //pour la generation et la resolution
    struct Cell* from; //pour la resolution, pour stocker la case precedente
    bool is_in_path; //pour la resolution
};

struct Grid{ //le labyrinthe au total
    struct Cell** cells;
    int colonne;
    int ligne;  
};

enum Direction { //les directions possibles
    NORTH,
    SOUTH,
    WEST,
    EAST,
};

struct Position { //la position d'une case dans le labyrinthe
    int x;
    int y;
};

struct Tab { //tableau dynamique de positions
    struct Position* positions;
    int taille;
    int capacite;
};

struct Queue { //queue de positions pour la resolution
    struct Position* positions;
    int debut;
    int fin;
    int capacite;
};

struct Queue* queueinit(int taille) { //initialisation de la queue
    struct Queue* q = malloc(sizeof(struct Queue));
    q->positions = malloc(taille * sizeof(struct Position));
    q->debut = 0;
    q->fin = 0;
    q->capacite = taille;
    return q;
}

void pushQueue(struct Queue* queue, struct Position element) { //ajout d'une position a la queue
    queue->positions[queue->fin] = element;
    queue->fin = (queue->fin + 1);
}

struct Position popQueue(struct Queue* queue){ //retrait d'une position a la queue
    if (queue->debut == queue->fin) {
        return queue->positions[0];
    }
    return queue->positions[queue->debut++];
}


struct Tab* tabinit(int taille) { //intialisation du tableau dynamique
    struct Tab* t = malloc(sizeof(struct Tab));
    t->positions = malloc(taille * sizeof(struct Position));
    t->taille = 0;
    t->capacite = taille;
    return t;
}

struct Tab* pushtab(struct Tab* tab, struct Position element) { //ajout d'une position au tableau dynamique
    if (tab->taille >= tab->capacite) {
        tab->capacite *= 2; 
        tab->positions = realloc(tab->positions, tab->capacite * sizeof(struct Position));
    }
    tab->positions[tab->taille] = element;
    tab->taille++;
    return tab;
}

struct Tab* poptab(struct Tab* tab) { //retrait d'une position au tableau dynamique
    if (tab->taille == 0) {
        return tab;
    }
    tab->taille--;
    return tab;
}


struct Cell cell_initialize() { //initialisation d'une case
    struct Cell cell;
    for (int i = 0; i < 4; i++) {
        cell.adjacent_cells[i] = NULL;
    }
    cell.is_visited = false;
    cell.from = NULL;
    cell.is_in_path = false;
    return cell;
}

struct Grid grid_initialize(int ligne, int colonne) { //initialisation du labyrinthe
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

void grid_print(struct Grid grid){ //affichage du labyrinthe
    for (int i = 0; i < grid.ligne; i++) {
        for (int j = 0; j < grid.colonne; j++) {
            if(i==0){
                printf("+---");
                continue;
            }
            if (grid.cells[i][j].adjacent_cells[NORTH] != NULL) //si la case du nord est accessible
            {
                printf("+   "); //on laisse un trou
            } 
            else
            {
                printf("+---"); //autrement on affiche un mur
            }
        }
        printf("+\n");

        for (int j = 0; j < grid.colonne; j++) {
            if (grid.cells[i][j].adjacent_cells[WEST] != NULL) //si la case de l'ouest est accessible
            {
                if (grid.cells[i][j].is_in_path == true){ //si la case fait partie du chemin
                    printf("  * "); //on affiche un trou avec une etoile
                }
                else{
                    printf("    "); //on affiche juste un trou
                }
            }
            else
            {
                if (grid.cells[i][j].is_in_path == true){ //si la case fait partie du chemin
                    printf("| * "); //on affiche un mur avec une etoile
                }
                else{
                    printf("|   "); //on affiche juste un mur
                }
            }

        }

    printf("|\n"); //mur de droite
    
    }

    for (int j = 0; j < grid.colonne; j++) { //mur du bas
        printf("+---");
    }
    printf("+\n");
}

void chemin(struct Grid grid,int ligne, int colonne){ //generation du labyrinthe
    struct Tab* tab = tabinit((ligne)*(colonne)); //tableau dynamique pour stocker les positions
    struct Position start = { rand() % ligne, rand() % colonne}; //position de depart aleatoire
    pushtab(tab, start); //on ajoute la position de depart au tableau
    grid.cells[start.x][start.y].is_visited = true; //on marque la case de depart comme visitee
    while(tab->taille > 0 ){ //tant que le tableau n'est pas vide (avec les pushtab)
        struct Position current = tab->positions[tab->taille - 1]; //on prend la derniere position ajoutee
        struct Position next; //position suivante
        if (((current.y - 1 < 0) || (grid.cells[current.x][current.y - 1].is_visited == true)) &&
            ((current.y + 1 >= colonne) || (grid.cells[current.x][current.y + 1].is_visited == true)) &&
            ((current.x - 1 < 0) || (grid.cells[current.x - 1][current.y].is_visited == true)) &&
            ((current.x + 1 >= ligne) || (grid.cells[current.x + 1][current.y].is_visited == true))){ //toutes les cases adjacentes ont ete visitees
            poptab(tab); //on fait demi-tour
            continue;
        }
        int random = rand() % 4; //choix aleatoire de la direction
        
        switch (random)
        {
        case 0: //NORTH
            next.x = current.x -1;
            next.y = current.y;
            if(next.x < 0 || grid.cells[next.x][next.y].is_visited){ //si la case est hors du labyrinthe ou deja visitée on
                continue;
            }
            grid.cells[current.x][current.y].adjacent_cells[NORTH] = &grid.cells[next.x][next.y]; //on relie les deux cases
            grid.cells[next.x][next.y].adjacent_cells[SOUTH] = &grid.cells[current.x][current.y];  //dans l'autre sens aussi
            break;
            
            //on fait pareil pour les autres directions

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

        current = next; //on met a jour la position courante
        pushtab(tab,current);
        grid.cells[current.x][current.y].is_visited = true; //on marque la case comme visitée
    }
    for (int i = 0; i < ligne; i++) {
        for (int j = 0; j < colonne; j++) {
            grid.cells[i][j].is_visited = false; //on remet toutes les cases comme non visitees pour la resolution
        }
    }
}


void resolution(struct Grid grid){ //la resolution du labyrinthe
    struct Queue* queue = queueinit((grid.ligne)*(grid.colonne)); //initialisation de la queue
    struct Position start = { grid.ligne -1, 0}; //position de depart en bas a gauche
    struct Position end = { 0, grid.colonne -1}; //position d'arrivee en haut a droite
    pushQueue(queue, start); //on ajoute la position de depart a la queue
    while(queue->debut != queue->fin){ //tant que la queue n'est pas vide, donc tant qu'on n'a pas trouve la position d'arrivee
        struct Position current = popQueue(queue); //on prend la premiere position de la queue
        int i = current.x; //coordonnees X de la position actuelle
        int j = current.y; //coordonnees Y de la position actuelle
        grid.cells[i][j].is_visited = true; //on marque la case comme visitée
        if(grid.cells[i][j].adjacent_cells[NORTH] != NULL && grid.cells[i-1][j].is_visited == false){ //si la case du nord est accessible et non visitée
            struct Position next = {i-1, j}; //on cree la position suivante
            pushQueue(queue, next); //on l'ajoute a la queue
            grid.cells[i-1][j].from = &grid.cells[i][j]; //on stocke la case precedente dans la case suivante
        }

        //on fait pareil pour les autres directions

        if(grid.cells[i][j].adjacent_cells[SOUTH] != NULL && grid.cells[i+1][j].is_visited == false){
            struct Position next = {i+1, j};
            pushQueue(queue, next);
            grid.cells[i+1][j].from = &grid.cells[i][j];
        }
        if(grid.cells[i][j].adjacent_cells[WEST] != NULL && grid.cells[i][j-1].is_visited == false){
            struct Position next = {i, j-1};
            pushQueue(queue, next);
            grid.cells[i][j-1].from = &grid.cells[i][j];
        }
        if(grid.cells[i][j].adjacent_cells[EAST] != NULL && grid.cells[i][j+1].is_visited == false){
            struct Position next = {i, j+1};
            pushQueue(queue, next);
            grid.cells[i][j+1].from = &grid.cells[i][j];
        }
    }
    grid.cells[0][grid.colonne - 1].is_in_path = true; //on marque la case d'arrivee comme faisant partie du chemin
    struct Cell* current = &grid.cells[0][grid.colonne - 1]; //on part de la case d'arrivee
    while(current->from != NULL){ //tant qu'on n'est pas arrive a la case de depart
        current = current->from; //on recule d'une case
        current->is_in_path = true; //et on marque la case comme faisant partie du chemin
    }
    
    
}

int main(int argc, char** argv) { 
    srand(time(NULL)); //initialisation du generateur de nombres aleatoires
    int ligne, colonne; //dimensions du labyrinthe
    ligne = atoi(argv[1]);
    colonne = atoi(argv[2]);
    struct Grid grid = grid_initialize(ligne, colonne); //initialisation du labyrinthe
    chemin(grid,ligne, colonne); //generation du labyrinthe
    resolution(grid); //resolution du labyrinthe
    grid_print(grid); //affichage du labyrinthe
    return 0;
}