/*
 *  enabler.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Sun Aug 24 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  Uses code derived from
 *    Jaako Jarvi, Jeremiah Willcock, Howard Hinnant & Andres Lumsdaine. "Function
 *    Overloading Based on Arbitary Properties of Types", C/C++ Users Journal, June 2003.
 *
 */

#ifndef _CT_ENABLER_H
#define _CT_ENABLER_H

#include <ct/typelist.h>

namespace ct {
	/** Enable metafunction
	 * Example:
	 *   template<typename T>
	 *   typename enable_if<is_arithmetic<T>::value, T>::type
	 *   foo(T t);
	 */
	template<bool B, typename T = void> struct enable_if {
		typedef T type;
	};
	template<typename T> struct enable_if<false, T> {
	};
	
	/** Disable metafunction
	 * Example:
	 *   template<typename T>
	 *   typename disable_if<is_arithmetic<T>::value, T>::type
	 *   foo(T t);
	 */
	template<bool B, typename T = void> struct disable_if {
		typedef T type;
	};
	template<typename T> struct disable_if<true, T> {
	};
} // End of ct namespace

#endif

