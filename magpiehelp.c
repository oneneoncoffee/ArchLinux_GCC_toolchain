// for this program to function please install lynx.
// command line : sudo apt -y install lynx
// Lynx is a text based html browser for bash and I recomend it.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    const char *a2command = "which lynx";
    const char *htmlFilePath = "help.html";
    char command[256];
    FILE *fp = popen(a2command, "r");
    if (fp == NULL) {
        perror("Error executing command");
        return EXIT_FAILURE;
    }

    char path[1024];
      if (fgets(path, sizeof(path), fp) != NULL) {
        printf("lynx is installed at: %s\n\n", path);
    } else {
        printf("lynx is not installed at the terminal type,\n");
        printf("sudo apt -y install lynx\n");
        return 0;
    }
    snprintf(command, sizeof(command), "lynx -dump %s", htmlFilePath);
    system(command);
    return 0;
}
