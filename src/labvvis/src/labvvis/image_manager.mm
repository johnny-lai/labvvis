/*
 *  util.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 14/05/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#import <labvvis/image_manager.h>

namespace lv {
	//= Image Manager ==========================================================
	image_manager image_manager::_instance;
	
	image_manager &image_manager::instance() {
		return _instance;
	}
	
	image_manager::image_manager() : _next_id(0) {
	}
	
	image_manager::~image_manager() {
		for(images_t::iterator i = _images.begin(); i != _images.end(); ++i) {
            delete i->second;
		}
		_images.clear();
	}
	
	image_id image_manager::new_image(const int width, const int height, const int channel_count) {
        ++_next_id;
        _images[_next_id] = new lv::image(width, height, channel_count);
        return _next_id;
	}
	
	void image_manager::delete_image(const image_id imgid) {
		delete _images[imgid];
		_images.erase(_images.find(imgid));
	}
	
	lv::image &image_manager::image(const image_id imgid) {
		return *_images[imgid];
	}
	
	int image_manager::allocated_count() const {
		return _images.size();
	}
	
	bool image_manager::image_exists(const image_id id) const {
		return _images.find(id) != _images.end();
	}
}
