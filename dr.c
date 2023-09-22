#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <math.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

// do function for dir size
long long int getDirectorySize(const char *path) {
    struct stat statbuf;
    long long int size = 0;

    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

            if (stat(filepath, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    size += getDirectorySize(filepath); // Recursive call for subdirectory
                } else {
                    size += statbuf.st_size; // Add the size of the file
                }
            } else {
                perror("Error getting file stat");
                return -1;
            }
        }
    }

    closedir(dir);
    return size;
}

// Next function - B KB and so on. 
void formatSize(long long int size, char *result, int resultSize) {
    const char *units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;

    while (size >= 1024 && unitIndex < 4) {
        size /= 1024;
        unitIndex++;
    }

    snprintf(result, resultSize, "%lld %s", size, units[unitIndex]);
}

int main(int argc, char *argv[]) {
DIR *directory; 
struct dirent *entry;
struct stat file_info;
 
if (argv[1] == NULL || argv[1][0] == '\0') {
printf("\nUSAGE:\n%s [directory]\n\n", argv[0]);
return 1; 
}

directory = opendir(argv[1]); 

if (directory == NULL) {
perror("opendir");
return 1;
}

while ((entry = readdir(directory))) {
if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) { continue; }
printf("%s \n", entry->d_name);
}
closedir(directory); 
const char *path = argv[1];
long long int directorySize = getDirectorySize(path);

    if (directorySize != -1) {
        char formattedSize[128];
        formatSize(directorySize, formattedSize, sizeof(formattedSize));
        printf("Size of directory %s: %s\n", path, formattedSize);
    }

  char choice;

    do {
        printf("Show my file sizes? (y/n): ");
        scanf(" %c", &choice); // Note the space before %c to consume any previous newline character

        if (choice == 'y' || choice == 'Y') {
   
// Show more information in next dir dump .. 
  struct dirent *entry2;
    struct stat fileStat;
    DIR *directory2;

    // Open the directory
    directory2 = opendir(argv[1]); 

    if (directory2 == NULL) {
        perror("opendir");
        printf("\n NOTE: \nThis search function will access any file path,");
        printf("sensitive files are only accessible to authorized users.\n");
        return 1;
    }

    // Iterate over each entry in the directory
    while ((entry2 = readdir(directory2))) {
        // Check if the entry is a regular file
        if (entry2->d_type == DT_REG) {
            // Get file information
            char *filename = entry2->d_name;
            if (stat(filename, &fileStat) < 0) {
                perror("stat");
                continue;
                return -1;
            }

            printf("File: %s, Size: %ld bytes\n", filename, fileStat.st_size);
        }
    }

    // Close the directory
    closedir(directory2);
    
    // show more information about storage space. 
 struct statvfs stat;   
 if (statvfs("/", &stat) == 0) {
        unsigned long long total_size = (unsigned long long)stat.f_frsize * stat.f_blocks;
        unsigned long long free_space = (unsigned long long)stat.f_frsize * stat.f_bfree;
        double used_percent = ((double)(total_size - free_space) / total_size) * 100.0;

        printf("\n\nTotal size: %llu bytes\n", total_size);
        printf("Free space: %llu bytes\n", free_space);
        printf("Used space: %.2f%%\n", used_percent);
    } else {
        perror("Error");
        return -1;
    }
    return 0;
     } else if (choice == 'n' || choice == 'N') {
            printf("\n");
             struct statvfs stat;
    
    if (statvfs("/", &stat) == 0) {
        unsigned long long total_space = stat.f_frsize * stat.f_blocks;
        printf("Total space available: %llu bytes\n", total_space);
    } else {
        perror("statvfs");
        return -1;
    }
    
            break; 
        } else {
            printf("Invalid choice. Please enter 'y' or 'n'.\n");
        }

    } while (1); // Infinite loop, will continue until 'n' is entered

//  Show more information about the files in our directory 
    const char *filename = argv[1];
    struct stat file_stat;

  if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return 1;
    }

    // Print file attributes in a readable format
    printf("File: %s\n", filename);
    printf("Size: %lld bytes\n", (long long)file_stat.st_size);
    printf("Inode: %ld\n", (long)file_stat.st_ino);
    printf("Links: %ld\n", (long)file_stat.st_nlink);
    printf("Permissions: %o\n", (unsigned int)file_stat.st_mode);
    
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    printf("Owner: %s (%d)\n", pw ? pw->pw_name : "N/A", (int)file_stat.st_uid);
    printf("Group: %s (%d)\n", gr ? gr->gr_name : "N/A", (int)file_stat.st_gid);
    
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    printf("Last modified: %s\n", time_str);

    return 0;
}
