#include "astrojag_internal.h"

// required for interface, because C++ is weird.
IPathable::~IPathable() {}

int IPathable::heuristic(const Point& src, const Point& dest) {
    return (int)src.distance(dest);
}

//=================================================================

DijkstraMap::DijkstraMap(IPathable *map, int fl) {
    host = map;
    flags = fl;
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
            if (host->blocked(Point(x,y), flags)) continue;
            val = data[(y*w)+x];
            ln = lowestNeighbor(x,y);
            // NB: weird construction for this conditional, but most values
            // will start out as INT_MAX and we don't want to do weird shit
            // due to integer overflow.
            if (val > ln && (val - ln) >= 2) {
                data[(y*w)+x] = ln+1;
                changes++;
            }
        }
    }
    return changes;
}

void DijkstraMap::fullScan(int fl) {
    flags = fl;
    fullScan();
}
void DijkstraMap::fullScan() {
    // todo: perform scan steps until no changes occur.
    int ch;
    do {
        ch = scanStep();
    } while (ch > 0);
}

void DijkstraMap::invert(float coefficient) {
    float flee_co = fabs(coefficient);
    int factor = (int)(flee_co * 1000);
    // todo: multiply values by a negative coefficient around -1.2
    //  + use fixed point?
    //  + variable for coefficient?
    // then call fullScan() again to recompute the map.
    for (int i = 0; i < w*h; i++) {
        if (data[i] != INT_MAX)
            //data[i] = ((int)floor(data[i] * -factor)) / 1000;
            data[i] = (int)(data[i] * -fabs(coefficient));
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

Point neighbors[] = {Point(-1,-1), Point(0,-1), Point( 1,-1),
                     Point(-1, 0),              Point( 1, 0),
                     Point(-1, 1), Point(0, 1), Point( 1, 1)};
Point DijkstraMap::randomAbove(Point p) {
    Point best = p;
    int bestvalue = INT_MIN;
    for (int i=0; i < 8; i++) {
        Point n = p + neighbors[i];
        if (!inBounds(n.x,n.y)) continue;
        if (at(n) > bestvalue) {
            bestvalue = at(n);
            best = n;
        }
    }
    return best;
}
Point DijkstraMap::randomBelow(Point p) {
    Point best = p;
    int bestvalue = INT_MAX;
    for (int i=0; i < 8; i++) {
        Point n = p + neighbors[i];
        if (!inBounds(n.x,n.y)) continue;
        if (at(n) < bestvalue) {
            bestvalue = at(n);
            best = n;
        }
    }
    return best;
}


//=================================================================
// pathfinding

void Path::walk(std::function<bool(Point)> callback) {
    for (auto iter = nodes.begin(); iter != nodes.end(); iter++) {
        auto pt = *iter;
        if (!callback(pt)) {
            break;
        }
    }
}

Pathfinder::Pathfinder(IPathable *_map) {
    map = _map;
}
Pathfinder::~Pathfinder() { }

shared_ptr<Path> Pathfinder::findPath(Point src, Point dst) {
    return findPath(src.x, src.y, dst.x, dst.y,0);
}
shared_ptr<Path> Pathfinder::findPath(int x1, int y1, int x2, int y2) {
    return findPath(x1,y1,x2,y2,0);
}
shared_ptr<Path> Pathfinder::findPath(Point src, Point dst, int flags) {
    return findPath(src.x, src.y, dst.x, dst.y, flags);
}

shared_ptr<Path> Pathfinder::findPath(int x1, int y1, int x2, int y2, int flags) {
    auto npath = make_shared<Path>();
    return npath;
}

