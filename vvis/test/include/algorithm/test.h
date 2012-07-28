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
#include <vvis/vvis.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	
	const int width = 20;
	const int height = 5;
	
	// 1 Channel Images
	vvis::image<scalarT> img1(width, height), img2(width, height), dest(width, height);
	for(int y = 0, i = 0; y < height; ++y, ++i) {
		for(int x = 0; x < width; ++x, ++i) {
			img1.pixel(x, y) = i % width + 60;
			img2.pixel(x, y) = 80 - (i % width);
		}
	}
	cout << "> img1 = " << img1 << endl;
	cout << "> img2 = " << img2 << endl;

	vvis::transform(img1, vvis::pixel_accessor<vvis::image<scalarT> >(),
		img2, vvis::pixel_accessor<vvis::image<scalarT> >(),
		dest, vvis::pixel_accessor<vvis::image<scalarT> >(),
		vvis::plus<>());
	cout << "> img1 + img2 = " << dest << endl;
	
	vvis::transform(img1, vvis::pixel_accessor<vvis::image<scalarT> >(),
		dest, vvis::pixel_accessor<vvis::image<scalarT> >(),
		vvis::bind2nd(vvis::less<scalarT>(), 50));
	cout << "> img1 < 50 = " << dest << endl;
	
	// RGB images
	vvis::rgb_image<scalarT> rgb1(width, height), rgb2(width, height), rgbd(width, height);
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
	vvis::transform(rgb1, vvis::pixel_accessor<vvis::rgb_image<scalarT> >(),
		rgb2, vvis::pixel_accessor<vvis::rgb_image<scalarT> >(),
		rgbd, vvis::pixel_accessor<vvis::rgb_image<scalarT> >(),
		vvis::plus<>());
	cout << "> rgb1 + rgb2 = " << rgbd << endl;
	vvis::transform(rgb1, vvis::pixel_accessor<vvis::rgb_image<scalarT> >(),
		rgbd, vvis::pixel_accessor<vvis::rgb_image<scalarT> >(),
		vvis::bind2nd(vvis::less<ct::tuple<CT_TYPELIST3(scalarT, scalarT, scalarT)> >(), 50));
	cout << "> rgb1 < 50 = " << rgbd << endl;
}