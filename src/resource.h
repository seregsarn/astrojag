// archive-based resource loader.
//==============================================
// a basic resource loader. Will look for the file in the filesystem first;
// if it's not found, it looks inside the given archive file.

class ResourceManager {
    struct zip *zf;
public:
    ResourceManager(std::string archiveFile);
    ~ResourceManager();
    std::string loadFile(std::string fname);
    jsonxx::Value loadJson(std::string fname);
};

