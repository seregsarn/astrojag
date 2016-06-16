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
#include <regex>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <cstdint>
using std::shared_ptr;
using std::make_shared;

using std::string;
using std::min;

#include "astrojag.h"
using namespace astrojag;

extern ThpLogger *internalLogger;
void internalLog(const char *fmt, ...);

