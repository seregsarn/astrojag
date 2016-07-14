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
    blockCache = (bool*)malloc(sizeof(bool) * w*h);
    clear(INT_MAX);
}

DijkstraMap::~DijkstraMap() {
    free(data);
    free(blockCache);
}

//----------------------------------------------------
// map-wide operations
void DijkstraMap::clear(int value) {
    for (int i = 0; i < w*h; i++) {
        data[i] = value;
        blockCache[i] = false;
    }
}

bool DijkstraMap::inBounds(int x, int y) {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return true;
}
bool DijkstraMap::inBounds(const Point& p) { return inBounds(p.x,p.y); }

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

void DijkstraMap::loadCache() {
    int idx, y, x;
    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            idx = (y * w) + x;
            blockCache[idx] = host->blocked(Point(x,y), flags);
        }
    }
}

int DijkstraMap::scanStep() {
    int changes = 0;
    int val, ln, idx;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            idx = (y * w) + x;
            if (blockCache[idx]) continue;
            val = data[idx];
            ln = lowestNeighbor(x,y);
            // NB: weird construction for this conditional, but most values
            // will start out as INT_MAX and we don't want to do weird shit
            // due to integer overflow.
            if (val > ln && (val - ln) >= 2) {
                data[idx] = ln+1;
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
void DijkstraMap::calculate() {
    loadCache();
    fullScan();
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
bool& DijkstraMap::blocked(const Point p) { return blocked(p.x, p.y); }
bool& DijkstraMap::blocked(const int x, const int y) {
    return blockCache[((y*w) + x)];
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

Point DijkstraMap::gradient(const Point& p) {
    Point gradient(0,0);
    int here = at(p);
    for (int i = 0; i < 8; i++) {
        Point n = p + neighbors[i];
        if (!inBounds(n)) continue;
        if (at(n) == INT_MAX || at(n) == INT_MIN) {
            int there = lowestNeighbor(n.x,n.y) + 1;
            if (there < here) gradient += neighbors[i];
            else if (there > here) gradient -= neighbors[i];
        } else if (at(n) < here) {
            gradient += neighbors[i];
        } else if (at(n) > here) {
            gradient -= neighbors[i];
        }
    }
    return gradient;
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

Pathfinder::Pathfinder(IPathable *_map) : cameFrom(_map->getDimensions().x, _map->getDimensions().y), bestCost(_map->getDimensions().x, _map->getDimensions().y) {
    map = _map;
}
Pathfinder::~Pathfinder() { }

std::shared_ptr<Path> Pathfinder::findPath(Point src, Point dst) {
    return findPath(src.x, src.y, dst.x, dst.y,0);
}
std::shared_ptr<Path> Pathfinder::findPath(int x1, int y1, int x2, int y2) {
    return findPath(x1,y1,x2,y2,0);
}
std::shared_ptr<Path> Pathfinder::findPath(Point src, Point dst, int flags) {
    return findPath(src.x, src.y, dst.x, dst.y, flags);
}

std::vector<Point> findNeighbors(Point p) {
    //Point d = getDimensions();
    std::vector<Point> v(8);
    v[0] = p + Point(-1,-1);
    v[1] = p + Point( 0,-1);
    v[2] = p + Point( 1,-1);
    v[3] = p + Point(-1, 0);
    v[4] = p + Point( 1, 0);
    v[5] = p + Point(-1, 1);
    v[6] = p + Point( 0, 1);
    v[7] = p + Point( 1, 1);
    return v;
}

std::shared_ptr<Path> Pathfinder::findPath(int x1, int y1, int x2, int y2, int flags) {
    int i = 0, score = 0;
    Point dims = map->getDimensions();
    auto start = Point(x1,y1);
    auto goal = Point(x2,y2);
    auto npath = make_shared<Path>();
    open.clear();
    closed.clear();
    cameFrom.clearTo(Point(-1,-1));
    bestCost.clearTo(INT_MAX);
    bestCost.at(start) = 0;
    open.insert(0, start);
    while (open.size() > 0) {
        auto curr = open.pop();
        if (curr == goal) {
            // reconstruct path
            //internalLog("testing: found path\n");
            while (curr != start) {
                npath->nodes.push_front(curr);
                curr = cameFrom.at(curr);
            }
            return npath;
        }
        closed.insert(curr);
        // look at neighbors
        for (int i=0; i < 8; i++) {
            Point n = curr + neighbors[i];
            if (n.x < 0 || n.y < 0 || n.x >= dims.x || n.y >= dims.y) continue; // out of bounds
            if (map->blocked(n, flags)) continue; // blocked.
            if (closed.find(n) != closed.end()) continue; // already closed.
            score = bestCost.at(curr) + n.manhattan(goal);
            if (cameFrom.at(n) != Point(-1,-1) && bestCost.at(n) <= score) continue; // not a better path
            bestCost.at(n) = bestCost.at(curr) + 1;
            cameFrom.at(n) = curr;
            open.insert(score, n);
        }
    }
    return npath;
}