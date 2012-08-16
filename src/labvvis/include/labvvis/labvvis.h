/*
 *  labvvis.h
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 16/03/05.
 *  Copyright 2005. All rights reserved.
 *
 */
#ifndef _LABVVIS_LABVVIS_H
#define _LABVVIS_LABVVIS_H

#include <map>
#include <labvvis/image.h>

typedef short hough_id_t;

enum unary_functions {
	bitwise_not, copy
};

enum binary_functions {
	plus_saturated, minus_saturated,
	multiplies, divides,
	modulus,
	equal_to, not_equal_to,
	greater, greater_equal,
	less, less_equal,
	logical_and, logical_or,
	bitwise_and, bitwise_or
};

extern "C" {
	int32 get_address(const image_id idnt);

	//= Image Allocation/Deallocation ==============================================
	image_id new_image(const int width, const int height, const int channel_count, LStrHandle error);
	void delete_image(const image_id idnt, LStrHandle error);
	int allocated_count();
	//image_id new_subimage_channel(const image_id oid, const channel_id ocid, const channnel_id ncid, LStrHandle error);

	//= Image Conversion ===========================================================
	void convert_from_pixmap(const unsigned long *pixmap, const image_id idnt, const channel_id cid, LStrHandle error);
	void convert_to_pixmap(const image_id idnt, const channel_id cid, unsigned long *pixmap, LStrHandle error);
	
	//= Image Initialisation =======================================================
	void fill(const image_id o, const channel_id ocid, const UInt8 c, LStrHandle error);
	void fill_rect(const image_id o, const channel_id ocid, const UInt8 c,
				   const int ix, const int iy, const int width, const int height,
				   LStrHandle error);
	//void copy(const image_id in, const channel_id icid, const image_id out, const channel_id ocid, LStrHandle error);
	
	//= Image Information ==========================================================
	void image_size(const image_id o, int *width, int *height, LStrHandle error);
	void get_pixel(const image_id iid, const channel_id cid, const int x, const int y, UInt8 *value, LStrHandle error);
	void get_pixel_rgba(const image_id iid, const channel_id cid, const int x, const int y,
						UInt8 *r, UInt8 *g, UInt8 *b, UInt8 *a,
						LStrHandle error);
	void set_pixel(const image_id imgid, const channel_id cid, const int x, const int y, const UInt8 value, LStrHandle error);
	void set_pixel_rgba(const image_id imgid, const channel_id cid, const int x, const int y,
						const UInt8 r, const UInt8 g, const UInt8 b, const UInt8 a,
						LStrHandle error);

	void count_pixels(const image_id iid, const channel_id cid, const UInt8 pixel, unsigned int *count, LStrHandle error);
	
	//= Image Operations ===========================================================
	void unary_operation(const unary_functions f, const image_id i1, const channel_id c1,
						 const image_id o, const channel_id co, LStrHandle error);
	
	void binary_operation(const binary_functions f, const image_id i1, const channel_id c1,
						  const image_id i2, const channel_id c2,
						  const image_id o, const channel_id co, LStrHandle error);
	
	void equalize(const image_id i1, const channel_id icid,
				  const image_id o, const channel_id ocid,
				  const UInt8 min, const UInt8 max, LStrHandle error);
		
	void threshold(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid, 
				   const int value, LStrHandle error);
	
	void linear_filter(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid,
					   SInt8 mask[], const int kw, const int kh, LStrHandle error);
	void max_filter(const image_id i1, const channel_id icid, const image_id o, const channel_id ocid,
					SInt8 mask[], const int kw, const int kh, LStrHandle error);

	void histogram(const image_id i1, const channel_id icid, int counts[], const int counts_size, LStrHandle error);
	
	void grow_blob(const image_id i1, const channel_id icid, 
				   const image_id o, const channel_id ocid,
				   const int blob_start, const int blob_step, LStrHandle error);
	
	void hough_transform(const image_id i1, const channel_id icid, hough_id_t *hough_id, LStrHandle error);
	void delete_hough(const int hough_id, LStrHandle error);
	
	
	typedef struct {
		double r;
		short q;
		short count;
		} hough_line;

	typedef struct {
		long dimSize;
		hough_line ALineFromHoughSTransform[1];
		} hough_lines;
	typedef hough_lines **hough_lines_handle;

	void get_hough_lines(const hough_id_t hough_id, const unsigned short threshold, hough_lines_handle hlines_hdl, 
		unsigned short *lines_count, LStrHandle error);
    
    //= Sequence Grabber ===========================================================
    void start_sequence_grabber(const int width, const int height, LStrHandle error);
    void grab_sequence_grabber(const image_id o, const channel_id ocid, LStrHandle error);
    void stop_sequence_grabber(LStrHandle error);
}

#endif

