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
        (void)context;
    });
    attroff(COLOR_PAIR(6)|A_BOLD);
}

void drawDM(DijkstraMap *dm) {
    const char *values = "0123456789ABCDEF ";
    int v;
    Point dim = dm->getDimensions();
    for (int y = 0; y < dim.y; y++) {
        move(y,0);
        for (int x = 0; x < dim.x; x++) {
            v = dm->at(x,y);
            if (v < 0) attron(A_REVERSE); else attroff(A_REVERSE);
            v = abs(v);
            if (v == INT_MAX) move(y,x+1);
                else addch(values[v % 16]);
        }
    }
}

void drawDM2(DijkstraMap *dm) {
    int v;
    Point grad;
    Point dim = dm->getDimensions();
    for (int y = 0; y < dim.y; y++) {
        move(y,0);
        for (int x = 0; x < dim.x; x++) {
            v = dm->at(x,y);
            grad = dm->gradient(Point(x,y));
            char dir = '.';
            if (grad.x < 0 && grad.y < 0) dir = '`';
            if (grad.x == 0 && grad.y < 0) dir = '^';
            if (grad.x > 0 && grad.y < 0) dir = '/';

            if (grad.x < 0 && grad.y == 0) dir = '<';
            if (grad.x > 0 && grad.y == 0) dir = '>';

            if (grad.x < 0 && grad.y > 0) dir = 'L';
            if (grad.x == 0 && grad.y > 0) dir = 'v';
            if (grad.x > 0 && grad.y > 0) dir = '\\';
            // draw
            if (v == INT_MAX || v == INT_MIN || (grad.x == 0 && grad.y == 0)) move(y,x+1);
            else addch(dir);
        }
    }
}

void drawPath(shared_ptr<Map> m, Point from, Point to) {
    // stuh!
    Pathfinder pf(m.get());
    auto path = pf.findPath(from, to);
    path->walk([](Point p)->bool {
        move(p.y, p.x);
        addch('*');
        chgat(0, 0, 6, NULL);
        return true;
    });
}
void drawLine(Point from, Point to) {
    // stuh!
    Line(from, to, [](Point& p)->bool {
        move(p.y, p.x);
        addch('*');
        chgat(0, 0, 6, NULL);
        return true;
    });
}

bool debugPhase = false;
bool move(Map *m, Point &p, Point dir) {
    Point dest = p + dir;
    if (!m->inBounds(dest.x, dest.y)) return false;
    if (!debugPhase && m->at(dest.x, dest.y) == '#') return false;
    // success
    p = dest; return true;
}

bool debugDM = false;
bool debugFlee = false;
bool debugPath = false;
bool debugLine = false;
RNG rng;

int main(void) {
    wchar_t c;
    logger = make_shared<ThpLogger>("debug.log");
    /*
    rng = RNG::deserialize("[\
        1705,\
        10092,\
        47666,\
        3041,\
        44468,\
        39821,\
        15483,\
        19805\
]");
    //*/
    //log("rng: %s\n", rng.serialize().c_str());
    initialize_curses();
    shared_ptr<MapTemplate> tpl = make_shared<MapTemplate>("fov.level");
    shared_ptr<Map> map = make_shared<Map>(tpl->w, tpl->h);
    for (int y = 0; y < tpl->h; y++)
        for (int x = 0; x < tpl->w; x++)
            map->at(x,y) = tpl->at(x,y);
    you.pos = Point(5,5);
    DijkstraMap dmap(map.get());
    DijkstraMap flee(map.get());
    while (!quit) {
        debugDraw(map.get());
        drawFov(map.get(), you.pos);
        if (debugDM) drawDM2(&dmap);
        if (debugFlee) drawDM2(&flee);
        if (debugPath) {
            if (debugLine) {
                drawLine(you.pos, you.cursor);
            } else {
                drawPath(map, you.pos, you.cursor);
            }
        }
        move(you.pos.y,you.pos.x);
        if (!debugLine) addch('@');
        if (debugPath) move(you.cursor.y, you.cursor.x);
        else move(you.pos.y,you.pos.x);
        refresh();
        c = getch();
        clear();
        switch(c) {
            case 'q':
                quit = true;
                break;
            case KEY_A1: case KEY_HOME:     move(map.get(), debugPath ? you.cursor : you.pos, Point(-1,-1)); break;
            case KEY_A3: case KEY_PPAGE:    move(map.get(), debugPath ? you.cursor : you.pos, Point(+1,-1)); break;
            case KEY_C1: case KEY_END:      move(map.get(), debugPath ? you.cursor : you.pos, Point(-1,+1)); break;
            case KEY_C3: case KEY_NPAGE:    move(map.get(), debugPath ? you.cursor : you.pos, Point(+1,+1)); break;
            case KEY_UP:                    move(map.get(), debugPath ? you.cursor : you.pos, Point( 0,-1)); break;
            case KEY_DOWN:                  move(map.get(), debugPath ? you.cursor : you.pos, Point( 0,+1)); break;
            case KEY_LEFT:                  move(map.get(), debugPath ? you.cursor : you.pos, Point(-1, 0)); break;
            case KEY_RIGHT:                 move(map.get(), debugPath ? you.cursor : you.pos, Point(+1, 0)); break;
            case KEY_B2: case '5':
                break;
            case 'z':
                debugPhase = !debugPhase;
                move(map->h+2, 0);
                printw("phase %s", debugPhase ? "on" : "off");
                break;
            case 'x':
                debugDM = !debugDM;
                move(map->h+2, 0);
                printw("dmap %s", debugDM ? "on" : "off");
                break;
            case 'c':
                debugFlee = !debugFlee;
                move(map->h+2, 0);
                printw("fleemap %s", debugFlee ? "on" : "off");
                break;
            case 'v':
                debugPath = !debugPath;
                debugLine = false;
                move(map->h+2, 0);
                printw("pathing %s", debugPath ? "on" : "off");
                you.cursor = you.pos;
                break;
            case 'b':
                if (debugPath) {
                    if (debugLine) {
                        debugLine = false;
                    } else {
                        debugLine = true;
                    }
                } else {
                    debugPath = debugLine = true;
                    you.cursor = you.pos;
                }
                move(map->h+2, 0);
                printw("line %s", debugLine ? "on" : "off");
                break;
            case 'w':
                move(map->h+2, 0);
                printw("random int: %u\njson: %s", rng.range(20,30), rng.serialize().c_str());
                break;
            case 'e': {
                move(map->h+2, 0);
                printw("current: %u, %u", rng.seeds[0], rng.seeds[1]);
                std::string ser = rng.serialize();
                move(map->h+4, 0);
                printw("current state: %s", ser.c_str());
                RNG temp = RNG::deserialize(ser);
                move(map->h+3, 0);
                printw("read back in: %u, %u", temp.seeds[0], temp.seeds[1]);
                break;
            }
            default:
                move(0,0); printw("unknown character: %o    \n", c);
                break;
        }
        dmap.clear();
        dmap.goal(you.pos);
        auto begin = std::chrono::high_resolution_clock::now();
        dmap.calculate();
        auto end = std::chrono::high_resolution_clock::now();
        move(15, 0); printw("dmap scan: %f us\n", (std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / 1000.0f));
        flee.clear();
        flee.goal(you.pos);
        flee.calculate();
        flee.invert(1.5);
    }
    shutdown_curses();
    return 0;
}

