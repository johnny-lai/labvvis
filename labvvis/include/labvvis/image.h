/*
 *  image.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 6/05/05.
 *  Copyright 2005. All rights reserved.
 *
 */

#ifndef LABVVIS_IMAGE_H
#define LABVVIS_IMAGE_H

#include <vvis/vvis.h>
#include <boost/shared_array.hpp>
#include <Accelerate/Accelerate.h>
#include "extcode.h"

//= LabVIEW ====================================================================
typedef struct {
	int32 dimSizes[2];
	uInt32 elt[1];
} TD1;
typedef TD1 **TD1Hdl;

//= labvvis ====================================================================
typedef short int image_id;
typedef short int channel_id;

namespace lv {
	typedef vvis::image<vvis::uint8> one_channel;
	typedef vvis::pixel_accessor<one_channel> pixel_accessor_1ch;
	typedef one_channel::pixel_type pixel_type_1ch;
	
	//= image ==================================================================
	class image {
	public:
		image();
		image(const int width, const int height, const int channel_count);
	public:
		const int channel_count();
		one_channel &operator[](const int idx);
		const one_channel &operator[](const int idx) const;
		const int height() const;
		const int width() const;
		image &operator=(const image &rhs);
		void delete_image();
	private:
		typedef boost::shared_array<one_channel*> many_channels;
		int _channel_count;
		many_channels _channels;
	};
	
} // End of namespace lv;

#endif
