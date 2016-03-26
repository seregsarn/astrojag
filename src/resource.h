// archive-based resource loader

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

