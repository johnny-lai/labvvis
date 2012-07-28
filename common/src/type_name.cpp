/*
 *  type_name.cpp
 *  common
 *
 *  Created by Bing-Chang Lai on Fri May 30 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <common/type_name.h>

static const char char_name[] = "char";
static const char uchar_name[] = "unsigned char";
static const char schar_name[] = "signed char";
static const char short_name[] = "short int";
static const char ushort_name[] = "unsigned short int";
static const char int_name[] = "int";
static const char uint_name[] = "unsigned int";
static const char long_name[] = "long int";
static const char ulong_name[] = "unsigned long int";
static const char float_name[] = "float";
static const char double_name[] = "double";
static const char ldouble_name[] = "long double";

const char* type_name(const std::type_info& i) {
	if(i == typeid(char)) {
		return char_name;
	} else if(i == typeid(unsigned char)) {
		return uchar_name;
	} else if(i == typeid(signed char)) {
		return schar_name;
	} else if(i == typeid(short int)) {
		return short_name;
	} else if(i == typeid(unsigned short int)) {
		return ushort_name;
	} else if(i == typeid(int)) {
		return int_name;
	} else if(i == typeid(unsigned int)) {
		return uint_name;
	} else if(i == typeid(long int)) {
		return long_name;
	} else if(i == typeid(unsigned long int)) {
		return ulong_name;
	} else if(i == typeid(float)) {
		return float_name;
	} else if(i == typeid(double)) {
		return double_name;
	} else if(i == typeid(long double)) {
		return ldouble_name;
	} /*else if(i == typeid(__vector signed char)) {
		return "__vector signed char";
	}*/
	return i.name();
}