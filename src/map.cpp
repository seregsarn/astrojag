#include "main.h"

MapTemplate::MapTemplate(int _w, int _h) {
    w = _w; h = _h;
    data = (char *)malloc(sizeof(char)*w*h);
    memset(data, 0, sizeof(char)*w*h);
}
MapTemplate::MapTemplate(const char *filename) {
    int i, len, start, end;
    char buf[512], *json;
    
    memset(buf, 0, 512*sizeof(char));
    FILE *fp = fopen(filename, "r");
    if (!fp) throw std::runtime_error("File Read Error.");
    fseek(fp, 0, SEEK_END);
    end = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fgets(buf, 511, fp);
//plog("buf: \"%s\"\n", buf);
    std::regex re("^MAP (\\d+),(\\d+)", std::regex::ECMAScript);
    
    std::cmatch cm;
    if (!std::regex_search(buf, cm, re)) {
        fclose(fp);
        throw std::runtime_error("Not a valid template file.");
    }
    //plog("matched!\n");
    w = atoi(cm[1].str().c_str());
    h = atoi(cm[2].str().c_str());
plog("loading a %d by %d map from file.\n", w, h);
    data = (char *)malloc(sizeof(char)*w*h);
    memset(data, 32, sizeof(char)*w*h);
    for (i = 0; i < h; i++) {
        fgets(buf, 511, fp);
        len = strlen(buf);
//plog("row %d: \"%s\"\n", i, buf);
        memcpy(&data[(w*i)], buf, len);
    }
//plog("final map data: \"\n%s\"\n", data);
    start = ftell(fp);
    len  = end-start;
    json = (char *)malloc(sizeof(char)*len);
    fread(json, sizeof(char), len, fp);
//plog("raw text: \"%s\"\n", json);
    if (!metadata.parse(json)) {
        // TODO: load default metadata in here.
        //metadata.parse("{ default parameters here }");
    }
plog("metadata: \"%s\"\n", metadata.json().c_str());
    free(json);
    fclose(fp);
    //plog("map: %d by %d\n", w,h);
}
MapTemplate::~MapTemplate() {
    free(data);
}

char& MapTemplate::at(const int x, const int y) {
    return data[(y*w) + x];
}