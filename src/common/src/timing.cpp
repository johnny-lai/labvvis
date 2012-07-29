#include <common/timing.h>

#ifdef WIN32
//=============================================================================
// Windows
//-----------------------------------------------------------------------------
timing_t timenow() {
	__int64 i64freq;
	timing_t now;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	now = ((__int64)li.HighPart << 32) + (__int64)li.LowPart;
	// Get Frequency
	if(TRUE == QueryPerformanceFrequency(&li)) {
		//Only if the system is supporting High Performance
		i64freq = ((__int64)li.HighPart << 32) + (__int64)li.LowPart;
	}
	//Transform in microseconds
	(now *= 1000000) /= i64freq;
	return now;
}
timingdiff_t timediff(const timing_t &end, const timing_t &start) {
	return end - start;
}
#else
//=============================================================================
// Unix
//-----------------------------------------------------------------------------
timing_t timenow() {
	timeval now;
	gettimeofday(&now, 0);
	return now;
}
timingdiff_t timediff(const timing_t &end, const timing_t &start) {
	return ((end.tv_sec - start.tv_sec) * 1000000) + end.tv_usec - start.tv_usec;
}
#endif
