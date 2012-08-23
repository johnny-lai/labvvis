/*
 *  image.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 6/05/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#ifndef LABVVIS_IMAGE_H
#define LABVVIS_IMAGE_H

#include <Accelerate/Accelerate.h>
#include "extcode.h"

//= LabVIEW ====================================================================
typedef struct {
	int32 dimSizes[2];
	uInt32 elt[1];
} TD1;
typedef TD1 **TD1Hdl;

//= labvvis ====================================================================
typedef short int image_id;
typedef short int channel_id;

namespace lv {
    static inline const UInt8 &pixel(const vImage_Buffer &a, const int x, const int y) {
        return ((UInt8 *)a.data)[a.rowBytes * y + x];
    }
    
    static inline UInt8 &pixel(vImage_Buffer &a, const int x, const int y) {
        return ((UInt8 *)a.data)[a.rowBytes * y + x];
    }
    
	//= image ==================================================================
	class image {
	public:
		image();
		image(const int width, const int height, const int channel_count);
        ~image();
	public:
		const int channel_count();
		const int height() const;
		const int width() const;
    public:
        vImage_Buffer &operator[](const int ch) {
            return _vimage_buffer[ch];
        }
		const vImage_Buffer &operator[](const int ch) const {
            return _vimage_buffer[ch];
        }
        UInt8 *begin(const int ch) {
            return bitmapData(ch);
        }
        UInt8 *end(const int ch) {
            return bitmapData(ch) + bytesCount(ch);
        }
    public:
        UInt8 *bitmapData(const int i);
        UInt8 &pixel(const int i, const int x, const int y) {
            return *(bitmapData(i) + (bytesPerRow() * y) + (bytesPerPixel() * x));
        }
        const int bytesPerPixel() {
            return 1;
        }
        const int bytesPerRow() {
            return _vimage_buffer[0].rowBytes;
        }
        const int bytesCount(const int ch) {
            return _vimage_buffer[ch].rowBytes * _vimage_buffer[ch].height;
        }
	private:
		int _channel_count;
        vImage_Buffer _vimage_buffer[4];
	};
	
} // End of namespace lv;

#endif
