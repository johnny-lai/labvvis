/*
 *  image.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Fri Mar 28 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_IMAGE_H
#define _VVIS_IMAGE_H

#include <ct/metafunction.h>
#include <vvm/vvm.h>
#include <vvis/vvm_tuple_support.h>
#include <vvis/image_base.h>
#include <vvis/storage.h>

namespace vvis {
	//- base_image -----------------------------------------------------------------------
	/** \class base_image "image.h" <vvis/image.h>
	 * Base image implementation
	 * \param typelistT Typelist containing component types
	 * \param storageP Storage Policy. Leave blank for default.
	 */
	template<
		typename typelistT,
		template<typename storagePTypelistT> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class base_image : public image_base, public storageP<typelistT> {
	public:
		typedef base_image<typelistT, storageP> self_type;
		typedef typelistT component_tl;
		typedef typename priv::pixel_impl<typelistT>::pixel_type pixel_type;
		typedef typename ct::apply<component_tl, boost::add_const>::type component_const_tl;
		typedef typename ct::apply<component_tl, boost::add_reference>::type component_ref_tl;
		typedef typename ct::apply<component_const_tl, boost::add_reference>::type component_const_ref_tl;
	public:
		/// Default Constructor
		base_image() : storageP<typelistT>() {
		}
		/// Copy Constructor
		base_image(const base_image& rhs) : storageP<typelistT>(rhs) {
		}
		/// Conversion Constructor
		/*
		template<template<typename other_typelistT> class other_storageP>
		base_image(const base_image<typelistT, other_storageP>& rhs)
		: storageP<typelistT>(static_cast<other_storageP<typelistT> >(rhs)) {
		}
		*/
		/// Constructs an image of the given width and height
		base_image(const coordinate_type width, const coordinate_type height)
		: storageP<typelistT>(width, height) {
		}
		/// Destructor
		virtual ~base_image() {
		}
	public:
		//TODO: Implement operator=
	public:
		template<int channel> typename ct::type_at<component_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) {
			typedef storageP<typelistT> storage_t;
            storage_t& storage = *this;
			return storage.template component<channel>(x, y);
		}
		template<int channel> typename ct::type_at<component_const_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) const {
			typedef storageP<typelistT> storage_t;
            const storage_t& storage = *this;
			return storage.template component<channel>(x, y);
		}
	};
	
	#define VVIS_DEFINE_INHERITED(CLASS) \
		typedef typename parent_type::coordinate_type coordinate_type; \
		typedef typename parent_type::component_tl component_tl; \
		typedef typename parent_type::component_const_tl component_const_tl; \
		typedef typename parent_type::component_ref_tl component_ref_tl; \
		typedef typename parent_type::component_const_ref_tl component_const_ref_tl; \
		CLASS() : parent_type() { \
		} \
		CLASS(const CLASS& rhs) : parent_type(rhs) { \
		} \
		CLASS(const coordinate_type width, const coordinate_type height) \
		: parent_type(width, height) { \
		} \
		virtual ~CLASS() { \
		} \
		template<int channel> typename ct::type_at<component_ref_tl, channel>::type \
		component(const coordinate_type x, const coordinate_type y) { \
			return static_cast<parent_type&>(*this).template component<channel>(x, y); \
		} \
		template<int channel> typename ct::type_at<component_const_ref_tl, channel>::type \
		component(const coordinate_type x, const coordinate_type y) const { \
			return static_cast<parent_type&>(*this).template component<channel>(x, y); \
		}

	//- image ----------------------------------------------------------------------------	
	/** \class image "image.h" <vvis/image.h>
	 * Single-channel image
	 * \param componentT Component Type
	 * \param storageP Storage Policy
	 */
	template<
		typename componentT,
		template<typename typelistT> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class image
	: public base_image<typename ct::cons_tl<componentT>::type, storageP> {
	private:
		typedef base_image<
			typename ct::cons_tl<componentT>::type,
			storageP> parent_type;
	public:
		VVIS_DEFINE_INHERITED(image);
	};
	
	/** \class rgb_image "image.h" <vvis/image.h>
	 * Represents a RGB image.
	 * typename componentT Component Type
	 * typename storageP Storage Policy
	 */
	template<
		typename componentT,
		template<typename typelistT> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class rgb_image
	: public base_image<typename ct::cons_tl_repeat<componentT, 3>::type, storageP> {
	private:
		typedef base_image<
			typename ct::cons_tl_repeat<componentT, 3>::type,
			storageP> parent_type;
	public:
		//VVIS_DEFINE_INHERITED(rgb_image);
		typedef typename parent_type::coordinate_type coordinate_type;
		typedef typename parent_type::component_tl component_tl;
		typedef typename parent_type::component_const_tl component_const_tl;
		typedef typename parent_type::component_ref_tl component_ref_tl;
		typedef typename parent_type::component_const_ref_tl component_const_ref_tl;
	public:
		rgb_image() : parent_type() {
		}
		rgb_image(const rgb_image& rhs) : parent_type(rhs) {
		}
		rgb_image(const coordinate_type width, const coordinate_type height)
		: parent_type(width, height) {
		}
		virtual ~rgb_image() {
		}
		template<int channel> typename ct::type_at<component_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) {
			return static_cast<parent_type&>(*this).template component<channel>(x, y);
		}
		template<int channel> typename ct::type_at<component_const_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) const {
			return static_cast<const parent_type&>(*this).template component<channel>(x, y);
		}
	public:
		/// Returns writable access to the red component
		typename ct::type_at<component_ref_tl, 0>::type
		red(const coordinate_type x, const coordinate_type y) {
			return component<0>(x, y);
		}
		/// Returns read-only access to the red component
		typename ct::type_at<component_const_ref_tl, 0>::type
		red(const coordinate_type x, const coordinate_type y) const {
			return component<0>(x, y);
		}
		/// Returns writable access to the green component
		typename ct::type_at<component_ref_tl, 1>::type
		green(const coordinate_type x, const coordinate_type y) {
			return component<1>(x, y);
		}
		/// Returns read-only access to the green component
		typename ct::type_at<component_const_ref_tl, 1>::type
		green(const coordinate_type x, const coordinate_type y) const {
			return component<1>(x, y);
		}
		/// Returns writable access to the blue component
		typename ct::type_at<component_ref_tl, 2>::type
		blue(const coordinate_type x, const coordinate_type y) {
			return component<2>(x, y);
		}
		/// Returns read-only access to the blue component
		typename ct::type_at<component_const_ref_tl, 2>::type \
		blue(const coordinate_type x, const coordinate_type y) const {
			return component<2>(x, y);
		}
	};

	/** \class rgba_image "image.h" <vvis/image.h>
		* Represents a RGBA image.
		* typename componentT Component Type
		* typename storageP Storage Policy
		*/
	template<
		typename componentT,
		template<typename typelistT> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class rgba_image
	: public base_image<typename ct::cons_tl_repeat<componentT, 4>::type, storageP> {
	private:
		typedef base_image<
			typename ct::cons_tl_repeat<componentT, 4>::type,
			storageP> parent_type;
	public:
		//VVIS_DEFINE_INHERITED(rgb_image);
		typedef typename parent_type::coordinate_type coordinate_type;
		typedef typename parent_type::component_tl component_tl;
		typedef typename parent_type::component_const_tl component_const_tl;
		typedef typename parent_type::component_ref_tl component_ref_tl;
		typedef typename parent_type::component_const_ref_tl component_const_ref_tl;
	public:
		rgba_image() : parent_type() {
		}
		rgba_image(const rgba_image& rhs) : parent_type(rhs) {
		}
		rgba_image(const coordinate_type width, const coordinate_type height)
		: parent_type(width, height) {
		}
		virtual ~rgba_image() {
		}
		template<int channel> typename ct::type_at<component_ref_tl, channel>::type
			component(const coordinate_type x, const coordinate_type y) {
				return static_cast<parent_type&>(*this).template component<channel>(x, y);
			}
		template<int channel> typename ct::type_at<component_const_ref_tl, channel>::type
			component(const coordinate_type x, const coordinate_type y) const {
				return static_cast<const parent_type&>(*this).template component<channel>(x, y);
			}
	public:
		/// Returns writable access to the red component
		typename ct::type_at<component_ref_tl, 0>::type
		red(const coordinate_type x, const coordinate_type y) {
			return component<0>(x, y);
		}
		/// Returns read-only access to the red component
		typename ct::type_at<component_const_ref_tl, 0>::type
			red(const coordinate_type x, const coordinate_type y) const {
				return component<0>(x, y);
			}
		/// Returns writable access to the green component
		typename ct::type_at<component_ref_tl, 1>::type
			green(const coordinate_type x, const coordinate_type y) {
				return component<1>(x, y);
			}
		/// Returns read-only access to the green component
		typename ct::type_at<component_const_ref_tl, 1>::type
			green(const coordinate_type x, const coordinate_type y) const {
				return component<1>(x, y);
			}
		/// Returns writable access to the blue component
		typename ct::type_at<component_ref_tl, 2>::type
			blue(const coordinate_type x, const coordinate_type y) {
				return component<2>(x, y);
			}
		/// Returns read-only access to the blue component
		typename ct::type_at<component_const_ref_tl, 2>::type \
			blue(const coordinate_type x, const coordinate_type y) const {
				return component<2>(x, y);
			}
		/// Returns writable access to the alpha component
		typename ct::type_at<component_ref_tl, 3>::type
			alpha(const coordinate_type x, const coordinate_type y) {
				return component<3>(x, y);
			}
		/// Returns read-only access to the alpha component
		typename ct::type_at<component_const_ref_tl, 3>::type \
			alpha(const coordinate_type x, const coordinate_type y) const {
				return component<3>(x, y);
			}
	};
	
	/** cmyk_image
	 * componentT: Component Type
	 * storageP: Storage Policy
	 */
	template<
		typename componentT,
		template<typename typelistT> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class cmyk_image
	: public base_image<typename ct::cons_tl_repeat<componentT, 4>::type, storageP> {
	private:
		typedef base_image<
			typename ct::cons_tl_repeat<componentT, 4>::type,
			storageP> parent_type;
	public:
		//VVIS_DEFINE_INHERITED(cmyk_image);
		typedef typename parent_type::coordinate_type coordinate_type;
		typedef typename parent_type::component_tl component_tl;
		typedef typename parent_type::component_const_tl component_const_tl;
		typedef typename parent_type::component_ref_tl component_ref_tl;
		typedef typename parent_type::component_const_ref_tl component_const_ref_tl;
	public:
		cmyk_image() : parent_type() {
		}
		cmyk_image(const cmyk_image& rhs) : parent_type(rhs) {
		}
		cmyk_image(const coordinate_type width, const coordinate_type height)
		: parent_type(width, height) {
		}
		virtual ~cmyk_image() {
		}
		template<int channel> typename ct::type_at<component_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) {
			return static_cast<parent_type&>(*this).template component<channel>(x, y);
		}
		template<int channel> typename ct::type_at<component_const_ref_tl, channel>::type
		component(const coordinate_type x, const coordinate_type y) const {
			return static_cast<parent_type&>(*this).template component<channel>(x, y);
		}
	public:
		/// Returns writable access to the cyan component
		typename ct::type_at<component_ref_tl, 0>::type
		cyan(const coordinate_type x, const coordinate_type y) {
			return component<0>(x, y);
		}
		/// Returns read-only access to the cyan component
		typename ct::type_at<component_const_ref_tl, 0>::type
		cyan(const coordinate_type x, const coordinate_type y) const {
			return component<0>(x, y);
		}
		/// Returns writable access to the magenta component
		typename ct::type_at<component_ref_tl, 1>::type
		magenta(const coordinate_type x, const coordinate_type y) {
			return component<1>(x, y);
		}
		/// Returns read-only access to the magenta component
		typename ct::type_at<component_const_ref_tl, 1>::type
		magenta(const coordinate_type x, const coordinate_type y) const {
			return component<1>(x, y);
		}
		/// Returns writable access to the yellow component
		typename ct::type_at<component_ref_tl, 2>::type
		yellow(const coordinate_type x, const coordinate_type y) {
			return component<2>(x, y);
		}
		/// Returns read-only access to the yellow component
		typename ct::type_at<component_const_ref_tl, 2>::type \
		yellow(const coordinate_type x, const coordinate_type y) const {
			return component<2>(x, y);
		}
		/// Returns writable access to the black component
		typename ct::type_at<component_ref_tl, 3>::type
		black(const coordinate_type x, const coordinate_type y) {
			return component<3>(x, y);
		}
		/// Returns read-only access to the black component
		typename ct::type_at<component_const_ref_tl, 3>::type \
		black(const coordinate_type x, const coordinate_type y) const {
			return component<3>(x, y);
		}
	};
	
	template<typename imageT> class image_with_xy {
	private:
		imageT _real_image;
	};
	
	#undef VVIS_DEFINE_COMPONENT
	
	/** operator<< for image
	 * Prints out contents of image. Useful for debugging
	 */
	template<typename componentT, template<typename> class storageP>
	std::ostream& operator<<(std::ostream& os, const base_image<componentT, storageP>& img) {
		for(int y = 0; y < img.height(); ++y) {
			for(int x = 0; x < img.width(); ++x) {
				os << img.pixel(x, y) << " ";
			}
			// Don't print end of line if it is the last row
			if(y != img.height() - 1)
				os << std::endl;
		}
		return os;
	}
	
	template<typename componentT, template<typename> class storageP>
	std::ostream& operator<<(std::ostream& os, const image<componentT, storageP>& img) {
		for(int y = 0; y < img.height(); ++y) {
			for(int x = 0; x < img.width(); ++x) {
				os << static_cast<typename ct::promote<componentT>::type>(img.pixel(x, y)) << " ";
			}
			// Don't print end of line if it is the last row
			if(y != img.height() - 1)
				os << std::endl;
		}
		return os;
	}
} // End of vvis namespace

#endif
