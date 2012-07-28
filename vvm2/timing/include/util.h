/*
 *  util.h
 *  Utility functions used by the test program and operations
 *
 *  Created by Bing-Chang Lai on Tues May 20 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */
 
#ifndef _VVM_TIMING_UTIL_H
#define _VVM_TIMING_UTIL_H

#include <vvm/vvm.h>
#include <common/timing.h>
#include <common/type_name.h>

#define TIME_INIT \
	timing_t start, end;

#define TIME_OP(OP) \
	start = timenow(); \
	for(int i = 0; i < count; ++i) { \
		OP; \
	} \
	end = timenow();

#define TIME_RESULT \
	timediff(end, start)

template<typename scalarT> void fill_rand(scalarT* data, const int count) {
	for(int i = 0; i < count; ++i) {
		data[i] = rand() % 30;
	}
}

#endif
