/*
 *  storage.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Jul 06 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

/*! @header vvis/storage.h
 * @abstract
 *   This header file contains VVIS storage definitions
 */

#ifndef _VVIS_STORAGE_H
#define _VVIS_STORAGE_H

#include <cassert>
#include <vector>
#include <ct/boost_type_traits.h>
#include <ct/tuple.h>
#include <ct/meta_meta.h>
#include <vvis/config.h>
#include <vvis/metafunction.h>
#include <vvis/macros.h>
#include <vvis/image_base.h>
#include <vvis/iterator.h>
#include <vvis/shape.h>
#include <vvis/iterator_helper.h>

namespace vvis {
	namespace priv {
		//= pixel_impl =========================================================
		template<typename typelistT> struct pixel_impl;
		//- 1-channel pixel_impl -----------------------------------------------
		template<
			typename T1
		> struct pixel_impl<ct::typelist<T1, ct::null_type> >  {
			typedef T1& writable_pixel;
			typedef const T1& readonly_pixel;
			typedef T1 pixel_type;
			/// Returns read-only access
			template<typename storageT>
			static readonly_pixel
			exec(const storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return s.template component<0>(x, y);
			}
			/// Returns writable access
			template<typename storageT>
			static writable_pixel
			exec(storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return s.template component<0>(x, y);
			}
		};
		//- 2-channel pixel_impl -----------------------------------------------
		template<
			typename T1,
			typename T2
		> struct pixel_impl<ct::typelist<T1, ct::typelist<T2, ct::null_type> > >  {
			typedef ct::tuple<
				ct::typelist<T1&, ct::typelist<T2&, ct::null_type> >
				> writable_pixel;
			typedef const ct::tuple<
				ct::typelist<const T1, ct::typelist<const T2, ct::null_type> >
				> readonly_pixel;
			typedef ct::tuple<
				ct::typelist<T1, ct::typelist<T2, ct::null_type> >
				> pixel_type;
			/// Returns read-only access
			template<typename storageT>
			static const readonly_pixel
			exec(const storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return readonly_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y));
			}
			/// Returns writable access
			template<typename storageT>
			static writable_pixel
			exec(storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return writable_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y));
			}
		};
		//- 3-channel pixel_impl -----------------------------------------------
		template<
			typename T1,
			typename T2,
			typename T3
		> struct pixel_impl<ct::typelist<T1, ct::typelist<T2, ct::typelist<T3, ct::null_type> > > > {
			typedef ct::tuple<
				ct::typelist<T1&, ct::typelist<T2&, ct::typelist<T3&, ct::null_type> > >
				> writable_pixel;
			typedef const ct::tuple<
				ct::typelist<const T1, ct::typelist<const T2, ct::typelist<const T3, ct::null_type> > >
				> readonly_pixel;
			typedef ct::tuple<
				ct::typelist<T1, ct::typelist<T2, ct::typelist<T3, ct::null_type> > >
				> pixel_type;
			/// Returns read-only access
			template<typename storageT>
			static const readonly_pixel
			exec(const storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return readonly_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y),
					s.template component<2>(x, y));
			}
			/// Returns writable access
			template<typename storageT>
			static writable_pixel
			exec(storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return writable_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y),
					s.template component<2>(x, y));
			}
		};
		//- 4-channel pixel_impl -----------------------------------------------
		template<
			typename T1,
			typename T2,
			typename T3,
			typename T4
		> struct pixel_impl<ct::typelist<T1, ct::typelist<T2, ct::typelist<T3, ct::typelist<T4, ct::null_type> > > > > {
			typedef ct::tuple<
				ct::typelist<T1&, ct::typelist<T2&, ct::typelist<T3&, ct::typelist<T4&, ct::null_type> > > >
				> writable_pixel;
			typedef const ct::tuple<
				ct::typelist<const T1, ct::typelist<const T2, ct::typelist<const T3, ct::typelist<const T4, ct::null_type> > > >
				> readonly_pixel;
			typedef ct::tuple<
				ct::typelist<T1, ct::typelist<T2, ct::typelist<T3, ct::typelist<T4, ct::null_type> > > >
				> pixel_type;
			/// Returns read-only access
			template<typename storageT>
			static const readonly_pixel
			exec(const storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return readonly_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y),
					s.template component<2>(x, y), s.template component<3>(x, y));
			}
			/// Returns writable access
			template<typename storageT>
			static writable_pixel
			exec(storageT& s, const image_base::coordinate_type x, const image_base::coordinate_type y) {
				return writable_pixel(
					s.template component<0>(x, y), s.template component<1>(x, y),
					s.template component<2>(x, y), s.template component<3>(x, y));
			}
		};
	} // End of priv namespace
	
	//==========================================================================
	// Base Storage
	//--------------------------------------------------------------------------	
	/*! @template base_storage
	 * @abstract
	 *   This is the parent of all VVIS storages
	 * @templatefield
	 *   typelistT A typelist of channel component types
	 */
	template<typename typelistT> class base_storage {
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		base_storage()
		: _left(0), _top(0), _width(0), _height(0) {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		base_storage(const base_storage& rhs)
		: _left(rhs._left), _top(rhs._top), _width(rhs._width), _height(rhs._height) {
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		base_storage(const image_base::coordinate_type w,
		const image_base::coordinate_type h)
		: _left(0), _top(0), _width(w), _height(h) {
		}
		///
		base_storage(const image_base::coordinate_type l,
		const image_base::coordinate_type t,
		const image_base::coordinate_type w,
		const image_base::coordinate_type h)
		: _left(l), _top(t), _width(w), _height(h) {
		}
	protected:
		/** Destructor
		 */
		~base_storage() {
		}
	public:
		/// Returns the width of the image
		const image_base::coordinate_type width() const {
			return _width;
		}
		/// Returns the height of the image
		const image_base::coordinate_type height() const {
			return _height;
		}
	protected:
		const image_base::coordinate_type left() const {
			return _left;
		}
		const image_base::coordinate_type top() const {
			return _top;
		}
		const image_base::coordinate_type right() const {
			return _left + _width;
		}
		const image_base::coordinate_type bottom() const {
			return _top - _height;
		}
	protected:
		image_base::coordinate_type _left, _top;
		image_base::coordinate_type _width, _height;
	};
	
	//==========================================================================
	// Contiguous Planar Storage
	//--------------------------------------------------------------------------
	/*! @template contiguous_planar_storage
	 * @abstract
	 *   Storage Policy for contiguous planar storage
	 * @discussion
	 *   Contiguous storage refers to schemes where the pixels are stored
	 *   one after the other. The pixels are all the same type.
	 *   A C/C++ 1-D array would be an example of contiguous storage.
	 * @templatefield
	 *   typelistT A typelist containing types for each channel
	 */
	template<typename typelistT> class contiguous_planar_storage 
	: public contiguous_aligned_storage, public rectangular, public base_storage<typelistT> {
	private:
		typedef base_storage<typelistT> parent_type;
		typedef typename priv::planar_iterator_helper<
			contiguous_planar_storage<typelistT>, typelistT > iterator_helper;
		typedef typename priv::pixel_impl<typelistT> pixel_helper;
		// iterator_helper is a friend
		friend class priv::planar_iterator_helper<
			contiguous_planar_storage<typelistT>, typelistT >;
	public:
		/// Value Type
		typedef typename pixel_helper::pixel_type value_type;
		/// Typelist containing types for components
		typedef typelistT component_tl;
		
	private:
		/// Typelist containing types for channels
		typedef typename ct::apply<component_tl, boost::add_pointer>::type component_ptr_tl;
		/// Number of channels
		enum { channel_count = ct::length<component_tl>::value };
	protected:
		/** Gets the beginnings
		 */
		struct get_begin {
			template<int i> struct Code {
				template<typename pixelPtrT, typename storageT>
				static void exec(pixelPtrT& p, storageT& s) {
					p.template get<i>() = s.template begin<i>();
				}
			};
		};
		/** Gets the ends
		 */
		struct get_end {
			template<int i> struct Code {
				template<typename pixelPtrT, typename storageT>
				static void exec(pixelPtrT& p, storageT& s) {
					p.template get<i>() = s.template end<i>();
				}
			};
		};
	public:
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = channel_count;
		/// Step to get to the next vector or scalar
		enum { vector_step = VVM_SCALAR_COUNT, scalar_step = 1 };
		/// Iterator
		typedef typename iterator_helper::iterator iterator;
		typedef typename iterator_helper::const_iterator const_iterator;
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		contiguous_planar_storage()
		: parent_type(), _data_size(0) {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		contiguous_planar_storage(const contiguous_planar_storage& rhs)
		: parent_type(rhs), _data_size(0) {
			allocate();
			meta::EFOR2<0, meta::Less, channel_count, +1, memcopy_channels>::exec(*this, rhs);
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		contiguous_planar_storage(const image_base::coordinate_type width,
		const image_base::coordinate_type height)
		: parent_type(width, height), _data_size(0) {
			allocate();
		}
	protected:
		/** Destructor
		 * Deallocates base_image data from heap if it was created
		 */
		~contiguous_planar_storage() {
			deallocate();
		}
	protected:
		void allocate() {
			assert(_data_size == 0);
			// Nothing to allocate if width is 0 or height is 0
			if(this->_width == 0 || this->_height == 0)
				return;
			// Add step, because unaligned algorithm requires one extra for
			// read-ahead
			_data_size = this->_width * this->_height + VVM_SCALAR_COUNT;
			meta::EFOR1<0, meta::Less, channel_count, +1, create_channels>::exec(*this);
		}
		void deallocate() {
			if(_data_size) {
				meta::EFOR1<0, meta::Less, channel_count, +1, delete_channels>::exec(*this);
				_data_size = 0;
			}
		}
	public:
		contiguous_planar_storage& operator=(const contiguous_planar_storage& rhs) {
			// Check if the same object
			if(this == &rhs)
				return *this;
			// Resize image if size is different
			if(rhs.width() != this->width() || rhs.height() != this->height()
			|| rhs.left() != this->left() || rhs.top() != this->top())
				resize(rhs.left(), rhs.top(), rhs.width(), rhs.height());
			// Perform copy
			meta::EFOR2<0, meta::Less, channel_count, +1, memcopy_channels>::exec(*this, rhs);
			return *this;
		}
	public:
	//= Image Interface ========================================================
		void resize(const image_base::coordinate_type l, const image_base::coordinate_type t,
		const image_base::coordinate_type width, const image_base::coordinate_type height) {
			this->_left = l;
			this->_top = t;
			// Check if resize is necessary
			if(width == this->_width && height == this->_height) {
				return;
			}
			// Deallocate everything and reallocate
			deallocate();
			this->_width = width;
			this->_height = height;
			allocate();
		}
		/** Returns a reference to a pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename ct::type_at<component_tl, i>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			return _data.template get<i>()[y*this->_width + x];
		}
		/** Returns a constant value of the pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename boost::add_const<typename ct::type_at<component_tl, i>::type>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			return _data.template get<i>()[y*this->_width + x];
		}
		/// Returns read-only access to pixel
		typename pixel_helper::readonly_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			return pixel_helper::exec(*this, x, y);
		}
		/// Returns writable access to pixel
		typename pixel_helper::writable_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			return pixel_helper::exec(*this, x, y);
		}
	public:
	//= Contiguous Storage Interface ===========================================
		/** Returns a non-const pointer to the beginning of the data
		 * This function is implemented only by contiguous_storage
		 */
		iterator begin() {
			return iterator_helper::begin(*this);
		}
		/** Returns a const pointer to the beginning of the data
		 * This function is implemented only by contiguous_storage
		 */
		const_iterator begin() const {
			return iterator_helper::begin(*this);
		}
		/** Returns a non-const pointer to the end of the data
		 * The pointer points to the first position after the end of the data
		 * This function is implemented only by contiguous_storage
		 */
		iterator end() {
			return iterator_helper::end(*this);
		}
		/** Returns a const pointer to the end of the data
		 * The pointer points to the first position after the end of the data
		 * This function is implemented only by contiguous_storage
		 */
		const_iterator end() const {
			return iterator_helper::end(*this);
		}
	_VVIS_COMPONENT_BEGIN_END_ACCESS:
		template<int i>
		typename ct::type_at<component_ptr_tl, i>::type
		begin() {
			return _data.template get<i>();
		}
		template<int i>
		typename boost::add_const<typename ct::type_at<component_ptr_tl, i>::type>::type
		begin() const {
			return _data.template get<i>();
		}
		template<int i>
		typename ct::type_at<component_ptr_tl, i>::type
		end() {
			return &_data.template get<i>()[this->_height * this->_width];
		}
		template<int i>
		typename boost::add_const<typename ct::type_at<component_ptr_tl, i>::type>::type
		end() const {
			return &_data.template get<i>()[this->_height * this->_width];
		}
	protected:
		/// Create channels
		struct create_channels {
			template<int i> struct Code {
				static void exec(contiguous_planar_storage<component_tl>& self) {
					// TODO: Use vvm::allocate instead
					self._data.template get<i>() = 
						new typename ct::type_at<component_tl, i>::type[self._data_size];
					if(self._data.template get<i>() == 0)
						_VVIS_ERROR("Unable to allocate block of size " << self._data_size);
				}
			};
		};
		/// Delete channels
		struct delete_channels {
			template<int i> struct Code {
				static void exec(contiguous_planar_storage<component_tl>& self) {
					delete[] self._data.template get<i>();
				}
			};
		};
		/** Memory Copy
		 * Copies using memcpy. This only works if the source is also a contiguous storage
		 */
		struct memcopy_channels {
			template<int i> struct Code {
				static void exec(contiguous_planar_storage<component_tl>& self, const contiguous_planar_storage<component_tl>& rhs) {
					memcpy(self._data.template get<i>(), rhs._data.template get<i>(),
						self._data_size * sizeof(typename ct::type_at<typelistT, i>::type));
				}
			};
		};
	protected:
		ct::tuple<component_ptr_tl> _data;
		std::size_t _data_size;
	};
	
	//==========================================================================
	// Contiguous Storage (Chunky Vector)
	//--------------------------------------------------------------------------
	/*! @template contiguous_chunky_storage
	 * @abstract
	 *   Storage Policy for contiguous storage
	 * @discussion
	 *   Contiguous storage refers to schemes where the pixels are stored
	 *   one after the other. The pixels are all the same type.
	 *   A C/C++ 1-D array would be an example of contiguous storage.
	 *   Supports both contiguous and illife storage
	 * @templatefield
	 *   typelistT A typelist containing types for each channel.
	 */
	template<typename typelistT> class contiguous_chunky_storage 
	: public contiguous_aligned_storage, public rectangular, public base_storage<typelistT> {
	private:
		typedef base_storage<typelistT> parent_type;
		typedef typename priv::chunky_iterator_helper<
			contiguous_chunky_storage<typelistT>, typelistT > iterator_helper;
		typedef typename priv::pixel_impl<typelistT> pixel_helper;
		// Iterator helper is a friend
		friend class priv::chunky_iterator_helper<
			contiguous_chunky_storage<typelistT>, typelistT >;
	public:
		/// Value Type
		typedef typename pixel_helper::pixel_type value_type;
		/// Typelist containing types for components
		typedef typelistT component_tl;
	private:
		/// Typelist containing types for channels
		typedef typename ct::apply<component_tl, boost::add_pointer>::type component_ptr_tl;
		/// Number of channels
		enum { channel_count = ct::length<component_tl>::value };
		/// Typelist containing component vectors
		typedef typename ct::apply<component_tl, vvm::add_vector>::type component_vec_tl;
		/// Chunky Type
		typedef ct::tuple<component_vec_tl> chunk;
	private:
		/** Gets the beginnings
		 */
		struct get_begin {
			template<int i> struct Code {
				template<typename pixelPtrT, typename storageT>
				static void exec(pixelPtrT& p, storageT& s) {
					p.template get<i>() = s.template begin<i>();
				}
			};
		};
		/** Gets the ends
		 */
		struct get_end {
			template<int i> struct Code {
				template<typename pixelPtrT, typename storageT>
				static void exec(pixelPtrT& p, storageT& s) {
					p.template get<i>() = s.template end<i>();
				}
			};
		};
	public:
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = 1;
		/// Step to get to the next vector or scalar
		enum { vector_step = sizeof(chunk), scalar_step = 1 };
		/// Iterator
		typedef typename iterator_helper::iterator iterator;
		typedef typename iterator_helper::const_iterator const_iterator;
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		contiguous_chunky_storage()
		: parent_type(), _data_size(0) {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		contiguous_chunky_storage(const contiguous_chunky_storage& rhs)
		: parent_type(rhs), _data_size(0) {
			allocate();
			memcpy(_data, rhs._data, _data_size * sizeof(chunk));
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		contiguous_chunky_storage(const image_base::coordinate_type width,
		const image_base::coordinate_type height)
		: parent_type(width, height), _data_size(0) {
			allocate();
		}
	protected:
		/** Destructor
		 * Deallocates base_image data from heap if it was created
		 */
		~contiguous_chunky_storage() {
			deallocate();
		}
	protected:
		void allocate() {
			assert(_data_size == 0);
			// Nothing to allocate if width is 0 or height is 0
			if(this->_width == 0 || this->_height == 0)
				return;
			// Add step, because unaligned algorithm requires one extra for
			// read-ahead
			const int total = this->_width * this->_height;
			_data_size = total / VVM_SCALAR_COUNT + 1;
			_data = new chunk[_data_size];
		}
		void deallocate() {
			if(_data_size) {
				delete[] _data;
				_data_size = 0;
			}
		}
	public:
		contiguous_chunky_storage& operator=(const contiguous_chunky_storage& rhs) {
			// Check if the same object
			if(this == &rhs)
				return *this;
			// Resize image if size is different
			resize(rhs.left(), rhs.top(), rhs.width(), rhs.height());
			// Perform copy
			memcpy(_data, rhs._data, _data_size * sizeof(chunk));
			return *this;
		}
	public:
	//= Image Interface ========================================================
		void resize(const image_base::coordinate_type l, const image_base::coordinate_type t,
		const image_base::coordinate_type width, const image_base::coordinate_type height) {
			this->_left = l;
			this->_top = t;
			// Check if resize is necessary
			if(width == this->_width && height == this->_height)
				return;
			// Deallocate everything and reallocate
			deallocate();
			this->_width = width;
			this->_height = height;
			allocate();
		}
		/** Returns a reference to a pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename ct::type_at<component_tl, i>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			const int index = y*this->_width + x;
			const int chunk_index = index / VVM_SCALAR_COUNT;
			const int scalar_index = index % VVM_SCALAR_COUNT;
			return _data[chunk_index].template get<i>().scalar(scalar_index);
		}
		/** Returns a constant value of the pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename boost::add_const<typename ct::type_at<component_tl, i>::type>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			const int index = y*this->_width + x;
			const int chunk_index = index / VVM_SCALAR_COUNT;
			const int scalar_index = index % VVM_SCALAR_COUNT;
			return _data[chunk_index].template get<i>().scalar(scalar_index);
		}
		/// Returns read-only access to pixel
		typename pixel_helper::readonly_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			return pixel_helper::exec(*this, x, y);
		}
		/// Returns writable access to pixel
		typename pixel_helper::writable_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			return pixel_helper::exec(*this, x, y);
		}
	public:
	//= Contiguous Storage Interface ===========================================
		iterator begin() {
			return iterator_helper::begin(*this);
		}
		const_iterator begin() const {
			return iterator_helper::begin(*this);
		}
		iterator end() {
			return iterator_helper::end(*this);
		}
		const_iterator end() const {
			return iterator_helper::end(*this);
		}
	_VVIS_COMPONENT_BEGIN_END_ACCESS:
		/** Returns a non-const pointer to the beginning of the data
		 * This function is implemented only by contiguous_storage
		 */
		template<int i>
		typename ct::type_at<component_ptr_tl, i>::type
		begin() {
			typedef typename ct::type_at<component_ptr_tl, i>::type return_t;
			return reinterpret_cast<return_t>(&(_data[0].template get<i>()));
		}
		/** Returns a const pointer to the beginning of the data
		 * This function is implemented only by contiguous_storage
		 */
		template<int i>
		typename boost::add_const<typename ct::type_at<component_ptr_tl, i>::type>::type
		begin() const {
			typedef typename boost::add_const<
				typename ct::type_at<component_ptr_tl, i>::type>::type return_t;
			return reinterpret_cast<return_t>(&(_data[0].template get<i>()));
		}
		/** Returns a non-const pointer to the end of the data
		 * The pointer points to the first position after the end of the data
		 * This function is implemented only by contiguous_storage
		 */
		template<int i>
		typename ct::type_at<component_ptr_tl, i>::type
		end() {
			typedef typename ct::type_at<component_ptr_tl, i>::type return_t;
			return reinterpret_cast<return_t>(&(_data[_data_size].template get<i>()));
		}
		/** Returns a const pointer to the end of the data
		 * The pointer points to the first position after the end of the data
		 * This function is implemented only by contiguous_storage
		 */
		template<int i>
		typename boost::add_const<typename ct::type_at<component_ptr_tl, i>::type>::type
		end() const {
			typedef typename boost::add_const<
				typename ct::type_at<component_ptr_tl, i>::type>::type return_t;
			return reinterpret_cast<return_t>(&(_data[_data_size].template get<i>()));
		}
	protected:
		chunk* _data;
		std::size_t _data_size;
	};

	//==========================================================================
	// Base Illife Storage
	//--------------------------------------------------------------------------
	namespace priv {
		template<typename storageT> class illife_row_storage : public storageT {
		};
	} // End of priv namespace
	/** Base Implementation for illife storage policies
	 */
	template<typename typelistT, template<typename> class rowStorageP>
	class base_illife_storage
	: public illife_storage, public rectangular, public base_storage<typelistT> {
	private:
		typedef base_storage<typelistT> parent_type;
	public:
		/// Typelist containing types for components
		typedef typelistT component_tl;
		/// Value type
		typedef priv::illife_row_storage<rowStorageP<typelistT> > value_type;
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = value_type::prefetch_channel_count;
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		base_illife_storage()
		: parent_type() {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		base_illife_storage(const base_illife_storage& rhs)
		: parent_type(rhs) {
			*this = rhs;
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		base_illife_storage(const image_base::coordinate_type width,
		const image_base::coordinate_type height)
		: parent_type(width, height) {
			allocate();
		}
	protected:
		/** Destructor
		 * Deallocates base_image data from heap if it was created
		 */
		~base_illife_storage() {
			deallocate();
		}
	protected:
		void allocate() {
			_data.resize(this->_height);
			for(int y = 0; y < this->_height; ++y) {
				_data[y].resize(this->_left, this->_height - y - 1, this->_width, 1);
			}
		}
		void deallocate() {
			// std::vector handles deallocation of data
		}
	public:
		base_illife_storage& operator=(const base_illife_storage& rhs) {
			// Check if the same object
			if(this == &rhs)
				return *this;
			// Copy data
			_data = rhs._data;
			return *this;
		}
	public:
	//= Image Interface ========================================================
		void resize(const image_base::coordinate_type width, const image_base::coordinate_type height) {
			// Check if resize is necessary
			if(width == this->_width && height == this->_height)
				return;
			deallocate();
			this->_width = width;
			this->_height = height;
			allocate();
		}
		/** Returns a reference to a pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename ct::type_at<component_tl, i>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			return _data[y].template component<i>(x, 0);
		}
		/** Returns a constant value of the pixel's component
		 * Returns the pixel at position x, y's component
		 */
		template<int i>
		typename boost::add_reference<typename boost::add_const<typename ct::type_at<component_tl, i>::type>::type>::type
		component(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			return _data[y].template component<i>(x, 0);
		}
		/// Returns read-only access to pixel
		typename priv::pixel_impl<typelistT>::readonly_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) const {
			return priv::pixel_impl<typelistT>::exec(*this, x, y);
		}
		/// Returns writable access to pixel
		typename priv::pixel_impl<typelistT>::writable_pixel
		pixel(const image_base::coordinate_type x, const image_base::coordinate_type y) {
			return priv::pixel_impl<typelistT>::exec(*this, x, y);
		}
	public:
	//= Illife Storage Interface ==================================================
		typedef typename std::vector<value_type>::iterator iterator;
		typedef typename std::vector<value_type>::const_iterator const_iterator;
		iterator begin() {
			return _data.begin();
		}
		const_iterator begin() const {
			return _data.begin();
		}
		iterator end() {
			return _data.end();
		}
		const_iterator end() const {
			return _data.end();
		}
#if 1
		value_type& operator[](const int i) {
			return _data[i];
		}
		const value_type& operator[](const int i) const {
			return _data[i];
		}
		const int size() const {
			return _data.size();
		}
#endif
	protected:
		std::vector<value_type> _data;
	};


	//==========================================================================
	// Illife Planar Storage
	//--------------------------------------------------------------------------
	/** Storage Policy for illife planar storage
	 * Illife storage refers to storage using illife vectors. Illife vectors
	 * are arrays of arrays. A C/C++ 2-D array are illife vectors.
	 */
	template<typename typelistT> class illife_planar_storage
	: public base_illife_storage<typelistT, contiguous_planar_storage> {
	private:
		typedef base_illife_storage<typelistT, contiguous_planar_storage> parent_type;
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		illife_planar_storage()
		: parent_type() {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		illife_planar_storage(const illife_planar_storage& rhs)
		: parent_type(rhs) {
			*this = rhs;
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		illife_planar_storage(const image_base::coordinate_type width,
		const image_base::coordinate_type height)
		: parent_type(width, height) {
			this->allocate();
		}
	protected:
		/** Destructor
		 * Deallocates base_image data from heap if it was created
		 */
		~illife_planar_storage() {
			// std::vector handles deallocation of data
		}
	};
	
	//==========================================================================
	// Illife Storage (Chunky Vector)
	//--------------------------------------------------------------------------
	/** Storage Policy for illife storage
	 * Illife storage refers to storage using illife vectors. Illife vectors
	 * are arrays of arrays. A C/C++ 2-D array are illife vectors.
	 */
	template<typename typelistT> class illife_chunky_storage
	: public base_illife_storage<typelistT, contiguous_chunky_storage> {
	private:
		typedef base_illife_storage<typelistT, contiguous_chunky_storage> parent_type;
	public:
		/** Default constructor
		 * Sets width and height to 0
		 */
		illife_chunky_storage()
		: parent_type() {
		}
		/** Copy constructor
		 * Makes a deep copy of source. This include a copy of the image data
		 */
		illife_chunky_storage(const illife_chunky_storage& rhs)
		: parent_type(rhs) {
			*this = rhs;
		}
		/** Initializes image with specified width and height
		 * Initializes image with the specified width and height. The image data area is
		 * created, but is not initialized to any set value.
		 * The size of the image data is at least large enough to hold width*height pixels.
		 */
		illife_chunky_storage(const image_base::coordinate_type width,
		const image_base::coordinate_type height)
		: parent_type(width, height) {
			this->allocate();
		}
	protected:
		/** Destructor
		 * Deallocates base_image data from heap if it was created
		 */
		~illife_chunky_storage() {
			// std::vector handles deallocation of data
		}
	};
} // End of vvis namespace

#endif

