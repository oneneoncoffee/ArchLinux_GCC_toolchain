#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    double loadavg[3];
    if (getloadavg(loadavg, 3) == -1) {
        perror("Error getting load averages");
        return 1;
    }

    printf("Load Average: %.2f, %.2f, %.2f\n", loadavg[0], loadavg[1], loadavg[2]);
    sleep(1);
    printf("Running Programs:\n");

    FILE *fp = popen("ps aux", "r");
    if (fp == NULL) {
        perror("Error executing ps command");
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
        sleep(1);
    }

    pclose(fp);
    printf("\nSystem Load: ");
    fflush(stdout);
    system("uptime | awk '{print $10,$11,$12}'");
    printf("\n\nRunning Programs:\n");
    fflush(stdout);
    system("ps -eo pid,comm --sort=-%cpu | head -n 5 | awk '{printf \"%-10s %s\\n\", $1, $2}'");
    if (getloadavg(loadavg, 3) != -1) {
        printf("Load averages: %.2f %.2f %.2f\n", loadavg[0], loadavg[1], loadavg[2]);
    } else {
        perror("Error getting load averages");
    }

    return 0;
}
