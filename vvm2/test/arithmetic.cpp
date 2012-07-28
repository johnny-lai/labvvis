/*
 *  arithmetic.cpp
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

template<typename scalarT> void test_int() {
	cout << "Testing " << type_name(typeid(scalarT))
		<< "(" << vector_traits<vector<scalarT> >::scalar_count << ", "
		<< vector_traits<vector<scalarT> >::vpvector_count << ")" << endl;
	scalarT a = 3, b = 4, c = 8;
	vector<scalarT> v1(a), v2(b), v3(c);
	BOOST_CHECK(all_eq(v1 + v2 + v3, vector<scalarT>(a + b + c)));
	BOOST_CHECK(all_eq(v1 - v2 - v3, vector<scalarT>(a - b - c)));
	BOOST_CHECK(all_eq(v1 * v2 * v3, vector<scalarT>(a * b * c)));
	BOOST_CHECK(all_eq(v1 / v2 / v3, vector<scalarT>(a / b / c)));
	BOOST_CHECK(all_eq(v1 % v2 % v3, vector<scalarT>(a % b % c)));
	BOOST_CHECK(all_eq(v3 / v1, vector<scalarT>(c / a)));
	BOOST_CHECK(all_eq((v1 + v2) * (v1 - v2), vector<scalarT>((a + b) * (a - b))));
	BOOST_CHECK(all_eq((v3 % v2) / v1, vector<scalarT>((c % b) / a)));
	BOOST_CHECK(all_eq(++v1, vector<scalarT>(++a)));
	BOOST_CHECK(all_eq(v1++, vector<scalarT>(a++)));
	BOOST_CHECK(all_eq(--v1, vector<scalarT>(--a)));
	BOOST_CHECK(all_eq(v1--, vector<scalarT>(a--)));
	BOOST_CHECK(all_eq(v1 += v2, vector<scalarT>(a += b)));
	BOOST_CHECK(all_eq(v1 -= v2, vector<scalarT>(a -= b)));
	BOOST_CHECK(all_eq(v1 *= v2, vector<scalarT>(a *= b)));
	BOOST_CHECK(all_eq(v1 /= v2, vector<scalarT>(a /= b)));
	BOOST_CHECK(all_eq(v1 %= v2, vector<scalarT>(a %= b)));
	BOOST_CHECK(all_eq(madd(v1, v2, v3), vector<scalarT>(a * b + c)));
	BOOST_CHECK(all_eq(nmsub(v1, v2, v3), vector<scalarT>(-(a * b - c))));
}

template<typename scalarT> void test_real() {
	cout << "Testing " << type_name(typeid(scalarT))
		<< "(" << vector_traits<vector<scalarT> >::scalar_count << ", "
		<< vector_traits<vector<scalarT> >::vpvector_count << ")" << endl;
	scalarT a = 3, b = 4, c = 8;
	vector<scalarT> v1(a), v2(b), v3(c);
	BOOST_CHECK(all_eq(v1 + v2 + v3, vector<scalarT>(a + b + c)));
	BOOST_CHECK(all_eq(v1 - v2 - v3, vector<scalarT>(a - b - c)));
	BOOST_CHECK(all_eq(v1 * v2 * v3, vector<scalarT>(a * b * c)));
	BOOST_CHECK(all_eq(v1 / v2 / v3, vector<scalarT>(a / b / c)));
	BOOST_CHECK(all_eq(v3 / v1, vector<scalarT>(c / a)));
	BOOST_CHECK(all_eq((v1 + v2) * (v1 - v2), vector<scalarT>((a + b) * (a - b))));
	BOOST_CHECK(all_eq(++v1, vector<scalarT>(++a)));
	BOOST_CHECK(all_eq(v1++, vector<scalarT>(a++)));
	BOOST_CHECK(all_eq(--v1, vector<scalarT>(--a)));
	BOOST_CHECK(all_eq(v1--, vector<scalarT>(a--)));
	BOOST_CHECK(all_eq(v1 += v2, vector<scalarT>(a += b)));
	BOOST_CHECK(all_eq(v1 -= v2, vector<scalarT>(a -= b)));
	BOOST_CHECK(all_eq(v1 *= v2, vector<scalarT>(a *= b)));
	BOOST_CHECK(all_eq(v1 /= v2, vector<scalarT>(a /= b)));
	BOOST_CHECK(all_eq(madd(v1, v2, v3), vector<scalarT>(a * b + c)));
	BOOST_CHECK(all_eq(nmsub(v1, v2, v3), vector<scalarT>(-(a * b - c))));
}

test_suite* init_unit_test_suite(int argc, char *argv[])
{
    test_suite* tests = BOOST_TEST_SUITE("VVM Arithmetic Test");

	tests->add(BOOST_TEST_CASE(&test_int<char>));
	tests->add(BOOST_TEST_CASE(&test_int<signed char>));
	tests->add(BOOST_TEST_CASE(&test_int<unsigned char>));
	tests->add(BOOST_TEST_CASE(&test_int<short int>));
	tests->add(BOOST_TEST_CASE(&test_int<unsigned short int>));
	tests->add(BOOST_TEST_CASE(&test_int<long int>));
	tests->add(BOOST_TEST_CASE(&test_int<unsigned long int>));
	tests->add(BOOST_TEST_CASE(&test_real<float>));
	tests->add(BOOST_TEST_CASE(&test_real<double>));
	tests->add(BOOST_TEST_CASE(&test_real<long double>));
	
    return tests;
}
