// pathfinding algorithm(s) for thplib.
//===================================================

class IPathable {
public:
    virtual ~IPathable()=0; // make destructors get called correctly.
    // accessibility information
    virtual Point getDimensions()=0;
    virtual bool blocked(Point p, int flags)=0;   // whether or not a tile is passable.
};

// TODO: add flags/etc.
class DijkstraMap {
protected:
    IPathable *host;
    int w, h;
    int *data;
    // helpers
    bool inBounds(const int x, const int y);
    int lowestNeighbor(int x, int y);
    int scanStep();    
public:
    DijkstraMap(IPathable *map);
    ~DijkstraMap();
    Point getDimensions() { return Point(w,h); }
    
    // map-wide operations
    void clear(int value=INT_MAX);      // max out all the values in the map.
    void fullScan();                    // perform the smoothing operation on the entire map.
    void invert();                      // perform the inversion to get an "avoidance map", e.g. for fleeing.
    // accessors
    int& at(const int x, const int y);
    int& at(const Point p);
    void goal(const int x, const int y); // equivalent to at(x,y) = 0;
    void goal(const Point p);            // equivalent to at(p) = 0;
    // speculative API:
    // Point rollUp(Point p); randomly select a point "above" this one. what if we're at the top?
    // Point rollDown(Point p); same deal in the other direction.
};

