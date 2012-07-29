/*
 *  quicktime.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Dec 28 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_QUICKTIME_H
#define _VVIS_QUICKTIME_H

#if defined _WIN32
// QuickTime Includes for Windows
#include <QTML.h>
#include <QuickTimeComponents.h>
#include <Quickdraw.h>
#include <TextUtils.h>
#include <Files.h>
#else
// QuickTime Includes for Macintosh
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>
#endif

namespace vvis {
	OSErr make_fsspec(FSSpec& fsspec, const char path[]);
	long get_row_bytes(PixMapHandle pixmap);
} // End of vvis namespace

#endif