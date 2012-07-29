/*
 *  image.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 7/12/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#include <iostream>
#include <labvvis/image.h>

namespace lv {
	image::image() : _channel_count(0) {
	}
	
	image::image(const int width, const int height, const int channel_count) 
	: _channel_count(channel_count), _channels(new one_channel*[_channel_count]) {
		// Allocate
		for(int i = 0; i < _channel_count; ++i) {
			_channels[i] = new one_channel(width, height);
		}
	}

	const int image::channel_count() {
		return _channel_count;
	}
	
	one_channel &image::operator[](const int idx) {
		return *(_channels[idx]);
	}		
	
	const one_channel &image::operator[](const int idx) const {
		return *(_channels[idx]);
	}

	const int image::height() const {
		return _channels[0]->height();
	}
	
	const int image::width() const {
		return _channels[0]->width();
	}

	image &image::operator=(const image &rhs) {
		if(this != &rhs) {
			_channels = rhs._channels;
			_channel_count = rhs._channel_count;
		}
		return *this;
	}
	
	void image::delete_image() {
		if(_channel_count) {
			for(int i = 0; i < _channel_count; ++i) {
				delete _channels[i];
			}
			_channel_count = 0;
		}
	}
	
}