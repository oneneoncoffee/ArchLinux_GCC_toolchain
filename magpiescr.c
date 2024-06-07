#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>

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

    return 0;
}

void setCursorPosition(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void setTextColor(int color) {
    switch (color) {
        case 0: printf("\033[0;30m"); break; // Black
        case 1: printf("\033[0;31m"); break; // Red
        case 2: printf("\033[0;32m"); break; // Green
        case 3: printf("\033[0;33m"); break; // Yellow
        case 4: printf("\033[0;34m"); break; // Blue
        case 5: printf("\033[0;35m"); break; // Magenta
        case 6: printf("\033[0;36m"); break; // Cyan
        case 7: printf("\033[0;37m"); break; // White
        default: printf("\033[0m"); break; // Reset to default
    }
}

void displayRandomCharacter() {
    char ch;
    int randomType = rand() % 4;

    if (randomType == 0) {
        ch = rand() % 94 + 33; // Random ASCII character
    } else if (randomType == 1) {
        ch = 'A' + rand() % 26; // Random uppercase letter
    } else if (randomType == 2) {
        ch = rand() % 2 ? '0' : '1'; // Random binary digit
    } else {
        ch = rand() % 16; // Random hexadecimal digit
        if (ch < 10) {
            ch += '0';
        } else {
            ch += 'A' - 10;
        }
        int randomType = rand() % 6;
        ch = rand()  % 99 + 51;
    }

    printf("%c", ch);
}

void clearScreen() {
    printf("\033[2J");
}

void scrollScreen(int lines) {
    printf("\033[%dB", lines);
}

char *jokes[] = {
    "Why don't scientists trust atoms? Because they make up everything!",
    "Parallel lines have so much in common. It's a shame they'll never meet.",
    "Why don't skeletons fight each other? They don't have the guts.",
    "I told my wife she should embrace her mistakes. She gave me a hug.",
    "Why don't oysters donate to charity? Because they are shellfish.",
    "I used to play piano by ear, but now I use my hands and fingers.",
    "Why did the scarecrow win an award? Because he was outstanding in his field!",
    "I only know 25 letters of the alphabet. I don't know y.",
    "Why couldn't the bicycle stand up by itself? It was two-tired.",
    "How do you organize a space party? You planet!",
    "What do you call a fish wearing a bowtie? Sofishticated.",
    "I told my computer I needed a break, and now it won't stop sending me spam vacation ads.",
    "I told the wife she was drawing her eyebrows too high. She looked surprised!",
    "What do you call fake spaghetti? An impasta.",
    "Why did the computer catch a cold? it left its Windows open.",
    "I am on a whiskey diet. I just lost three days already.",
    "Why do programmers hate nature? it has too many bugs.",
    "Why do programmers prefer dark mode? Because light attracts bugs."
};

void displayRandomMessage() {
    int numJokes = sizeof(jokes) / sizeof(jokes[0]);
    int randomIndex = rand() % numJokes;

    setTextColor(rand() % 8);
    printf("\n%s\n", jokes[randomIndex]);
    setTextColor(7);
}

volatile sig_atomic_t clear_screen_flag = 0;
volatile sig_atomic_t random_message_flag = 0;

void clear_screen_handler(int signum) {
    clear_screen_flag = 1;
}

void random_message_handler(int signum) {
    random_message_flag = 1;
}

int main() {
    srand((unsigned int)time(NULL));
    int vslloop = 0;
    clock_t start_time, end_time;
    double cpu_time_used;
    int loopCounter = 0;
    int loopThreshold = 1000;
    start_time = clock();
    clearScreen();
    setTextColor(7);
    displayRandomMessage();

    int colorChangeInterval = 30000; // 30000 milliseconds
    int elapsedTime = 0;
    int displayInterval = 40;       // 40 milliseconds (faster scrolling)
    int loopCount = 1000;
    int clearScreenInterval = 30000; // 30000 milliseconds
    int randomMessageInterval = 60; // 60 seconds (1 minute)
    int terminalWidth, terminalHeight;

    // Get the terminal size using ioctl
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    terminalWidth = w.ws_col;
    terminalHeight = w.ws_row;

    // Set up the timer for clearing the screen
    struct itimerval clear_screen_timer;
    signal(SIGALRM, clear_screen_handler);
    clear_screen_timer.it_value.tv_sec = clearScreenInterval / 1000;
    clear_screen_timer.it_value.tv_usec = (clearScreenInterval % 1000) * 1000;
    clear_screen_timer.it_interval.tv_sec = clearScreenInterval / 1000;
    clear_screen_timer.it_interval.tv_usec = (clearScreenInterval % 1000) * 1000;
    setitimer(ITIMER_REAL, &clear_screen_timer, NULL);

    // Set up the timer for displaying random messages
    struct itimerval random_message_timer;
    signal(SIGALRM, random_message_handler);
    random_message_timer.it_value.tv_sec = randomMessageInterval;
    random_message_timer.it_value.tv_usec = 0;
    random_message_timer.it_interval.tv_sec = randomMessageInterval;
    random_message_timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &random_message_timer, NULL);

    scrollScreen(terminalHeight); // Scroll down to fill the screen

    for (int loop = 0; loop < loopCount; ++loop) {
        while (!kbhit()) {
            int randomX = rand() % terminalWidth;
            int randomY = rand() % terminalHeight;

            loopCounter++;
            if (loopCounter % loopThreshold == 0) {
            loopCounter = 0;
            clearScreen();
            displayRandomMessage();
            }

            setTextColor(rand() % 8);
            setCursorPosition(randomX, randomY);
            displayRandomCharacter();

            if (elapsedTime >= colorChangeInterval) {
                elapsedTime = 0;
                setTextColor(rand() % 8);
            }

            usleep(displayInterval * 1000);
            elapsedTime += displayInterval;
        }

        if (kbhit()) {
            clearScreen();
            break;
        }

        if (clear_screen_flag) {
            clear_screen_flag = 0;
            clearScreen();
        }

        if (random_message_flag) {
            random_message_flag = 0;
            displayRandomMessage();
        }

        setTextColor(7);
        vslloop++;
        if (vslloop > 1000) {
        clearScreen();
        displayRandomMessage();
        } 
    }
    end_time = clock();
    clearScreen();
    setTextColor(7);
    displayRandomMessage();
    cpu_time_used = ((double) (end_time = start_time)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);
    return 0;
}
