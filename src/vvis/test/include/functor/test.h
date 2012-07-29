/*
 *  functor.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Mon Jan 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <iostream>
#include <common/type_name.h>
#include <vvis/vvis.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
}

template<> void test<sint8>() {
	typedef sint8 type;
	cout << "Testing " << type_name(typeid(type)) << endl;
	
	const int width = 20;
	const int height = 5;
	
	// 1 Channel Images
	vvis::image<type> img1(width, height), img2(width, height), dest(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			img1.pixel(x, y) = i % width + 60;
			img2.pixel(x, y) = 80 - (i % width);
		}
	}
	cout << "> img1 = " << img1 << endl;
	cout << "> img2 = " << img2 << endl;

	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::negate<>());
	cout << "> -img1 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::plus<>());
	cout << "> img1 + img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::minus<>());
	cout << "> img1 - img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::multiplies<>());
	cout << "> img1 * img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::divides<>());
	cout << "> img1 / img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::modulus<>());
	cout << "> img1 % img2 = " << dest << endl;

	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::equal_to<>());
	cout << "> img1 == img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::not_equal_to<>());
	cout << "> img1 != img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::less<>());
	cout << "> img1 < img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::greater<>());
	cout << "> img1 > img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::less_equal<>());
	cout << "> img1 <= img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::greater_equal<>());
	cout << "> img1 >= img2 = " << dest << endl;

	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::bitwise_and<>());
	cout << "> img1 & img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::bitwise_or<>());
	cout << "> img1 | img2 = " << dest << endl;	
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::bitwise_not<>());
	cout << "> ~img1 = " << dest << endl;
	
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::logical_and<>());
	cout << "> img1 && img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::logical_or<>());
	cout << "> img1 || img2 = " << dest << endl;	
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::logical_not<>());
	cout << "> !img1 = " << dest << endl;
	
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::plus_saturated<>());
	cout << "> adds(img1, img2) = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::minus_saturated<>());
	cout << "> subs(img1, img2) = " << dest << endl;

	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::equalize<type>(30, 70));
	cout << "> equalize(img1, 30, 70) = " << dest << endl;
		
	vvis::transform(img1, dest, vvis::pixel_accessor(),
		vvis::bind2nd(vvis::less<type>(), 50));
	
	// RGB images
	vvis::rgb_image<type> rgb1(width, height), rgb2(width, height), rgbd(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			rgb1.red(x, y) = i % width;
			rgb1.green(x, y) = rgb1.red(x, y) + 1;
			rgb1.blue(x, y) = rgb1.green(x, y) + 2;
			rgb2.red(x, y) = i % (width / 2);
			rgb2.green(x, y) = rgb2.red(x, y) + 1;
			rgb2.blue(x, y) = rgb2.green(x, y) + 2;
		}
	}
	cout << "> rgb1 = " << rgb1 << endl;
	cout << "> rgb2 = " << rgb2 << endl;
	vvis::transform(rgb1, rgb2, rgbd, vvis::pixel_accessor(), vvis::plus<>());
	cout << "> rgb1 + rgb2 = " << rgbd << endl;
}

template<> void test<float>() {
	typedef float type;
	cout << "Testing " << type_name(typeid(type)) << endl;
	
	const int width = 20;
	const int height = 5;
	
	// 1 Channel Images
	vvis::image<type> img1(width, height), img2(width, height), dest(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			img1.pixel(x, y) = i % width + 60;
			img2.pixel(x, y) = 80 - (i % width);
		}
	}
	cout << "> img1 = " << img1 << endl;
	cout << "> img2 = " << img2 << endl;
	/*
	//vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::negate<>());
	//cout << "> -img1 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::plus<>());
	cout << "> img1 + img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::minus<>());
	cout << "> img1 - img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::multiplies<>());
	cout << "> img1 * img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::divides<>());
	cout << "> img1 / img2 = " << dest << endl;
	//vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::modulus<>());
	//cout << "> img1 % img2 = " << dest << endl;
	
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::equal_to<>());
	cout << "> img1 == img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::not_equal_to<>());
	cout << "> img1 != img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::less<>());
	cout << "> img1 < img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::greater<>());
	cout << "> img1 > img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::less_equal<>());
	cout << "> img1 <= img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::greater_equal<>());
	cout << "> img1 >= img2 = " << dest << endl;
	*/
	/* No bitwise functors
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::bitwise_and<>());
	cout << "> img1 & img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::bitwise_or<>());
	cout << "> img1 | img2 = " << dest << endl;	
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::bitwise_not<>());
	cout << "> ~img1 = " << dest << endl;
	*/
	/*
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::logical_and<>());
	cout << "> img1 && img2 = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::logical_or<>());
	cout << "> img1 || img2 = " << dest << endl;	
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::logical_not<>());
	cout << "> !img1 = " << dest << endl;
	
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::plus_saturated<>());
	cout << "> adds(img1, img2) = " << dest << endl;
	vvis::transform(img1, img2, dest, vvis::pixel_accessor(), vvis::minus_saturated<>());
	cout << "> subs(img1, img2) = " << dest << endl;
	*/
	//vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::equalize<type>(30, 70));
	//cout << "> equalize(img1, 30, 70) = " << dest << endl;
	
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::logarithmic<>());
	cout << "> logarithmic(img1) = " << dest << endl;
	vvis::transform(img1, dest, vvis::pixel_accessor(), vvis::exponential<>());
	cout << "> exponential(img1) = " << dest << endl;
		
	//vvis::transform(img1, dest, vvis::pixel_accessor(),
	//	vvis::bind2nd(vvis::less<type>(), 50));
	
	// RGB images
	vvis::rgb_image<type> rgb1(width, height), rgb2(width, height), rgbd(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			rgb1.red(x, y) = i % width;
			rgb1.green(x, y) = rgb1.red(x, y) + 1;
			rgb1.blue(x, y) = rgb1.green(x, y) + 2;
			rgb2.red(x, y) = i % (width / 2);
			rgb2.green(x, y) = rgb2.red(x, y) + 1;
			rgb2.blue(x, y) = rgb2.green(x, y) + 2;
		}
	}
	cout << "> rgb1 = " << rgb1 << endl;
	cout << "> rgb2 = " << rgb2 << endl;
	vvis::transform(rgb1, rgb2, rgbd, vvis::pixel_accessor(), vvis::plus<>());
	cout << "> rgb1 + rgb2 = " << rgbd << endl;
	
	vvis::transform(rgb1, rgbd, vvis::pixel_accessor(), vvis::logarithmic<>());
	cout << "> logarithmic(rgb1) = " << rgbd << endl;
	vvis::transform(rgb1, rgbd, vvis::pixel_accessor(), vvis::exponential<>());
	cout << "> exponential(rgb1) = " << rgbd << endl;
}