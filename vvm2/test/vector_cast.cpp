/*
 *  vector_cast.cpp
 *  vvm
 *
 *  Created by Bing-Chang Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
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
	scalarT a = 7;
	vector<scalarT> v1(a);
	BOOST_CHECK(all_eq(vector_cast<vector<char> >(v1), vector<char>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<signed char> >(v1), vector<signed char>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<unsigned char> >(v1), vector<unsigned char>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<short int> >(v1), vector<short int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<unsigned short int> >(v1), vector<unsigned short int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<int> >(v1), vector<int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<unsigned int> >(v1), vector<unsigned int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<long int> >(v1), vector<long int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<unsigned long int> >(v1), vector<unsigned long int>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<float> >(v1), vector<float>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<double> >(v1), vector<double>(a)));
	BOOST_CHECK(all_eq(vector_cast<vector<long double> >(v1), vector<long double>(a)));
}

test_suite* init_unit_test_suite(int argc, char *argv[])
{
    test_suite* tests = BOOST_TEST_SUITE("VVM Logical Test");

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