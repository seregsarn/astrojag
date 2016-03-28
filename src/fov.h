// map interface for thplib algorithms.
//===================================================

class IFieldOfView {
public:
    virtual ~IFieldOfView()=0; // make destructors get called correctly.
    // visibility and accessibility information
    virtual Point getDimensions()=0; // basic info
    virtual bool inBounds(int x, int y)=0;
    virtual bool opaque(Point p, int flags, void *userdata)=0; // check if FOV is blocked.
};

class FOVImmediate {
public:
    typedef std::function<void(Point, int f, void *ud)> callback;
protected:
    IFieldOfView *map;
    Point pos;
    int flags;
    void *userdata;
    // internal helper functions
    Point transformOctant(Point vec, int octant);
    void shadowcastOctant(int octant, callback visibility);
public:
    // maximum distance to see. default INT_MAX.
    int maxDistance;
    // callbacks
    // different configuration options
    FOVImmediate(IFieldOfView *m, Point p);
    FOVImmediate(IFieldOfView *m, Point p, int flags);
    FOVImmediate(IFieldOfView *m, Point p, int flags, void *userdata);
    // walk the FOV.
    void visit(callback cb);
    void visit(callback cb, int flags);
    void visit(callback cb, int flags, void *userdata);
};

// cached version of the FOV.
class FOV : public FOVImmediate {
protected:
    // map of stuff
    int w, h;
    bool *data;
public:
    typedef FOVImmediate::callback callback;
    FOV(IFieldOfView *m, Point p);
    FOV(IFieldOfView *m, Point p, int flags);
    FOV(IFieldOfView *m, Point p, int flags, void *userdata);
    ~FOV();
    // read out data.
    bool inFOV(Point p) const;
    bool inFOV(int x, int y) const;
};
