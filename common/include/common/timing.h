#ifndef TIMING_H
#define TIMING_H
/** Timing Functions. Handles difference between Windows and Unix platforms
 */

#ifdef WIN32
//=============================================================================
// Windows
//-----------------------------------------------------------------------------
#include "stdafx.h"
typedef __int64 timing_t;
typedef __int64 timingdiff_t;
#else
//=============================================================================
// Unix
//-----------------------------------------------------------------------------
#include <sys/time.h>
typedef timeval timing_t;
typedef long timingdiff_t;
#endif

timing_t timenow();
timingdiff_t timediff(const timing_t &end, const timing_t &start);

#define TIME(A, B) { \
		timing_t ___begin, ___end; \
		___begin = timenow(); \
		{ B; } \
		___end = timenow(); \
		std::cout << A << "\t" << timediff(___end, ___begin) << std::endl; \
	}

#define TIME_START \
		timing_t ___begin, ___end; \
		___begin = timenow();

#define TIME_STOP(A) \
		___end = timenow(); \
		std::cout << A << "\t" << timediff(___end, ___begin) << std::endl;

#endif
