/*
 *  util.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 2005/12/26.
 *  Copyright 2005. All rights reserved.
 *
 */

#include "labvvis/util.h"

namespace lv {
	//= Utitlity functions =====================================================
	int lstrlen(LStrHandle handle) {
		return LStrLen(*handle);
	}
	
	MgErr to_lstr(LStrHandle handle, const char *str) {
		int32 new_size = strlen(str);
		MgErr err = NumericArrayResize(uB, 1L, (UHandle *)&handle, new_size);
		if (!err) {
			MoveBlock(str, LStrBuf(*handle), new_size);
			LStrLen(*handle) = new_size; /* update the dimension (length) */
        }
		return err;
	}
	
	MgErr to_lstr(LStrHandle handle, const std::string str) {
		return to_lstr(handle, str.c_str());
	}
}
