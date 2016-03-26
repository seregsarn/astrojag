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

int debugOctant;

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
    std::list<Shadow> line;
    bool inShadow(const Shadow& projection) {
        for (auto iter = line.begin(); iter != line.end(); iter++) {
            if ((*iter).contains(projection)) return true;
        }
        return false;
    }
    bool isFullShadow() {
        if (line.size() != 1) return false;
        auto s = line.front();
        return (s.begin <= 0.001f && s.end >= 0.999f);
    }
    void add(Shadow& s) {
        bool overlapPrev = false, overlapNext = false;
        // find start position for the new shadow.
        auto next = line.begin();
        decltype(next) prev, spos;
logger->print("\nadding shadow: [%f,%f]", s.begin, s.end); logger->flush();
        for (; next != line.end(); next++) {
            auto other = (*next);
            if (other.begin >= s.begin) break;
        }
        spos = next; spos--;
        // find overlaps
        if (spos != line.begin()) {
            prev = spos; prev--;
logger->print(" prev(%f > %f)", (*prev).end, s.begin); logger->flush();
            if ((*prev).end > s.begin) overlapPrev = true;
        }
        if (next != line.end()) {
logger->print(" next(%f < %f)", (*next).begin, s.end); logger->flush();
            if ((*next).begin < s.end) overlapNext = true;
        }
logger->print(" overlaps: %s", overlapPrev && overlapNext ? "both" : (overlapPrev ? "prev" : (overlapNext ? "next" : "none") ) ); logger->flush();
        // insert and merge with existing shadows.
        if (overlapNext) {
            if (overlapPrev) {
                // overlaps both, so skip this shadow and merge the other two.
logger->print("\n --> [%f,%f] eats [%f,%f] and [%f,%f]\n", (*prev).begin, (*prev).end, s.begin, s.end, (*next).begin, (*next).end);
                (*prev).end = (*next).end;
                line.erase(next);
            } else {
                // merge with next shadow.
                (*next).begin = s.begin;
            }
        } else { // no end overlap
            if (overlapPrev) {
                // merge with prev shadow.
                (*prev).end = s.end;
            } else { // no overlap, just add it
                line.insert(spos, s);
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
    logger->print("=================\n(%d,%d), o%d\n", pos.x, pos.y, octant);
    logger->flush();
    attron(COLOR_PAIR(6)|A_BOLD|A_REVERSE);
    for (row = 1;; row++) {
        // quit when we leave bounds
        vec = pos + transformOctant(Point(0, row), octant);
        if (!m->inBounds(vec.x, vec.y)) break;
        logger->print("row %d (%d,%d), ", row, vec.x, vec.y);
        line.describe();
        logger->flush();
        
        for (col = 0; col <= row; col++) {
            logger->print("   c%d r%d", col, row);
            logger->flush();
            auto proj = projectTile(col, row);
            logger->print(" [%f,%f]", proj.begin, proj.end);
            logger->flush();
            vec = pos + transformOctant(Point(col, row), octant);
            x = vec.x; y = vec.y;
            logger->print(" => (%d,%d)", x, y);
            logger->flush();
            if (!m->inBounds(x,y)) break;
            if (!fullShadow) {
                if (!line.inShadow(proj)) {
                    // visible
                    logger->print(" >>> visible"); logger->flush();
                    if (m->at(x,y) == '#') {
                        logger->print(", blocking"); logger->flush();
                        // add this shadow
                        line.add(proj);
                        fullShadow = line.isFullShadow();
                    }
                    move(y,x);
                    addch(m->at(x,y));
                }
            } else {
                // blocked
                logger->print(" >>> blocked"); logger->flush();
            }
            logger->print("\n"); logger->flush();
        }
    }
    attroff(COLOR_PAIR(6)|A_REVERSE|A_BOLD);
    // todo: do some shadow volume stuff
}

void drawFov(Map* m, Point pos) {
    // todo: loop over debugOctant from 0 to 7.
    shadowcast_octant(m, pos, debugOctant);
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
    debugOctant = 0;
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
                break;
            case 'z': debugOctant = (--debugOctant < 0 ? 7 : debugOctant); break;
            case 'x': debugOctant = (++debugOctant > 7 ? 0 : debugOctant); break;
            default:
                move(0,0); printw("unknown character: %o    \n", c);
                break;
        }
    }
    shutdown_curses();
    return 0;
}

