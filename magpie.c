#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 256
#define MAX_KEYWORD  120
#define CONTEXT_LENGTH 100

void searchForKeyword(FILE *file2, const char *keyword) {
    char line[MAX_LINE];
    char buffer[MAX_LINE * 2];
    int lineNumber = 0;
    while (fgets(line, sizeof(line), file2) != NULL) {
        lineNumber++;
        char *found = strstr(line, keyword);
        if (found != NULL) {
            int position = found - line;
            int start = (position - CONTEXT_LENGTH < 0) ? 0 : position - CONTEXT_LENGTH;
            int end = position + strlen(keyword) + CONTEXT_LENGTH;
            strncpy(buffer, line + start, end - start);
            buffer[end - start] = '\0';
            printf("Keyword found in line %d, position %d:\n", lineNumber, position);
            printf("Context: %s\n", buffer);
        }
    }
}

int searchforkeyword(FILE *file, const char *keyword) {
char line[MAX_LINE];
int linenum = 0;
int keywordFound  = 0;

printf("\nSearching,\n");
while (fgets(line, sizeof(line), file) !=NULL) {
if (strstr(line, keyword) != NULL) {
linenum++;
if (strstr(line, keyword) != NULL) {
keywordFound = 1;
printf("%d: %s\n", linenum, line);
}
}
}
return keywordFound;
}

void processFile(const char *filename) {
    FILE *file3;
    char ch;
    file3 = fopen(filename, "rb");

    if (file3 == NULL) {
        perror("Error opening file");
        return;
    }

    printf("\nText contents in the binary file:\n");
    while (fread(&ch, 1, 1, file3) == 1) {
        if (ch >= 32 && ch <= 126) {
            putchar(ch);
        }
    }
    printf("\n");
    fclose(file3);
    return;
}

int main(int argc, char *argv[]) {
FILE *file;
char filename[MAX_LINE];
char keyword[MAX_KEYWORD];

if (argc == 3) {
strcpy(filename, argv[1]);
strcpy(keyword, argv[2]);
} else if (argc == 2) {
  strcpy(filename, argv[1]);
  printf("Enter the keyword to search: ");
  scanf("%s", keyword);
} else {
if (argc != 2) {
fprintf(stderr, "Enter a filename and keyword to search for?\n");
}
printf("Enter the filename: ");
scanf("%s", filename);
printf("Enter the keyword to search: ");
scanf("%s", keyword);
}

file = fopen(filename, "r");
if (file == NULL) {
perror("Error opening file");
return 1;
}
int keywordFound = searchforkeyword(file, keyword);
fclose(file);

if (!keywordFound) {
printf("Keyword '%s' not found in the file.\n", keyword);
return 1;
}
FILE *file2;
file2 = fopen(filename, "r");
searchForKeyword(file2, keyword);
fclose(file2);
char choice;
if (argv[1] == NULL) { return 0;
} else {
do {
printf("Display all file contents, do you want to continue? (Y/N): ");
scanf(" %c", &choice);
if (choice == 'y' || choice == 'Y') {
processFile(argv[1]);
break;
} else if (choice == 'n' || choice == 'N') {
printf("Exiting the program.\n");
break;
} else {
printf("Invalid choice. Please enter 'y' or 'n'.\n");
}
} while (1);
return 0;
}
}
