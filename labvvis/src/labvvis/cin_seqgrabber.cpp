/*
 *  CINsource.cpp
 *  Grab pictures using QuickTime
 * 
 *  Created by Bing-Chang Lai.
 *  Copyright (c) 2004. All rights reserved.
 * 
 *  For unknown reasons, sgimporter cannot be initialised in getPicture for both
 *  CINs and Dynamic Libraries
 *
 */

#include <iostream>
#include <vvis/vvis.h>
#include <labvvis/image.h>
#include <labvvis/util.h>
#include "extcode.h"

extern "C" {
	MgErr CINInit();
	MgErr CINDispose();
	MgErr CINRun(int8 *operation, int32 *ptr, int *width, int *height, LStrHandle error);
}

vvis::sgimporter<vvis::sync_capture>* sgi = 0;

MgErr CINInit() {
	const int width = 320;
	const int height = 240;
	std::clog << "[labvvis 1.0.2 b6] Initialising sequence grabber" << std::endl;
	sgi = new vvis::sgimporter<vvis::sync_capture>(vvis::sgsettings(width, height));
	return noErr;
}

MgErr CINDispose() {
	delete sgi;
	return noErr;
}

MgErr CINRun(int8 *operation, int32 *ptr, int *width, int *height, LStrHandle error) {
	MgErr err = noErr;

	if(sgi == 0 || !sgi->good()) {
		lv::to_lstr(error, "Unable to open sequence grabber");
		return err;
	}
	
	switch(*operation) {
		case 0: { // Returns the size of the image
			*width = sgi->settings().width;
			*height = sgi->settings().height;
		} break;
		case 1: { // Returns an image
			lv::image lvimg = *((lv::image *)(*ptr));
			
			switch(lvimg.channel_count()) {
				case 1: {
					*sgi >> lvimg[0];
				}	break;
				case 4: {
					GWorldPtr gworld = sgi->wait_for_locked_gworld();
					
					const int w = sgi->settings().width;
					const int h = sgi->settings().height;
					lvimg[0].resize(w, h);
					lvimg[1].resize(w, h);
					lvimg[2].resize(w, h);
					lvimg[3].resize(w, h);
					
					PixMapHandle pixmap = GetGWorldPixMap(gworld);
					const vvis::uint8* addr = reinterpret_cast<vvis::uint8*>(GetPixBaseAddr(pixmap));
					long row_bytes = vvis::get_row_bytes(pixmap);
					
					// Assuming ARGB
					for(int y = 0; y < h; ++y) {
						for(int x = 0, i = 0; x < w; ++x, i += 4) {
							lvimg[3].pixel(x, y) = addr[i];
							lvimg[0].pixel(x, y) = addr[i+1];
							lvimg[1].pixel(x, y) = addr[i+2];
							lvimg[2].pixel(x, y) = addr[i+3];
						}
						addr += row_bytes;
					}
					
					sgi->release_locked_gworld();
					
					//*sgi >> *((image_t *)lvimg.image_ptr());
				}	break;
			}
				
		} break;
		default:
			lv::to_lstr(error, "Unknown operation encountered");
	}
	return err;
}
