#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <sys/stat.h>
#include <json/jsonxx.h>
namespace json = jsonxx;
//---
#include <memory>
#include <string>
using std::string;
#include <regex>
#include <exception>
#include <stdexcept>
using std::shared_ptr;
using std::make_shared;

#include "thplib.h"

extern ThpLogger *internalLogger;
void internalLog(const char *fmt, ...);