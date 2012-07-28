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
#include <vvm/functional.h>
#include <iostream>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	typedef vector<typename scalar_traits<scalarT>::bool_type> vector_bool_type;
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	cout << "+   scalar_count: " << vector_traits<vector<scalarT> >::scalar_count << endl;
	cout << "+ vpvector_count: " << vector_traits<vector<scalarT> >::vpvector_count << endl;
	vector<scalarT> v1(7), v2(static_cast<scalarT>(0));
	cout << std::hex;
	cout << ">                    v1 = " << v1 << endl;
	cout << ">                    v2 = " << v2 << endl;
	cout << ">       greater(v1, v2) = " << vvm::functional::greater<scalarT>()(v1, v2) << endl;
	cout << ">          less(v1, v2) = " << vvm::functional::less<scalarT>()(v1, v2) << endl;
	cout << "> greater_equal(v1, v2) = " << vvm::functional::greater_equal<scalarT>()(v1, v2) << endl;
	cout << ">    less_equal(v1, v2) = " << vvm::functional::less_equal<scalarT>()(v1, v2) << endl;
	cout << ">      equal_to(v1, v2) = " << vvm::functional::equal_to<scalarT>()(v1, v2) << endl;
	cout << ">  not_equal_to(v1, v2) = " << vvm::functional::not_equal_to<scalarT>()(v1, v2) << endl;
	cout << std::dec;
}

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
	// Should not compile for the following
	// test<float>();
	// test<double>();
	// test<long double>();
	return 0;
}