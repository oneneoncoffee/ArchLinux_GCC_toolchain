#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <time.h> 

// Do our functions in this section of our outline. 
// Function to calculate and print network packets 
void printNetworkPackets() {
    char interface_name[] = "wlan0";
    char stats_file_path[256];
    FILE *file;
    unsigned long long int rx_packets1, tx_packets1, rx_packets2, tx_packets2;
    time_t time1, time2;
    snprintf(stats_file_path, sizeof(stats_file_path), "/sys/class/net/%s/statistics/rx_packets", interface_name);
        file = fopen(stats_file_path, "r");
        if (file == NULL) {
            perror("Error opening statistics file");
            exit(1);
        }
        fscanf(file, "%llu", &rx_packets1);
        fclose(file);

        snprintf(stats_file_path, sizeof(stats_file_path), "/sys/class/net/%s/statistics/tx_packets", interface_name);
        file = fopen(stats_file_path, "r");
        if (file == NULL) {
            perror("Error opening statistics file");
            exit(1);
        }
        fscanf(file, "%llu", &tx_packets1);
        fclose(file);

        time(&time1); // Get current time
        sleep(1); 
        snprintf(stats_file_path, sizeof(stats_file_path), "/sys/class/net/%s/statistics/rx_packets", interface_name);
        file = fopen(stats_file_path, "r");
        if (file == NULL) {
            perror("Error opening statistics file");
            exit(1);
        }
        fscanf(file, "%llu", &rx_packets2);
        fclose(file);

        snprintf(stats_file_path, sizeof(stats_file_path), "/sys/class/net/%s/statistics/tx_packets", interface_name);
        file = fopen(stats_file_path, "r");
        if (file == NULL) {
            perror("Error opening statistics file");
            exit(1);
        }
        fscanf(file, "%llu", &tx_packets2);
        fclose(file);

        time(&time2); // Get current time

        // Calculate download and upload speeds in packets per second
        double download_speed = (rx_packets2 - rx_packets1) / difftime(time2, time1);
        double upload_speed = (tx_packets2 - tx_packets1) / difftime(time2, time1);

        printf("Download speed: %.2f packets/s\n", download_speed);
        printf("Upload speed: %.2f packets/s\n", upload_speed);
    
}

// Function to calculate and print network speed
void printNetworkSpeed(const char* interface) {
    FILE* statFile;
    unsigned long long int prevRxBytes = 0, prevTxBytes = 0;
    unsigned long long int currRxBytes = 0, currTxBytes = 0;
    unsigned long long int rxSpeed, txSpeed;
   // while (1) {  // bad idea will cuase scrolling of information. 
        // Read network interface statistics
        char statFilePath[100];
        snprintf(statFilePath, sizeof(statFilePath), "/sys/class/net/%s/statistics", interface);

        statFile = fopen(statFilePath, "r");
        if (statFile == NULL) {
            fprintf(stderr, "Error opening interface statistics file.\n");
            exit(1);
        }

        fscanf(statFile, "rx_bytes %llu\n", &currRxBytes);
        fscanf(statFile, "tx_bytes %llu\n", &currTxBytes);

        fclose(statFile);

        // Calculate network speeds
        rxSpeed = (currRxBytes - prevRxBytes) / 1024; // in KB/s
        txSpeed = (currTxBytes - prevTxBytes) / 1024; // in KB/s

        // Print network speeds
        printf("Network speed tests [/sys/class/net/%s],\n", interface);
        printf("Download: %llu KB/s\t", rxSpeed);
        printf("Upload: %llu KB/s\n", txSpeed);

        // Update previous values
        prevRxBytes = currRxBytes;
        prevTxBytes = currTxBytes;
  FILE *file = fopen("/proc/net/dev", "r");
    char line[256];
    
    if (file == NULL) {
        perror("Error opening /proc/net/dev");
        exit(1);
    }

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "wlan0")) {  // you can replace "wlan0" okay.  
            unsigned long long int rx_bytes, tx_bytes;
            sscanf(line + 5, "%llu %*u %*u %*u %*u %*u %*u %*u %*u %llu", &rx_bytes, &tx_bytes);
            printf("Open searching path [/proc/net/dev],\n");
            printf("Received: %llu bytes\t", rx_bytes);
            printf("Transmitted: %llu bytes\n", tx_bytes);
        }
    }
    fclose(file);
    
        // Sleep for 1 second (adjust as needed)
        sleep(1);
   // }   // we can close our while(1) loop here such a bad idea i guess.
          
}

void printNetworkInfo(const char* interface) {
    FILE* iwFile;
    char signalStrength[100];
    char networkInfo[100];
   // while (1) {
        // Read signal strength using iw command
        char iwCommand[100];
        snprintf(iwCommand, sizeof(iwCommand), "iw dev %s link | grep signal | awk '{print $2}'", interface);

        iwFile = popen(iwCommand, "r");
        if (iwFile == NULL) {
            fprintf(stderr, "Error running iw command.\n");
            exit(1);
        }

        fgets(signalStrength, sizeof(signalStrength), iwFile);
        pclose(iwFile);
        // Print signal strength
        printf("Signal Strength 1st pass: %s", signalStrength);
       // Read network information using iw command
       
        snprintf(iwCommand, sizeof(iwCommand), "iw dev %s link", interface);

        iwFile = popen(iwCommand, "r");
        if (iwFile == NULL) {
            fprintf(stderr, "Error running iw command.\n");
            exit(1);
        }

        // Read and display network information
        printf("Network Information for %s:\n", interface);
        while (fgets(networkInfo, sizeof(networkInfo), iwFile) != NULL) {
            printf("%s", networkInfo);
        }
        
        pclose(iwFile);

    // }
}

// End functions and start our main part of program.

int main() {
    int numIterations = 40; // loop a total of 40 times. 

    for (int i = 0; i < numIterations; i++) {
        // Clear the screen using ANSI escape code
        printf("\033[H\033[J");
        system("clear"); 
        int networkStatus;
        char wifiSpeed[100];
        char ipAddress[100];
        char macAddress[100];
        char dnsServers[100];
        char gatewayAddress[100];

        // Check network status using system command (Linux/Unix)
        networkStatus = system("ping -c 1 google.com > /dev/null 2>&1");

        // Retrieve WiFi speed using system command (Linux/Unix)
        FILE* speedFile = popen("iwconfig wlan0 | grep 'Bit Rate'", "r");
        if (speedFile != NULL) {
            fgets(wifiSpeed, sizeof(wifiSpeed), speedFile);
            pclose(speedFile);
        } else {
            strcpy(wifiSpeed, "Speed information not available");
        }

        // Retrieve IP address using system command (Linux/Unix)
        FILE* ipFile = popen("ip -4 addr show wlan0 | grep -oP '(?<=inet\\s)\\d+(\\.\\d+){3}'", "r");
        if (ipFile != NULL) {
            fgets(ipAddress, sizeof(ipAddress), ipFile);
            pclose(ipFile);
        } else {
            strcpy(ipAddress, "IP address not available");
        }
         
        // Check network status result
        printf("Network status "); 
        if (WIFEXITED(networkStatus) && WEXITSTATUS(networkStatus) == 0) {
            printf("[Network is up]\n");
        } else {
            printf("[Network is down]\n");
        }

// Retrieve MAC address using system command (Linux/Unix) ifconfig,
// We can use this comamnd but it is not installed by defualt. But we can, 
// rethink this to run faster with another command all orange pi zero 3's have. 
//        FILE* macFile = popen("ifconfig | grep -Eo '([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})' | head -1", "r");
//        if (macFile != NULL) {
//            fgets(macAddress, sizeof(macAddress), macFile);
//            pclose(macFile);
//        } else {
//            strcpy(macAddress, "MAC address not available");
//        }
        
        FILE* macFile = popen("ip link show dev wlan0 | awk '/ether/ {print $2}'", "r");
        if (macFile != NULL) {
            fgets(macAddress, sizeof(macAddress), macFile);
            pclose(macFile);
        } else {
            strcpy(macAddress, "MAC address not available");
        }

        // Retrieve DNS server addresses using system command (Linux/Unix)
        FILE* dnsFile = popen("cat /etc/resolv.conf | grep 'nameserver' | awk '{print $2}' | tr '\n' ' '", "r");
        if (dnsFile != NULL) {
            fgets(dnsServers, sizeof(dnsServers), dnsFile);
            pclose(dnsFile);
        } else {
            strcpy(dnsServers, "DNS servers not available");
        }

        // Retrieve default gateway address using system command (Linux/Unix)
        FILE* gatewayFile = popen("ip route | grep 'default' | awk '{print $3}'", "r");
        if (gatewayFile != NULL) {
            fgets(gatewayAddress, sizeof(gatewayAddress), gatewayFile);
            pclose(gatewayFile);
        } else {
            strcpy(gatewayAddress, "Gateway address not available");
        }


        printf("MAC Address: %s", macAddress);
        printf("DNS Servers: %s\n", dnsServers);
        printf("Gateway Address: %s", gatewayAddress);
        // Display WiFi signal strength 
        printNetworkInfo("wlan0");   
        // Display WiFi speed and IP address
        printf("Google ping WiFi Speed.. %sIP Address %s", wifiSpeed, ipAddress);
        printNetworkSpeed("wlan0");  // keep this going in the loop. don't go fruit-loops on me now.       
        printNetworkPackets();
        usleep(4000000); // sleep for 4 seconds
    } // end loop in 5 seconds total time. 
     
    return 0;  // clean exit or program. 
}
