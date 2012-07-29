/*
 *  main.cpp
 *  vvm
 *
 *  Created by Bing-Chang Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

//#define VVM_USER_SCALAR_COUNT 16

#include "test.h"

int main() {
	test<char>();
	test<signed char>();
	test<unsigned char>();
	test<short int>();
	test<unsigned short int>();
	test<int>();
	test<unsigned int>();
	test<long int>();
	test<unsigned long int>();
	test<float>();
	test<double>();
	test<long double>();
	return 0;
}