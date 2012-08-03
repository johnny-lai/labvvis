/*
 *  image.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 7/12/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#include <iostream>
#include <labvvis/image.h>

namespace lv {
	image::image() : _channel_count(0), _width(0), _height(0), _bitmap_data(nil), _ci_image(nil) {
	}
	
	image::image(const int width, const int height, const int channel_count) 
	: _channel_count(channel_count), _width(width), _height(height) {
        _bitmap_data = [[NSMutableData alloc] initWithCapacity:(bytesPerRow() * _height)];
        _ci_image = [[CIImage alloc] initWithBitmapData:_bitmap_data
                                            bytesPerRow:bytesPerRow()
                                                   size:CGSizeMake(_width, _height)
                                                 format:kCIFormatARGB8
                                             colorSpace:nil];
	}

    image::~image() {
        [_ci_image release];
        [_bitmap_data release];
    }
    
	const int image::channel_count() {
		return _channel_count;
	}
	
	const int image::height() const {
		return _height;
	}
	
	const int image::width() const {
		return _width;
	}
    
    UInt8 *image::bitmapData() {
        return (UInt8 *)[_bitmap_data mutableBytes];
    }
	
}