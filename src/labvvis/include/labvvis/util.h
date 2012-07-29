/*
 *  util.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 2005/12/26.
 *  Copyright 2005. All rights reserved.
 *
 */

#ifndef LABVVIS_UTIL_H
#define LABVVIS_UTIL_H

#include <string>
#include "extcode.h"

namespace lv {
	//= Utitlity functions =====================================================
	int lstrlen(LStrHandle handle);
	MgErr to_lstr(LStrHandle handle, const char *str);
	MgErr to_lstr(LStrHandle handle, const std::string str);	
} // End of lv namespace

#endif