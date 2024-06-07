#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define WARNING_THRESHOLD 85.0
#define NUM_CORES 4
#define MAX_ITERATIONS 17

// Function to read temperature from thermal zone file
float read_temp(const char* thermal_zone, bool* has_warning) {
    FILE* file = fopen(thermal_zone, "r");
    if (file == NULL) {
        perror("Error opening file");
        *has_warning = true;
        exit(EXIT_FAILURE);
    }

    float temp;
    fscanf(file, "%f", &temp);
    fclose(file);
       // Check for warning condition
    if (temp >= WARNING_THRESHOLD) {
        *has_warning = true;
    } else {
        *has_warning = false;
    }
    return temp / 1000; // Convert millidegrees Celsius to degrees Celsius
}

// Function to calculate the average temperature of all CPU cores
float calculate_average_temp(const char** thermal_zones,  bool* has_warning) {
    float total_temp = 0.0;
    for (int i = 0; i < NUM_CORES; ++i) {
        total_temp += read_temp(thermal_zones[i], has_warning);
    }
    return total_temp / NUM_CORES;
}

// Function to calculate CPU usage percentage for each core
void calculate_cpu_usage(float* cpu_usage) {
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "cpu", 3) == 0) {
            int core_index;
            unsigned long long int user, nice, system, idle;
            if (sscanf(line, "cpu%d %llu %llu %llu %llu", &core_index, &user, &nice, &system, &idle) == 5 && core_index >= 0 && core_index < NUM_CORES) {
                unsigned long long int total_time = user + nice + system + idle;
                unsigned long long int idle_time = idle;
                float usage = 100.0 * (1.0 - (idle_time * 1.0 / total_time));
                cpu_usage[core_index] = usage;
            }
        }
    }

    fclose(file);
}

// Function to print CPU information
void print_cpu_info() {
    FILE* file2 = fopen("/proc/cpuinfo", "r");
    if (file2 == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int cpu_count = 0;
    char model_name[256] = "";
    char cpu_mhz[256] = "";
    char BogoMIPS[256] = "";

    while (fgets(line, sizeof(line), file2)) {
        if (strncmp(line, "processor", 9) == 0) {
            cpu_count++;
        } else if (strncmp(line, "model name", 10) == 0) {
            char* colon_pos = strchr(line, ':');
            if (colon_pos != NULL) {
                strcpy(model_name, colon_pos + 2);
            }
        } else if (strncmp(line, "cpu MHz", 7) == 0) {
            char* colon_pos = strchr(line, ':');
            if (colon_pos != NULL) {
                strcpy(cpu_mhz, colon_pos + 2);
            }
        } else if (strncmp(line, "BogoMIPS", 7) == 0) {
            char* colon_pos = strchr(line, ':');
            if (colon_pos != NULL) {
                strcpy(BogoMIPS, colon_pos + 2);
            }
     }
    }

    fclose(file2);

    printf("CPU Information searching /proc/cpuinfo:\n");
    printf("Number of CPU cores: %d\n", cpu_count);
    printf("Other information:\nmodle_name: %s\n", model_name);
    printf("cpu_mhz: %s\n", cpu_mhz);
    printf("BogoMIPS: %s\n", BogoMIPS);
}




int main() {
system("clear");
int i;
    for (i = 0; i < MAX_ITERATIONS; ++i) {
        printf("Iteration %d:\n", i + 1);

    const char* thermal_zones[NUM_CORES] = {"/sys/devices/virtual/thermal/thermal_zone0/temp",
                                             "/sys/devices/virtual/thermal/thermal_zone1/temp",
                                             "/sys/devices/virtual/thermal/thermal_zone2/temp",
                                             "/sys/devices/virtual/thermal/thermal_zone3/temp"};

  bool has_warning = false; 
   printf("\n");
    for (int i = 0; i < NUM_CORES; ++i) {
        float temp = read_temp(thermal_zones[i], &has_warning);
        printf("CPU Core %d temperature: %.2f°C\n", i, temp);
    }

    float average_temp = calculate_average_temp(thermal_zones, &has_warning);
    printf("\nAverage Temperature of all Cores: %.2f°C\n", average_temp);

//    if (has_warning) {
//        printf("Warning: High CPU temperature detected!\n");
//    } else {
//        printf("CPU temperature is normal.\n");
//    }

    float cpu_usage[NUM_CORES] = {0};
    calculate_cpu_usage(cpu_usage);
    printf("CPU Usage:\n");
    for (int i = 0; i < NUM_CORES; ++i) {
        printf("Core %d: %.2f%%\n", i, cpu_usage[i]);
    }
    printf("\n");
    print_cpu_info();
    sleep(1);
    system("clear");
}


    sleep(1);    
return 0;
}
