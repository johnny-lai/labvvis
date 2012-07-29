/*
 *  distinct_typedef.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Tue Feb 27 2007.
 *  Copyright (c) 2007. All rights reserved.
 *
 */

#ifndef _CT_DISTINCT_TYPEDEF_H
#define _CT_DISTINCT_TYPEDEF_H

namespace ct {
	/** Wrapper class
	 * Used to allow typedefs to be distinct types.
	 * The resulting type is convertible to the original type.
	 */
	template<typename T, int discriminator> class distinct_typedef {
		T _value;
	public:
		distinct_typedef() {}
		distinct_typedef(const T value) : _value(value) {
		}
		operator T() const {
			return _value;
		}
	};
} // End of ct namespace
	
#endif