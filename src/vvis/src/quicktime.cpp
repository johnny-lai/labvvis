/*
 *  quicktime.cpp
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Jan 4 2004.
 *  Copyright (c) 2004 Bing-Chang Lai. All rights reserved.
 *
 */

#include <cstring>
#include <vvis/quicktime.h>

namespace vvis {

	/// Returns a FSSpec from a C-style path
	OSErr make_fsspec(FSSpec& fsspec, const char path[]) {
	#ifdef _WIN32	// Windows Implementation
		char buffer[255];
		
		std::strcpy(buffer, path);
		c2pstr(buffer);
		return FSMakeFSSpec(0, 0L, reinterpret_cast<ConstStr255Param>(buffer), &fsspec);
	#else			// Macintosh Implementation
		FSRef fsref;
		OSErr err = noErr;
		err = FSPathMakeRef(reinterpret_cast<const UInt8*>(path), &fsref, 0);
		if(err != noErr)
			return err;
		return FSGetCatalogInfo(&fsref, kFSCatInfoNone, 0, 0, &fsspec, 0);
	#endif
	}

	long get_row_bytes(PixMapHandle pixmap) {
	#ifdef _WIN32	// Windows Implementation
		// Windows does not have GetPixRowBytes
		// Top two bits are flags
		// If bit 15 is set, then it means it is pointing to a PixMap,
		// otherwise it is a BitMap
		return (**pixmap).rowBytes & 0x3fff;
	#else			// Macintosh Implementation
		return QTGetPixMapHandleRowBytes(pixmap);
	#endif
	}

    #ifndef _WIN32
    CGContextRef create_cgcontext_rgba(UInt8 *rawData, const size_t width, const size_t height)
    {
        // First get the image into your data buffer
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        size_t bytesPerPixel = 4;
        size_t bytesPerRow = bytesPerPixel * width;
        size_t bitsPerComponent = 8;
        CGContextRef context = CGBitmapContextCreate(rawData, width, height,
                        bitsPerComponent, bytesPerRow, colorSpace,
                        kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        CGColorSpaceRelease(colorSpace);

        return context;
    }
    
    void draw_rgba(UInt8 *rawData, CGImageRef imageRef)
    {
        size_t width = CGImageGetWidth(imageRef);
        size_t height = CGImageGetHeight(imageRef);
        
        CGContextRef context = create_cgcontext_rgba(rawData, width, height);
        CGContextDrawImage(context, CGRectMake(0, 0, width, height), imageRef);
        CGContextRelease(context);

        // Now your rawData contains the image data in the RGBA8888 pixel format.
    }
    #endif
} // End of vvis namespace