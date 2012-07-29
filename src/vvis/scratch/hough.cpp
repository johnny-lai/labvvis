/*
 *  hough.cpp
 *  vvis
 *
 *  Created by Lai Bing-Chang on 2006/6/21.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <vvis/vvis.h>

#define PI 3.14159265



int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << argv[0] << " <input> <output>" << std::endl;
		return 1;
	}
	
	vvis::fimporter sgi(argv[1]);
	if(!sgi.is_open()) {
		std::cerr << "Unable to open " << argv[1] << std::endl;
		return 1;
	}
	
	typedef vvis::image<vvis::uint8> image_t;
	image_t img;
	sgi >> img;
	
	std::cout << vvis::uses_contiguous_storage<image_t::value_type>::value << "\n";
	std::cout << img.width() << " " <<
		img.height() << " " << std::endl;
	std::cout << "Starting Hough Transform" << std::endl;
	
	// Create a hough tranform object for image img
	typedef vvis::hough<vvis::xy_accessor<vvis::pixel_accessor<image_t> > > hough_t;
	hough_t hf;
	vvis::xy_accessor<vvis::pixel_accessor<image_t> > h;
	vvis::for_each(img, h, hf);

	std::cout << "Results:" << std::endl;
	std::vector<hough_t::value_type> lines = hf.result(5);
	for(std::vector<hough_t::value_type>::const_iterator i = lines.begin();
		i != lines.end(); ++i) {
		std::cout << i->r << ":" << i->q << "=" << i->count << " ";
	}
	return 0;
}