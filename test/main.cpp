#include "main.h"

struct player_s you;
bool quit = false;

shared_ptr<ThpLogger> logger;

void debugDraw(Map* m) {
    int ch;
    for (int y = 0; y < m->h; y++) {
        move(y,0);
        for (int x = 0; x < m->w; x++) {
            ch = m->at(x,y);
            addch(ch);
        }
    }
    //ch = m->at(1,1);
    //printw("%d %d c: %c", m->w, m->h, ch);
}

void drawFov(Map* m, Point pos) {
    attron(COLOR_PAIR(6)|A_BOLD);
    FOV fov(m, pos);
    fov.maxDistance = 10;
    fov.visit([m, pos](FOV* context, Point p) {
        if (pos.manhattan(p) > 10) return;
        move(p.y, p.x);
        addch(m->at(p.x,p.y));
    });
    attroff(COLOR_PAIR(6)|A_BOLD);
}

bool debugPhase = false;
bool move(Map *m, Point &p, Point dir) {
    Point dest = p + dir;
    if (!m->inBounds(dest.x, dest.y)) return false;
    if (!debugPhase && m->at(dest.x, dest.y) == '#') return false;
    // success
    p = dest; return true;
}

int main(void) {
    wchar_t c;
    logger = make_shared<ThpLogger>("debug.log");
    initialize_curses();
    shared_ptr<MapTemplate> tpl = make_shared<MapTemplate>("fov.level");
    shared_ptr<Map> map = make_shared<Map>(tpl->w, tpl->h);
    for (int y = 0; y < tpl->h; y++)
        for (int x = 0; x < tpl->w; x++)
            map->at(x,y) = tpl->at(x,y);
    you.pos = Point(5,5);
    while (!quit) {
        clear();
        debugDraw(map.get());
        drawFov(map.get(), you.pos);
        move(you.pos.y,you.pos.x);
        addch('@');
        move(you.pos.y,you.pos.x);
        refresh();
        c = getch();
        switch(c) {
            case 'q':
                quit = true;
                break;
            case KEY_A1: case KEY_HOME:     move(map.get(), you.pos, Point(-1,-1)); break;
            case KEY_A3: case KEY_PPAGE:    move(map.get(), you.pos, Point(+1,-1)); break;
            case KEY_C1: case KEY_END:      move(map.get(), you.pos, Point(-1,+1)); break;
            case KEY_C3: case KEY_NPAGE:    move(map.get(), you.pos, Point(+1,+1)); break;
            case KEY_UP:                    move(map.get(), you.pos, Point( 0,-1)); break;
            case KEY_DOWN:                  move(map.get(), you.pos, Point( 0,+1)); break;
            case KEY_LEFT:                  move(map.get(), you.pos, Point(-1, 0)); break;
            case KEY_RIGHT:                 move(map.get(), you.pos, Point(+1, 0)); break;
            case KEY_B2: case '5':
                break;
            case 'z':
                debugPhase = !debugPhase;
                printw("phase %s", debugPhase ? "on" : "off");
                break;
            default:
                move(0,0); printw("unknown character: %o    \n", c);
                break;
        }
    }
    shutdown_curses();
    return 0;
}

