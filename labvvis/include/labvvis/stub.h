/*
 *  stub.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 2/06/05.
 *  Copyright 2005 University of Wollongong. All rights reserved.
 *
 */

#ifndef LABVVIS_STUB_H
#define LABVVIS_STUB_H

#include <vvis/vvis.h>

using namespace vvis;

// Image Types
typedef image<uint8>     image_0;
typedef rgba_image<uint8> image_4;
typedef rgba_image<uint8> image_r;
typedef rgba_image<uint8> image_g;
typedef rgba_image<uint8> image_b;
typedef rgba_image<uint8> image_a;

// Accessor Types
typedef pixel_accessor<image_0>        accessor_0;
typedef pixel_accessor<image_4>        accessor_4;
typedef component_accessor<image_r, 0> accessor_r;
typedef component_accessor<image_g, 1> accessor_g;
typedef component_accessor<image_b, 2> accessor_b;
typedef component_accessor<image_a, 2> accessor_a;

// Pixel Types
typedef image_0::pixel_type pixel_type_0;
typedef image_4::pixel_type pixel_type_4;
typedef uint8 pixel_type_r;
typedef uint8 pixel_type_g;
typedef uint8 pixel_type_b;
typedef uint8 pixel_type_a;

#endif