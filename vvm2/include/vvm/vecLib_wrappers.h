/*
 *  vecLib_wrappers.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Oct 23 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  vecLib wrappers for operations that the AltiVec vector processor
 *  implementation uses. The aim of the wrapper is to group the different
 *  vecLib functions for the same operation (but different type) under the
 *  same function name. This makes it easier for templated code to use them
 *
 */

#ifndef _VVM_VECLIB_WRAPPERS_H
#define _VVM_VECLIB_WRAPPERS_H

#include <vecLib/vecLib.h>

namespace vvm {
	namespace priv {
		//- vecLib_div ---------------------------------------------------------
		__vector signed char vecLib_div(__vector signed char a, __vector signed char b, __vector signed char* r);
		__vector unsigned char vecLib_div(__vector unsigned char a, __vector unsigned char b, __vector unsigned char* r);
		__vector signed short vecLib_div(__vector signed short a, __vector signed short b, __vector signed short* r);
		__vector unsigned short vecLib_div(__vector unsigned short a, __vector unsigned short b, __vector unsigned short* r);
		__vector signed int vecLib_div(__vector signed int a, __vector signed int b, __vector signed int* r);
		__vector unsigned int vecLib_div(__vector unsigned int a, __vector unsigned int b, __vector unsigned int* r);
		//- vecLib_mul ---------------------------------------------------------
		__vector signed char vecLib_mul(__vector signed char a, __vector signed char b);
		__vector unsigned char vecLib_mul(__vector unsigned char a, __vector unsigned char b);
		__vector signed short vecLib_mul(__vector signed short a, __vector signed short b);
		__vector unsigned short vecLib_mul(__vector unsigned short a, __vector unsigned short b);
		__vector signed int	vecLib_mul(__vector signed int a, __vector signed int b);
		__vector unsigned int vecLib_mul(__vector unsigned int a, __vector unsigned int b);
	} // End of priv namespace
}

#endif