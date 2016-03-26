// map interface for thplib algorithms.
//===================================================

class IMap {
public:
    // map metadata
    virtual Point getDimensions();
    // information about map cells
    virtual bool solid(Point p, int flags, void *userdata);  // check if travel is blocked.
    virtual bool opaque(Point p, int flags, void *userdata); // check if FOV is blocked.
    // todo: maybe automatically handle applying map templates? e.g.
    //virtual bool stampTemplateGlyph(Point p, char glyph, MapTemplate *t);
    
    //=============================================
    /*
    // TODO: custom callbacks?
    typedef std::function<bool(Point)> reader;
    */
};