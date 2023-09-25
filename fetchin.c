#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

float celsiusToFahrenheit(float celsius) {
    return (celsius * 9 / 5) + 32;
}

int main() {
        time_t now;
        struct tm *tm_info;
	struct sysinfo info;
        struct statvfs vfs; 
        time(&now);
        tm_info = localtime(&now);
        char time_str[12]; // 12 hour format 
        strftime(time_str, sizeof(time_str), "%I:%M %p", tm_info);
        FILE *input_file = fopen("/proc/zoneinfo", "r"); 
        
// get volume stats error checking 
if (statvfs("/", &vfs) != 0) {
        perror("statvfs");
        return 1;
}
// get systems information error checking 
if(sysinfo(&info) !=0) {
    perror("sysinfo"); 
    return 1;
}
// new log file open write output error checking
if (input_file == NULL) {
        perror("fopen");
        return 1;
}
    FILE *output_file = fopen("fetchin_zoneinfo.log", "w");
if (output_file == NULL) {
        perror("fopen");
        fclose(input_file);
        return 1;
}
// get memory info from system /proc/meminfo
  FILE *memfile = fopen("/proc/meminfo", "r");
if (memfile == NULL) {
        perror("fopen");
        return 1;
}
  // Get the username
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if (pwd == NULL) {
        perror("getpwuid");
        return 1;
    }
    char *username = pwd->pw_name;

    // Get the hostname
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        perror("gethostname");
        return 1;
    }

    // Get the primary group name
    gid_t gid = getgid();
    struct group *grp = getgrgid(gid);
    if (grp == NULL) {
        perror("getgrgid");
        return 1;
    }
    char *groupname = grp->gr_name;


printf("Current time: %s\n", time_str);
printf("Uptime %ld seconds\n", info.uptime);
printf("Load Average (1min/5min/15min): %.2f %.2f %.2f\n", 
           (float)info.loads[0] / (1 << SI_LOAD_SHIFT),
           (float)info.loads[1] / (1 << SI_LOAD_SHIFT),
           (float)info.loads[2] / (1 << SI_LOAD_SHIFT));
printf("Host %s@%s \nGroup:%s\n", username, hostname, groupname);
    unsigned long long total_space = vfs.f_frsize * vfs.f_blocks;
    unsigned long long free_space = vfs.f_frsize * vfs.f_bfree;
    unsigned long long used_space = total_space - free_space;
    double usage_percentage = (double)used_space / total_space * 100.0;
printf("Hard Drive Usage: %.2f%%\n", usage_percentage);
    long total_mem = 0;
    long free_mem = 0;
    long total_swap = 0;
    long free_swap = 0;
    char line[256];
    while (fgets(line, sizeof(line), memfile)) {
if (sscanf(line, "MemTotal: %ld kB", &total_mem) == 1) {
            continue;
        }
        if (sscanf(line, "MemFree: %ld kB", &free_mem) == 1) {
            break;
        }
}
if (total_mem > 0) {
        double used_mem_percentage = ((double)(total_mem - free_mem) / total_mem) * 100.0;
        printf("RAM Usage: %.2f%%\n", used_mem_percentage);
}
    // do swap percentage vodo now 
    while (fgets(line, sizeof(line), memfile)) {
        if (sscanf(line, "SwapTotal: %ld kB", &total_swap) == 1) {
            continue;
        }
        if (sscanf(line, "SwapFree: %ld kB", &free_swap) == 1) {
            break;
        }
    }

    if (total_swap > 0) {
        double used_swap_percentage = ((double)(total_swap - free_swap) / total_swap) * 100.0;
        printf("Swap Usage: %.2f%%\n", used_swap_percentage);
    }

    fclose(memfile); 
    // get cpu temps 
  FILE *temp_file = fopen("/sys/class/hwmon/hwmon0/temp1_input", "r");

    if (temp_file == NULL) {
        perror("fopen");
        return 1;
    }

    int temperature;
    if (fscanf(temp_file, "%d", &temperature) != 1) {
        perror("fscanf");
        fclose(temp_file);
        return 1;
    }

    fclose(temp_file);

    // Convert to degrees Celsius
    float celsius_temp = temperature / 1000.0;

    // Convert to degrees Fahrenheit
    float fahrenheit_temp = celsiusToFahrenheit(celsius_temp);

    printf("CPU Topend Temperature: %.2f°C\n", celsius_temp);
    printf("CPU Topend Temperature: %.2f°F\n", fahrenheit_temp);
    // lets go real deep up in that by searching out each core.
       int num_cores = 0;  // Number of CPU cores with temperature sensors
    char temp_file_path[256];

    // Determine the number of CPU cores with temperature sensors
    while (1) {
        snprintf(temp_file_path, sizeof(temp_file_path), "/sys/class/hwmon/hwmon%d/temp1_input", num_cores);
        FILE *temp_file_multi = fopen(temp_file_path, "r");
        if (temp_file_multi == NULL) {
            // No more cores with temperature sensors
            break;
        }
        fclose(temp_file_multi);
        num_cores++;
    }

    if (num_cores == 0) {
        printf("No temperature sensors found.\n");
        return 1;
    }

    printf("CPU zone temperatures:\n");

    for (int core = 0; core < num_cores; core++) {
        snprintf(temp_file_path, sizeof(temp_file_path), "/sys/class/hwmon/hwmon%d/temp1_input", core);
        FILE *temp_file_multi = fopen(temp_file_path, "r");

        if (temp_file_multi == NULL) {
            perror("fopen");
            return 1;
        }

        int temperature;
        if (fscanf(temp_file_multi, "%d", &temperature) != 1) {
            perror("fscanf");
            fclose(temp_file_multi);
            return 1;
        }

        fclose(temp_file_multi);

        // Convert to degrees Celsius
        float celsius_temp = temperature / 1000.0;

        // Convert to degrees Fahrenheit
        float fahrenheit_temp = celsiusToFahrenheit(celsius_temp);

        printf("Core %d: %.2f°C, %.2f°F\n", core, celsius_temp, fahrenheit_temp);
    }

    // Now we output to our log file and clean up.
    while (fgets(line, sizeof(line), input_file)) {
     fputs(line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    printf("Data saved to fetchin_zoneinfo.log\n");
    char response[2];
    printf("Do you want to remove the log file? (y/n): ");
    
    if (fgets(response, sizeof(response), stdin) != NULL) {
        if (response[0] == 'y' || response[0] == 'Y') {
            // User wants to remove the log file
            if (remove("fetchin_zoneinfo.log") == 0) {
                printf("Log file removed successfully.\n");
            } else {
                perror("Error removing log file");
            }
        } else if (response[0] == 'n' || response[0] == 'N') {
            // User chose not to remove the log file
            printf("Log file not removed.\n");
        } else {
            // Invalid response
            printf("Invalid response. Please enter 'y' or 'n'.\n");
        }
    } else {
        // Error reading user input
        perror("Error reading user input");
    }

return 0;
}
