// Display temperature sensor data of CPU's for the orange pi zero 3 
// Celsius, Fahrenheit, Kelvin and it calculates the average core temp.  
// We also have a temperature is critical alarm. 
// 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_CPU_CORES 4

// Function to monitor the temperature of all CPU cores and calculate the average
void monitorAndCalculateAverageTemperature(const char* temperatureFiles[], int numCores, int threshold) {
    
        int totalTemperature = 0;
        for (int core = 0; core < numCores; core++) {
            FILE* file = fopen(temperatureFiles[core], "r");
            if (file != NULL) {
                int temperature;
                if (fscanf(file, "%d", &temperature) == 1) {
                    // Convert temperature to millidegrees Celsius
                    printf("Core #%d ", core);
                    totalTemperature += temperature;
                    
                    if (temperature >= threshold) {
                        printf("CPU Core %d temperature is critical: %d millidegrees Celsius\n", core, temperature);
                        // You can trigger an alert mechanism here, e.g., send an email, SMS, etc.
                    }
                }
                fclose(file);
            }
       

        // Calculate the average temperature
        int averageTemperature = totalTemperature / numCores;
        printf("Average CPU temperature: %d millidegrees Celsius\n", averageTemperature);

        // Sleep for a period (e.g., 1 second) before checking again
        sleep(1);
    }
}

// Function to display text on one line
void displayTextOnLine(const char* text) {
    printf("\e[1;1H\e[2J");
    printf("%s\r", text);
    fflush(stdout); // Flush the output to make sure it's displayed immediately
}

// Function to read the CPU temperature from a thermal zone file
float readCPUTemperature(int core) {
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/thermal/thermal_zone%d/temp", core);
    
    FILE *temp_file = fopen(path, "r");
    if (temp_file == NULL) {
        perror("Failed to open temperature file");
        exit(1);
    }
    
    float temp;
    fscanf(temp_file, "%f", &temp);
    fclose(temp_file);
    
    return temp / 1000.0; // Convert millidegrees to degrees Celsius
}

// Function to convert Celsius to Fahrenheit
float celsiusToFahrenheit(float celsius) {
    return (celsius * 9.0 / 5.0) + 32.0;
}

// Function to convert Celsius to Kelvin
float celsiusToKelvin(float celsius) {
    return celsius + 273.15;
}

// Function to check if a key has been pressed
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    printf("\n"); 
    return 0;
}
// Function to read CPU core usage from /proc/stat
float readCPUUsage(int core) {
    FILE *stat_file = fopen("/proc/stat", "r");
    if (stat_file == NULL) {
        perror("Failed to open /proc/stat");
        exit(1);
    }

    char line[256];
    char core_name[10];
    snprintf(core_name, sizeof(core_name), "cpu%d", core);

    float usage = 0;
    while (fgets(line, sizeof(line), stat_file) != NULL) {
        if (strncmp(line, core_name, strlen(core_name)) == 0) {
            int user, nice, system, idle, iowait, irq, softirq, steal;
            sscanf(line, "cpu%d %d %d %d %d %d %d %d %d", &core, &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
            
            float total = user + nice + system + idle + iowait + irq + softirq + steal;
            float busy = total - idle;

            if (total > 0) {
                usage = (busy / total) * 100;
            }

            break;
        }
    }

    fclose(stat_file);

    return usage;
}

// Function to monitor the temperature of all CPU cores
void monitorCPUTemperature(const char* temperatureFiles[], int numCores, int threshold) {
        for (int core = 0; core < numCores; core++) {
            FILE* file = fopen(temperatureFiles[core], "r");
            if (file != NULL) {
                int temperature;
                if (fscanf(file, "%d", &temperature) == 1) {
                    // Convert temperature to millidegrees Celsius
                    printf("CPU Core %d temperature: %d millidegrees Celsius\n", core, temperature);

                    if (temperature >= threshold) {
                        printf("CPU Core %d temperature is critical: %d millidegrees Celsius\n", core, temperature);
                        // You can trigger an alert mechanism here, e.g., send an email, SMS, etc.
                    }
              }
              fclose(file); 
             }
            FILE* file2 = fopen(temperatureFiles[core], "r");
            if (file != NULL) {
                int temperature;
                if (fscanf(file2, "%d", &temperature) == 1) {
                    // Return data about cpu cores as alarm
                    printf("CPU Core %d temperature is normal.\n", core);

                    if (temperature >= threshold) {
                        printf("CPU Core %d temperature is critical.\n", core);
                        // You can trigger an alert mechanism here, e.g., send an email, SMS, etc.
                    }
                }
                fclose(file2);
        }
       }

        // Sleep for a period (e.g., 1 second) before checking again
        sleep(1);
}

int main() {
  for (int iteration = 1; iteration <= 1000; iteration++) {
        
        // Clear the screen using escape sequences
        printf("\e[2J\e[H");

    char cpu_info[100];
    char temp_info[100];

    FILE *cpuinfo = popen("lscpu | grep 'Model name'", "r");
    FILE *tempinfo = popen("sensors | grep 'Package id 0'", "r");

    if (cpuinfo == NULL || tempinfo == NULL) {
        perror("Failed to execute a command");
        return 1;
    }

    fgets(cpu_info, sizeof(cpu_info), cpuinfo);
    fgets(temp_info, sizeof(temp_info), tempinfo);

    pclose(cpuinfo);
    pclose(tempinfo);

    printf("%s", cpu_info);

    int core_count = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Total CPU Cores: %d\n", core_count);

    // Display individual core temperatures in °C, °F, and K
    for (int i = 0; i < core_count; i++) {
        float temp_celsius = readCPUTemperature(i);
        float temp_fahrenheit = celsiusToFahrenheit(temp_celsius);
        float temp_kelvin = celsiusToKelvin(temp_celsius);
        float usage = readCPUUsage(i);

        printf("Core %d Temperature: %.1f°C - ", i, temp_celsius);
        printf("%.1f°F - ", temp_fahrenheit);
        printf("%.2fK [", temp_kelvin);
        printf("Usage: %.1f%%]\n", usage);
    }

    // Paths to the temperature files for each CPU core
    const char* temperatureFiles[] = {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/thermal/thermal_zone1/temp",
        "/sys/class/thermal/thermal_zone2/temp",
        "/sys/class/thermal/thermal_zone3/temp"
    };

    // Desired temperature threshold (in millidegrees Celsius)
    const int threshold = 98000;  // 98 degrees Celsius
    printf("\n");

    // Call the function to monitor CPU temperatures
    monitorCPUTemperature(temperatureFiles, 4, threshold);
    printf("\n");
 
    // Desired temperature threshold (in millidegrees Celsius)
    //const int threshold = 98500;  // 98.5 degrees Celsius
    monitorAndCalculateAverageTemperature(temperatureFiles, 4, threshold);
 
    printf("\e[2J\e[H");

}


   // Wait for a keypress to exit
    while (!kbhit()) {
    displayTextOnLine("Press any key to continue..\nOr Ctrl+C to exit..\n");
  
    for (int i = 0; i < 4; i++) {
        float temp_celsius = readCPUTemperature(i);
        float temp_fahrenheit = celsiusToFahrenheit(temp_celsius);
        float temp_kelvin = celsiusToKelvin(temp_celsius);
        float usage = readCPUUsage(i);

        printf("Core %d Temperature: %.1f°C - ", i, temp_celsius);
        printf("%.1f°F - ", temp_fahrenheit);
        printf("%.2fK [", temp_kelvin);
        printf("Usage: %.1f%%]\n", usage);
    }
    usleep(8099);

    // The path to the temperature file for the CPU core you want to monitor
    // You may need to replace this with the actual path on your system.
    const char* temperatureFile = "/sys/class/thermal/thermal_zone0/temp";
    // Desired temperature threshold (in millidegrees Celsius)
    const int threshold = 98000;  // 98 degrees Celsius
    FILE* file = fopen(temperatureFile, "r");
        if (file != NULL) {
            int temperature;
            if (fscanf(file, "%d", &temperature) == 1) {
                // Convert temperature to millidegrees Celsius
                if (temperature >= threshold) {
                    printf("CPU temperature is critical: %d millidegrees Celsius\n\n", temperature);
                    // You can trigger an alert mechanism here, e.g., send an email, SMS, etc.
                } else {
                    printf("CPU temperature is normal: %d millidegrees Celsius\n\n", temperature);
                }
            }
            fclose(file);
    }
    // Paths to the temperature files for each CPU core
    const char* path_temperatureFiles[] = {
        "/sys/class/thermal/thermal_zone0/temp",
        "/sys/class/thermal/thermal_zone1/temp",
        "/sys/class/thermal/thermal_zone2/temp",
        "/sys/class/thermal/thermal_zone3/temp"
    };
    monitorCPUTemperature(path_temperatureFiles, 4, threshold);
 
    usleep(9900); 
    }
    printf("\e[2J\e[H");

    return 0;
}
