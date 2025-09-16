#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

void update_time(int *hour, int *min, int *sec) {
    time_t now;
    time(&now);
    struct tm *timeinfo = localtime(&now);
    *min = timeinfo->tm_min;
    *hour = timeinfo->tm_hour;
    *sec = timeinfo->tm_sec;
}

int main(void) {
    time_t now;
    int min, hour, second;
    printf("Current Time:\n %s", ctime(&now));
    printf("testing:\n");

    // get current time
    struct tm *timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    min = timeinfo->tm_min;
    hour = timeinfo->tm_hour;
    second = timeinfo->tm_sec;


    // starting ncursers drawing
    initscr();
    // main loop
    while(1) {
        update_time(&hour, &min, &second); // update time
        clear();
        mvprintw(0, 0, "%02d:%02d:%02d\n", hour, min, second);
        refresh();
        sleep(1);
    }
    getch();
    endwin();

    return 0;
}