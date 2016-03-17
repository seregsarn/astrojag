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
using std::shared_ptr;
using std::make_shared;

// main.cpp
struct player_s {
    int x, y;
};
extern struct player_s you;
extern bool quit;
void plog(const char *fmt, ...);

// map.cpp
#include "map.h"

// curses.cpp
void shutdown_curses(void);
void initialize_curses(void);

