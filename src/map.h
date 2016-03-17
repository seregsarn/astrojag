// map stuff

class MapTemplate {
private:
    char *data;
    json::Object metadata;
public:
    int w, h;
    MapTemplate(int w, int h);
    MapTemplate(const char *filename);
    ~MapTemplate();
    
    char& at(const int x, const int y);
    
};
