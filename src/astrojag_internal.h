// system-specific stuff.
#ifdef unix
#include <stdio.h>
#include <unistd.h>
#define _AJ_CONST_FUNCTION
#endif
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <io.h>
#define dup _dup
#define fileno _fileno
#define fdopen _fdopen
#define _AJ_CONST_FUNCTION constexpr
#endif

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

#define ZIP_EXTERN
#include "zip.h"

#include "astrojag.h"
using namespace astrojag;

extern ThpLogger *internalLogger;
void internalLog(const char *fmt, ...);

