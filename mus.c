// Rewrite of the version for the Orange Pi Zero 3,
// Cromebook version 3.6c

#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <unistd.h>

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define RESET   "\x1B[0m"
#define program_name "mus"
#define program_version "3.6c"

void print_usage(); 

void print_usage() {
    printf("Usage: %s <path_to_directory> \n", program_name);
    printf("Examples:\n%s .. \n%s /\n", program_name, program_name);
    printf("Program: %s Version %s\n", program_name, program_version);
    printf("Magic Utility Storage analytics for linux.\n"); 
}

int main(int argc, char *argv[]) {
   if (argc != 2) {
        print_usage();
        return 1;
    }
   
    struct statvfs vfs;    
    const char *path = "/";
    const char *path_argv = argv[1];
    struct statvfs stat;   
    if (statvfs(path, &vfs) != 0) {
        perror("statvfs");
        return 1;
    }    
    if (statvfs(path_argv, &stat) != 0) {
        perror("Error while getting file system statistics");
        return 1;
    }

    unsigned long long block_size = stat.f_frsize;
    unsigned long long total_blocks = stat.f_blocks;
    unsigned long long free_blocks = stat.f_bfree;
    double used_percent = ((total_blocks - free_blocks) * 100.0) / total_blocks;
    double free_percent = (free_blocks * 100.0) / total_blocks;
    printf("Disk Usage for %s:\n", path_argv);
    printf("Total Space.: %.2f MB\n", (total_blocks * block_size) / (1024.0 * 1024));
    printf("Used Space..: %.2f MB (%.2f%%)\n", ((total_blocks - free_blocks) * block_size) / (1024.0 * 1024), used_percent);
    printf("Free Space..: %.2f MB (%.2f%%)\n", (free_blocks * block_size) / (1024.0 * 1024), free_percent);
    if (used_percent >= 90.0) {
        printf(RED "Warning: Disk usage is high!\n" RESET);
    } else if (used_percent >= 80.0) {
        printf(YELLOW "Warning: Disk usage is moderate.\n" RESET);
    } else {

        printf(GREEN "Disk usage is normal.\n" RESET);
    }
    unsigned long long total_space = (unsigned long long)vfs.f_blocks * vfs.f_frsize;
    unsigned long long free_space = (unsigned long long)vfs.f_bfree * vfs.f_frsize;
    unsigned long long used_space = total_space - free_space;
    double percent_free = (double)free_space / total_space * 100.0;
    double percent_used = (double)used_space / total_space * 100.0;    

    printf("Total space.: %llu bytes\n", total_space);
    printf("Used space..: %llu bytes (%.2lf%%)\n", used_space, percent_used);
    printf("Free space..: %llu bytes (%.2lf%%)\n", free_space, percent_free);   
    printf("Total Space.: %.2f GB (%.2f TB)\n", (total_blocks * block_size) / (1024.0 * 1024 * 1024), (total_blocks * block_size) / (1024.0 * 1024 * 1024 * 1024));
    printf("Used Space..: %.2f GB (%.2f%%)\n", ((total_blocks - free_blocks) * block_size) / (1024.0 * 1024 * 1024), used_percent);
    printf("Free Space..: %.2f GB (%.2f%%)\n", (free_blocks * block_size) / (1024.0 * 1024 * 1024), free_percent);
unsigned int delayMillis = 1000;    
useconds_t delayMicros = delayMillis * 1000;
usleep(delayMicros); 

  char choice;

    while (1) {
        printf("This will loop 1600 times and then display this exit prompt,\n");
        printf("Do you want to continue (y/n)? ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            printf("Continuing...\n");
        } else if (choice == 'n' || choice == 'N') { break;
        } else {
            printf("Invalid input. Please enter 'y' or 'n'.\n");
            continue;
        }
    for (int x =0; x<1600; x++) {    
    unsigned long long block_size = stat.f_frsize;
    unsigned long long total_blocks = stat.f_blocks;
    unsigned long long free_blocks = stat.f_bfree;
    double used_percent = ((total_blocks - free_blocks) * 100.0) / total_blocks;
    double free_percent = (free_blocks * 100.0) / total_blocks;
    printf("Disk Usage for %s:\n", path_argv);
    printf("Total Space.: %.2f MB\n", (total_blocks * block_size) / (1024.0 * 1024));
    printf("Used Space..: %.2f MB (%.2f%%)\n", ((total_blocks - free_blocks) * block_size) / (1024.0 * 1024), used_percent);
    printf("Free Space..: %.2f MB (%.2f%%)\n", (free_blocks * block_size) / (1024.0 * 1024), free_percent);
    if (used_percent >= 90.0) {
        printf(RED "Warning: Disk usage is high!\n" RESET);
    } else if (used_percent >= 80.0) {
        printf(YELLOW "Warning: Disk usage is moderate.\n" RESET);
    } else {

        printf(GREEN "Disk usage is normal.\n" RESET);
    }
    unsigned long long free_space = (unsigned long long)vfs.f_bfree * vfs.f_frsize;
    unsigned long long used_space = total_space - free_space;
    double percent_free = (double)free_space / total_space * 100.0;
    double percent_used = (double)used_space / total_space * 100.0; 
    printf("Used space..: %llu bytes (%.2lf%%)\n", used_space, percent_used);
    printf("Free space..: %llu bytes (%.2lf%%)\n", free_space, percent_free);   
    printf("Total Space.: %.2f GB (%.2f TB)\n", (total_blocks * block_size) / (1024.0 * 1024 * 1024), (total_blocks * block_size) / (1024.0 * 1024 * 1024 * 1024));
    printf("Used Space..: %.2f GB (%.2f%%)\n", ((total_blocks - free_blocks) * block_size) / (1024.0 * 1024 * 1024), used_percent);
    printf("Free Space..: %.2f GB (%.2f%%)\n", (free_blocks * block_size) / (1024.0 * 1024 * 1024), free_percent);
    printf("\nTotal loop count out of 1600 is %d....\n", x);
usleep(delayMicros); 
system("clear");
}
}
return 0;
}
