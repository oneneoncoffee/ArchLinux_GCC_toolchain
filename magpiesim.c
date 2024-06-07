#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // For usleep()
#include <string.h>

#define WIDTH 20
#define HEIGHT 20
#define MAX_GAME_TIME 200
#define MAX_AGGRESSION 100
#define AGGRESSION_THRESHOLD 20

// Function to clear the screen
void clearScreen() {
    printf("\033[2J\033[1;1H"); // ANSI escape sequence to clear screen
}


// Function to print a character with a delay
void printWithDelay(char c, unsigned int microseconds) {
    putchar(c);
    fflush(stdout); // Flush buffer to ensure immediate output
    usleep(microseconds); // Delay in microseconds
}

// Function to print a string character by character with a delay
void printStringWithDelay(const char* str, unsigned int microseconds) {
    while (*str) {
        printWithDelay(*str++, microseconds);
    }
}


void displayStats(int userCells, int computerCells, int userAgg, int compAgg);
// Function prototype for countAliveCells
int countAliveCells(int player);

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;
    int isAlive;
    int color;
    int player; // 0 for user, 1 for computer
} Cell;

Cell board[HEIGHT][WIDTH];
int userColor, computerColor;
int userAggression, computerAggression;
time_t startTime;

void initBoard() {
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            board[i][j].pos.x = j;
            board[i][j].pos.y = i;
            board[i][j].isAlive = rand() % 2; // Randomly alive or dead
            board[i][j].player = rand() % 2; // Random player (0 or 1)
            board[i][j].color = board[i][j].player == 0 ? userColor : computerColor;
        }
    }
}

void displayBoard(int elapsedTime) {
    printf("\033[H"); // Move cursor to the top left corner
    printf("Game Time: %d seconds (Max Time: %d seconds)\n", elapsedTime, MAX_GAME_TIME);
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(board[i][j].isAlive) {
                if(board[i][j].player == 0) {
                    printf("\033[%dm", userColor);
                } else {
                    printf("\033[%dm", computerColor);
                }
                printf("O ");
                printf("\033[0m"); // Reset color
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

int countNeighbors(int x, int y, int player) {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            int nx = (x + i + WIDTH) % WIDTH;
            int ny = (y + j + HEIGHT) % HEIGHT;
            if(board[ny][nx].isAlive && board[ny][nx].player == player) {
                count++;
            }
        }
    }
    return count;
}

void aggressiveUpdate(int x, int y, int aggressionLevel) {
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            int nx = (x + i + WIDTH) % WIDTH;
            int ny = (y + j + HEIGHT) % HEIGHT;
            if(rand() % 100 < aggressionLevel) { // Aggression based on probability
                if(board[ny][nx].isAlive && board[ny][nx].player != board[y][x].player) {
                    board[ny][nx].isAlive = 0; // Kill the other player's cell
                }
            }
        }
    }
}

void updateBoard() {
    Cell newBoard[HEIGHT][WIDTH];
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            int aliveNeighborsUser = countNeighbors(j, i, 0);
            int aliveNeighborsComp = countNeighbors(j, i, 1);
            newBoard[i][j] = board[i][j];

            if(board[i][j].isAlive) {
                if(aliveNeighborsUser + aliveNeighborsComp < 2 || aliveNeighborsUser + aliveNeighborsComp > 3) {
                    newBoard[i][j].isAlive = 0; // Dies
                } else if(board[i][j].player == 1) {
                    aggressiveUpdate(j, i, computerAggression); // Computer attacks neighboring user cells
                } else if(board[i][j].player == 0) {
                    aggressiveUpdate(j, i, userAggression); // User attacks neighboring computer cells
                }
            } else {
                if(aliveNeighborsUser + aliveNeighborsComp == 3) {
                    newBoard[i][j].isAlive = 1; // Becomes alive
                    newBoard[i][j].player = (aliveNeighborsUser > aliveNeighborsComp) ? 0 : 1; // Determine new owner
                    newBoard[i][j].color = newBoard[i][j].player == 0 ? userColor : computerColor;
                }
            }
        }
    }
    
    // Check if aggression needs to be increased due to low cell count
    int userCells = countAliveCells(0);
    int computerCells = countAliveCells(1);
    if(userCells < AGGRESSION_THRESHOLD) {
        userAggression += rand() % 10 + 1; // Increase aggression randomly
        userAggression = userAggression > MAX_AGGRESSION ? MAX_AGGRESSION : userAggression;
    }
    if(computerCells < AGGRESSION_THRESHOLD) {
        computerAggression += rand() % 10 + 1; // Increase aggression randomly
        computerAggression = computerAggression > MAX_AGGRESSION ? MAX_AGGRESSION : computerAggression;
    }
    
    // Update the board with the new values
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            board[i][j] = newBoard[i][j];
        }
    }
}

void selectColorsAndAggression() {
    printf("Select color for user,\n(31 for red, 32 for green, 33 for yellow, 34 for blue): ");
    scanf("%d", &userColor);
    printf("Select color for computer,\n(31 for red, 32 for green, 33 for yellow, 34 for blue): ");
    scanf("%d", &computerColor);
    printf("Select aggression level for user (0-%d): ", MAX_AGGRESSION);
    scanf("%d", &userAggression);
    userAggression = userAggression > MAX_AGGRESSION ? MAX_AGGRESSION : userAggression;
    //   printf("Select aggression level for computer (0-%d): ", MAX_AGGRESSION);
    //   scanf("%d", &computerAggression);
    srand(time(NULL));
    int computerAggression = rand() % 100;
    computerAggression = computerAggression > MAX_AGGRESSION ? MAX_AGGRESSION : computerAggression;
    printf("Computer aggression level: %d\n", computerAggression);
}
int countAliveCells(int player) {;
    int count = 0;
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            if(board[i][j].isAlive && board[i][j].player == player) {
                count++;
            }
        }
    }
    return count;
}

void displayStats(int userCells, int computerCells, int userAgg, int compAgg) {
    int totalCells = userCells + computerCells;
    double userPercentage = (totalCells == 0) ? 0 : (double)userCells / totalCells * 100;
    double computerPercentage = (totalCells == 0) ? 0 : (double)computerCells / totalCells * 100;
    printf("\033[%dmUser cells: %d (%.2f%%)\033[0m \033[37mVS.\033[0m \033[%dmComputer cells: %d (%.2f%%)\033[0m\n", 
           userColor, userCells, userPercentage, computerColor, computerCells, computerPercentage);
    printf("\033[33mUser Aggression: %d, Computer Aggression: %d\033[0m\n", userAgg, compAgg);
}

int main() {
   char input[20];
   clearScreen();
    // Print "SIM" character by character with a delay of 100000 microseconds (0.1 seconds)
    // Print "S" in red
    printf("\033[1;31m"); // Set text color to bright red
    printStringWithDelay("S", 300000);
    // Print "I" in green
    printf("\033[1;32m"); // Set text color to bright green
    printStringWithDelay("I", 400000);
    // Print "M" in blue
    printf("\033[1;34m"); // Set text color to bright blue
    printStringWithDelay("M", 500000);
   // Reset text color to default
    printf("\033[0m"); // Reset text color
   // Print the remaining text with a delay of 50000 microseconds (0.05 seconds)
    printStringWithDelay(" - Simulated Virus Fighting league of legends welcome you.....\n", 60000+19);
    srand(time(NULL));
    selectColorsAndAggression();
    while(1) {
    initBoard();
    startTime = time(NULL);
    int userCells = countAliveCells(0);
    printf("\033[2J"); // Clear the screen initially
    while(1) {
        time_t currentTime = time(NULL);
        int elapsedTime = (int)difftime(currentTime, startTime);

        displayBoard(elapsedTime);
        int userCells = countAliveCells(0);
        int computerCells = countAliveCells(1);

        // Move cursor to the line below the grid for stats
        printf("\033[%d;1H", HEIGHT + 3);
        displayStats(userCells, computerCells, userAggression, computerAggression);

        updateBoard();
        usleep(500000); // Pause for half a second

        if(userCells == 0 || computerCells == 0 || elapsedTime > MAX_GAME_TIME) {
            // Move cursor to the line below the grid for the game over message
            printf("\033[%d;1H", HEIGHT + 5);
            if(elapsedTime > MAX_GAME_TIME) {
                printf("Game ends in a draw!\n");
  // Seed the random number generator
    srand(time(NULL));
    
    // Generate a random amount of money as the prize (between 500 and 2000)
    int prizeMoney = rand() % 1501 + 500; // Generates a random number between 500 and 2000
    
    // Simulate the game result (0 for lose, 1 for win, 2 for draw)
    int gameResult = rand() % 3; // Generates either 0 (lose), 1 (win), or 2 (draw)
    
    if (gameResult == 1) {
        // Display "You Win" message and the amount of money won
        printf("\033[1;32mYOU WIN!\033[0m\n");
        printf("Congratulations! You've won %d credits!\n", prizeMoney);
    } else if (gameResult == 0) {
        // Display "Game Over" message and the amount of money lost
        printf("\033[1;31mGAME OVER\033[0m\n");
        printf("Better luck next time! No prize credits buster..\n");
    } else {
        // Display "It's a Draw" message and still give a random chance of prize money
        printf("\033[1;33mIT'S A DRAW\033[0m\n");
        printf("You still have a chance to win! You've won %d credits!\n", prizeMoney);
    }

            } else if(userCells > computerCells) {
                printf("User wins!\n");
    // Seed the random number generator
    srand(time(NULL));
    
    // Generate a random amount of money as the prize (between 500 and 2000)
    int prizeMoney = rand() % 1501 + 500; // Generates a random number between 500 and 2000
    
    // Simulate a win (you can replace this with your actual win condition)
    int win = rand() % 2; // Generates either 0 (lose) or 1 (win)
    
    if (win) {
        // Display "You Win" message and the amount of money won
        printf("\033[1;32mYOU WIN!\033[0m\n");
        printf("Congratulations! You've won %d credits!\n", prizeMoney);
    } else {
        // Display "Game Over" message and the amount of money lost
        printf("\033[1;31mGAME OVER\033[0m\n");
        printf("Better luck next time!\n");
    }

            } else if(computerCells > userCells) {
                printf("Computer wins!\n");
    // Seed the random number generator
    srand(time(NULL));
    // Generate a random amount of money lost (between 100 and 1000)
    int lostMoney = rand() % 901 + 100; // Generates a random number between 100 and 1000
    // Display "Game Over" message and the amount of money lost
    printf("\033[1;31mGAME OVER\033[0m\n");
    printf("You've lost $%d in your latest bet. Better luck next time!\n", lostMoney);
            } else {
                printf("It's a tie!\n No credit prize at this time sorry loser.");
            }
            break;
        }
    }
      printf("Press enter to play or type 'exit game' to quit: ");
        fgets(input, sizeof(input), stdin);
        
        // Remove newline character if present
        if (input[strlen(input) - 1] == '\n') {
            input[strlen(input) - 1] = '\0';
        }
        
        // Check if the user wants to exit
        if (strcmp(input, "exit game") == 0) {
            printf("Exiting the game...\n");
            break;
        }
   }
    return 0;
}
