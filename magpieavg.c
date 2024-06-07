#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> // for sleep function

#define NUM_CORES 4
#define WARNING_THRESHOLD 85000 // Threshold in millidegrees Celsius
#define DEFAULT_LOOPS 1

// Function to convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
    return celsius * 9.0 / 5.0 + 32;
}

// Function to calculate the average temperature for each core
void calculateAverages(float temps[][3], int num_cores, int num_readings, float averages[]) {
    for (int core = 0; core < num_cores; ++core) {
        float sum = 0.0;
        for (int reading = 0; reading < num_readings; ++reading) {
            sum += temps[core][reading];
        }
        averages[core] = sum / num_readings;
    }
}

// Function to read temperature from thermal zone file
float read_temp(const char* thermal_zone, bool* has_warning) {
    FILE* file = fopen(thermal_zone, "r");
    if (file == NULL) {
        perror("Error opening file");
        *has_warning = true;
        return 0.0;
    }

    float temp;
    if (fscanf(file, "%f", &temp) != 1) {
        perror("Error reading temperature");
        fclose(file);
        *has_warning = true;
        return 0.0;
    }
    fclose(file);

    temp /= 1000; // Convert millidegrees Celsius to degrees Celsius

    // Print temperature for debugging
    printf("Temperature: %.2f°C\n", temp);

    // Check for warning condition
    if (temp * 1000 >= WARNING_THRESHOLD) {
        *has_warning = true;
    }

    return temp;
}

// Function to calculate the average temperature of all CPU cores
float calculate_average_temp(const char** thermal_zones, bool* has_warning) {
    float total_temp = 0.0;
    for (int i = 0; i < NUM_CORES; ++i) {
        total_temp += read_temp(thermal_zones[i], has_warning);
    }
    return total_temp / NUM_CORES;
}

// Function to print usage and help
void print_usage(const char* program_name) {
    printf("Usage: %s [Number of loops]\n", program_name);
    printf("If no number of loops is provided, the program will run %d loops by default.\n", DEFAULT_LOOPS);
    printf("Also the number of loops given must be a positive integer.\n");
    printf("\nOther command line arguments:\n");
    printf("%s [-h and/or --help]\n", program_name);
}


int main(int argc, char *argv[]) {
    const char* thermal_zones[NUM_CORES] = {
        "/sys/devices/virtual/thermal/thermal_zone0/temp",
        "/sys/devices/virtual/thermal/thermal_zone1/temp",
        "/sys/devices/virtual/thermal/thermal_zone2/temp",
        "/sys/devices/virtual/thermal/thermal_zone3/temp"
    };

    int num_loops = DEFAULT_LOOPS; // Default number of loops

   if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments.\n");
        print_usage(argv[0]);
        return 1;
    }
    if (argc == 2) {
        num_loops = atoi(argv[1]);
        if (num_loops <= 0) {
            fprintf(stderr, "The number of loops must be a positive integer.\n");
            return 1;
        }
    }

    bool has_warning = false;

    for (int loop = 0; loop < num_loops; ++loop) {
        float average_temp = calculate_average_temp(thermal_zones, &has_warning);

        if (has_warning) {
            printf("Warning: High CPU temperature detected!\n");
        } else {
            printf("CPU temperature is normal.\n");
        }

        printf("Average Temperature of all Cores: %.2f°C\n", average_temp);
        sleep(1);
    }

    // Sample temperature data in Celsius for each core for demonstration
    float temps[NUM_CORES][3] = {
        {30.0, 31.5, 29.0},  // Core 1
        {35.0, 34.5, 36.0},  // Core 2
        {40.0, 39.5, 38.0},  // Core 3
        {45.0, 44.5, 46.0}   // Core 4
    };
    float averages[NUM_CORES]; // Array to hold average temperatures

    calculateAverages(temps, NUM_CORES, 3, averages);

    for (int i = 0; i < NUM_CORES; ++i) {
        printf("Average core temp %d: %.2f°F\n", i + 1, celsiusToFahrenheit(averages[i]));
    }

    return 0;
}
