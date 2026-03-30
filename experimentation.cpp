#include <bits/stdc++.h>
#include <curses.h>
using namespace std;

int main() {
    // initialize screen
    initscr();

    // print to screen
    // printw("Hello World\n");
    
    // refresh screen output
    // refresh();
    // mvprintw(10, 20, "Hello World\n");

    // create new window
    WINDOW *win = newwin(15, 17, 2, 10);
    refresh();

    // box(win, 1, 0.5);

    // write in window
    mvwprintw(win, 0, 1, "Hello World\n");
    wrefresh(win);

    // pause output
    getch();

    // deallocate memory and end ncurses
    endwin();
    return 0;
}