/*
 *  macros.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Mon Nov 10 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_MACROS_H
#define _VVIS_MACROS_H

#include <iostream>

#define VVIS_DEFAULT_CAPTURE_POLICY sync_capture

#define VVIS_DEFAULT_STORAGE_POLICY illife_planar_storage

#define _VVIS_CHECK_OSERR(op, fail) { \
	OSErr err = op; \
	if(err != noErr) { \
		std::clog << "vvis warning: " #op " OSErr=" << err << std::endl; \
		fail; \
	} \
}

#define _VVIS_WARN(msg) \
	std::clog << "vvis warning: " << msg << std::endl;

#define _VVIS_ERROR(msg) \
	std::cerr << "vvis error: " << msg << std::endl;

#endif
