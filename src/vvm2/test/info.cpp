/*
 *  main.cpp
 *  vvm
 *
 *  Created by Johnny Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */
//#define VVM_USER_SCALAR_COUNT 32

#include <vvm/vvm.h>
#include <iostream>

using namespace std;
using namespace vvm;

int main() {
	cout << "VVM Implementation Information" << endl;
	cout << "Implementation: " << VVM_INFO_IMPLEMENTATION << endl;
	cout << "        Author: " << VVM_INFO_AUTHOR << endl;
	cout << "       Company: " << VVM_INFO_COMPANY << endl;
	cout << "       Version: " << VVM_INFO_VERSION << endl;
	cout << "         Build: " << VVM_INFO_BUILD << endl;
	cout << endl;
	
	cout << "Vector Processor Information" << endl;
	cout << "      Vector Processor: " << VVM_INFO_VECTOR_PROCESSOR << endl;
	cout << "          Scalar Count: " << VVM_SCALAR_COUNT << endl;
	
	//cout << "     User Scalar Count: " << VVM_USER_SCALAR_COUNT << endl;
	cout << "Preferred Scalar Count: " << VVM_PREFERRED_SCALAR_COUNT << endl;
	
	return 0;
}