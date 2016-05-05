#include "astrojag_internal.h"

// required for interface, because C++ is weird.
IFieldOfView::~IFieldOfView() {}

//-------------------------------------------------------------
// internal data structures for the FOV code.

class Shadow {
public:
    float begin, end;
    Shadow(float b, float e) : begin(b), end(e) {}
    bool contains(const Shadow& other);
};

bool Shadow::contains(const Shadow& other) {
    return ((begin <= other.begin) && (end >= other.end));
}

class ShadowLine {
public:
    std::vector<Shadow> line;
    bool isFullShadow();
    bool inShadow(const Shadow& projection);
    void add(Shadow& s);
    void describe();
};

bool ShadowLine::inShadow(const Shadow& projection) {
    unsigned int i;
    for (i = 0; i < line.size(); i++) {
        if (line[i].contains(projection)) return true;
    }
    return false;
}
bool ShadowLine::isFullShadow() {
    if (line.size() != 1) return false;
    return (line[0].begin <= 0.001f && line[0].end >= 0.999f);
}
void ShadowLine::add(Shadow& s) {
    bool overlapPrev = false, overlapNext = false;
    // find start position for the new shadow.
    unsigned int idx;
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
    // insert and merge with existing shadows.
    if (overlapNext) {
        if (overlapPrev) {
            // overlaps both, so skip this shadow and merge the other two.
            
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
void ShadowLine::describe() {
/*
    logger->print("shadows: { ");
    for (auto iter = line.begin(); iter != line.end(); iter++) {
        auto s = (*iter);
        logger->print("[%f,%f]", s.begin, s.end);
    }
    logger->print(" }\n");
*/
}

//=================================================================
// FOV: the basic fov algorithm, calling back on demand.

FOV::FOV(IFieldOfView *m, Point p) : FOV(m,p,0,nullptr) {}
FOV::FOV(IFieldOfView *m, Point p, int flags) : FOV(m,p,flags, nullptr) {}
FOV::FOV(IFieldOfView *m, Point p, int fl, void *ud) {
    map = m;
    pos = p;
    flags = fl;
    userdata = ud;
    maxDistance = INT_MAX;
}

// helper function not part of the class.
Shadow projectTile(int col, int row) {
    float topLeft = (float)(col) / (float)(row + 2);
    float bottomRight = (float)(col + 1) / (float)(row + 1);
    return Shadow(topLeft, bottomRight);
}

Point FOV::transformOctant(Point vec, int octant) {
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

void FOV::shadowcastOctant(int octant, callback visible) {
    int row, col, x, y;
    Point vec;
    ShadowLine line;
    bool fullShadow = false;
    // keep going through the rows until we reach the edge of the map or our max distance.
    for (row = 1; row < maxDistance; row++) {
        // quit when we leave bounds
        vec = pos + transformOctant(Point(0, row), octant);
        if (!map->inBounds(vec.x, vec.y)) break;
        // loop over a widening "wedge" area to create the octant shape.
        for (col = 0; col <= row; col++) {
            auto proj = projectTile(col, row);
            vec = pos + transformOctant(Point(col, row), octant);
            x = vec.x; y = vec.y;
            if (!map->inBounds(x,y)) break;
            if (!fullShadow) {
                if (!line.inShadow(proj)) {
                    // visible
                    if (map->opaque(this, Point(x,y))) {
                        // add this shadow
                        line.add(proj);
                        fullShadow = line.isFullShadow();
                    }
                    visible(this, Point(x,y));
                }
            } else {
                // blocked
            }
        }
    }
    // todo: do some shadow volume stuff
}

void FOV::visit(FOV::callback cb) {
    // start square always in FOV.
    cb(this, pos);
    // loop over all the octants and shadowcast them.
    for (int oct = 0; oct < 8; oct++) {
        shadowcastOctant(oct, cb);
    }
}
void FOV::visit(FOV::callback cb, int fl) {
    flags = fl;
    visit(cb);
}
void FOV::visit(FOV::callback cb, int fl, void *ud) {
    flags = fl;
    userdata = ud;
    visit(cb);
}

//=================================================================
// cached FOV map. immediately runs the FOV and builds a boolean map.

FOVMap::FOVMap(IFieldOfView *m, Point p) : FOVMap(m,p,0,nullptr) {}
FOVMap::FOVMap(IFieldOfView *m, Point p, int flags) : FOVMap(m,p,flags, nullptr) {}
FOVMap::FOVMap(IFieldOfView *m, Point p, int fl, void *ud) : FOV(m, p, fl, ud) {
    map = m;
    pos = p;
    flags = fl;
    userdata = ud;
    Point dim = m->getDimensions();
    w = dim.x; h = dim.y;
    // immediately build a map of the FOV.
    data = (bool *)malloc(sizeof(bool) * w*h);
    for (int i = 0; i < w*h; i++) {
        data[i] = false;
    }
    // ignore context, just set the visibility.
    callback cb = [this](FOV *context, Point p) {
        this->data[(p.y*w)+p.x] = true;
        (void)context;
    };
    visit(cb);
}

FOVMap::~FOVMap() {
    free(data);
}

bool FOVMap::inFOV(Point p) const { return inFOV(p.x, p.y); }
bool FOVMap::inFOV(int x, int y) const {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return data[(y*w) + x];
}

