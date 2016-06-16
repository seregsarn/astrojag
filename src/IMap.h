// Map Interface for libastrojag.
//===================================================
// some of our algorithms, e.g. FOV, are set up to work on any map object
// that implements this interface.

class IMap {
public:
    // map metadata
    virtual Point getDimensions();
    // information about map cells
    virtual bool solid(Point p, int flags, void *userdata);  // check if travel is blocked.
    virtual bool opaque(Point p, int flags, void *userdata); // check if FOV is blocked.
};

