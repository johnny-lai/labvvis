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
#include "labvvis-exports.h"
//#include <labvvis/stub.h>

#include <QuartzCore/QuartzCore.h>

void testObjectiveC(const int in, int *out) {
        CIImage *image = [[CIImage alloc] init];
        if (image) {
            *out = 1;
        } else {
            *out = 2;
        }
}

