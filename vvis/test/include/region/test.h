/*
 *  test.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Mon Jan 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  Test program for VVIS regions
 *
 */

#include <iostream>
#include <common/type_name.h>
#include <vvis/vvis.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	typedef scalarT type;
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
	
	vvis::rect_region<vvis::image<type> > roi;
	roi.admit(img1, vvis::rect(1, 1, img1.width() - 2, img1.height() - 2));
	vvis::transform(roi, vvis::pixel_accessor(), roi, vvis::pixel_accessor(),
		vvis::constant<typename vvis::image<type>::pixel_type>(0));
	roi.release();
	cout << "> roi(0) = " << img1 << endl;
	
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
	
	vvis::rect_region<vvis::rgb_image<type> > roi_rgb;
	roi_rgb.admit(rgb1, vvis::rect(1, 1, rgb1.width() - 2, rgb1.height() - 2));
	vvis::transform(roi_rgb, vvis::pixel_accessor(), roi_rgb, vvis::pixel_accessor(),
		vvis::constant<typename vvis::rgb_image<type>::pixel_type>(0));
	roi_rgb.release();
	cout << "> roi(0) = " << rgb1 << endl;
}
