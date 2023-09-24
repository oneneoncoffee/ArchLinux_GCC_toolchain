#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <time.h> 
#include <errno.h>
#include <string.h>

// Function to display file attributes with sizes in human-readable format
void displayFileAttributes(const char *filename) {
    struct stat fileStat;

    if (stat(filename, &fileStat) == -1) {
        perror("stat");
        return;
    }

    printf("File: %s\n", filename);

    // File size in bytes
    long long fileSize = (long long)fileStat.st_size;

    // Define size units and corresponding divisor
    const char *sizeUnits[] = {"B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    const int unitCount = sizeof(sizeUnits) / sizeof(sizeUnits[0]);
    int unitIndex = 0;

    while (fileSize > 1024 && unitIndex < unitCount - 1) {
        fileSize /= 1024;
        unitIndex++;
    }

    printf("Size: %lld %s\n", fileSize, sizeUnits[unitIndex]);

    printf("File Permissions: ");   
   
    printf("File: %s\n", filename);
    printf("Size: %lld bytes\n", (long long)fileStat.st_size);
    printf("File Permissions: ");
    printf((S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n");

    printf("Inode Number: %lld\n", (long long)fileStat.st_ino);
    printf("Number of Links: %lld\n", (long long)fileStat.st_nlink);
    printf("User ID of Owner: %d\n", fileStat.st_uid);
    printf("Group ID of Owner: %d\n", fileStat.st_gid);
    printf("Device ID (if special file): %lld\n", (long long)fileStat.st_rdev);
    printf("Last Access Time: %lld\n", (long long)fileStat.st_atime);
    printf("Last Modification Time: %lld\n", (long long)fileStat.st_mtime);
    printf("Last Status Change Time: %lld\n", (long long)fileStat.st_ctime);
    printf("\n");
}

void analyzeDiskUsage(const char *path) {
    struct stat fileStat;
    if (stat(path, &fileStat) < 0) {
        perror("stat");
        return;
    }

    // Get file owner information
    struct passwd *pw = getpwuid(fileStat.st_uid);
    if (!pw) {
        perror("getpwuid");
        return;
    }

    // Get file group information
    struct group *gr = getgrgid(fileStat.st_gid);
    if (!gr) {
        perror("getgrgid");
        return;
    }

    // Calculate disk usage in kilobytes
    long long int diskUsageKB = (fileStat.st_blocks * 512) / 1024;

    // Print user, group, and disk usage information
    printf("File: %s\n", path);
    printf("Owner: %s\n", pw->pw_name);
    printf("Group: %s\n", gr->gr_name);
    printf("Disk Usage: %lld KB\n", diskUsageKB);

}

int main(int argc, char *argv[]) {
    clock_t start_time, end_time;
    double cpu_time_used;

    // Record the start time
    start_time = clock();

    if (argc != 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }


    const char *dirPath = argv[1];
    DIR *dir = opendir(dirPath);

    if (!dir) {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] != '.') { // Ignore hidden files/directories
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            analyzeDiskUsage(fullPath);

        }
    }
    closedir(dir);
// Okay now we search for files 
  DIR *dir2;
    struct dirent *entry2;

    dir2 = opendir(argv[1]); // Open the current directory
    if (dir2 == NULL) {
        perror("opendir");
        exit(1);
    }

    // Iterate through the files in the directory
    while ((entry2 = readdir(dir2)) != NULL) {
        // Get file attributes
        struct stat file_info;
        if (stat(entry2->d_name, &file_info) == -1) {
            continue; // Skip to the next file 
        }

        // Print file attributes
        printf("File: %s\n", entry2->d_name);
        printf("Size: %lld bytes\n", (long long)file_info.st_size);
        printf("Permissions: ");
        printf((file_info.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_info.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_info.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_info.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_info.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_info.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_info.st_mode & S_IROTH) ? "r" : "-");
        printf((file_info.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_info.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");

        // Get and print owner and group names
        struct passwd *owner_info = getpwuid(file_info.st_uid);
        struct group *group_info = getgrgid(file_info.st_gid);
        if (owner_info != NULL) {
            printf("Owner: %s\n", owner_info->pw_name);
        }
        if (group_info != NULL) {
            printf("Group: %s\n", group_info->gr_name);
        }

        // Print modification time
        struct tm *time_info = localtime(&file_info.st_mtime);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
        printf("Last Modified: %s\n\n", time_str);
    }

    // Close the directory
    closedir(dir2);
// okay lets tidy-up! 
    char choice;

    do {
        printf("Do you want to continue (y/n)? ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            // Add your code to continue the program here
            printf("Continuing the program...\n");

    const char *directory_name = argv[1];

    DIR *dir3 = opendir(directory_name);
    if (dir3 == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry3;
    while ((entry3 = readdir(dir3)) != NULL) {
        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", directory_name, entry3->d_name);

        // Skip "." and ".." directories
        if (strcmp(entry3->d_name, ".") == 0 || strcmp(entry3->d_name, "..") == 0) {
            continue;
        }

        displayFileAttributes(fullpath);
    }

    closedir(dir3);
    break;

     } else if (choice == 'n' || choice == 'N') {
            printf("Exiting program..\n");
            break; // Exit the loop and program
        } else {
            printf("Invalid choice. Please enter 'y' to continue or 'n' to exit.\n");
        }
    } while (1); // Infinite loop, can be exit

    // Record the end time
    end_time = clock();
    // Calculate the CPU time used in seconds
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Program runtime: %f seconds\n", cpu_time_used);
    
    return 0;
}
