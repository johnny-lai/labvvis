/*
 *  type_name.h
 *  common
 *
 *  Created by Bing-Chang Lai on Fri May 30 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _COMMON_TYPE_NAME_H
#define _COMMON_TYPE_NAME_H

#include <typeinfo>

const char* type_name(const std::type_info& i);

#endif

