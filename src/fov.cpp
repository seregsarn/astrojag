#include "thplib_internal.h"

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


/*class FOVImmediate {
protected:
    IFieldOfView *map;
    Point pos;
    int flags;
    void *userdata;
public:
    typedef std::function<void(Point)> callback;
    FOVImmediate(IFieldOfView *m, Point p);
    FOVImmediate(IFieldOfView *m, Point p, int flags);
    FOVImmediate(IFieldOfView *m, Point p, int flags, void *userdata);
    // walk the FOV.
    void visit(callback cb) const;
};

// cached version of the FOV.
class FOV : public FOVImmediate {
protected:
    // map of stuff
    int w, h;
    vector<bool> data;
public:
    FOV(IFieldOfView *m, Point p);
    FOV(IFieldOfView *m, Point p, int flags);
    FOV(IFieldOfView *m, Point p, int flags, void *userdata);
    // read out data.
    bool inFOV(Point p) const;
    bool inFOV(int x, int y) const;
};
*/

FOVImmediate::FOVImmediate(IFieldOfView *m, Point p) : FOVImmediate(m,p,0,nullptr) {}
FOVImmediate::FOVImmediate(IFieldOfView *m, Point p, int flags) : FOVImmediate(m,p,flags, nullptr) {}
FOVImmediate::FOVImmediate(IFieldOfView *m, Point p, int fl, void *ud) {
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

Point FOVImmediate::transformOctant(Point vec, int octant) {
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

void FOVImmediate::shadowcastOctant(int octant, callback visible) {
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
                    if (map->opaque(Point(x,y),flags,userdata)) {
                        // add this shadow
                        line.add(proj);
                        fullShadow = line.isFullShadow();
                    }
                    visible(Point(x,y), flags, userdata);
                }
            } else {
                // blocked
            }
        }
    }
    // todo: do some shadow volume stuff
}

void FOVImmediate::visit(FOVImmediate::callback cb) {
    // loop over all the octants and shadowcast them.
    for (int oct = 0; oct < 8; oct++) {
        shadowcastOctant(oct, cb);
    }
}
void FOVImmediate::visit(FOVImmediate::callback cb, int fl) {
    flags = fl;
    visit(cb);
}
void FOVImmediate::visit(FOVImmediate::callback cb, int fl, void *ud) {
    flags = fl;
    userdata = ud;
    visit(cb);
}

FOV::FOV(IFieldOfView *m, Point p) : FOV(m,p,0,nullptr) {}
FOV::FOV(IFieldOfView *m, Point p, int flags) : FOV(m,p,flags, nullptr) {}
FOV::FOV(IFieldOfView *m, Point p, int fl, void *ud) : FOVImmediate(m, p, fl, ud) {
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
    // ignore flags and udata, just set the visibility.
    callback cb = [data](Point p, int flags, void *udata) {
        data[(p.y*w)+p.x] = true;
        (void)flags;
        (void)udata;
    };
    visit(cb, fl, ud);
}

FOV::~FOV() {
    free(data);
}

bool FOV::inFOV(Point p) const { return inFOV(p.x, p.y); }
bool FOV::inFOV(int x, int y) const {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return data[(y*w) + x];
}

//================================================================================


