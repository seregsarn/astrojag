// Map Templates
//=============================================
// very basic template system with metadata, basically just ascii art and
// some dimension info and metadata. I'm trying not to make this too specific
// and leave the semantics entirely up to the client code.

class MapTemplate {
public:
    char *data; //array [w*h]
    json::Object metadata;
    int w, h;

    // memory management
    MapTemplate(int w, int h);
    MapTemplate(const char *filename);
    ~MapTemplate();
    
    // accessors/etc.
    char& at(const int x, const int y);
    
};

