#include "main.h"

struct player_s you;
bool quit = false;

FILE *logFile = NULL;
void plog(const char *fmt, ...) {
	va_list s;
	if (!logFile) return;
	va_start(s, fmt);
	vfprintf(logFile, fmt, s);
	va_end(s);
}

void debugDraw(MapTemplate* m) {
    int ch;
    for (int y = 0; y < m->h; y++) {
        move(y,0);
        for (int x = 0; x < m->w; x++) {
            ch = m->at(x,y);
            addch(ch);
        }
    }
    ch = m->at(1,1);
    //printw("%d %d c: %c", m->w, m->h, ch);
}

int main(void) {
    wchar_t c;
    logFile = fopen("debug.log", "w");
    initialize_curses();
    shared_ptr<MapTemplate> map = make_shared<MapTemplate>("fov.level");
//plog("map(0,0): %c\n", map->at(0,0));
//plog("map(1,1): %c\n", map->at(1,1));
    you.pos = Point(5,5);
    while (!quit) {
        clear();
        debugDraw(map.get());
        move(you.pos.y,you.pos.x);
        addch('@');
        move(you.pos.y,you.pos.x);
        refresh();
        c = getch();
        //fprintf(log, "key pressed: %o\n", c);
        switch(c) {
            case 'q':
                quit = true;
                break;
            case KEY_A1: case KEY_HOME:
                if (you.pos.y > 0) you.pos.y -= 1;
                if (you.pos.x > 0) you.pos.x -= 1;                
                break;
            case KEY_A3: case KEY_PPAGE:
                if (you.pos.y > 0) you.pos.y -= 1;
                if (you.pos.x < 79) you.pos.x += 1;                
                break;
            case KEY_C1: case KEY_END:
                if (you.pos.y > 20) you.pos.y += 1;
                if (you.pos.x > 0) you.pos.x -= 1;                
                break;
            case KEY_C3: case KEY_NPAGE:
                if (you.pos.y < 20) you.pos.y += 1;
                if (you.pos.x < 79) you.pos.x += 1;                
                break;
            case KEY_UP:
                if (you.pos.y > 0) you.pos.y -= 1;
                break;
            case KEY_DOWN:
                if (you.pos.y < 20) you.pos.y += 1;
                break;
            case KEY_LEFT:
                if (you.pos.x > 0) you.pos.x -= 1;
                break;
            case KEY_RIGHT:
                if (you.pos.x < 79) you.pos.x += 1;
                break;
            case KEY_B2: case '5':
                //fprintf(log, "b2\n");
                break;
            default:
                move(0,0); printw("unknown character: %o    \n", c);
                break;
        }
    }
    fclose(logFile);
    shutdown_curses();
    return 0;
}

