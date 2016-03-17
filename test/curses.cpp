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
	/*
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_RED, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	*/
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
