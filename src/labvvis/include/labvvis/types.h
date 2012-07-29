/*
 *  types.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 6/05/05.
 *  Copyright 2005 University of Wollongong. All rights reserved.
 *
 */

#ifndef LABVVIS_TYPES_H
#define LABVVIS_TYPES_H

#include <vvis/vvis.h>
#include "extcode.h"

//= LabVIEW ====================================================================
typedef struct {
	int32 dimSizes[2];
	uInt32 elt[1];
} TD1;
typedef TD1 **TD1Hdl;

//= labvvis ====================================================================
typedef short int image_id;

namespace lv {
	enum image_type {
		grey = '0',
		rgba = '4', rgba_r = 'r', rgba_g = 'g', rgba_b = 'b', rgba_a = 'a'
	};
	
	typedef vvis::image<vvis::uint8> grey_image_t;
	typedef vvis::rgba_image<vvis::uint8> rgba_image_t;
	
	//= image ==================================================================
	class image {
	public:
		image(void *image_ptr, const image_type type)
		: _image_ptr(image_ptr), _type(type), allocated(false) {
			switch(type) {
				case grey: {
					typedef vvis::image<vvis::uint8> image_t;
					image_t &img = *((image_t *)_image_ptr);
					_width = img.width();
					_height = img.height();
				}	break;
				case rgba:
				case rgba_r:
				case rgba_g:
				case rgba_b:
				case rgba_a: {
					typedef vvis::rgba_image<vvis::uint8> image_t;
					image_t &img = *((image_t *)_image_ptr);
					_width = img.width();
					_height = img.height();					
				}	break;
			}
		}
		
		image(const image_type type, const int width, const int height)
		: _type(type), _width(width), _height(height), allocated(true) {
			switch(type) {
				case grey: _image_ptr = new vvis::image<vvis::uint8>(width, height); break;
				case rgba:
				case rgba_r:
				case rgba_g:
				case rgba_b:
				case rgba_a:	
					_image_ptr = new vvis::rgba_image<vvis::uint8>(width, height); break;
			}
		}
	public:
		void *image_ptr() {
			return _image_ptr;
		}
		const image_type type() const {
			return _type;
		}
		const bool is_image_allocated() const {
			return allocated;
		}
		const int height() const {
			return _height;
		}
		const int width() const {
			return _width;
		}
	public:
		void delete_image() {
			if(allocated) {
				switch(_type) {
					case grey: delete (vvis::image<vvis::uint8>*)_image_ptr; break;
					case rgba:
					case rgba_r:
					case rgba_g:
					case rgba_b:
					case rgba_a:
						delete (vvis::rgba_image<vvis::uint8>*)_image_ptr; break;
				}
				_image_ptr = 0;
				allocated = false;
			}
		}
	private:
		void *_image_ptr;
		image_type _type;
		int _width, _height;
		bool allocated;
	};
	
} // End of namespace lv;

#endif
