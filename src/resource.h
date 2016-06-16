// archive-based resource loader.
//==============================================
// a basic resource loader. Will look for the file in the filesystem first;
// if it's not found, it looks inside the given archive file.

#define ZIP_EXTERN
#include "zip.h"

class ResourceManager {
    struct zip *zf;
public:
    ResourceManager(std::string archiveFile);
    ~ResourceManager();
    std::string loadFile(std::string fname);
    json::Value loadJson(std::string fname);
};

