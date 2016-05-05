// archive-based resource loader

#include "astrojag_internal.h"

ResourceManager::ResourceManager(string archiveFile) {
    int err;
    zf = zip_open(archiveFile.c_str(), ZIP_CHECKCONS, &err);
    if (!zf) {
        // do warning stuff
        internalLog("warning: can't load archive\n");
    }
}
ResourceManager::~ResourceManager() {
    if (zf) {
        zip_close(zf);
    }
}

string ResourceManager::loadFile(string fname) {
    char *data;
    string out;
    struct zip_stat zstat;
    struct zip_file *zfp;
    struct stat fstat;
    FILE *fp;
    int err;
    if (!stat(fname.c_str(), &fstat)) {
        data = (char *) malloc(fstat.st_size+1);
		memset(data, 0, sizeof(char) * fstat.st_size+1);
#ifdef _WIN32
		fopen_s(&fp, fname.c_str(), "r+");
#else
        fp = fopen(fname.c_str(), "r+");
#endif
            fread(data, 1, fstat.st_size, fp);
            data[fstat.st_size] = 0;
            out.assign(data);
        fclose(fp);
        free(data);
        return out;
    } else if (zf) {
        err = zip_stat(zf, fname.c_str(), 0, &zstat);
        if (err >= 0) {
            data = (char *) malloc((size_t)zstat.size+1);
            zfp = zip_fopen(zf, fname.c_str(), 0);
                zip_fread(zfp, data, zstat.size);
                data[zstat.size] = 0;
                out.assign(data);
            zip_fclose(zfp);
            free(data);
            return out;
        } else goto error;
    } else {
        error:
        internalLog("error: Can't find file \"%s\"\n", fname.c_str());
        throw ENOENT;
    }
}

json::Value ResourceManager::loadJson(string fname) {
	string data = loadFile(fname);
	json::Object parsed;
	parsed.parse(data);
	return json::Value(parsed);
}
