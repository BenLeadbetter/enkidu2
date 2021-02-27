#pragma once

#include <cassert>

#ifdef NDEBUG
#define ENKIDU_ASSERT(x)
#else
#define ENKIDU_ASSERT(x) assert(x)
#endif
