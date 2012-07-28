/*
 *  comparison.cpp
 *  vvm
 *
 *  Created by Bing-Chang Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <common/type_name.h>
#include <vvm/vvm.h>

using namespace std;
using namespace vvm;
using namespace boost::unit_test_framework;

template<typename scalarT> void test() {
	cout << "Testing " << type_name(typeid(scalarT))
		<< "(" << vector_traits<vector<scalarT> >::scalar_count << ", "
		<< vector_traits<vector<scalarT> >::vpvector_count << ")" << endl;
	typedef vector_traits<vector<scalarT> >::bool_type vector_bool_type;
	scalarT a = 7, b = 0;
	vector<scalarT> v1(a), v2(b);
	BOOST_CHECK(all_eq(v1 > v2, vector_bool_type(a > b)));
	BOOST_CHECK(all_eq(v1 < v2, vector_bool_type(a < b)));
	BOOST_CHECK(all_eq(v1 >= v2, vector_bool_type(a >= b)));
	BOOST_CHECK(all_eq(v1 <= v2, vector_bool_type(a <= b)));
	BOOST_CHECK(all_eq(v1 == v2, vector_bool_type(a == b)));
	BOOST_CHECK(all_eq(v1 != v2, vector_bool_type(a != b)));
}

test_suite* init_unit_test_suite(int argc, char *argv[])
{
    test_suite* tests = BOOST_TEST_SUITE("VVM Comparison Test");

	tests->add(BOOST_TEST_CASE(&test<char>));
	tests->add(BOOST_TEST_CASE(&test<signed char>));
	tests->add(BOOST_TEST_CASE(&test<unsigned char>));
	tests->add(BOOST_TEST_CASE(&test<short int>));
	tests->add(BOOST_TEST_CASE(&test<unsigned short int>));
	tests->add(BOOST_TEST_CASE(&test<long int>));
	tests->add(BOOST_TEST_CASE(&test<unsigned long int>));
	tests->add(BOOST_TEST_CASE(&test<float>));
	tests->add(BOOST_TEST_CASE(&test<double>));
	tests->add(BOOST_TEST_CASE(&test<long double>));
	
    return tests;
}