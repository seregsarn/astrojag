// pathfinding algorithm(s) for thplib.
//===================================================

class IPathable {
public:
    virtual ~IPathable()=0; // make destructors get called correctly.
    //typedef std::function<bool(Point p, int flags)> callback;
    // accessibility information
    virtual Point getDimensions()=0;
    virtual int heuristic(const Point& src, const Point& dest); // A*-appropriate approximation of the distance from src to dest. Defaults to distance if not provided.
    virtual bool blocked(Point p, int flags)=0;   // whether or not a tile is passable.
};

class DijkstraMap {
protected:
    IPathable *host;
    int flags;
    int w, h;
    int *data;
    // helpers
    bool inBounds(const int x, const int y);
    int lowestNeighbor(int x, int y);
    int scanStep();    
public:
    DijkstraMap(IPathable *map, int flags=0);
    ~DijkstraMap();
    Point getDimensions() { return Point(w,h); }
    
    // map-wide operations
    void clear(int value=INT_MAX);      // max out all the values in the map.
    void fullScan();                    // perform the smoothing operation on the entire map.
    void fullScan(int flags);
    void invert(float coefficient=1.5f); // perform the inversion to get an "avoidance map", e.g. for fleeing.
    // accessors
    int& at(const int x, const int y);
    int& at(const Point p);
    void goal(const int x, const int y); // equivalent to at(x,y) = 0;
    void goal(const Point p);            // equivalent to at(p) = 0;
    // speculative API:
    Point randomAbove(Point p);         // randomly select a point "above" this one. If at the top, return the same point.
    Point randomBelow(Point p);         // same deal in the other direction.
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
    //std::function<int(IPathable *map, const Point&)> heuristic;
public:
    Pathfinder(IPathable *map);
    //Pathfinder(IPathable *map, IPathable::callback cb);
    ~Pathfinder();
    
    shared_ptr<Path> findPath(Point src, Point dst);
    shared_ptr<Path> findPath(int x1, int y1, int x2, int y2);
    shared_ptr<Path> findPath(Point src, Point dst, int flags);
    shared_ptr<Path> findPath(int x1, int y1, int x2, int y2, int flags);
};
