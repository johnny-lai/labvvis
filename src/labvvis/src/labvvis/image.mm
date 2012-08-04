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
	image::image() : _channel_count(0) {
	}
	
	image::image(const int width, const int height, const int channel_count) 
	: _channel_count(channel_count) {
        size_t bytesPerPixel = 1;
        size_t rowBytes = width * bytesPerPixel;

        //Widen rowBytes out to a integer multiple of 16 bytes
        rowBytes = (rowBytes + 15) & ~15;
        
        //Make sure we are not an even power of 2 wide. 
        //Will loop a few times for rowBytes <= 16.
        while( 0 == (rowBytes & (rowBytes - 1) ) )
            rowBytes += 16;	//grow rowBytes
        
        for(int i = 0; i < _channel_count; ++i) {
            //Set up the buffer
            _vimage_buffer[i].height = height;
            _vimage_buffer[i].width = width;
            _vimage_buffer[i].rowBytes = rowBytes;
            _vimage_buffer[i].data = malloc( rowBytes * height );
        }
	}

    image::~image() {
        for(int i = 0; i < _channel_count; ++i) {
            if (_vimage_buffer[i].data) {
                free(_vimage_buffer[i].data);
                _vimage_buffer[i].data = NULL;
            }
        }
        _channel_count = 0;
    }
    
	const int image::channel_count() {
		return _channel_count;
	}
	
	const int image::height() const {
		return _vimage_buffer[0].height;
	}
	
	const int image::width() const {
		return _vimage_buffer[0].width;
	}
    
    UInt8 *image::bitmapData(const int i) {
        return (UInt8 *)_vimage_buffer[i].data;
    }
	
}