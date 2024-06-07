#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#define PAGE_SIZE 20
#define LINE_WIDTH 20
#define BUFFER_SIZE 1024

static const unsigned long crc32_table[256] = {
    0x00000000UL, 0x77073096UL, 0xee0e612cUL, 0x990951baUL, 0x076dc419UL, 0x706af48fUL, 0xe963a535UL, 0x9e6495a3UL,
    0x0edb8832UL, 0x79dcb8a4UL, 0xe0d5e91eUL, 0x97d2d988UL, 0x09b64c2bUL, 0x7eb17cbdUL, 0xe7b82d07UL, 0x90bf1d91UL,
    0x1db71064UL, 0x6ab020f2UL, 0xf3b97148UL, 0x84be41deUL, 0x1adad47dUL, 0x6ddde4ebUL, 0xf4d4b551UL, 0x83d385c7UL,
    0x136c9856UL, 0x646ba8c0UL, 0xfd62f97aUL, 0x8a65c9ecUL, 0x14015c4fUL, 0x63066cd9UL, 0xfa0f3d63UL, 0x8d080df5UL,
    0x3b6e20c8UL, 0x4c69105eUL, 0xd56041e4UL, 0xa2677172UL, 0x3c03e4d1UL, 0x4b04d447UL, 0xd20d85fdUL, 0xa50ab56bUL,
    0x35b5a8faUL, 0x42b2986cUL, 0xdbbbc9d6UL, 0xacbcf940UL, 0x32d86ce3UL, 0x45df5c75UL, 0xdcd60dcfUL, 0xabd13d59UL,
    0x26d930acUL, 0x51de003aUL, 0xc8d75180UL, 0xbfd06116UL, 0x21b4f4b5UL, 0x56b3c423UL, 0xcfba9599UL, 0xb8bda50fUL,
    0x2802b89eUL, 0x5f058808UL, 0xc60cd9b2UL, 0xb10be924UL, 0x2f6f7c87UL, 0x58684c11UL, 0xc1611dabUL, 0xb6662d3dUL,
    0x76dc4190UL, 0x01db7106UL, 0x98d220bcUL, 0xefd5102aUL, 0x71b18589UL, 0x06b6b51fUL, 0x9fbfe4a5UL, 0xe8b8d433UL,
    0x7807c9a2UL, 0x0f00f934UL, 0x9609a88eUL, 0xe10e9818UL, 0x7f6a0dbbUL, 0x086d3d2dUL, 0x91646c97UL, 0xe6635c01UL,
    0x6b6b51f4UL, 0x1c6c6162UL, 0x856530d8UL, 0xf262004eUL, 0x6c0695edUL, 0x1b01a57bUL, 0x8208f4c1UL, 0xf50fc457UL,
    0x65b0d9c6UL, 0x12b7e950UL, 0x8bbeb8eaUL, 0xfcb9887cUL, 0x62dd1ddfUL, 0x15da2d49UL, 0x8cd37cf3UL, 0xfbd44c65UL,
    0x4db26158UL, 0x3ab551ceUL, 0xa3bc0074UL, 0xd4bb30e2UL, 0x4adfa541UL, 0x3dd895d7UL, 0xa4d1c46dUL, 0xd3d6f4fbUL,
    0x4369e96aUL, 0x346ed9fcUL, 0xad678846UL, 0xda60b8d0UL, 0x44042d73UL, 0x33031de5UL, 0xaa0a4c5fUL, 0xdd0d7cc9UL,
    0x5005713cUL, 0x270241aaUL, 0xbe0b1010UL, 0xc90c2086UL, 0x5768b525UL, 0x206f85b3UL, 0xb966d409UL, 0xce61e49fUL,
    0x5edef90eUL, 0x29d9c998UL, 0xb0d09822UL, 0xc7d7a8b4UL, 0x59b33d17UL, 0x2eb40d81UL, 0xb7bd5c3bUL, 0xc0ba6cadUL,
    0xedb88320UL, 0x9abfb3b6UL, 0x03b6e20cUL, 0x74b1d29aUL, 0xead54739UL, 0x9dd277afUL, 0x04db2615UL, 0x73dc1683UL,
    0xe3630b12UL, 0x94643b84UL, 0x0d6d6a3eUL, 0x7a6a5aa8UL, 0xe40ecf0bUL, 0x9309ff9dUL, 0x0a00ae27UL, 0x7d079eb1UL,
    0xf00f9344UL, 0x8708a3d2UL, 0x1e01f268UL, 0x6906c2feUL, 0xf762575dUL, 0x806567cbUL, 0x196c3671UL, 0x6e6b06e7UL,
    0xfed41b76UL, 0x89d32be0UL, 0x10da7a5aUL, 0x67dd4accUL, 0xf9b9df6fUL, 0x8ebeeff9UL, 0x17b7be43UL, 0x60b08ed5UL,
    0xd6d6a3e8UL, 0xa1d1937eUL, 0x38d8c2c4UL, 0x4fdff252UL, 0xd1bb67f1UL, 0xa6bc5767UL, 0x3fb506ddUL, 0x48b2364bUL,
    0xd80d2bdaUL, 0xaf0a1b4cUL, 0x36034af6UL, 0x41047a60UL, 0xdf60efc3UL, 0xa867df55UL, 0x316e8eefUL, 0x4669be79UL,
    0xcb61b38cUL, 0xbc66831aUL, 0x256fd2a0UL, 0x5268e236UL, 0xcc0c7795UL, 0xbb0b4703UL, 0x220216b9UL, 0x5505262fUL,
    0xc5ba3bbeUL, 0xb2bd0b28UL, 0x2bb45a92UL, 0x5cb36a04UL, 0xc2d7ffa7UL, 0xb5d0cf31UL, 0x2cd99e8bUL, 0x5bdeae1dUL,
    0x9b64c2b0UL, 0xec63f226UL, 0x756aa39cUL, 0x026d930aUL, 0x9c0906a9UL, 0xeb0e363fUL, 0x72076785UL, 0x05005713UL,
    0x95bf4a82UL, 0xe2b87a14UL, 0x7bb12baeUL, 0x0cb61b38UL, 0x92d28e9bUL, 0xe5d5be0dUL, 0x7cdcefb7UL, 0x0bdbdf21UL,
    0x86d3d2d4UL, 0xf1d4e242UL, 0x68ddb3f8UL, 0x1fda836eUL, 0x81be16cdUL, 0xf6b9265bUL, 0x6fb077e1UL, 0x18b74777UL,
    0x88085ae6UL, 0xff0f6a70UL, 0x66063bcaUL, 0x11010b5cUL, 0x8f659effUL, 0xf862ae69UL, 0x616bffd3UL, 0x166ccf45UL,
    0xa00ae278UL, 0xd70dd2eeUL, 0x4e048354UL, 0x3903b3c2UL, 0xa7672661UL, 0xd06016f7UL, 0x4969474dUL, 0x3e6e77dbUL,
    0xaed16a4aUL, 0xd9d65adcUL, 0x40df0b66UL, 0x37d83bf0UL, 0xa9bcae53UL, 0xdebb9ec5UL, 0x47b2cf7fUL, 0x30b5ffe9UL,
    0xbdbdf21cUL, 0xcabac28aUL, 0x53b39330UL, 0x24b4a3a6UL, 0xbad03605UL, 0xcdd70693UL, 0x54de5729UL, 0x23d967bfUL,
    0xb3667a2eUL, 0xc4614ab8UL, 0x5d681b02UL, 0x2a6f2b94UL, 0xb40bbe37UL, 0xc30c8ea1UL, 0x5a05df1bUL, 0x2d02ef8dUL
};

unsigned long calculateCRC(const char *filename) {
    FILE *file4 = fopen(filename, "rb");
    if (!file4) {
        perror("Error opening file");
        return 0;
    }

    unsigned long crc = 0xffffffffUL;
    int c;
    while ((c = fgetc(file4)) != EOF) {
        crc = (crc >> 8) ^ crc32_table[(crc & 0xff) ^ c];
    }

    fclose(file4);
    return crc ^ 0xffffffffUL;
}
void displayFileInformation(const char *filename) {
    struct stat fileStat;
    if (stat(filename, &fileStat) < 0) {
        perror("Error");
        return;
    }
    printf("File Information for '%s':\n", filename);
    printf("Last modified: %s", ctime(&fileStat.st_mtime));
    bool isHidden = (filename[0] == '.');
    double fileSize = (double)fileStat.st_size;
    const char *units[] = {"bytes", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    while (fileSize >= 1024 && unitIndex < 4) {
        fileSize /= 1024;
        unitIndex++;
    }
    printf("File size: %.2f %s\n", fileSize, units[unitIndex]);
    printf("File permissions: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x\n" : "-\n");
    printf("Is our file hidden: %s\n", isHidden ? "Yes" : "No");
    unsigned long crc = calculateCRC(filename); 
    printf("CRC: %lu\n", crc);
        if (S_ISREG(fileStat.st_mode)) {
        printf("File type: Regular file\n");
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("File type: Directory\n");
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("File type: Symbolic link\n");
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("File type: FIFO/pipe\n");
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("File type: Socket\n");
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("File type: Character device\n");
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("File type: Block device\n");
    } else {
        printf("File type: Unknown\n");
    }
}
void pauseExecution(int microseconds) { usleep(microseconds); }
void convertFileSize(long long size, char *format, double *convertedSize) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    while (size > 1024 && unitIndex < 4) {
        size /= 1024;
        unitIndex++;
    }
    *convertedSize = size;
    sprintf(format, "%.2f %s", *convertedSize, units[unitIndex]);
}
void clear_screen() { printf("\033[2J\033[H");  }
void display_file_dump(FILE *file, const char *filename) {
    struct stat file_stat; 
    if(stat(filename, &file_stat) < 0) {
    perror("Error");
    return;
    }

    if (S_ISDIR(file_stat.st_mode)) {
        printf("Error File type: Directory - ");
    } else if (S_ISLNK(file_stat.st_mode)) {
        printf("Error: File type: Symbolic link - ");
    } else if (S_ISFIFO(file_stat.st_mode)) {
        printf("Error File type: FIFO/pipe - ");
    } else if (S_ISSOCK(file_stat.st_mode)) {
        printf("Error File type: Socket - ");
    } else if (S_ISCHR(file_stat.st_mode)) {
        printf("Error File type: Character device - ");
    } else if (S_ISBLK(file_stat.st_mode)) {
        printf("Error File type: Block device - ");
    } else {
        printf("Error File type: Unknown - ");
    }

    if (!S_ISREG(file_stat.st_mode)) {
    printf("'%s' is not a regular file.\n", filename);
    return;
    }

    unsigned char buffer[LINE_WIDTH];
    size_t bytesRead;
    long offset = 0;
    int ch;
    bool running = true;
    long totalFileSize = 0;
    long currentPercentage = 0;
    fseek(file, 0, SEEK_END);
    totalFileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    clear_screen();
    while (running) {
        fseek(file, offset * LINE_WIDTH, SEEK_SET);
        for (int i = 0; i < PAGE_SIZE; ++i) {
            if ((bytesRead = fread(buffer, 1, LINE_WIDTH, file)) == 0) {
                running = false;
                break;
            }
            for (size_t j = 0; j < bytesRead; ++j) {
                printf("%c", isprint(buffer[j]) ? buffer[j] : '.');
            }
            for (size_t j = bytesRead; j < LINE_WIDTH; ++j) {
                printf(" ");
            }
            for (size_t j = 0; j < bytesRead; ++j) {
                printf("%02x ", buffer[j]);
            }

            printf("\n");
        }
        currentPercentage = ftell(file) * 100 / totalFileSize;
        printf("Dumped: %ld%%\n", currentPercentage);

        printf("Press 'j' to scroll down, 'k' to scroll up, or 'q' to quit.\n");
        ch = getchar();
        if (ch == 'q')
            break;
        else if (ch == 'j')
            offset += PAGE_SIZE;
        else if (ch == 'k') {
            if (offset >= PAGE_SIZE)
                offset -= PAGE_SIZE;
            else
                offset = 0;
        }

        clear_screen();
    }
}

int main(int argc, char *argv[]) {
if (argc != 2) { printf("usage: %s <filename>\n", argv[0]); return 0; }
FILE *file;
FILE *file2;
FILE *file3;
struct stat fileStat;
char *filename = argv[1];
char command[100]; 
char input[100]; 
clear_screen();
printf("Menu:\n");
printf("0. Display verbosely file information.\n");
printf("1. Dump a file content with scrolling.\n");
printf("2. Display file size information.\n");
printf("3. Safe dump of file contents [TEXT ONLY] no scrolling.\n");
printf("4. Show disassembled output of file.\n");
printf("5. Quit program.\n"); 
printf("Enter your choice: ");

  fgets(input, sizeof(input), stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    int isNumber = 1;
    for (int i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i])) {
            isNumber = 0;
            break;
        }
    }
    if (isNumber) {
        int choice = atoi(input);

switch(choice) {
case 0: 
displayFileInformation(argv[1]);
break;
case 1: 
file = fopen(argv[1], "rb");
if (!file) { perror("File opening failed"); return 1; }
display_file_dump(file, argv[1]);
fclose(file);
break;
case 2:
 if (stat(filename, &fileStat) < 0) {
        perror("Error");
        return 1;
}
printf("Last modified: %s", ctime(&fileStat.st_mtime));
printf("File size: ");
    char format[20];
    double convertedSize;
    convertFileSize(fileStat.st_size, format, &convertedSize);
printf("%s\n", format);
pauseExecution(300000);
break; 
case 3:
file2 = fopen(filename, "rb");
if (file2 == NULL) {
perror("Error can't find that file?");
return 1;
}
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file2)) > 0) {
        for (size_t i = 0; i < bytesRead; i++) {
            if (isprint(buffer[i]) || isspace(buffer[i])) {
                putchar(buffer[i]);
            }
        }
        pauseExecution(200000);
        }
printf("\n\n");
fclose(file2);
break; 
case 4:
    sprintf(command, "objdump -d %s && objdump -d %s >>%s.obj", argv[1], argv[1], argv[1]);    
    file3 = fopen(argv[1], "r");
    if (file3 == NULL) {
        perror("Error");
        printf("Failed to open file '%s'\n", argv[1]);
        return 1;
    }
    fclose(file3); 
    printf("Disassembled output of binary file '%s':\n", argv[1]);
    int result = system(command);
    if (result == -1) {
        perror("Error");
        return 1;
    }
break;  
case 5: 
printf("Exiting program..\n"); 
return 0; 
default: printf("\n[Invalid choice]\n");
}
} else {
    if (strcmp(input, "Quit") == 0) { 
    printf("Exiting program..\n");
    return 0;  
    } 
    if (strcmp(input, "quit") == 0) {
            printf("Exiting program...\n");
        } else {
            printf("Invalid input\n");
        }
 }
 return 0;
}
