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


class Shadow {
public:
    float begin, end;
    Shadow(float b, float e) : begin(b), end(e) {}
    bool contains(const Shadow& other) {
        return ((begin <= other.begin) && (end >= other.end));
    }
};
class ShadowLine {
public:
    std::vector<Shadow> line;
    bool inShadow(const Shadow& projection) {
        unsigned int i;
        for (i = 0; i < line.size(); i++) {
            if (line[i].contains(projection)) return true;
        }
        return false;
    }
    bool isFullShadow() {
        if (line.size() != 1) return false;
        return (line[0].begin <= 0.001f && line[0].end >= 0.999f);
    }
    void add(Shadow& s) {
        bool overlapPrev = false, overlapNext = false;
        // find start position for the new shadow.
        unsigned int idx;
//logger->print("\nadding shadow: [%f,%f]", s.begin, s.end); logger->flush();
        for (idx = 0; idx < line.size(); idx++) {
            if (line[idx].begin >= s.begin) break;
        }
        // find overlaps
        if (idx > 0 && line[idx - 1].end > s.begin) {
            overlapPrev = true;
        }
        if (idx < line.size() && line[idx].begin < s.end) {
            overlapNext = true;
        }
//logger->print(" overlaps: %s", overlapPrev && overlapNext ? "both" : (overlapPrev ? "prev" : (overlapNext ? "next" : "none") ) ); logger->flush();
        // insert and merge with existing shadows.
        if (overlapNext) {
            if (overlapPrev) {
                // overlaps both, so skip this shadow and merge the other two.
                
//logger->print("\n --> [%f,%f] eats [%f,%f] and [%f,%f]\n", line[idx - 1].begin, line[idx - 1].end, s.begin, s.end, line[idx].begin, line[idx].end);
                line[idx - 1].end = line[idx].end;
                line.erase(line.begin() + idx);
            } else {
                // merge with next shadow.
                line[idx].begin = s.begin;
            }
        } else { // no end overlap
            if (overlapPrev) {
                // merge with prev shadow.
                line[idx - 1].end = s.end;
            } else { // no overlap, just add it
                line.insert(line.begin() + idx, s);
            }
        }
    }
    void describe() {
        logger->print("shadows: { ");
        for (auto iter = line.begin(); iter != line.end(); iter++) {
            auto s = (*iter);
            logger->print("[%f,%f]", s.begin, s.end);
        }
        logger->print(" }\n");
    }
};

Point transformOctant(Point vec, int octant) {
    switch (octant) {
        case 0: return Point( vec.x,-vec.y);
        case 1: return Point( vec.y,-vec.x);
        case 2: return Point( vec.y, vec.x);
        case 3: return Point( vec.x, vec.y);
        case 4: return Point(-vec.x, vec.y);
        case 5: return Point(-vec.y, vec.x);
        case 6: return Point(-vec.y,-vec.x);
        case 7: return Point(-vec.x,-vec.y);
    }
    return Point(0,0);
}

Shadow projectTile(int col, int row) {
    float topLeft = (float)(col) / (float)(row + 2);
    float bottomRight = (float)(col + 1) / (float)(row + 1);
    return Shadow(topLeft, bottomRight);
}

void shadowcast_octant(Map* m, Point pos, int octant) {
    int row, col, x, y;
    Point vec;
    ShadowLine line;
    bool fullShadow = false;
    // todo: octant casting
//logger->print("=================\n(%d,%d), o%d\n", pos.x, pos.y, octant); logger->flush();
    attron(COLOR_PAIR(6)|A_BOLD|A_REVERSE);
    for (row = 1;; row++) {
        // quit when we leave bounds
        vec = pos + transformOctant(Point(0, row), octant);
        if (!m->inBounds(vec.x, vec.y)) break;
//logger->print("row %d (%d,%d), ", row, vec.x, vec.y); line.describe(); logger->flush();
        
        for (col = 0; col <= row; col++) {
//logger->print("   c%d r%d", col, row); logger->flush();
            auto proj = projectTile(col, row);
//logger->print(" [%f,%f]", proj.begin, proj.end); logger->flush();
            vec = pos + transformOctant(Point(col, row), octant);
            x = vec.x; y = vec.y;
//logger->print(" => (%d,%d)", x, y); logger->flush();
            if (!m->inBounds(x,y)) break;
            if (!fullShadow) {
                if (!line.inShadow(proj)) {
                    // visible
//logger->print(" >>> visible"); logger->flush();
                    if (m->at(x,y) == '#') {
//logger->print(", blocking"); logger->flush();
                        // add this shadow
                        line.add(proj);
                        fullShadow = line.isFullShadow();
                    }
                    move(y,x);
                    addch(m->at(x,y));
                }
            } else {
                // blocked
//logger->print(" >>> blocked"); logger->flush();
            }
//logger->print("\n"); logger->flush();
        }
    }
    attroff(COLOR_PAIR(6)|A_REVERSE|A_BOLD);
    // todo: do some shadow volume stuff
}

void drawFov(Map* m, Point pos) {
    // todo: loop over debugOctant from 0 to 7.
    for (int oct = 0; oct < 8; oct++) {
        shadowcast_octant(m, pos, oct);
    }
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

