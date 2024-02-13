#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Δομή για την αναπαράσταση ενός κόμβου στον αλγόριθμο αναζήτησης
typedef struct node{
    int x, y;            // Συντεταγμένες του κόμβου
    int g, h;            // Τιμές g και h
    struct node* parent; // Δείκτης στον γονικό κόμβο
} node;

// Ευκλείδια απόσταση
double euclidean(int x1, int y1, int x2, int y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Συνάρτηση f
int f(node* node){
    return node->g + node->h;
}

// Συνάρτηση για τη δημιουργία ενός νέου κόμβου
node* create_node(int x, int y, int g, int h, node* parent){
    node* nod = (node*)malloc(sizeof(node));
    nod->x = x;
    nod->y = y;
    nod->g = g;
    nod->h = h;
    nod->parent = parent;
    return nod;
}

// Συνάρτηση για την εύρεση του κόμβου με τη μικρότερη τιμή f στο σύνολο ανοιχτών κόμβων
node* find_min_f(node** open_set, int test_size){
    node* min_node = open_set[0];
    for(int i = 1; i < test_size; ++i) {
        if (f(open_set[i])< f(min_node)){
            min_node = open_set[i];
        }
    }
    return min_node;
}

// Συνάρτηση για την ανακατασκευή του μονοπατιού από τον τελικό κόμβο στον αρχικό κόμβο
char* path(node* current){
    char* moves = (char*)malloc(sizeof(char) * (current->g + 1));
    int cur = current->g;
    moves[cur] = '\0';
    while(current->parent != NULL){
        if(current->parent->x == current->x + 1)
            moves[--cur] = 'U'; // Πάνω
        else if(current->parent->x == current->x - 1)
            moves[--cur] = 'D'; // Κάτω
        else if(current->parent->y == current->y + 1)
            moves[--cur] = 'L'; // Αριστερά
        else if(current->parent->y == current->y - 1)
            moves[--cur] = 'R'; // Δεξιά
        current = current->parent;
    }
    return moves;
}

//Συνάρτηση εύρεσης θέσεων
void find_moves(int n, int** room, int zoomba_x, int zoomba_y, int target_x, int target_y){
    int dx[] = {-1, 1, 0, 0}; // Πιθανές αλλαγές στον άξονα x
    int dy[] = {0, 0, -1, 1}; // Πιθανές αλλαγές στον άξονα y

    node* start = create_node(zoomba_x, zoomba_y, 0, euclidean(zoomba_x, zoomba_y, target_x, target_y), NULL);
    node* goal = create_node(target_x, target_y, 0, 0, NULL);

    node** open_set = (node**)malloc(n * n * sizeof(node*)); // Σύνολο ανοιχτών κόμβων προς εξέταση
    int test_size = 0;

    node** closed_set = (node**)malloc(n * n * sizeof(node*)); // Σύνολο αξιολογημένων κόμβων

    bool **visited = (bool **)malloc(n * sizeof(bool *));
    for(int i = 0; i < n; i++){
        visited[i] = (bool *)calloc(n * n,sizeof(bool));
    }

    open_set[test_size++] = start; // Προσθήκη του αρχικού κόμβου στο σύνολο ανοιχτών κόμβων

    while(test_size > 0){

        node* current = find_min_f(open_set, test_size); // Εύρεση κόμβου με ελάχιστη τιμή f

        closed_set[test_size - 1] = current;
        visited[current->x][current->y] = true;
        
        // Αφαίρεση του τρέχοντος κόμβου από το σύνολο ανοιχτών κόμβων
        int k;
        for (k = 0; k < test_size; ++k) {
            if (open_set[k] == current)
                break;
        }
        open_set[k] = open_set[test_size - 1];
        test_size--;

        // Δημιουργία διαδόχων κόμβων και προσθήκη τους στο σύνολο ανοιχτών κόμβων
        for(int i = 0; i < 4; ++i){

            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if(nx >= 0 && nx < n && ny >= 0 && ny < n && room[nx][ny] == 0 && !visited[nx][ny]){

                node* successor = create_node(nx, ny, current->g + 1, euclidean(nx, ny, target_x, target_y), current);

                if(successor->x == goal->x && successor->y == goal->y){ // Εάν έχει επιτευχθεί ο στόχος
                    char* moves = path(successor); // Ανακατασκευή μονοπατιού
                    printf("%s\n", moves); // Εκτύπωση κινήσεων
                    free(moves);
                    free(successor);
                    return;
                }
                else{
                    int f2 = f(successor);
                    for (k = 0; k < test_size; ++k) {
                        if (open_set[k] && open_set[k]->x == successor->x && open_set[k]->y == successor->y){
                            if(f2 >= f(open_set[k])) {
                                visited[successor->x][successor->y] = true;
                                break;
                            }
                        }
                    }
                    if (!visited[successor->x][successor->y]){
                        open_set[test_size++] = successor;
                        visited[successor->x][successor->y] = true;
                    }
                    else{
                        free(successor);
                    }
                }
            }
        }
    }

    printf("0\n"); // Εάν ο στόχος δεν είναι προσβάσιμος

    // Απελευθέρωση μνήμης
    for (int i = 0; i < n; i++) {
        free(visited[i]);
    }
    free(visited);
}

// Κύρια συνάρτηση
int main(){

    int n;
    if(scanf("%d", &n) != 1){
        return 1;
    }

    if(n <= 0 || n > 10000){
        return 1;
    }

    int zoomba_x, zoomba_y, target_x, target_y;
    if(scanf("%d %d %d %d", &zoomba_x, &zoomba_y, &target_x, &target_y) != 4){
        return 1;
    }
    
    if(zoomba_x < 0 || zoomba_x >= n || zoomba_y < 0 || zoomba_y >= n ||
       target_x < 0 || target_x >= n || target_y < 0 || target_y >= n){
        return 1;
    }

    // Δέσμευση μνήμης για τον πίνακα room
    int **room = (int **)malloc(n * sizeof(int *));
    for(int i = 0; i < n; ++i){
        room[i] = (int *)malloc(n * sizeof(int));
    }

    // Ανάγνωση τιμών για τον πίνακα room
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if (scanf("%1d", &room[i][j]) != 1){
                return 1;
            }
        }
    }

    if(room[zoomba_x][zoomba_y] != 0 || room[target_x][target_y] != 0){
        return 1;
    }

    find_moves(n, room, zoomba_x, zoomba_y, target_x, target_y);

    // Απελευθέρωση μνήμης για τον πίνακα room
    for (int i = 0; i < n; ++i) {
        free(room[i]);
    }
    free(room);

    return 0;
}