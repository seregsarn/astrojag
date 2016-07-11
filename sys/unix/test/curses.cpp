#include "main.h"

bool curses_initialized=false;

void shutdown_curses(void) {
    if (!curses_initialized) return;
	endwin();
    curses_initialized = false;
}
void initialize_curses(void) {
	initscr();
	clear();
	start_color();
    use_default_colors();
	init_pair(1, COLOR_BLUE, -1);
	init_pair(2, COLOR_GREEN, -1);
	init_pair(3, COLOR_CYAN, -1);
	init_pair(4, COLOR_RED, -1);
	init_pair(5, COLOR_MAGENTA, -1);
	init_pair(6, COLOR_YELLOW, -1);
	init_pair(7, COLOR_WHITE, -1);
	init_pair(8, COLOR_WHITE, -1);
    if (COLORS >= 16)
    {
        init_pair(9, COLOR_WHITE, -1);
        init_pair(10, COLOR_RED + 8, -1);
        init_pair(11, COLOR_GREEN + 8, -1);
        init_pair(12, COLOR_YELLOW + 8, -1);
        init_pair(13, COLOR_BLUE + 8, -1);
        init_pair(14, COLOR_MAGENTA + 8, -1);
        init_pair(15, COLOR_CYAN + 8, -1);
        init_pair(16, COLOR_WHITE + 8, -1);
    }
	nonl();
	noecho();
	cbreak();
	curs_set(2);
	intrflush(stdscr, FALSE);
	scrollok(stdscr, FALSE);
	keypad(stdscr, TRUE);
	atexit(shutdown_curses);
	curses_initialized = true;
}
