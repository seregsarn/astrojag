#include "thplib_internal.h"

// required for interface, because C++ is weird.
IPathable::~IPathable() {}

//=================================================================

DijkstraMap::DijkstraMap(IPathable *map) {
    host = map;
    Point dims = map->getDimensions();
    w = dims.x;
    h = dims.y;
    data = (int*)malloc(sizeof(int) * w*h);
    clear(INT_MAX);
}

DijkstraMap::~DijkstraMap() {
    free(data);
}

//----------------------------------------------------
// map-wide operations
void DijkstraMap::clear(int value) {
    for (int i = 0; i < w*h; i++) {
        data[i] = value;
    }
}

bool DijkstraMap::inBounds(int x, int y) {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return true;
}

int DijkstraMap::lowestNeighbor(int x, int y) {
    int ln = INT_MAX;
    if (inBounds(x-1,y-1)) ln = min(ln, at(x-1,y-1));
    if (inBounds(x  ,y-1)) ln = min(ln, at(x  ,y-1));
    if (inBounds(x+1,y-1)) ln = min(ln, at(x+1,y-1));
    if (inBounds(x-1,y  )) ln = min(ln, at(x-1,y  ));
    if (inBounds(x+1,y  )) ln = min(ln, at(x+1,y  ));
    if (inBounds(x-1,y+1)) ln = min(ln, at(x-1,y+1));
    if (inBounds(x  ,y+1)) ln = min(ln, at(x  ,y+1));
    if (inBounds(x+1,y+1)) ln = min(ln, at(x+1,y+1));
    return ln;
}

int DijkstraMap::scanStep() {
    int changes = 0;
    int val, ln;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            if (host->blocked(Point(x,y), 0)) continue;
            val = data[(y*w)+x];
            ln = lowestNeighbor(x,y);
            // NB: weird construction for this conditional, but most values
            // will start out as INT_MAX and we don't want to do weird shit
            // due to integer overflow.
            if (val > ln && (val - ln) > 2) {
                data[(y*w)+x] = ln+1;
                changes++;
            }
        }
    }
    return changes;
}

void DijkstraMap::fullScan() {
    // todo: perform scan steps until no changes occur.
    int ch;
    do {
        ch = scanStep();
    } while (ch > 0);
}

void DijkstraMap::invert() {
    // todo: multiply values by a negative coefficient around -1.2
    //  + use fixed point?
    //  + variable for coefficient?
    // then call fullScan() again to recompute the map.
    for (int i = 0; i < w*h; i++) {
        if (data[i] != INT_MAX)
            data[i] = (int)(data[i] * -1.5);
    }
    fullScan();
}

//----------------------------------------------------
// accessors
int& DijkstraMap::at(const Point p) { return at(p.x, p.y); }
int& DijkstraMap::at(const int x, const int y) {
    return data[((y*w) + x)];
}

void DijkstraMap::goal(const Point p) { goal(p.x, p.y); }
void DijkstraMap::goal(const int x, const int y) {
    at(x,y) = 0;
}

