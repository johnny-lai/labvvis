/*
 *  main.cpp
 *  This is the main program for timing programs
 *
 *  Created by Bing-Chang Lai on Tues May 20 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <vvm/vvm.h>
#include <iostream>
#include <common/type_name.h>
#include "util.h"
#include "operation.h"

using namespace std;
using namespace vvm;

template<typename scalarT> void time(const int op_count, const int count) {
	timingdiff_t scalar_time = time_scalar<scalarT>(op_count, count);
	timingdiff_t vvm_time = time_vvm<scalarT>(op_count, count);
	timingdiff_t scalar_time2 = time_scalar<scalarT>(op_count, count);
	cout << VVM_INFO_VECTOR_PROCESSOR << "\t" << type_name(typeid(scalarT)) << "\tscalar\t" << op_count << "\t" << scalar_time << endl;
	cout << VVM_INFO_VECTOR_PROCESSOR << "\t" << type_name(typeid(scalarT)) << "\tvvm\t" << op_count << "\t" << vvm_time << endl;
	cout << VVM_INFO_VECTOR_PROCESSOR << "\t" << type_name(typeid(scalarT)) << "\tscalar\t" << op_count << "\t" << scalar_time2 << endl;
}

int main(int argc, char* argv[]) {
	if(argc != 3) {
		cout << argv[0] << " <operation-count> <number-of-iterations>" << endl;
		return 1;
	}
	int op_count = atoi(argv[1]);
	int count = atoi(argv[2]);

	// Time char twice to balance out cache
	time<char>(op_count, count);
	time<char>(op_count, count);
	time<signed char>(op_count, count);
	time<unsigned char>(op_count, count);
	time<short int>(op_count, count);
	time<unsigned short int>(op_count, count);
	time<int>(op_count, count);
	time<unsigned int>(op_count, count);
	time<long int>(op_count, count);
	time<unsigned long int>(op_count, count);
	time<float>(op_count, count);
	time<double>(op_count, count);
	time<long double>(op_count, count);
	return 0;
}
