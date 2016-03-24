// archive-based resource loader

#define ZIP_EXTERN
#include "zip.h"

class ResourceManager {
    struct zip *zf;
public:
    ResourceManager(string archiveFile);
    ~ResourceManager();
    string loadFile(string fname);
    json::Value loadJson(string fname);
};

