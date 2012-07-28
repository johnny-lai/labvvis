/*
 *  stub.cpp
 *  labvvis
 *
 *  Created by Bing-Chang Lai on 6/05/05.
 *  Copyright 2005 University of Wollongong. All rights reserved.
 *
 *  Stub for compiling operations. The following #defines are used:
 *   + TAG
 */

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <labvvis/stub.h>

#define PIXEL_TYPE BOOST_PP_CAT(pixel_type_, TAG)

#define IMAGE BOOST_PP_CAT(image_, TAG)
#define ACCESSOR BOOST_PP_CAT(accessor_, TAG)

#define F(x) BOOST_PP_CAT(x, TAG)

#define TAG_NS BOOST_PP_CAT(ns_, TAG)

// Histogram should only be defined for types that have one channel
#if BOOST_PP_EQUAL(CHANNEL_COUNT, 1)
#	define HAS_HISTOGRAM
#endif

extern "C" {
	void F(fill)(void * const out, const vvis::uint8 value);
	void F(fill_rect)(void * const out, const vvis::uint8 value, const int ix, const int iy, const int width, const int height);
#ifdef HAS_HISTOGRAM
	void F(histogram)(void * const in, int *counts);
#endif
}

void F(fill)(void * const out, const vvis::uint8 value) {
	IMAGE &o = *((IMAGE*)out);
	vvis::transform(o, ACCESSOR(), o, ACCESSOR(),
					vvis::constant<PIXEL_TYPE>(value));
}

void F(fill_rect)(void * const out, const vvis::uint8 value, const int ix, const int iy, const int width, const int height) {
	int ex = ix + width;
	int ey = iy + height;
	IMAGE &o = *((IMAGE*)out);
	if(ex >= o.width())
		ex = o.width();
	if(ey >= o.height())
		ey = o.height();
	for(int y = iy; y < height; ++y) {
		for(int x = ix; x < width; ++x) {
			o.pixel(x, y) = value;
		}
	}
}

#ifdef HAS_HISTOGRAM
namespace TAG_NS {
	struct histogram {
		typedef typename vvm::add_vector<PIXEL_TYPE>::type vector_type;
		histogram(int *counts)
		: _counts(counts) {
			for(int i = 0; i < 256; ++i) {
				_counts[i] = 0;
			}
		}
		void operator()(const PIXEL_TYPE p) {
			++_counts[p];
		}
		void operator()(const vector_type &v) {
			for(int i = 0; i < vvm::vector_traits<vector_type>::scalar_count; ++i)
				++_counts[v.scalar(i)];
		}
	private:
		int *_counts;
	};
} // End of TAG_NS namespace
/*
namespace vvis {
	template<> struct is_vectorised_functor<TAG_NS::histogram> {
		static const bool value = false;
	};
} // End of vvis namespace
*/

void F(histogram)(void * const in, int *counts) {
	IMAGE &i = *((IMAGE*)in);
	TAG_NS::histogram h(counts);
	vvis::for_each(i, ACCESSOR(), h);
}
#endif
