/*
 *  main.cpp
 *  vvm
 *
 *  Created by Johnny Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

//#define VVM_USER_SCALAR_COUNT 16

#include <common/type_name.h>
#include <vvm/vvm.h>
#include <iostream>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	const scalarT a = 7, b = 3;

	vector<scalarT> v1(a), v2(b);
	//cout << all_eq(min(v1, v2), a < b ? v1 : v2)) << endl;
	
	v1.scalar(0) = b;
	v2.scalar(0) = a;
	cout <<max(v1, v2)<< endl;
	cout << vector<scalarT>((a > b) ? a : b) << endl;
	cout << all_eq(max(v1, v2), vector<scalarT>((a > b) ? a : b)) << endl;
	
	typename vector_traits<vector<scalarT> >::bool_type v3(~0);
	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << select(v3, v1, v2) << endl;
	cout << sizeof(double) << endl;
	cout << sizeof(long long) << endl;
}

int main() {
/*
	test<char>();
	test<signed char>();
	test<unsigned char>();
	test<short int>();
	test<unsigned short int>();
	test<int>();
	test<unsigned int>();
	test<long int>();
	test<unsigned long int>();*/
	// Should not compile for the following
	//test<int>();
	// test<long double>();
	// test<long double>();
	vector<int> c(10);
	cout << vector_cast<vector<float> >(c) << endl;
	return 0;
}
