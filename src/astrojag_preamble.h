// libAstrojag
// a collection of algorithms and other useful stuff for roguelikes.
//======================================================================
#include <stdio.h>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_set>
#include <functional>
#include "json/jsonxx.h"

#ifdef unix
#define _AJ_CONST_FUNCTION
#endif

#ifdef _WIN32
#define _AJ_CONST_FUNCTION constexpr
#endif

namespace astrojag {
