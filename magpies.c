#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int isAesEncrypted(FILE *file) {
char magicNumber[] = "AES_ENCRYPTION_MAGIC";
char buffer[sizeof(magicNumber)];
fread(buffer, 1, sizeof(buffer) - 1, file);
buffer[sizeof(buffer) -1] = '\0';
return strcmp(buffer, magicNumber) == 0;
}

int isDesEncrypted(FILE *file) {
char magicNumber[] = "DES_ENCRYPTION_MAGIC";
char buffer[sizeof(magicNumber)];
fread(buffer,1,sizeof(buffer) -1, file);
buffer[sizeof(buffer) -1] = '\0';
return strcmp(buffer, magicNumber) == 0;
}

int isHashFile(const char *filePath) {
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char buffer[1024];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);
    if (bytesRead >= 32 && bytesRead <= 128) {
        int i;
        for (i = 0; i < bytesRead && i < 32; i++) {
            if (!((buffer[i] >= '0' && buffer[i] <= '9') ||
                  (buffer[i] >= 'a' && buffer[i] <= 'f') ||
                  (buffer[i] >= 'A' && buffer[i] <= 'F'))) {
                break;
            }
        }
        for (; i < bytesRead && (buffer[i] == ' ' || buffer[i] == '\t'); i++) {}
        if (i < bytesRead && buffer[i] != '\n' && buffer[i] != '\r') {
            return 1;
        }
    }
    return 0;
}

int isRsaEncrypted(FILE *file) {
    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("Error seeking file");
        return -1;
    }
    char buffer[512];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
    if (bytesRead == 0) {
        if (feof(file)) {
            printf("The file is empty.\n");
        } else {
            perror("Error reading file");
        }
        return -1;
    }
    buffer[bytesRead] = '\0';

    if (strstr(buffer, "-----BEGIN RSA PRIVATE KEY-----") != NULL ||
        strstr(buffer, "-----BEGIN PUBLIC KEY-----") != NULL ||
        strstr(buffer, "Modulus:") != NULL ||
        strstr(buffer, "Exponent:") != NULL) {
        printf("[Encryption type is RSA]\n");
    } else {
        printf(".");
    }

}

void checkXorEncryption(FILE *file) {
    fseek(file, 0, SEEK_SET);
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    int *byteFrequency = (int *)calloc(256, sizeof(int));

    if (byteFrequency == NULL) {
        perror("Memory allocation error");
        exit(1);
    }

    int c;
    while ((c = fgetc(file)) != EOF) {
        byteFrequency[c]++;
    }
    int threshold = fileSize / 1024;
    int distinctBytes = 0;

    for (int i = 0; i < 256; i++) {
        if (byteFrequency[i] > 0) {
            distinctBytes++;
        }
        if (distinctBytes > threshold) {
            free(byteFrequency);
            printf("....Done.\n");
            fclose(file);
            exit(0);
        }
    }

    free(byteFrequency);
    printf("[Encryption type XOR]\n");
    fclose(file);
    exit(0);
}

void checkTeaEncryption(FILE *file) {
    fseek(file, 0, SEEK_SET);
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (fileSize % 8 != 0) {
        printf(".");
        return;
    }
    int *byteFrequency = (int *)calloc(256, sizeof(int));
    if (byteFrequency == NULL) {
        perror("Memory allocation error");
        exit(1);
    }
    int c;
    while ((c = fgetc(file)) != EOF) {
        byteFrequency[c]++;
    }
    int threshold = fileSize / 100;
    int distinctBytes = 0;

    for (int i = 0; i < 256; i++) {
        if (byteFrequency[i] > 0) {
            distinctBytes++;
        }
        if (distinctBytes > threshold) {
            free(byteFrequency);
            printf(".");
            return;  // Not TEA encrypted
        }
    }
    free(byteFrequency);
    printf("[Encryption type is TEA]\n");
}

void checkEncryption(FILE *file) {
    fseek(file, 0, SEEK_SET);
    char buffer[512];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, file);
    buffer[bytesRead] = '\0';
    if (strstr(buffer, "-----BEGIN PGP MESSAGE-----") != NULL) {
        printf("[Encryption is likely PGP/GPG encrypted]\n");
    } else if (strstr(buffer, "-----BEGIN CERTIFICATE-----") != NULL) {
        printf("[The file may contain certificates.]\n");
    } else if (strstr(buffer, "AES_ENCRYPTION_MAGIC") != NULL) {
        printf("[Encryption is likely using AES]\n");
    } else if (strstr(buffer, "Blowfish_MAGIC_STRING") != NULL) {
        printf("[Encryption is likely using Blowfish.]\n");
    } else if (strlen(buffer) == 32 || strlen(buffer) == 40 || strlen(buffer) == 64) {
        printf("[The file contains a hash (MD5/SHA-1/SHA-256)]\n");
    } else if (strpbrk(buffer, "+/=") != NULL) {
        printf("[The file may be Base64 encoded]\n");
    } else {
        printf(".");
    }
}

void printPermissions(mode_t mode) {
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

void printFileSize(off_t size) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    int unitIndex = 0;
    while (size > 1024 && unitIndex < sizeof(units) / sizeof(units[0]) - 1) {
        size /= 1024;
        unitIndex++;
    }
    printf("Size: %.2f %s\n", (double)size, units[unitIndex]);
}

int isHidden(const char *fileName) { return fileName[0] == '.'; }

void printFileAttributes(const char *filePath) {
    struct stat fileStat;
    if (stat(filePath, &fileStat) == 0) {
        printf("\nFile name: %s", filePath);
        if (S_ISDIR(fileStat.st_mode)) {
            printf(" Type: Directory ");
        } else {
            printf(" Type: File ");
        }
        if(isHidden(filePath)) {
        printf("Hidden: yes\n");
        } else {
        printf("Hidden: no\n");
        }
        if (S_ISDIR(fileStat.st_mode)) {
        fprintf(stderr,"The specified file path is a directory.\nNow exiting.\n");
        exit(EXIT_FAILURE);
        }
        printFileSize(fileStat.st_size);
        printf("Permissions: ");
        printPermissions(fileStat.st_mode);
        printf("UID: %d - ", fileStat.st_uid);
        printf("GID: %d\n", fileStat.st_gid);
        printf("Creation Time: %s", ctime(&fileStat.st_ctime));
        printf("Modification Time: %s", ctime(&fileStat.st_mtime));
    } else {
        perror("Error getting file attributes");
    }
}

int isDirectory(const char *path) {
    struct stat pathStat;
    if (stat(path, &pathStat) != 0) {
        perror("Error getting file information");
        exit(EXIT_FAILURE);
    }
    return S_ISDIR(pathStat.st_mode);
}

int isWhirlpoolHash(const char *hash) {
    size_t len = strlen(hash);
    if (len == 128) {
        for (size_t i = 0; i < len; i++) {
            char c = hash[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void detectWhirlpoolHashes(const char *filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (isWhirlpoolHash(buffer)) {
            printf("\n[Whirlpool Hash found]\n %s\n", buffer);
        }
    }
    printf(".");
    fclose(file);
}

int main(int argc, char *argv[]) {
if (argc !=2) {
fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
return 1;
}
printFileAttributes(argv[1]);
const char *fp = argv[1];
FILE *file = fopen(fp, "rb");
printf("\nChecking file [%s]..", argv[1]);
if (file == NULL) {
perror("error opening file");
return 1;
}
checkEncryption(file);
if (isAesEncrypted(file)) {
printf("[Encryption type is AES]\n");
} else if (isDesEncrypted(file)) {
printf("[Encryption type is DES]\n");
} else {
isRsaEncrypted(file);
checkTeaEncryption(file);
int result = isHashFile(fp);
if (result == -1) {
fprintf(stderr, "Error checking if it's a hash file.\n");
} else if (result == 1) {
printf("\n[The file is a hash file]\n");
} else {
printf(".");
}
const char *FilePath = argv[1];
detectWhirlpoolHashes(FilePath);
checkXorEncryption(file);
}

fclose(file);
return 0;
}
