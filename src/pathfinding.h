// pathfinding algorithm(s) for libastrojag.
//===================================================
// uses A* pathfinding.

class IPathable {
public:
    virtual ~IPathable()=0; // make destructors get called correctly.
    // accessibility information
    virtual Point getDimensions()=0;
    // A*-appropriate approximation of the distance from src to dest.
    // Defaults to distance if not provided.
    virtual int heuristic(const Point& src, const Point& dest);
    virtual bool blocked(Point p, int flags)=0; // returns whether or not the tile is passable.
};

class DijkstraMap {
protected:
    IPathable *host;
    int flags;
    int w, h;
    int *data;
    bool *blockCache;
    // helpers
    bool inBounds(const int x, const int y);
    bool inBounds(const Point& p);
    int lowestNeighbor(int x, int y);
    int scanStep();
public:
    DijkstraMap(IPathable *map, int flags=0);
    ~DijkstraMap();
    Point getDimensions() { return Point(w,h); }
    
    // map-wide operations
    void loadCache();                    // load the cache with all the blocked values.
    void clear(int value=INT_MAX);       // max out all the values in the map.
    void calculate();                    // load blocking data and compute the map.
    void fullScan();                     // perform the smoothing operation on the entire map.
    void fullScan(int flags);
    void invert(float coefficient=1.5f); // perform the inversion to get an "avoidance map", e.g. for fleeing.
    // accessors
    int& at(const int x, const int y);
    int& at(const Point p);
    void goal(const int x, const int y); // equivalent to at(x,y) = 0;
    void goal(const Point p);            // equivalent to at(p) = 0;
    bool& blocked(const int x, const int y);  // cached solidity value at (x,y)
    bool& blocked(const Point p);             // cached solidity value at p
    // these functions might be useful
    Point randomAbove(Point p);         // randomly select a point "above" this one. If at the top, return the same point.
    Point randomBelow(Point p);         // randomly select a point "below" this one. If at the bottom, return the same point.
    Point gradient(const Point& p);    // find the gradient vector at this point.
};

//=================================================

class Path {
public:
    std::deque<Point> nodes;
    int position;
    void walk(std::function<bool(Point)> callback);
};

class Pathfinder {
protected:
    IPathable *map;
    PriorityQueue<Point> open;
    std::set<Point> closed;
    Grid<Point> cameFrom;
    Grid<int> bestCost;
public:
    Pathfinder(IPathable *map);
    ~Pathfinder();
    
    std::shared_ptr<Path> findPath(Point src, Point dst);
    std::shared_ptr<Path> findPath(int x1, int y1, int x2, int y2);
    std::shared_ptr<Path> findPath(Point src, Point dst, int flags);
    std::shared_ptr<Path> findPath(int x1, int y1, int x2, int y2, int flags);
};

