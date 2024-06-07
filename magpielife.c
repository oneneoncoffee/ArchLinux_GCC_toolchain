#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>

#define MIN_WIDTH 20
#define MIN_HEIGHT 10

void getTerminalSize(int *width, int *height);
void initialize(int **grid, int width, int height);
void display(int **grid, int width, int height, int redCount, int blueCount);
void update(int **grid, int **newGrid, int width, int height);
int countNeighbors(int **grid, int width, int height, int x, int y);
void copyGrid(int **source, int **destination, int width, int height);
char randomChar();

int main() {
    int width, height;
    getTerminalSize(&width, &height);

    // Ensure minimum size
    if (width < MIN_WIDTH) width = MIN_WIDTH;
    if (height < MIN_HEIGHT) height = MIN_HEIGHT;

    int **grid = malloc(height * sizeof(int *));
    int **newGrid = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(int));
        newGrid[i] = malloc(width * sizeof(int));
    }

    srand(time(NULL));
    initialize(grid, width, height);

    int redCount, blueCount;

    while (1) {
        redCount = 0;
        blueCount = 0;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (grid[i][j] == 1) redCount++;
                if (grid[i][j] == 2) blueCount++;
            }
        }
        display(grid, width, height, redCount, blueCount);
        update(grid, newGrid, width, height);
        copyGrid(newGrid, grid, width, height);
       // usleep(100000); // Sleep for 100 milliseconds
    }

    for (int i = 0; i < height; i++) {
        free(grid[i]);
        free(newGrid[i]);
    }
    free(grid);
    free(newGrid);

    return 0;
}

void getTerminalSize(int *width, int *height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    *width = w.ws_col;
    *height = w.ws_row;
}

void initialize(int **grid, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = rand() % 3; // 0 (dead), 1 (red), or 2 (blue)
        }
    }
}

void display(int **grid, int width, int height, int redCount, int blueCount) {
    printf("\033[H"); // Move cursor to the top-left corner
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char ch = randomChar();
            if (grid[i][j] == 1) {
                printf("\033[31m%c\033[0m", ch); // Red cell
            } else if (grid[i][j] == 2) {
                printf("\033[34m%c\033[0m", ch); // Blue cell
            } else {
                printf(" "); // Dead cell
            }
        }
        printf("\n");
    }

    // Display statistics
    //printf("Red cells: %d   Blue cells: %d\n", redCount, blueCount);
    fflush(stdout);
}

void update(int **grid, int **newGrid, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int neighbors = countNeighbors(grid, width, height, i, j);
            if (grid[i][j] != 0 && (neighbors == 2 || neighbors == 3)) {
                newGrid[i][j] = grid[i][j]; // Keep the same color
            } else if (grid[i][j] == 0 && neighbors == 3) {
                // New cell becomes the majority color of neighbors
                int redCount = 0, blueCount = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int nx = i + di, ny = j + dj;
                        if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                            if (grid[nx][ny] == 1) redCount++;
                            if (grid[nx][ny] == 2) blueCount++;
                        }
                    }
                }
                newGrid[i][j] = (redCount > blueCount) ? 1 : 2;
            } else if (grid[i][j] != 0) {
                // Rule for red and blue cells to "eat" each other
                int redCount = 0, blueCount = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        if (di == 0 && dj == 0) continue;
                        int nx = i + di, ny = j + dj;
                        if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                            if (grid[nx][ny] == 1) redCount++;
                            if (grid[nx][ny] == 2) blueCount++;
                        }
                    }
                }
                if ((grid[i][j] == 1 && blueCount > 0) || (grid[i][j] == 2 && redCount > 0)) {
                    newGrid[i][j] = 0; // Cell dies
                } else {
                    newGrid[i][j] = grid[i][j]; // Cell remains
                }
            } else {
                newGrid[i][j] = 0; // Dead cell
            }
        }
    }
}

int countNeighbors(int **grid, int width, int height, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
                if (grid[nx][ny] != 0) count++;
            }
        }
    }
    return count;
}
void copyGrid(int **source, int **destination, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            destination[i][j] = source[i][j];
        }
    }
}

char randomChar() {
    const char *chars = "|+-_./=[]{}()*#@%";
    return chars[rand() % strlen(chars)];
}
