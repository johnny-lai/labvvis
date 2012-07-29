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
		return GetPixRowBytes(pixmap);
	#endif
	}

} // End of vvis namespace