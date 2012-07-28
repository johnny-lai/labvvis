/*
 *  stub_iio.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 2/06/05.
 *  Copyright 2005 University of Wollongong. All rights reserved.
 *
 *  Stub for compiling operations. The following #defines are used:
 *   + TAG_I1
 *   + TAG_I2
 *   + TAG_O
 */

#include <boost/preprocessor/cat.hpp>
#include <labvvis/stub.h>

#define IMAGE_I1 BOOST_PP_CAT(image_, TAG_I1)
#define ACCESSOR_I1 BOOST_PP_CAT(accessor_, TAG_I1)

#define IMAGE_I2 BOOST_PP_CAT(image_, TAG_I2)
#define ACCESSOR_I2 BOOST_PP_CAT(accessor_, TAG_I2)

#define IMAGE_O BOOST_PP_CAT(image_, TAG_O)
#define ACCESSOR_O BOOST_PP_CAT(accessor_, TAG_O)

#define TAG BOOST_PP_CAT(BOOST_PP_CAT(TAG_I1, TAG_I2), TAG_O)
#define F(x) BOOST_PP_CAT(x, TAG)

//==============================================================================
#define DEFINE_BINARY_FUNCTION(FNAME) \
	extern "C" { \
	void F(FNAME)(void * const in1, void * const in2, void * const out); \
	} \
	void F(FNAME)(void * const in1, void * const in2, void * const out) { \
		IMAGE_I1 &i1 = *((IMAGE_I1*)in1); \
		IMAGE_I2 &i2 = *((IMAGE_I2*)in2); \
		IMAGE_O &o = *((IMAGE_O*)out); \
		transform(i1, ACCESSOR_I1(), i2, ACCESSOR_I2(), o, ACCESSOR_O(), \
				  FNAME<>()); \
	}

DEFINE_BINARY_FUNCTION(plus_saturated)
DEFINE_BINARY_FUNCTION(minus_saturated)
DEFINE_BINARY_FUNCTION(multiplies)
DEFINE_BINARY_FUNCTION(divides)
DEFINE_BINARY_FUNCTION(modulus)

DEFINE_BINARY_FUNCTION(equal_to)
DEFINE_BINARY_FUNCTION(not_equal_to)
DEFINE_BINARY_FUNCTION(greater)
DEFINE_BINARY_FUNCTION(greater_equal)
DEFINE_BINARY_FUNCTION(less)
DEFINE_BINARY_FUNCTION(less_equal)

DEFINE_BINARY_FUNCTION(logical_and)
DEFINE_BINARY_FUNCTION(logical_or)

DEFINE_BINARY_FUNCTION(bitwise_and)
DEFINE_BINARY_FUNCTION(bitwise_or)



