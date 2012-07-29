/*
 *  external_image.h
 *  vvis
 *
 *  Created by Lai Bing-Chang on 2007/4/29.
 *  Copyright 2007 Bing-Chang Lai. All rights reserved.
 *
 */

#ifndef _VVIS_EXTERNAL_IMAGE_H
#define _VVIS_EXTERNAL_IMAGE_H

#include <ct/boost_type_traits.h>
#include <ct/typelist.h>
#include <vvis/image_base.h>
#include <vvis/storage.h>	// For pixel_impl

namespace vvis {
	//- external_storage -------------------------------------------------------
	// Chunky Scalar External Storage
	// Unknown storage
	template<
		typename T0 = boost::tuples::null_type,
		typename T1 = boost::tuples::null_type,
		typename T2 = boost::tuples::null_type,
		typename T3 = boost::tuples::null_type,
		typename T4 = boost::tuples::null_type,
		typename T5 = boost::tuples::null_type,
		typename T6 = boost::tuples::null_type,
		typename T7 = boost::tuples::null_type,
		typename T8 = boost::tuples::null_type,
		typename T9 = boost::tuples::null_type
	> class chunkys_extstorage : public image_base, public unknown_storage {
	public:
		typedef boost::tuple<T0, T1, T2, T3, T4,
		                     T5, T6, T7, T8, T9> pixel_type;
	public:
		/// Default Constructor
		chunkys_extstorage() : _base_address(0), _pixel_bytes(0), _pixel_count(0) {
		}
		///
		chunkys_extstorage(void *addr, unsigned char pixel_bytes,
		const coordinate_type width)
		: _base_address(reinterpret_cast<unsigned char *>(addr)), _pixel_bytes(pixel_bytes), _pixel_count(width) {
		}
		/// Destructor
		virtual ~chunkys_extstorage() {
		}
	public:
		void admit(void *addr, unsigned char pixel_bytes, const coordinate_type pixel_count) {
			_base_address = reinterpret_cast<unsigned char *>(addr);
			_pixel_bytes = pixel_bytes;
			_pixel_count = pixel_count;
		}
		void release() {
			_base_address = 0;
			_pixel_bytes = 0;
			_pixel_count = 0;
		}
	//= Storage Interface ======================================================
	// external_image is a Contiguous Storage
	public:
		typedef int iterator;
	public:
		/** Returns an iterator representing the start of the data
		 */
		iterator begin() {
			return 0;
		}
		/** Returns a non-const pointer to the end of the data
		 * The pointer points to the first position after the end of the data
		 * This function is implemented only by contiguous_storage
		 */
		iterator end() {
			return _pixel_count;
		}
	//= Storage-Accessor Interface =============================================
	public:
        typedef pixel_type value_type;
		pixel_type &operator[](const iterator i) {
			return *(reinterpret_cast<pixel_type *>(_base_address + (_pixel_bytes * i)));
		}
		const pixel_type operator[](const iterator i) const {
			return *(reinterpret_cast<pixel_type *>(_base_address + (_pixel_bytes * i)));
		}
	protected:
		unsigned char *_base_address;
		unsigned char _pixel_bytes;
		coordinate_type _pixel_count;
	};

} // End of vvis namespace
#endif