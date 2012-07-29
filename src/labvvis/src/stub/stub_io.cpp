/*
 *  stub.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 6/05/05.
 *  Copyright 2005 University of Wollongong. All rights reserved.
 *
 *  Stub for compiling operations. The following #defines are used:
 *   + TAG_I
 *   + TAG_O
 */

#include <map>
#include <boost/preprocessor/cat.hpp>
#include <labvvis/stub.h>

#define PIXEL_TYPE BOOST_PP_CAT(pixel_type_, TAG_I)

#define IMAGE_I BOOST_PP_CAT(image_, TAG_I)
#define ACCESSOR_I BOOST_PP_CAT(accessor_, TAG_I)

#define IMAGE_O BOOST_PP_CAT(image_, TAG_O)
#define ACCESSOR_O BOOST_PP_CAT(accessor_, TAG_O)

#define TAG BOOST_PP_CAT(TAG_I, TAG_O)
#define F(x) BOOST_PP_CAT(x, TAG)

extern "C" {
	void F(copy)(void * const in, void * const out);
	void F(threshold)(void * const in, void * const out, const vvis::uint8 value);
	void F(equalize)(void * const in, void * const out, const vvis::uint8 min, const vvis::uint8 max);
	void F(linear_filter)(void * const in, void * const out, vvis::sint8 mask[], const int kw, const int kh);
	void F(max_filter)(void * const in, void * const out, vvis::sint8 mask[], const int kw, const int kh);
#if defined HAS_GROW_BLOB
	std::map<PIXEL_TYPE, vvis::rect> F(grow_blob)(void * const in, void * const out, const int blob_start, const int blob_step);
#endif
}

void F(copy)(void * const in, void * const out) {
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	vvis::transform(a, ACCESSOR_I(), o, ACCESSOR_O(), vvis::noop<>());
}

void F(equalize)(void * const in, void * const out, const vvis::uint8 min, const vvis::uint8 max) {
	std::clog << "Equalize " << min << " " << max << std::endl;
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	vvis::transform(a, ACCESSOR_I(), o, ACCESSOR_O(), vvis::equalize<PIXEL_TYPE>(min, max));
	std::clog << "Done Equalize " << std::endl;
}

void F(threshold)(void * const in, void * const out, const vvis::uint8 value) {
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	vvis::transform(a, ACCESSOR_I(), o, ACCESSOR_O(),
					vvis::bind2nd(vvis::greater<PIXEL_TYPE>(), value));
}

void F(linear_filter)(void * const in, void * const out, vvis::sint8 mask[], const int kw, const int kh) {
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	vvis::kernel2d<vvis::sint8> kernel(mask, kw, kh);
	vvis::convolute(a, ACCESSOR_I(),
					o, ACCESSOR_O(),
					vvis::linear_filter<vvis::sint8, PIXEL_TYPE>(kernel));
}

void F(max_filter)(void * const in, void * const out, vvis::sint8 mask[], const int kw, const int kh) {
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	vvis::kernel2d<vvis::sint8> kernel(mask, kw, kh);
	vvis::convolute(a, ACCESSOR_I(),
					o, ACCESSOR_O(),
					vvis::max_filter<vvis::sint8, PIXEL_TYPE>(kernel));
}

#if defined HAS_GROW_BLOB
namespace priv {
	PIXEL_TYPE F(pixel)(IMAGE_I &a, const int x, const int y) {
		if(x < 0 || y < 0 || x >= a.width() || y >= a.height()) {
			return 0;
		} else {
			return a.pixel(x, y) > 0 ? 1 : 0;
		}
	}
	int F(window_state)(IMAGE_I &a, const int x, const int y) {
		return F(pixel)(a, x, y) +
			2*F(pixel)(a, x, y-1) +
			4*F(pixel)(a, x-1, y-1) +
			8*F(pixel)(a, x-1, y);
	}
}

std::map<PIXEL_TYPE, vvis::rect> F(grow_blob)
(void * const in, void * const out, const int blob_start, const int blob_step) {
	std::map<PIXEL_TYPE, vvis::rect> blobs;	// Blob number : First row
	IMAGE_I &a = *((IMAGE_I*)in);
	IMAGE_O &o = *((IMAGE_O*)out);
	PIXEL_TYPE current_blob = blob_start - blob_step;
	// Fill out with 0
	vvis::transform(o, ACCESSOR_O(), o, ACCESSOR_O(), vvis::constant<PIXEL_TYPE>(0));
	for(int y = 0; y < a.height(); ++y) {
		for(int x = 0; x < a.width(); ++x) {
			int window_state = F(::priv::window_state)(a, x, y);
			switch(window_state) {
				case 0:
				case 2:
				case 6:
				case 8:
				case 10:
				case 12:
					// 1 Connect to background blob
					break;
				case 1:
					// 2 Start new object blob
					current_blob += blob_step;
					o.pixel(x, y) = current_blob;
					blobs[current_blob] = vvis::rect(x, y, 1, 1);
					break;
				case 3:
				case 7:
				case 15: {
					// 3 Connect to object blob (all cases assumed to have previous on top)
					o.pixel(x, y) = o.pixel(x, y-1);
					// Adjust rect
					vvis::rect &r = blobs[o.pixel(x, y)];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;
				case 9:
				case 13: {
					// 3 Connect to object blob (all cases assumed to have previous in front)
					o.pixel(x, y) = o.pixel(x-1, y);
					// Adjust rect
					vvis::rect &r = blobs[o.pixel(x, y)];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;					
				case 4: {
					// 4 End of object blob, connect to background blob and merge background blobs
					vvis::rect &r = blobs[o.pixel(x-1, y-1)];
					r.height = y - r.top + 1;
				}	break;
				case 5: {
					// 5 Start new object blob and end old object blob
					vvis::rect &r = blobs[o.pixel(x-1, y-1)];
					r.height = y - r.top + 1;
					
					current_blob += blob_step;
					o.pixel(x, y) = current_blob;
					blobs[current_blob] = vvis::rect(x, y, 1, 1);
				}	break;
				case 11: {
					// 6 End of background blob, connect to object blob and merge object blobs
					PIXEL_TYPE this_blob = o.pixel(x-1, y);
					// y must be >0 since there is a 1 above
					PIXEL_TYPE to_replace = o.pixel(x, y-1);
					vvis::rect &old_r = blobs[to_replace];
					blobs[this_blob] = old_r;
					for(int y_ = old_r.top; y_ <= y; ++y_) {
						for(int x_ = old_r.left; x_ < (old_r.left + old_r.width); ++x_) {
							if(o.pixel(x_, y_) == to_replace) {
								o.pixel(x_, y_) = this_blob;
							}
						}
					}
					blobs.erase(to_replace);
					o.pixel(x, y) = this_blob;
					// Adjust rect
					vvis::rect &r = blobs[this_blob];
					if(x < r.left)
						r.left = x;
					int w = x - r.left + 1;
					if(w > r.width)
						r.width = w;
				}	break;
				case 14:
					// 7 Start new background blob
					break;
			}
		}
	}
	return blobs;
}
#endif

//==============================================================================
#define DEFINE_UNARY_FUNCTION(FNAME) \
	extern "C" { \
		void F(FNAME)(void * const in, void * const out); \
	} \
	void F(FNAME)(void * const in, void * const out) { \
		IMAGE_I &i = *((IMAGE_I*)in); \
		IMAGE_O &o = *((IMAGE_O*)out); \
		transform(i, ACCESSOR_I(), o, ACCESSOR_O(), \
				  FNAME<>()); \
	}

DEFINE_UNARY_FUNCTION(negate)
DEFINE_UNARY_FUNCTION(logical_not)
DEFINE_UNARY_FUNCTION(bitwise_not)
//DEFINE_UNARY_FUNCTION(logarithmic)
//DEFINE_UNARY_FUNCTION(exponential)
