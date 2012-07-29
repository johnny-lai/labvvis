/*
 *  altivec_types.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Sun Sep 28 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_ALTIVEC_TYPES_H
#define _VVM_ALTIVEC_TYPES_H

#include <ct/typelist.h>

namespace vvm {
	namespace priv {
		typedef ct::cons_tl<
			__vector signed char, __vector unsigned char, __vector bool char,
			__vector signed short, __vector unsigned short, __vector bool short,
			__vector signed int, __vector unsigned int, __vector bool int,
			__vector float
			>::type altivec_types;
		typedef ct::cons_tl<
			__vector signed char, __vector unsigned char, __vector bool char,
			__vector signed short, __vector unsigned short, __vector bool short,
			__vector signed int, __vector unsigned int, __vector bool int
			>::type altivec_integer_types;
		typedef ct::cons_tl<
			__vector float
			>::type altivec_float_types;

		typedef ct::cons_tl<
			__vector unsigned char,
			__vector unsigned short,
			__vector unsigned int
			>::type altivec_shift_types;
						
		// Types that support mladd
		typedef ct::cons_tl<
			__vector signed short,
			__vector unsigned short
			>::type altivec_mladd_types;
	} // End of priv namespace
} // End of vvm namespace

#endif
