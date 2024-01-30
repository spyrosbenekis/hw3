#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ROOM_DIMENSION 10000

typedef struct {
    int x, y; // Θέση (x, y)
} Position;

typedef struct {
    Position pos;
    int steps; // Αριθμός βημάτων από την αρχική θέση
} QueueNode;

QueueNode queue[MAX_ROOM_DIMENSION * MAX_ROOM_DIMENSION];
bool visited[MAX_ROOM_DIMENSION][MAX_ROOM_DIMENSION];
char room[MAX_ROOM_DIMENSION][MAX_ROOM_DIMENSION];
int front = -1, rear = -1;

// Έλεγχος εάν η θέση (x, y) είναι εντός των ορίων του δωματίου
bool isValid(int x, int y, int n) {
    return (x >= 0 && x < n && y >= 0 && y < n && room[x][y] == '0' && !visited[x][y]);
}

// Προσθήκη θέσης στην ουρά
void enqueue(int x, int y, int steps) {
    if (rear == MAX_ROOM_DIMENSION * MAX_ROOM_DIMENSION - 1) {
        printf("Queue overflow\n");
        exit(1);
    }
    if (front == -1)
        front = 0;
    rear++;
    queue[rear].pos.x = x;
    queue[rear].pos.y = y;
    queue[rear].steps = steps;
}

// Αφαίρεση θέσης από την ουρά
QueueNode dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue underflow\n");
        exit(1);
    }
    QueueNode item = queue[front];
    front++;
    return item;
}

// Παγκόσμιος πίνακας για τις κινήσεις Zoomba
char moves[MAX_ROOM_DIMENSION * MAX_ROOM_DIMENSION];
int counter = 0; // Αρχικόποιηση της μεταβλητής καταμέτρησης κινήσεων

// Αναζήτηση βέλτιστης διαδρομής με χρήση BFS
void findShortestPath(int n, int startX, int startY, int targetX, int targetY) {
    int dx[] = {-1, 1, 0, 0}; // Πιθανές μετακινήσεις σε x
    int dy[] = {0, 0, -1, 1}; // Πιθανές μετακινήσεις σε y

    // Εισάγετε την αρχική θέση στην ουρά
    enqueue(startX, startY, 0);
    visited[startX][startY] = true;

    while (front != -1) {
        QueueNode current = dequeue();
        int x = current.pos.x;
        int y = current.pos.y;
        int steps = current.steps;

        // Έλεγχος αν έχουμε φτάσει στον στόχο
        if (x == targetX && y == targetY) {
            printf("Shortest path found in %d steps\n", steps);

            // Εκτύπωση των κινήσεων Zoomba
            printf("Moves: ");
            for (int i = 0; i < steps; i++) {
                printf("%c", moves[i]);
            }
            printf("\n");
            return;
        }

        // Εξερεύνηση των τετραγώνων γύρω από την τρέχουσα θέση
        for (int i = 0; i < 4; i++) {
            int nextX = x + dx[i];
            int nextY = y + dy[i];

            // Έλεγχος εάν η επόμενη θέση είναι έγκυρη
            if (isValid(nextX, nextY, n)) {
                // Εισάγετε την επόμενη θέση στην ουρά
                enqueue(nextX, nextY, steps + 1);
                visited[nextX][nextY] = true;

                if(nextY==y){
                    if(nextX>x)
                        moves[counter++] = 'R';
                    else   
                        moves[counter++] = 'L';
                } 
                else{
                    if(nextY>y)
                        moves[counter++] = 'D';
                    else   
                        moves[counter++] = 'U';
                }
            }
        }
    }

    // Αν δεν βρέθηκε διαδρομή
    printf("No path found\n");
}

int main() {
    int n, startX, startY, targetX, targetY;

    // Διάβασε τις διαστάσεις του δωματίου
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_ROOM_DIMENSION) {
        fprintf(stderr, "Invalid room dimension input.\n");
        return 1;
    }

    // Διάβασε τις αρχικές θέσεις Zoomba και τον στόχο
    if (scanf("%d %d %d %d", &startX, &startY, &targetX, &targetY) != 4) {
        fprintf(stderr, "Invalid Zoomba position or target input.\n");
        return 1;
    }

    // Διάβασε τον χάρτη του δωματίου
    for (int i = 0; i < n; i++) {
        if (scanf("%s", room[i]) != 1) {
            fprintf(stderr, "Invalid room layout input.\n");
            return 1;
        }
    }

    // Καθαρισμός του πίνακα επισκεψιμότητας
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i][j] = false;
        }
    }

    // Εύρεση της βέλτιστης διαδρομής με χρήση BFS
    findShortestPath(n, startX, startY, targetX, targetY);

    return 0;
}