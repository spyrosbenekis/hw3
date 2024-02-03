#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 10
#define MAX_COLS 10

// Δήλωση των συναρτήσεων
void find_moves(int n, int room[MAX_ROWS][MAX_COLS], int zoomba_x, int zoomba_y, int target_x, int target_y);

int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Failed to read input for n\n");
        return 1;
    }

    int zoomba_x, zoomba_y, target_x, target_y;
    if (scanf("%d %d %d %d", &zoomba_x, &zoomba_y, &target_x, &target_y) != 4) {
        fprintf(stderr, "Failed to read input for zoomba and target positions\n");
        return 1; // or handle the error appropriately
    }

    int room[MAX_ROWS][MAX_COLS]; // Δημιουργούμε έναν πίνακα για το δωμάτιο

    // Διαβάζουμε τα δεδομένα του δωματίου
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (scanf("%1d", &room[i][j]) != 1) {
                fprintf(stderr, "Failed to read input for room[%d][%d]\n", i, j);
                return 1; // or handle the error appropriately
            }
        }
    }

    // Καλούμε τη συνάρτηση για την εύρεση των κινήσεων
    find_moves(n, room, zoomba_x, zoomba_y, target_x, target_y);

    return 0;
}

// Υλοποίηση της συνάρτησης
void find_moves(int n, int room[MAX_ROWS][MAX_COLS], int zoomba_x, int zoomba_y, int target_x, int target_y) {
    // Προσδιορίζουμε τις κατευθύνσεις που μπορεί να κινηθεί η zoomba
    int dx[] = {-1, 1, 0, 0}; // Αριστερά, δεξιά, πάνω, κάτω
    int dy[] = {0, 0, -1, 1};

    // Πίνακας για να αποθηκεύσουμε τις κινήσεις
    char *moves[] = {"U", "D", "L", "R"};

    // Επιλέγουμε την καλύτερη κίνηση που μπορεί να κάνει η zoomba μέχρι να φτάσει στον στόχο
    while (zoomba_x != target_x || zoomba_y != target_y) {
        int best_move = -1;
        int min_distance = n * n; // Αρχικοποιούμε την ελάχιστη απόσταση με το μέγιστο δυνατό αριθμό

        for (int i = 0; i < 4; i++) {
            int nx = zoomba_x + dx[i];
            int ny = zoomba_y + dy[i];

            // Ελέγχουμε αν η κίνηση είναι εντός των ορίων του δωματίου και αν δεν είναι τοίχος
            if (nx >= 0 && nx < n && ny >= 0 && ny < n && room[nx][ny] == 0) {
                // Υπολογίζουμε την απόσταση από τον στόχο
                int distance = abs(nx - target_x) + abs(ny - target_y);

                // Αν η νέα απόσταση είναι μικρότερη από την τρέχουσα ελάχιστη, ενημερώνουμε την κίνηση και την ελάχιστη απόσταση
                if (distance < min_distance) {
                    min_distance = distance;
                    best_move = i;
                }
            }
        }

        // Κάνουμε την καλύτερη δυνατή κίνηση
        if (best_move != -1) {
            zoomba_x += dx[best_move];
            zoomba_y += dy[best_move];
            printf("%s", moves[best_move]);
        }
    }
    printf("\n");
}