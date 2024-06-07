#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>

int main() {
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Unable to open display\n");
        return 1;
    }

    char **font_list;
    int count;

    // Get the list of available fonts
    font_list = XListFonts(display, "*", 1000, &count);

    if (font_list) {
        printf("Available Fonts:\n");
        for (int i = 0; i < count; i++) {
            printf("%s\n", font_list[i]);
        }

        // Free the font list
        XFreeFontNames(font_list);
    } else {
        fprintf(stderr, "Failed to get font list\n");
    }

    XCloseDisplay(display);

    return 0;
}
