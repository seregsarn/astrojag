#include "main.h"

Map::Map(int _w, int _h) {
    w = _w; h = _h;
    tiles = (char *)(malloc(sizeof(char) * w * h));
    memset(tiles, 0, sizeof(char)*w*h);
}
Map::~Map() {
    free(tiles);
}
char& Map::at(const int x, const int y) {
    return (tiles[(y*w) + x]);
}
char& Map::at(const Point p) {
    return at(p.x,p.y);
}

bool Map::inBounds(const Point& p) { return inBounds(p.x,p.y); }
bool Map::inBounds(const int x, const int y) {
    if (x < 0 || y < 0 || x >= w || y >= h) return false;
    return true;
}

bool Map::opaque(FOV *context, Point p) {
    if (at(p.x,p.y) == '#') return true;
    return false;
    (void)context;
}