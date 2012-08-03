/*
 *  util.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 14/05/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#ifndef LABVVIS_IMAGE_MANAGER_H
#define LABVVIS_IMAGE_MANAGER_H

#include <string>
#include <map>
#include <labvvis/image.h>
#include "extcode.h"


namespace lv {
	//= Image Manager ==========================================================
	typedef std::map<image_id, lv::image *> images_t;
	
	class image_manager {
	public:
		static image_manager &instance();
		
		image_id new_image(const int width, const int height, const int channel_count);
		void delete_image(const image_id imgid);
		lv::image &image(const image_id imgid);
		int allocated_count() const;
	
		bool image_exists(const image_id imgid) const;
	private:
		image_manager();
		~image_manager();
		
		static image_manager _instance;
		images_t _images;
		int _next_id;
	};
} // End of lv namespace

//= Globals ====================================================================

#endif