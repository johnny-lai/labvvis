/*
 *  math.cpp
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Jan 15 2004.
 *  Copyright (c) 2004 University of Wollongong. All rights reserved.
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
}

template<typename scalarT> void test_real() {
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	cout << "+   scalar_count: " << vector_traits<vector<scalarT> >::scalar_count << endl;
	cout << "+ vpvector_count: " << vector_traits<vector<scalarT> >::vpvector_count << endl;
	vector<scalarT> v1(-10.4), v2(0.9), v3(30.5);
	cout << ">         abs(v1) = " << vvm::abs(v1) << endl;
	cout << ">        acos(v2) = " << vvm::acos(v2) << endl;
	cout << ">        asin(v2) = " << vvm::asin(v2) << endl;
	cout << ">        atan(v2) = " << vvm::atan(v2) << endl;
	cout << ">       atan2(v2) = " << vvm::atan2(v2, v2) << endl;
	cout << ">        ceil(v1) = " << vvm::ceil(v1) << endl;
	cout << ">         cos(v2) = " << vvm::cos(v2) << endl;
	cout << ">        cosh(v2) = " << vvm::cosh(v2) << endl;
	cout << ">         exp(v2) = " << vvm::exp(v1) << endl;
	//cout << ">        fabs(v1) = " << vvm::fabs(v1) << endl;
	cout << ">       floor(v1) = " << vvm::floor(v1) << endl;
	cout << ">        fmod(v1) = " << vvm::fmod(v1, v2) << endl;
	//cout << ">       frexp(v1) = " << vvm::frexp(v1, int*) << endl;
	//cout << ">       ldexp(v1) = " << vvm::ldexp(v1, int) << endl;
	cout << ">         log(v2) = " << vvm::log(v2) << endl;
	cout << ">       log10(v2) = " << vvm::log10(v2) << endl;
	//cout << ">        modf(v1) = " << vvm::modf(v1, float*) << endl;
	cout << ">     pow(v2, v2) = " << vvm::pow(v2, v2) << endl;
	//cout << ">         pow(v1) = " << vvm::pow(v1, int) << endl;
	cout << ">         sin(v2) = " << vvm::sin(v2) << endl;
	cout << ">        sinh(v2) = " << vvm::sinh(v2) << endl;
	cout << ">        sqrt(v2) = " << vvm::sqrt(v2) << endl;
	cout << ">         tan(v2) = " << vvm::tan(v2) << endl;
	cout << ">        tanh(v2) = " << vvm::tanh(v2) << endl;
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
	test_real<float>();
	test_real<double>();
	test_real<long double>();
	return 0;
}
