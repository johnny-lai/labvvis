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

template<typename scalarT> void output(vvis::image<scalarT>& image) {
	typedef typename vvis::image<scalarT>::iterator_type y_iterator;
	typedef typename vvis::image<scalarT>::iterator_type::value_type x_iterator;
	upper_left = image.begin();
	lower_right = image.end();
	for(y_iterator iy = image.begin(); iy != image.end(); ++iy) {
		for(x_iterator ix = (*iy).begin(); ix != (*iy).end(); ++ix) {
			cout << ix.component(0) << " ";
		}
		cout << endl;
	}
	vvis::image<scalarT>::iterator_type iterator;
	for(iterator iy = image.begin(); i
}

template<typename scalarT> void test() {
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	vvis::image<scalarT> img(10, 5);
	for(int y = 0, i = 0; y < img.height(); ++y, ++i) {
		for(int x = 0; x < img.width(); ++x, ++i) {
			img.pixel(x, y) = i;
		}
	}
	cout << ">  img = " << endl;
	output(img);
}