#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ncurses.h>
#include <json/jsonxx.h>
namespace json = jsonxx;
//---
#include <memory>
#include <string>
#include <regex>
#include <exception>
#include <stdexcept>
#include <vector>
#include <list>
using std::shared_ptr;
using std::make_shared;

#include "astrojag.h"
using namespace astrojag;

// main.cpp
struct player_s {
    Point pos;
    Point cursor;
};
extern struct player_s you;
extern bool quit;
void plog(const char *fmt, ...);


// curses.cpp
void shutdown_curses(void);
void initialize_curses(void);

#define log(fmt,...) logger->print(fmt,__VA_ARGS__)

// map stuff
class Map : public IFieldOfView, public IPathable {
protected:
    char *tiles; // w*h
public:
    int w, h;

    Map(int w, int h);
    ~Map();
    Point getDimensions() { return Point(w,h); } // IFieldOfView
    char& at(const int x, const int y);
    char& at(const Point p);
    bool inBounds(const int x, const int y);
    bool inBounds(const Point& p);
    
    // IFieldOfView
    bool opaque(FOV *context, Point p);
    // IPathable
    bool blocked(Point p, int flags);
};
