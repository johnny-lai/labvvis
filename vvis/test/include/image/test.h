/*
 *  test.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Fri May 30 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <iostream>
#include <common/type_name.h>
#include <vvis/image.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	
	// 1 Channel Images
	vvis::image<scalarT> img(10, 5);
	for(int y = 0, i = 0; y < img.height(); ++y, ++i) {
		for(int x = 0; x < img.width(); ++x, ++i) {
			img.pixel(x, y) = i;
		}
	}
	cout << "> img = " << img << endl;
	vvis::image<scalarT> img2(img);
	cout << "> img2 = " << img2 << endl;
	// RGB images
	vvis::rgb_image<scalarT> rgb_img(16, 8);
	for(int y = 0, i = 0; y < rgb_img.height(); ++y) {
		for(int x = 0; x < rgb_img.width(); ++x, ++i) {
			rgb_img.red(x, y) = i;
			rgb_img.green(x, y) = i + 1;
			rgb_img.blue(x, y) = i + 2;
		}
	}
	cout << "> rgb_img = " << rgb_img << endl;
	cout << endl;
}