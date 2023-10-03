#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#define MAX_PORT 65535
// Global array to track open sockets
int sockets[MAX_PORT + 1];
// Global variable to track whether a timeout occurred
volatile int timeout_occurred = 0;

// Signal handler for the alarm
void handle_alarm(int signum) {
    timeout_occurred = 1;
}


void scan_ports(const char *host, int start_port, int end_port) {
    struct sockaddr_in target;
    int sock;
    int err;

    for (int port = start_port; port <= end_port; port++) {
        memset(&target, 0, sizeof(target));
        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = inet_addr(host);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        err = connect(sock, (struct sockaddr *)&target, sizeof(target));
        if (err == 0) {
            printf("%d [ACTIVE SOCKET FOUND]\n", port);
        }

        close(sock);
      
    }
}

void scan_ports_services(const char *host, int start_port, int end_port) {
    struct sockaddr_in target;
    int sock;
    int err;
    struct servent *service;

    for (int port = start_port; port <= end_port; port++) {
        memset(&target, 0, sizeof(target));
        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = inet_addr(host);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        err = connect(sock, (struct sockaddr *)&target, sizeof(target));
        if (err == 0) {
            service = getservbyport(htons(port), "tcp");
            if (service) {
                printf("Port %d (%s) is open\n", port, service->s_name);
            } else {
                printf("Port %d (unknown) is open\n", port);
            }
        }

        close(sock);
        
    }
}





int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <hostname> <start_port> <end_port>\n", argv[0]);
        printf("Example: %s 192.168.0.100 1 65535\n", argv[0]);
        printf("NOTE:\n");
        printf("The max port range is 1 to 65535 and will error out if any ");
        printf("number given is over.\n");
        printf("\nABOUT: pscan A basic port scanner tool for the orange PI zero 3 .. \n");
        sleep(1);  
        exit(EXIT_FAILURE);
    }

    const char *host = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);
    int timeout_seconds = 5; // Set a 5-second timeout (adjust as needed)
    
    if (start_port < 1 || start_port > MAX_PORT || end_port < 1 || end_port > MAX_PORT || start_port > end_port) {
        printf("Invalid port range\n");
        exit(EXIT_FAILURE);
    }
   
    printf("Scanning for open ports,\n");
    scan_ports(host, start_port, end_port);
    sleep(5); 
    printf("Listing open ports and services found,\n");
    scan_ports_services(host, start_port, end_port);
    sleep(5); 

    // Initialize the sockets array
    for (int i = 1; i <= MAX_PORT; i++) {
        sockets[i] = -1; // Initialize all elements as closed
    }
    printf("Searching first onehundred ports,");

    // Set up the signal handler for alarm
    signal(SIGALRM, SIG_IGN);

    for (int port = start_port; port <= 100; port++) {
        struct sockaddr_in target;
        int sock;
        int err;

        memset(&target, 0, sizeof(target));
        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = inet_addr(host);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
             
        }

        // Set the alarm timeout
        alarm(timeout_seconds);

        err = connect(sock, (struct sockaddr *)&target, sizeof(target));
        if (err == 0) {
            printf("\n[Port %d is open]\n", port);

            // Perform banner grabbing for the open port
            char banner[1024]; // Adjust buffer size as needed
            ssize_t bytes_read = recv(sock, banner, sizeof(banner) - 1, 0);
            if (bytes_read > 0) {
                banner[bytes_read] = '\0';
                printf("\nBanner for Port %d: %s\n", port, banner);
            } else {
                printf("\n[Banner for Port %d: N/A]\n", port);
            }
        } else {
            usleep(11901);
        }
        
        // Reset the alarm and close the socket
        alarm(0);
        close(sock);
    }


    // Set up the signal handler for alarm
    signal(SIGALRM, SIG_IGN);

    for (int port = start_port; port <= end_port; port++) {
        struct sockaddr_in target;
        int sock;
        int err;

        memset(&target, 0, sizeof(target));
        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        target.sin_addr.s_addr = inet_addr(host);

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Set a timeout for the connect operation
        struct timeval timeout;
        timeout.tv_sec = timeout_seconds;
        timeout.tv_usec = 0;

        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        // Set the alarm timeout
        alarm(timeout_seconds);

        err = connect(sock, (struct sockaddr *)&target, sizeof(target));
        if (err == 0) {
            printf("Port %d is open\n", port);
            // Perform banner grabbing for the open port (if needed)
        } else {
            if (errno == ETIMEDOUT) {
                printf("Port %d connection timed out\n", port);
            } else {
                //printf("Port %d is closed\n", port);
            
            }
        }

        // Reset the alarm and close the socket
        alarm(0);
        close(sock);
    }

  printf("\n[EXIT]\n");
  return 0;

}
