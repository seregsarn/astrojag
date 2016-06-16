// Field of View
//===================================================
// Your map object has to implement IFieldOfView in order for these algos to
// work properly. There are two versions of the FOV code, which use the same
// algorithm and parameters.
class FOV;

class IFieldOfView {
public:
    virtual ~IFieldOfView()=0; // make destructors get called correctly.
    // visibility and accessibility information
    virtual Point getDimensions()=0; // basic info
    virtual bool inBounds(int x, int y)=0;
    virtual bool opaque(FOV *context, Point p)=0; // check if FOV is blocked.
};

// FOV
//--------------------------
// immediately visits all the visible cells with a callback function, which
// is useful for things like "light all the cells that this lantern can see."
class FOV {
public:
    // valid callback functions fit this format.
    typedef std::function<void(FOV *context, Point)> callback;
    // user data. accessible from the context element in callbacks.
    int flags;
    void *userdata;
    // maximum distance to see. default INT_MAX.
    int maxDistance;
protected:
    IFieldOfView *map;
    Point pos;
    // internal helper functions
    Point transformOctant(Point vec, int octant);
    void shadowcastOctant(int octant, callback visibility);
public:
    // different configuration options
    FOV(IFieldOfView *m, Point p);
    FOV(IFieldOfView *m, Point p, int flags);
    FOV(IFieldOfView *m, Point p, int flags, void *userdata);
    // walk the FOV.
    void visit(callback cb);
    void visit(callback cb, int flags);
    void visit(callback cb, int flags, void *userdata);
};

// FOVMap
//--------------------------
// caches the FOV calculations in a permanent data structure you can then
// query to find out if a given space is visible or not.
class FOVMap : public FOV {
protected:
    // map of stuff
    int w, h;
    // vector<bool> has issues, so.
    bool *data;
public:
    typedef FOV::callback callback;
    FOVMap(IFieldOfView *m, Point p);
    FOVMap(IFieldOfView *m, Point p, int flags);
    FOVMap(IFieldOfView *m, Point p, int flags, void *userdata);
    ~FOVMap();
    // read out data.
    bool inFOV(Point p) const;
    bool inFOV(int x, int y) const;
};

