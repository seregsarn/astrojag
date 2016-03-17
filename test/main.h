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

#include "thplib.h"

// main.cpp
struct player_s {
    Point pos;
};
extern struct player_s you;
extern bool quit;
void plog(const char *fmt, ...);


// curses.cpp
void shutdown_curses(void);
void initialize_curses(void);

