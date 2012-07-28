/*
 *  config.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat Feb 7 2004.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

// Access for begin<>, end<> functions in storage policies
// Default is protected. Use VVIS_USER_COMPONENT_BEGIN_END_ACCESS to
// override
#if !defined VVIS_USER_COMPONENT_BEGIN_END_ACCESS
	#define _VVIS_COMPONENT_BEGIN_END_ACCESS protected
#else
	#define _VVIS_COMPONENT_BEGIN_END_ACCESS VVIS_USER_COMPONENT_BEGIN_END_ACCESS
#endif

// If scalar count is 1, no storages are treated as contiguous
// Define VVIS_USER_NO_AUTO_UNKNOWN to turn off this feature
#if !defined VVIS_USER_NO_AUTO_UNKNOWN && VVM_SCALAR_COUNT == 1
	#define _VVIS_TREAT_CONTIGUOUS_AS_UNKNOWN
#endif

// Specialized Iterators
// Define VVIS_USER_NO_SPECIALIZED_ITERATORS to turn off this feature
#if !defined VVIS_USER_NO_SPECIALIZED_ITERATORS
	#define _VVIS_SPECIALIZED_ITERATORS
#endif

// Default is to turn off prefetch
#if defined VVIS_USER_PREFETCH
	#define VVIS_DEFAULT_PREFETCH_POLICY vvis::prefetch<>
#elif defined VVIS_USER_NO_PREFETCH
	#define VVIS_DEFAULT_PREFETCH_POLICY vvis::no_prefetch
#else
	#define VVIS_DEFAULT_PREFETCH_POLICY vvis::prefetch<>
#endif
#define VVIS_DEFAULT_PREFETCH_USAGE vvm::read_write
#define VVIS_DEFAULT_PREFETCH_COUNT 128

