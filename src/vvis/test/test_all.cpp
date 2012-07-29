/*
 *  test_all.cpp
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat Apr 03 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <iostream>
#include <boost/test/unit_test.hpp>
#include <common/type_name.h>
#include <vvis/vvis.h>

using namespace std;
using namespace vvis;
using namespace boost::unit_test_framework;

#define TEST_ALGORITHM

#ifdef TEST_ALL
	#define TEST_IMAGE
	#define TEST_ITERATOR
	#define TEST_ACCESSOR
	#define TEST_ALGORITHM
#endif

//= test_image =================================================================
#ifdef TEST_IMAGE
template<typename imageT> void test_image() {
	// Check Image
	imageT img(10, 10);
	for(int y = 0, i = 0; y < img.height(); ++y, ++i) {
		for(int x = 0; x < img.width(); ++x, ++i) {
			img.pixel(x, y) = i;
			BOOST_CHECK(img.pixel(x, y) == typename imageT::pixel_type(i));
		}
	}
}

test_suite* test_suite_image() {
	test_suite* suite = BOOST_TEST_SUITE("Image");
#define _ADD_TEST_IMAGE(T)  \
	suite->add(BOOST_TEST_CASE((&test_image<image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<image<T, illife_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<rgb_image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<rgb_image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<rgb_image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_image<rgb_image<T, illife_chunky_storage> >)));
	
	_ADD_TEST_IMAGE(unsigned char);
	_ADD_TEST_IMAGE(float);
#undef _ADD_TEST_IMAGE

	return suite;
}
#endif

//= test_iterator ==============================================================
#ifdef TEST_ITERATOR
template<typename imageT> void test_iterator_1d() {
	imageT img(30, 8);
	// Check Iterator
	typename imageT::iterator begin = img.begin();
	typename imageT::iterator end = img.end();
	const int total = img.width() * img.height();
	BOOST_CHECK(end - begin == total);
	++begin.vector; BOOST_CHECK_MESSAGE((end - begin) == (total - VVM_SCALAR_COUNT), "++i.vector failed");
	--begin.vector; BOOST_CHECK_MESSAGE(end - begin == total, "--i.vector failed");
	++begin.scalar; BOOST_CHECK_MESSAGE(end - begin == total - 1, "++i.scalar failed");
	--begin.scalar; BOOST_CHECK_MESSAGE(end - begin == total, "--i.scalar failed");
	BOOST_CHECK(end - (++begin) == total - 1);
	BOOST_CHECK(end - (--begin) == total);
}

template<typename imageT> void test_iterator_2d() {
	imageT img(30, 8);
	// Check Iterator
	typename imageT::iterator begin = img.begin();
	typename imageT::iterator end = img.end();
	BOOST_CHECK(end - begin == img.height());
	for(int y = 0; y < img.height(); ++y) {
		typename imageT::value_type::iterator begin = img[y].begin();
		typename imageT::value_type::iterator end = img[y].end();
		const int total = img.width();
		BOOST_CHECK(end - begin == total);
		++begin.vector; BOOST_CHECK_MESSAGE(end - begin == total - VVM_SCALAR_COUNT, "++i.vector failed");
		--begin.vector; BOOST_CHECK_MESSAGE(end - begin == total, "--i.vector failed");
		++begin.scalar; BOOST_CHECK_MESSAGE(end - begin == total - 1, "++i.scalar failed");
		--begin.scalar; BOOST_CHECK_MESSAGE(end - begin == total, "--i.scalar failed");
		BOOST_CHECK(end - (++begin) == total - 1);
		BOOST_CHECK(end - (--begin) == total);
	}
}

test_suite* test_suite_iterator() {
	test_suite* suite = BOOST_TEST_SUITE("Iterator");
#define _ADD_TEST_ITERATOR(T)  \
	suite->add(BOOST_TEST_CASE((&test_iterator_1d<image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_1d<image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<image<T, illife_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_1d<rgb_image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_1d<rgb_image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<rgb_image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<rgb_image<T, illife_chunky_storage> >)));
	
	_ADD_TEST_ITERATOR(unsigned char);
	_ADD_TEST_ITERATOR(float);
#undef _ADD_TEST_ITERATOR
	return suite;
}
#endif

//= test_accessor ==============================================================
#ifdef TEST_ACCESSOR
template<typename imageT> void test_accessor_contiguous_1ch() {
	typedef typename imageT::pixel_type pixel_type;
	typedef typename vvm::add_vector<typename imageT::pixel_type>::type vector_type;
	// Check Image
	imageT img(30, 8);
	for(int y = 0, i = 0; y < img.height(); ++y, ++i) {
		for(int x = 0; x < img.width(); ++x, ++i) {
			img.pixel(x, y) = i;
			BOOST_CHECK(img.pixel(x, y) == pixel_type(i));
		}
	}
	typename imageT::iterator begin = img.begin();
	// Check pixel_accessor
	pixel_accessor<imageT> a;
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	BOOST_CHECK(vvm::all_eq(a.get_vector(begin), vector_type(45))); 
}

template<typename imageT> void test_accessor_contiguous_rgb() {
	typedef typename imageT::pixel_type pixel_type;
	typedef typename vvm::add_vector<typename imageT::pixel_type>::type vector_type;
	// Check Image
	imageT img(30, 8);
	for(int y = 0, i = 0; y < img.height(); ++y, ++i) {
		for(int x = 0; x < img.width(); ++x, ++i) {
			img.pixel(x, y) = i;
			BOOST_CHECK(img.pixel(x, y) == pixel_type(i));
		}
	}
	typename imageT::iterator begin = img.begin();
	// Check pixel_accessor
	pixel_accessor<imageT> a;
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	vector_type v = a.get_vector(begin);
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK_MESSAGE(v.template get<0>().scalar(i) == 45, "get_vector failed");
	}
}

template<typename imageT> void test_accessor_illife_1ch() {
	typedef typename imageT::pixel_type pixel_type;
	typedef typename vvm::add_vector<typename imageT::pixel_type>::type vector_type;
	// Check Image
	imageT img(30, 8);
	for(int y = 0; y < img.height(); ++y) {
		for(int x = 0; x < img.width(); ++x) {
			img.pixel(x, y) = y*img.width() + x;
			BOOST_CHECK(img.pixel(x, y) == pixel_type(y*img.width() + x));
		}
	}
	// Check pixel_accessor
	typename imageT::value_type::iterator begin = img[0].begin();
	pixel_accessor<imageT> a;
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	BOOST_CHECK(vvm::all_eq(a.get_vector(begin), vector_type(45))); 
	begin = img[1].begin();
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i + img.width()));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	BOOST_CHECK(vvm::all_eq(a.get_vector(begin), vector_type(45))); 
}

template<typename imageT> void test_accessor_illife_rgb() {
	typedef typename imageT::pixel_type pixel_type;
	typedef typename vvm::add_vector<typename imageT::pixel_type>::type vector_type;
	// Check Image
	imageT img(30, 8);
	for(int y = 0; y < img.height(); ++y) {
		for(int x = 0; x < img.width(); ++x) {
			img.pixel(x, y) = y*img.width() + x;
			BOOST_CHECK(img.pixel(x, y) == pixel_type(y*img.width() + x));
		}
	}
	// Check pixel_accessor
	typename imageT::value_type::iterator begin = img[0].begin();
	pixel_accessor<imageT> a;
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	vector_type v = a.get_vector(begin);
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK_MESSAGE(v.template get<0>().scalar(i) == 45, "get_vector failed");
	}
	begin = img[1].begin();
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(i + img.width()));
		a.set_scalar(pixel_type(45), (begin + i));
		BOOST_CHECK(a.get_scalar(begin + i) == pixel_type(45));
	}
	v = a.get_vector(begin);
	for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
		BOOST_CHECK_MESSAGE(v.template get<0>().scalar(i) == 45, "get_vector failed");
	}
}

test_suite* test_suite_accessor() {
	test_suite* suite = BOOST_TEST_SUITE("Accessor");
#define _ADD_TEST_ACCESSOR(T)  \
	suite->add(BOOST_TEST_CASE((&test_accessor_contiguous_1ch<image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_contiguous_1ch<image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_contiguous_rgb<rgb_image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_contiguous_rgb<rgb_image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_illife_1ch<image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_illife_1ch<image<T, illife_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_illife_rgb<rgb_image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_accessor_illife_rgb<rgb_image<T, illife_chunky_storage> >)));

	_ADD_TEST_ACCESSOR(unsigned char);
	_ADD_TEST_ACCESSOR(float);
#undef _ADD_TEST_ACCESSOR
	return suite;
}
#endif


//= test_algorithm =============================================================
#ifdef TEST_ALGORITHM
template<typename imageT> void test_algorithm_1ch() {
	const int width = 20;
	const int height = 5;
	typename imageT::pixel_type min, max;
	// 1 Channel Images
	imageT img1(width, height), img2(width, height), dest(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			img1.pixel(x, y) = i % width + 60;
			img2.pixel(x, y) = 80 - (i % width);
		}
	}
	max = min = img1.pixel(0, 0);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			if(img1.pixel(x, y) < min)
				min = img1.pixel(x, y);
			if(img1.pixel(x, y) > max)
				max = img1.pixel(x, y);
		}
	}
	

	vvis::min_max<typename imageT::pixel_type> m;
	vvis::for_each(img1, vvis::pixel_accessor<imageT>(), m);
	BOOST_CHECK(m.min() == min);
	BOOST_CHECK(m.max() == max);
	
	vvis::transform(img1, vvis::pixel_accessor<imageT>(),
		img2, vvis::pixel_accessor<imageT>(),
		dest, vvis::pixel_accessor<imageT>(),
		vvis::plus<>());
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			BOOST_REQUIRE_MESSAGE(dest.pixel(x, y) == img1.pixel(x, y) + img2.pixel(x, y),
				"Image Addition failed");
		}
	}
	
	vvis::transform(img1, vvis::pixel_accessor<imageT>(),
		dest, vvis::pixel_accessor<imageT>(),
		vvis::bind2nd(vvis::less<typename imageT::pixel_type>(), 50));
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			BOOST_REQUIRE_MESSAGE(dest.pixel(x, y) == (img1.pixel(x, y) < 50) ? VVM_TRUE : VVM_FALSE,
				"Image < 50 failed");
		}
	}
}

test_suite* test_suite_algorithm() {
	test_suite* suite = BOOST_TEST_SUITE("Iterator");
#define _ADD_TEST_ALGORITHM(T)  \
	suite->add(BOOST_TEST_CASE((&test_algorithm_1ch<image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_algorithm_1ch<image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_algorithm_1ch<image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_algorithm_1ch<image<T, illife_chunky_storage> >))); \
	//suite->add(BOOST_TEST_CASE((&test_iterator_1d<rgb_image<T, contiguous_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_1d<rgb_image<T, contiguous_chunky_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<rgb_image<T, illife_planar_storage> >))); \
	suite->add(BOOST_TEST_CASE((&test_iterator_2d<rgb_image<T, illife_chunky_storage> >)));
	
	_ADD_TEST_ALGORITHM(unsigned char);
	_ADD_TEST_ALGORITHM(float);
#undef _ADD_TEST_ALGORITHM
	return suite;
}
#endif

//= main test suite ============================================================
test_suite* init_unit_test_suite(int argc, char *argv[])
{
	BOOST_MESSAGE("Test Suite was compiled for vector processor ("
		<< VVM_INFO_VECTOR_PROCESSOR << ")");
	
    test_suite* tests = BOOST_TEST_SUITE("VVIS Test Suite");

	#ifdef TEST_IMAGE
		tests->add(test_suite_image());
	#endif
	
	#ifdef TEST_ITERATOR
		tests->add(test_suite_iterator());
	#endif
	
	#ifdef TEST_ACCESSOR
		tests->add(test_suite_accessor());
	#endif

	#ifdef TEST_ALGORITHM
		tests->add(test_suite_algorithm());
	#endif
	
    return tests;
}