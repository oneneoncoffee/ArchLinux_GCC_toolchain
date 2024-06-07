#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINES 2000
#define MAX_CHARS 100

char text[MAX_LINES][MAX_CHARS];
int numLines = 0;
char currentFilename[MAX_CHARS] = "Untitled";

int visibleLines = 10;
int startLine = 0;

void clearScreen() {
int fileResult = system("clear");
if (fileResult != 0) {
printf("\033[H\033[J");
}
}

void xdelay(unsigned int milliseconds) {
usleep(milliseconds * 1000);
}

void displayDoubleLine() {
    int terminalWidth = 80;
    const char *tputCmd = "tput cols";

    FILE *terminal = popen(tputCmd, "r");
    if (terminal != NULL) {
        fscanf(terminal, "%d", &terminalWidth);
        pclose(terminal);
    }

    for (int i = 0; i < terminalWidth; i++) {
        putchar('_');
    }
    printf("\n");
}

void displayText() {
    printf("Options: I <lineNum> <text>  D <lineNum>  S <filename>  G <lineNum>  J <Down>  K <Up>  Q <Exit>\n");

    displayDoubleLine();

    if (numLines == 0) {
        printf("Text editor is empty.\n");
    } else {
        printf("Total of %d ", numLines);
        printf("Lines:\n");
        for (int i = startLine; i < startLine + visibleLines && i < numLines; i++) {
            printf("%3d | %s\n", i + 1, text[i]);
        }
    }
    displayDoubleLine();
    printf("\nCurrent File: %s\n", currentFilename);
    displayDoubleLine();
    printf("\033[1000D");
    printf("\n> ");
}

void insertLine(int lineNum, const char *newLine) {
    if (numLines >= MAX_LINES) {
        printf("Text editor is full. Cannot insert more lines.\n");
        xdelay(1000);
        return;
    }

    if (lineNum < 1 || lineNum > numLines + 1) {
        printf("Invalid line number. Line must be between 1 and %d.\n", numLines + 1);
        xdelay(1000);
        return;
    }

    // Shift lines down to make space for the new line
    for (int i = numLines; i >= lineNum; i--) {
        strcpy(text[i], text[i - 1]);
    }

    strcpy(text[lineNum - 1], newLine);
    numLines++;
}

void deleteLine(int lineNum) {
    if (lineNum < 1 || lineNum > numLines) {
        printf("Invalid line number. Line must be between 1 and %d.\n", numLines);
        xdelay(1000);
        return;
    }

    // Shift lines up to fill the gap left by the deleted line
    for (int i = lineNum - 1; i < numLines - 1; i++) {
        strcpy(text[i], text[i + 1]);
    }

    numLines--;
}

void saveToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        xdelay(1000);
        return;
    }

    for (int i = 0; i < numLines; i++) {
        fprintf(file, "%s\n", text[i]);
    }

    fclose(file);
    printf("File saved successfully.\n");
    xdelay(1000);
    strncpy(currentFilename, filename, MAX_CHARS - 1);
    currentFilename[MAX_CHARS - 1] = '\0';
}

void loadFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        xdelay(1000);
        return;
    }

    numLines = 0;
    while (fgets(text[numLines], MAX_CHARS, file) != NULL && numLines < MAX_LINES) {
        text[numLines][strlen(text[numLines]) - 1] = '\0';
        numLines++;
    }

    fclose(file);
    strncpy(currentFilename, filename, MAX_CHARS - 1);
    currentFilename[MAX_CHARS - 1] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        loadFromFile(argv[1]);
    }

    char command;
    int lineNum;
    char newLine[MAX_CHARS];
    char filename[MAX_CHARS];

    do {
        clearScreen();
        displayText();
        scanf(" %c", &command);

        switch (command) {
            case 'I':
            case 'i':
                if (scanf("%d %[^\n]", &lineNum, newLine) != 2) {
                    printf("Invalid input. Please provide line number and text.\n");
                    xdelay(1000);
                    while (getchar() != '\n');
                } else {
                    insertLine(lineNum, newLine);
                }
                break;

            case 'D':
            case 'd':
                if (scanf("%d", &lineNum) != 1) {
                    printf("Invalid input. Please provide line number.\n");
                    xdelay(1000);
                    while (getchar() != '\n');
                } else {
                    deleteLine(lineNum);
                }
                break;

            case 'S':
            case 's':
                printf("Enter filename: ");
                scanf("%s", filename);
                saveToFile(filename);
                break;

            case 'G':
            case 'g':
                printf("Enter line number to go to: ");
                if (scanf("%d", &lineNum) != 1) {
                    printf("Invalid input. Please provide a valid line number.\n");
                    xdelay(1000);
                    while (getchar() != '\n');
                } else if (lineNum < 1 || lineNum > numLines) {
                    printf("Invalid line number. Line must be between 1 and %d.\n", numLines);
                    xdelay(1000);
                } else {
                    startLine = lineNum - 1;
                }
                break;

            case 'J':
            case 'j':
                if (startLine + visibleLines < numLines) {
                    startLine++;
                }
                break;

            case 'K':
            case 'k':
                if (startLine > 0) {
                    startLine--;
                }
                break;

            case 'Q':
            case 'q':
                printf("Exiting text editor.\n");
                break;

            default:
                printf("Invalid command. Please try again.\n");
                xdelay(1000);
                while (getchar() != '\n');
        }

    } while (command != 'Q' && command != 'q');

    return 0;
}
