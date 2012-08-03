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

#import <QuartzCore/CIImage.h>
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
        UInt8 *bitmapData();
        UInt8 *pixelData(const int x, const int y) {
            return bitmapData() + (bytesPerRow() * y) + (bytesPerPixel() * x);
        }
        const int bytesPerPixel() const {
            return 4;
        }
        const int bytesPerRow() const {
            return _width * bytesPerPixel();
        }
	private:
		int _channel_count;
        int _width, _height;
		NSMutableData *_bitmap_data;
        CIImage *_ci_image;
	};
	
} // End of namespace lv;

#endif
