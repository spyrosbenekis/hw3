#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Δομή για την αναπαράσταση ενός κόμβου στον αλγόριθμο αναζήτησης
typedef struct node {
    int x, y;            // Συντεταγμένες του κόμβου
    int g, h;            
    struct node* parent; // Δείκτης στον γονικό κόμβο
} node;

//Απόσταση Manhattan
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Συνάρτηση για τη δημιουργία ενός νέου κόμβου
node* create_node(int x, int y, int g, int h, node* parent) {
    
    node* nod = (node*)malloc(sizeof(node));
    
    nod->x = x;
    nod->y = y;
    nod->g = g;
    nod->h = h;
    nod->parent = parent;
    return nod;
}

// Συνάρτηση για την εύρεση του κόμβου με τη μικρότερη τιμή f στο σύνολο ανοιχτών κόμβων
node* find_min_f(node** open_set, int test_size) {
    node* min_node = open_set[0];
    for(int i = 1; i < test_size; ++i) {
        if ((open_set[i]->g + open_set[i]->h) < (min_node->g + min_node->h)) {
            min_node = open_set[i];
        }
    }
    return min_node;
}

// Συνάρτηση για την ανακατασκευή του μονοπατιού από τον τελικό κόμβο στον αρχικό κόμβο
char* path(node* current) {

    char* moves = (char*)malloc(sizeof(char) * (current->g + 1));

    int cur = current->g;
    moves[cur] = '\0';

    while(current->parent != NULL) {
        if(current->parent->x == current->x + 1) {
            moves[--cur] = 'U'; // Πάνω
        } else if(current->parent->x == current->x - 1) {
            moves[--cur] = 'D'; // Κάτω
        } else if(current->parent->y == current->y + 1) {
            moves[--cur] = 'L'; // Αριστερά
        } else if(current->parent->y == current->y - 1) {
            moves[--cur] = 'R'; // Δεξιά
        }
        current = current->parent;
    }

    return moves;
}

// Συνάρτηση για την εύρεση των δυνατών κινήσεων για το Zoomba
void find_moves(int n, int** room, int zoomba_x, int zoomba_y, int target_x, int target_y) {
    int dx[] = {-1, 1, 0, 0}; // Πιθανές αλλαγές στον άξονα x
    int dy[] = {0, 0, -1, 1}; // Πιθανές αλλαγές στον άξονα y

    node* start = create_node(zoomba_x, zoomba_y, 0, heuristic(zoomba_x, zoomba_y, target_x, target_y), NULL);
    node* goal = create_node(target_x, target_y, 0, 0, NULL);

    node** open_set = (node**)malloc(n * n * sizeof(node*)); // Σύνολο ανοιχτών κόμβων προς εξέταση
    
    int test_size = 0;

    bool **closed_set = (bool **)malloc(n * sizeof(bool *));
    
    for(int i = 0; i < n; ++i) {
        closed_set[i] = (bool *)calloc(n, sizeof(bool));
    }

    open_set[test_size++] = start; // Προσθήκη του αρχικού κόμβου στο σύνολο ανοιχτών κόμβων

    while(test_size > 0) {

        node* current = find_min_f(open_set, test_size); // Εύρεση κόμβου με ελάχιστη τιμή f

        if (current->x == goal->x && current->y == goal->y) { // Εάν έχει επιτευχθεί ο στόχος
            char* moves = path(current); // Ανακατασκευή μονοπατιού
            printf("%s\n", moves); // Εκτύπωση κινήσεων
            return;
        }

            // Δημιουργία διαδόχων κόμβων και προσθήκη τους στο σύνολο ανοιχτών κόμβων
            for(int i = 0; i < 4; ++i) {
                int nx = current->x + dx[i];
                int ny = current->y + dy[i];

                if(nx >= 0 && nx < n && ny >= 0 && ny < n && room[nx][ny] == 0 && !closed_set[nx][ny]) {
                    node* successor = create_node(nx, ny, current->g + 1, heuristic(nx, ny, target_x, target_y), current);
                    open_set[test_size++] = successor;
                    closed_set[nx][ny] = 1;
                }
            }

            // Αφαίρεση του τρέχοντος κόμβου από το σύνολο ανοιχτών κόμβων
            for(int i = 0; i < test_size; ++i) {
                if (open_set[i] == current) {
                    open_set[i] = open_set[test_size - 1];
                    test_size--;
                    break;
                }
            }
            closed_set[current->x][current->y] = 1; // Σήμανση του τρέχοντος κόμβου ως αξιολογημένο
    }

    printf("0\n"); // Εάν ο στόχος δεν είναι προσβάσιμος
}

// Κύρια συνάρτηση
int main(){

    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Failed to read input for n\n");
        return 1;
    }   
    
    int zoomba_x, zoomba_y, target_x, target_y;
    if (scanf("%d %d %d %d", &zoomba_x, &zoomba_y, &target_x, &target_y) != 4) {
        fprintf(stderr, "Failed to read input for zoomba and target positions\n");
        return 1;
    }

    if(zoomba_x < 0 || zoomba_x >= n || zoomba_y < 0 || zoomba_y >= n ||
       target_x < 0 || target_x >= n || target_y < 0 || target_y >= n){
        fprintf(stderr, "Wrong values for zoomba or target");
        return 1;
    }

    // Δέσμευση μνήμης για τον πίνακα room
    int **room = (int **)malloc(n * sizeof(int *));
    
    for(int i = 0; i < n; ++i) {
        room[i] = (int *)malloc(n * sizeof(int));
    }

    // Ανάγνωση τιμών για τον πίνακα room
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (scanf("%1d", &room[i][j]) != 1) {
                fprintf(stderr, "Failed to read input for room[%d][%d]\n", i, j);
                return 1;
            }
        }
    }

    if((*room[zoomba_x] + *room[zoomba_y] + *room[target_x] + *room[target_y]) != 0){
        fprintf(stderr, "Wrong values for zoomba or target");
    }   

    find_moves(n, room, zoomba_x, zoomba_y, target_x, target_y);

    return 0;
}