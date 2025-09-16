#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// A small 5-row ASCII font for digits 0-9 and colon
static const char *DIGITS[10][5] = {
    { " ### ", "#   #", "#   #", "#   #", " ### " }, // 0
    { "  #  ", " ##  ", "  #  ", "  #  ", " ### " }, // 1
    { " ### ", "#   #", "   # ", "  #  ", "#####" }, // 2
    { " ### ", "#   #", "  ## ", "#   #", " ### " }, // 3
    { "#   #", "#   #", "#####", "    #", "    #" }, // 4
    { "#####", "#    ", "#### ", "    #", "#### " }, // 5
    { " ### ", "#    ", "#### ", "#   #", " ### " }, // 6
    { "#####", "    #", "   # ", "  #  ", "  #  " }, // 7
    { " ### ", "#   #", " ### ", "#   #", " ### " }, // 8
    { " ### ", "#   #", " ####", "    #", " ### " }  // 9
};
static const char *COLON[5] = {
    "  ",
    " #",
    "  ",
    " #",
    "  "
};

static void update_time(int *hour, int *min, int *sec) {
    time_t now;
    time(&now);
    struct tm *ti = localtime(&now);
    *hour = ti->tm_hour;
    *min  = ti->tm_min;
    *sec  = ti->tm_sec;
}

static void draw_big_clock_centered(int hour, int min, int sec) {
    // Build "HH:MM:SS" as characters to render with the big font
    char text[9];
    snprintf(text, sizeof(text), "%02d:%02d:%02d", hour, min, sec);

    // Determine the big glyph width per character
    int widths[8];
    int total_width = 0;
    for (int i = 0; i < 8; i++) {
        widths[i] = (text[i] == ':') ? 2 : 5; // our fonts above
        total_width += widths[i];
        if (i < 7) total_width += 1; // 1 column spacing between glyphs
    }
    int glyph_height = 5;

    // Center in the terminal
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int start_y = (rows - glyph_height) / 2;
    if (start_y < 0) start_y = 0;
    int start_x = (cols - total_width) / 2;
    if (start_x < 0) start_x = 0;

    // Draw line by line
    for (int row = 0; row < glyph_height; row++) {
        int x = start_x;
        for (int i = 0; i < 8; i++) {
            const char *slice;
            if (text[i] == ':') {
                slice = COLON[row];
            } else {
                int d = text[i] - '0';
                slice = DIGITS[d][row];
            }
            mvprintw(start_y + row, x, "%s", slice);
            x += widths[i] + 1; // move past glyph + spacing
        }
    }
}

int main(void) {
    // If you want to also print the current time once with stdio:
    time_t now;
    time(&now);
    printf("Current Time:\n %s\n", ctime(&now));
    printf("Starting ncurses clock. Press 'q' to quit.\n");

    // Init ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);   // non-blocking getch

    // Main loop
    while (1) {
        int h, m, s;
        update_time(&h, &m, &s);
        clear();
        draw_big_clock_centered(h, m, s);
        refresh();

        // Quit on 'q'
        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        // Sleep ~1s without blocking input too long
        napms(200);
        // re-check a few times to feel smoother and catch 'q' quickly
        for (int i = 0; i < 4; i++) {
            ch = getch();
            if (ch == 'q' || ch == 'Q') { endwin(); return 0; }
            napms(200);
        }
    }

    endwin();
    return 0;
}

