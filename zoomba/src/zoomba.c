#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROWS 10000
#define MAX_COLS 10000

// Structure to represent a node in the search
typedef struct Node {
    int x, y;           // Coordinates of the node
    int g, h;           // Cost and heuristic values
    struct Node* parent; // Pointer to the parent node
} Node;

// Heuristic function (Manhattan distance)
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Function to create a new node
Node* create_node(int x, int y, int g, int h, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->parent = parent;
    return node;
}

// Function to free memory allocated for a node
void destroy_node(Node* node) {
    free(node);
}

// Function to find the node with the minimum f value in the open set
Node* find_min_f(Node** open_set, int open_set_size) {
    Node* min_node = open_set[0];
    for (int i = 1; i < open_set_size; ++i) {
        if ((open_set[i]->g + open_set[i]->h) < (min_node->g + min_node->h)) {
            min_node = open_set[i];
        }
    }
    return min_node;
}

// Function to reconstruct the path from the goal node to the start node
char* reconstruct_path(Node* current) {
    char* moves = (char*)malloc(sizeof(char) * (current->g + 1));
    if (moves == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int index = current->g;
    moves[index] = '\0';

    while (current->parent != NULL) {
        if (current->parent->x == current->x + 1) {
            moves[--index] = 'U'; // Up
        } else if (current->parent->x == current->x - 1) {
            moves[--index] = 'D'; // Down
        } else if (current->parent->y == current->y + 1) {
            moves[--index] = 'L'; // Left
        } else if (current->parent->y == current->y - 1) {
            moves[--index] = 'R'; // Right
        }
        current = current->parent;
    }

    return moves;
}

// Function to find possible moves for Zoomba
void find_moves(int n, int** room, int zoomba_x, int zoomba_y, int target_x, int target_y) {
    int dx[] = {-1, 1, 0, 0}; // Possible changes in x direction
    int dy[] = {0, 0, -1, 1}; // Possible changes in y direction

    Node* start = create_node(zoomba_x, zoomba_y, 0, heuristic(zoomba_x, zoomba_y, target_x, target_y), NULL);
    Node* goal = create_node(target_x, target_y, 0, 0, NULL);

    Node** open_set = (Node**)malloc(MAX_ROWS * MAX_COLS * sizeof(Node*)); // Open set of nodes to be evaluated
    if (open_set == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    int open_set_size = 0;

    bool **closed_set = (bool **)malloc(n * sizeof(bool *));
    if (closed_set == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < n; ++i) {
        closed_set[i] = (bool *)malloc(n * sizeof(bool));
        if (closed_set[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        for (int j = 0; j < n; ++j) {
            closed_set[i][j] = false;
        }
    }

    open_set[open_set_size++] = start; // Add start node to open set

    while (open_set_size > 0) {
        Node* current = find_min_f(open_set, open_set_size); // Find node with minimum f value
        if (current->x == goal->x && current->y == goal->y) { // If goal is reached
            char* moves = reconstruct_path(current); // Reconstruct path
            printf("%s\n", moves); // Print moves
            free(moves);
            destroy_node(start);
            destroy_node(goal);
            for (int i = 0; i < open_set_size; ++i) {
                destroy_node(open_set[i]);
            }
            for (int i = 0; i < n; ++i) {
                free(closed_set[i]);
            }
            free(closed_set);
            free(open_set);
            return;
        }

        // Generate successors and add them to open set
        for (int i = 0; i < 4; ++i) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];

            if (nx >= 0 && nx < n && ny >= 0 && ny < n && room[nx][ny] == 0 && !closed_set[nx][ny]) {
                Node* successor = create_node(nx, ny, current->g + 1, heuristic(nx, ny, target_x, target_y), current);
                open_set[open_set_size++] = successor;
                closed_set[nx][ny] = true;
            }
        }

        // Remove current node from open set
        for (int i = 0; i < open_set_size; ++i) {
            if (open_set[i] == current) {
                open_set[i] = open_set[open_set_size - 1];
                open_set_size--;
                break;
            }
        }
        closed_set[current->x][current->y] = true; // Mark current node as evaluated
    }

    printf("impossible\n"); // If goal is not reachable
    destroy_node(start);
    destroy_node(goal);
    for (int i = 0; i < open_set_size; ++i) {
        destroy_node(open_set[i]);
    }
    for (int i = 0; i < n; ++i) {
        free(closed_set[i]);
    }
    free(closed_set);
    free(open_set);
}

// Main function
int main() {
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

    // Δυναμική δέσμευση μνήμης για τον πίνακα room
    int **room = (int **)malloc(n * sizeof(int *));
    if (room == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        room[i] = (int *)malloc(n * sizeof(int));
        if (room[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
    }

    // Διάβασμα τιμών για τον πίνακα room
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%1d", &room[i][j]) != 1) {
                fprintf(stderr, "Failed to read input for room[%d][%d]\n", i, j);
                return 1;
            }
        }
    }

    find_moves(n, room, zoomba_x, zoomba_y, target_x, target_y);

    // Αποδέσμευση μνήμης για τον πίνακα room
    for (int i = 0; i < n; ++i) {
        free(room[i]);
    }
    free(room);

    return 0;
}