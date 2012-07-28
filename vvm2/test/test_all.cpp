/*
 *  test_all.cpp
 *  vvm
 *
 *  Created by Bing-Chang Lai on Wed Feb 29 2004.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <common/type_name.h>
#include <vvm/vvm.h>


using namespace vvm;
using namespace boost::unit_test_framework;

#define VVM_BOOL(x) (x) ? ~0 : 0

#ifdef TEST_ALL
	#define TEST_ARITHMETIC
	#define TEST_BITWISE
	#define TEST_COMPARISON
	#define TEST_LOGICAL
	#define TEST_SHIFT
	#define TEST_VECTOR
	#define TEST_PREDICATE
	#define TEST_VECTOR_CAST
#endif

//= test_arithmetic ============================================================
#ifdef TEST_ARITHMETIC
template<typename scalarT> void test_arithmetic_int() {
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

template<typename scalarT> void test_arithmetic_real() {
	scalarT a = 3, b = 4, c = 8;
	vvm::vector<scalarT> v1(a), v2(b), v3(c);
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

test_suite* test_suite_arithmetic() {
	test_suite* arithmetic = BOOST_TEST_SUITE("Arithmetic");
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<char>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<signed char>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<unsigned char>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<short int>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<unsigned short int>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<long int>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_int<unsigned long int>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_real<float>));
	arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_real<double>));
	//arithmetic->add(BOOST_TEST_CASE(&test_arithmetic_real<long double>));
	return arithmetic;
}
#endif

//= test_bitwise ===============================================================
#ifdef TEST_BITWISE
template<typename scalarT> void test_bitwise() {
	scalarT a = 7, b = 8, c = 16;
	vector<scalarT> v1(a), v2(b), v3(c);
	BOOST_CHECK(all_eq(~v1, vector<scalarT>(~a)));
	BOOST_CHECK(all_eq(v1 ^ v2 ^ v3, vector<scalarT>(a ^ b ^ c)));
	BOOST_CHECK(all_eq(v1 & v2 & v3, vector<scalarT>(a & b & c)));
	BOOST_CHECK(all_eq(v1 | v2 | v3, vector<scalarT>(a | b | c)));
}

test_suite* test_suite_bitwise() {
	test_suite* bitwise = BOOST_TEST_SUITE("Bitwise");
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<char>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<signed char>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<unsigned char>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<short int>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<unsigned short int>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<long int>));
	bitwise->add(BOOST_TEST_CASE(&test_bitwise<unsigned long int>));
	return bitwise;
}
#endif

//= test_comparison ============================================================
#ifdef TEST_COMPARISON
template<typename scalarT> void test_comparison() {
	typedef typename vector_traits<vector<scalarT> >::bool_type vector_bool_type;
	scalarT a = 7, b = 0;
	vector<scalarT> v1(a), v2(b);
	BOOST_CHECK(all_eq(v1 > v2, vector_bool_type((a > b) ? ~0 : 0)));
	BOOST_CHECK(all_eq(v1 < v2, vector_bool_type((a < b) ? ~0 : 0)));
	BOOST_CHECK(all_eq(v1 >= v2, vector_bool_type((a >= b) ? ~0 : 0)));
	BOOST_CHECK(all_eq(v1 <= v2, vector_bool_type((a <= b) ? ~0 : 0)));
	BOOST_CHECK(all_eq(v1 == v2, vector_bool_type((a == b) ? ~0 : 0)));
	BOOST_CHECK(all_eq(v1 != v2, vector_bool_type((a != b) ? ~0 : 0)));
}

test_suite* test_suite_comparison() {
	test_suite* comparison = BOOST_TEST_SUITE("Comparison");
	comparison->add(BOOST_TEST_CASE(&test_comparison<char>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<signed char>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<unsigned char>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<short int>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<unsigned short int>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<long int>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<unsigned long int>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<float>));
	comparison->add(BOOST_TEST_CASE(&test_comparison<double>));
	//comparison->add(BOOST_TEST_CASE(&test_comparison<long double>));
	return comparison;
}
#endif

//= test_logical ===============================================================
#ifdef TEST_LOGICAL
template<typename scalarT> void test_logical() {
	typedef typename vector_traits<vector<scalarT> >::bool_type vector_bool_type;
	scalarT a = 7, b = 0;
	vector<scalarT> v1(a), v2(b);
	BOOST_CHECK(all_eq(!v2, vector_bool_type(VVM_BOOL(!b))));
	BOOST_CHECK(all_eq(v1 || v2, vector_bool_type(VVM_BOOL(a || b))));
	BOOST_CHECK(all_eq(v1 && v2, vector_bool_type(VVM_BOOL(a && b))));
}

test_suite* test_suite_logical() {
	test_suite* logical = BOOST_TEST_SUITE("Logical");
	logical->add(BOOST_TEST_CASE(&test_logical<char>));
	logical->add(BOOST_TEST_CASE(&test_logical<signed char>));
	logical->add(BOOST_TEST_CASE(&test_logical<unsigned char>));
	logical->add(BOOST_TEST_CASE(&test_logical<short int>));
	logical->add(BOOST_TEST_CASE(&test_logical<unsigned short int>));
	logical->add(BOOST_TEST_CASE(&test_logical<long int>));
	logical->add(BOOST_TEST_CASE(&test_logical<unsigned long int>));
	logical->add(BOOST_TEST_CASE(&test_logical<float>));
	logical->add(BOOST_TEST_CASE(&test_logical<double>));
	//logical->add(BOOST_TEST_CASE(&test_logical<long double>));
	return logical;
}
#endif

//= test_shift =================================================================
#ifdef TEST_SHIFT
template<typename scalarT> void test_shift() {
	const scalarT a = 7, b = 3;
	BOOST_ASSERT(a != b);
	vector<scalarT> v1(a), v2(b);
	BOOST_CHECK(all_eq((v1 << v2), vector<scalarT>(a << b)));
	BOOST_CHECK(all_eq((v1 >> v2), vector<scalarT>(a >> b)));
	BOOST_CHECK(all_eq((v1 << 3) >> 3, v1));
}

test_suite* test_suite_shift() {
	test_suite* vsuite = BOOST_TEST_SUITE("Shift");
	vsuite->add(BOOST_TEST_CASE(&test_shift<char>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<signed char>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<unsigned char>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<short int>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<unsigned short int>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<long int>));
	vsuite->add(BOOST_TEST_CASE(&test_shift<unsigned long int>));
	//vsuite->add(BOOST_TEST_CASE(&test_shift<float>));
	//vsuite->add(BOOST_TEST_CASE(&test_shift<double>));
	//vsuite->add(BOOST_TEST_CASE(&test_shift<long double>));
	return vsuite;
}
#endif

//= test_vector =============================================================
#ifdef TEST_VECTOR
template<typename scalarT> void test_vector() {
	const scalarT a = 7, b = 3;
	BOOST_ASSERT(a != b);
	vector<scalarT> v1(a), v2(b);
	typename vector_traits<vector<scalarT> >::bool_type v3(0);
	BOOST_CHECK(all_eq(select(v3, v1, v2), v1));
	BOOST_CHECK(all_eq(select(v3, v2, v1), v2));
	//BOOST_CHECK(mergeh(v1, v1) == (a == a));
	//BOOST_CHECK(mergel(v1, v2) == (a >= b));
	BOOST_CHECK(all_eq(min(v1, v2), a < b ? v1 : v2));
	BOOST_CHECK(all_eq(max(v1, v2), a > b ? v1 : v2));
	BOOST_CHECK(sum(v1) == (a * VVM_SCALAR_COUNT));
	// The following tests are only applicable if there is more than one
	// scalar in a vvm::vector
	if(vector_traits<vector<scalarT> >::scalar_count > 1) {
		BOOST_MESSAGE("Additional Vector Tests");
		v1.scalar(0) = b;
		v2.scalar(0) = a;
		BOOST_CHECK(all_eq(min(v1, v2), vector<scalarT>((a < b) ? a : b)));
		BOOST_CHECK(all_eq(max(v1, v2), vector<scalarT>((a > b) ? a : b)));
		BOOST_CHECK(sum(v1) == (a * (VVM_SCALAR_COUNT - 1)) + b);
	}
}

test_suite* test_suite_vector() {
	test_suite* vsuite = BOOST_TEST_SUITE("Vector");
	vsuite->add(BOOST_TEST_CASE(&test_vector<char>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<signed char>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<unsigned char>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<short int>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<unsigned short int>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<long int>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<unsigned long int>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<float>));
	vsuite->add(BOOST_TEST_CASE(&test_vector<double>));
	//vsuite->add(BOOST_TEST_CASE(&test_vector<long double>));
	return vsuite;
}
#endif

//= test_predicate =============================================================
#ifdef TEST_PREDICATE
template<typename scalarT> void test_predicate() {
	const scalarT a = 7, b = 0;
	BOOST_ASSERT(a != b);
	vector<scalarT> v1(a), v2(b);
	BOOST_CHECK(all_eq(v1, v2) == (a == b));
	BOOST_CHECK(all_eq(v1, v1) == (a == a));
	BOOST_CHECK(all_ge(v1, v2) == (a >= b));
	BOOST_CHECK(all_gt(v1, v2) == (a > b));
	BOOST_CHECK(all_le(v1, v2) == (a <= b));
	BOOST_CHECK(all_lt(v1, v2) == (a < b));
	BOOST_CHECK(all_ne(v1, v2) == !(a == b));
	BOOST_CHECK(all_ne(v1, v1) == !(a == a));
	BOOST_CHECK(all_nge(v1, v2) == !(a >= b));
	BOOST_CHECK(all_ngt(v1, v2) == !(a > b));
	BOOST_CHECK(all_nle(v1, v2) == !(a <= b));
	BOOST_CHECK(all_nlt(v1, v2) == !(a < b));
	BOOST_CHECK(any_eq(v1, v2) == (a == b));
	BOOST_CHECK(any_eq(v1, v1) == (a == a));
	BOOST_CHECK(any_ge(v1, v2) == (a >= b));
	BOOST_CHECK(any_gt(v1, v2) == (a > b));
	BOOST_CHECK(any_le(v1, v2) == (a <= b));
	BOOST_CHECK(any_lt(v1, v2) == (a < b));
	BOOST_CHECK(any_ne(v1, v2) == !(a == b));
	BOOST_CHECK(any_ne(v1, v1) == !(a == a));
	BOOST_CHECK(any_nge(v1, v2) == !(a >= b));
	BOOST_CHECK(any_ngt(v1, v2) == !(a > b));
	BOOST_CHECK(any_nle(v1, v2) == !(a <= b));
	BOOST_CHECK(any_nlt(v1, v2) == !(a < b));
	// The following tests are only applicable if there is more than one
	// scalar in a vvm::vector
	if(vector_traits<vector<scalarT> >::scalar_count > 1) {
		BOOST_MESSAGE("Additional Predicate Tests");
		vector<scalarT> v3(a);
		v3.scalar(0) = a - 1;
		v3.scalar(1) = a + 1;
		BOOST_CHECK(all_eq(v1, v3) == false);
		BOOST_CHECK(all_ge(v1, v3) == false);
		BOOST_CHECK(all_gt(v1, v3) == false);
		BOOST_CHECK(all_le(v1, v3) == false);
		BOOST_CHECK(all_lt(v1, v3) == false);
		BOOST_CHECK(all_ne(v1, v3) == false);
		BOOST_CHECK(all_nge(v1, v3) == false);
		BOOST_CHECK(all_ngt(v1, v3) == false);
		BOOST_CHECK(all_nle(v1, v3) == false);
		BOOST_CHECK(all_nlt(v1, v3) == false);
		BOOST_CHECK(any_eq(v1, v3) == true);
		BOOST_CHECK(any_ge(v1, v3) == true);
		BOOST_CHECK(any_gt(v1, v3) == true);
		BOOST_CHECK(any_le(v1, v3) == true);
		BOOST_CHECK(any_lt(v1, v3) == true);
		BOOST_CHECK(any_ne(v1, v3) == true);
		BOOST_CHECK(any_nge(v1, v3) == true);
		BOOST_CHECK(any_ngt(v1, v3) == true);
		BOOST_CHECK(any_nle(v1, v3) == true);
		BOOST_CHECK(any_nlt(v1, v3) == true);
	}
}

test_suite* test_suite_predicate() {
	test_suite* predicate = BOOST_TEST_SUITE("Predicate");
	predicate->add(BOOST_TEST_CASE(&test_predicate<char>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<signed char>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<unsigned char>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<short int>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<unsigned short int>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<long int>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<unsigned long int>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<float>));
	predicate->add(BOOST_TEST_CASE(&test_predicate<double>));
	//predicate->add(BOOST_TEST_CASE(&test_predicate<long double>));
	return predicate;
}
#endif

//= test_vector_cast ===========================================================
#ifdef TEST_VECTOR_CAST
template<typename scalarT> void test_vector_cast() {
	const int a = 7;
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
	//BOOST_CHECK(all_eq(vector_cast<vector<long double> >(v1), vector<long double>(a)));
}

test_suite* test_suite_vector_cast() {
	test_suite* cast = BOOST_TEST_SUITE("Vector Cast");
	cast->add(BOOST_TEST_CASE(&test_vector_cast<char>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<signed char>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<unsigned char>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<short int>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<unsigned short int>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<long int>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<unsigned long int>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<float>));
	cast->add(BOOST_TEST_CASE(&test_vector_cast<double>));
	//cast->add(BOOST_TEST_CASE(&test_vector_cast<long double>));
	return cast;
}
#endif

test_suite* init_unit_test_suite(int argc, char *argv[])
{
	BOOST_MESSAGE("Test Suite was compiled for vector processor ("
		<< VVM_INFO_VECTOR_PROCESSOR << ")");
	
    test_suite* tests = BOOST_TEST_SUITE("VVM Test Suite");

	#ifdef TEST_ARITHMETIC
		tests->add(test_suite_arithmetic());
	#endif

	#ifdef TEST_BITWISE
		tests->add(test_suite_bitwise());
	#endif

	#ifdef TEST_COMPARISON
		tests->add(test_suite_comparison());
	#endif

	#ifdef TEST_LOGICAL
		tests->add(test_suite_logical());
	#endif

	#ifdef TEST_SHIFT
		tests->add(test_suite_shift());
	#endif
	
	#ifdef TEST_VECTOR
		tests->add(test_suite_vector());
	#endif
	
	#ifdef TEST_PREDICATE
		tests->add(test_suite_predicate());
	#endif

	#ifdef TEST_VECTOR_CAST
		tests->add(test_suite_vector_cast());
	#endif
	
    return tests;
}
