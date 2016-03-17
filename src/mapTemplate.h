//=============================================
// map stuff

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
