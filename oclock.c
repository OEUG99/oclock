#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

int get_term_length(void);

int main(void) {
    time_t now;
    int min, hour, second;
    int term_length;
    struct tm *timeinfo;
    time(&now);
    printf("Current Time:\n %s", ctime(&now));
    printf("testing:\n");
    timeinfo = localtime(&now);
    min = timeinfo->tm_min;
    hour = timeinfo->tm_hour;
    second = timeinfo->tm_sec;
    printf("%d:%d:%d\n", hour, min, second);
    
    term_length = get_term_length();

    // starting ncursers test
    initscr();
    printw("ncurses works!\n");
    refresh();
    getch();
    endwin();

    return 0;
}

int get_term_length(void){
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        return -1; // fallback in case of error
    }
    return w.ws_col; // number of columns
}

