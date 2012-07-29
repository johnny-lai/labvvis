/*
 *  region.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Wed Oct 08 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_REGION_H
#define _VVIS_REGION_H

#include <vector>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <vvm/vvm.h>
#include <ct/boost_type_traits.h>
#include <ct/typelist.h>
#include <vvis/storage.h>
#include <vvis/metafunction.h>

namespace vvis {
	struct rect {
		rect() {
		}
		rect(image_base::coordinate_type l, image_base::coordinate_type t,
		image_base::coordinate_type w, image_base::coordinate_type h)
		: left(l), top(t), width(w), height(h) {
		}
		image_base::coordinate_type left, top, width, height;
	};

	namespace priv {
		struct using_contiguous_storage {};
		struct using_illife_storage {};
		struct using_unknown_storage {};
		// Returns the fastest storage type
		template<typename typelistT> struct fastest_storage {
			typedef typename boost::mpl::if_c<
				ct::all<typelistT, priv::uses_contiguous_storage>::value,
				using_contiguous_storage,
				typename boost::mpl::if_c<
					ct::all<typelistT, priv::uses_illife_storage>::value,
					using_illife_storage,
					using_unknown_storage
					>::type
				>::type type;
		};
		template<typename storageT, typename specializedT = void>
		struct real_effective_storage {
			typedef storageT type;
		};
		template<typename storageT>
		struct real_effective_storage<storageT,
			typename ct::enable_if<
				priv::uses_illife_storage<storageT>::value
			>::type
		> {
			typedef typename storageT::value_type type;
		};
		template<typename storageT> struct effective_storage {
			typedef typename real_effective_storage<storageT>::type type;
		};
		template<typename typelistT> struct select_illife_unknown {
			typedef typename boost::mpl::if_c<
				ct::all<
					typename ct::apply<typelistT, effective_storage>::type,
					uses_contiguous_storage>::value == true,
				using_contiguous_storage,
				using_unknown_storage
				>::type type;
		};
		// Rectangular Copy using only pixel
		template<typename typelistT, typename methodTag>
		struct rect_admit {
			template<typename srcStorageT, typename destStorageT>
			static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
				//_VVIS_WARN("rect_admit using pixel");
				for(int y = 0; y < r.height; ++y) {
					for(int x = 0; x < r.width; ++x) {
						dest.pixel(x, y) = src.pixel(x + r.left, y + r.top);
					}
				}
			}
		};
		template<typename typelistT, typename methodTag>
		struct rect_release {
			template<typename srcStorageT, typename destStorageT>
			static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
				//_VVIS_WARN("rect_release using pixel");
				for(int y = 0; y < r.height; ++y) {
					for(int x = 0; x < r.width; ++x) {
						dest.pixel(x + r.left, y + r.top) = src.pixel(x, y);
					}
				}
			}
		};
		/*
		// Rectangular Copy using illife
		template<typename typelistT>
		struct rect_copy<typelistT, priv::using_illife_storage> {
			template<typename srcStorageT, typename destStorageT>
			static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
				std::cout << "Copying using illife" << endl;
				meta::EFOR3<0, meta::Less, ct::length<typelistT>::value, +1, memcopy_channels>
					::exec(src, dest, r);
			}
			// Memory copy
			struct memcopy_channels {
				template<int i> struct Code {
					template<typename srcStorageT, typename destStorageT>
					static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
						for(int y = 0; y < r.height; ++y) {
							memcpy(dest.template begin<i>(y), src.template begin<i>(y),
								r.width * sizeof(typename ct::type_at<typelistT, i>::type));
						}
					}
				};
			};
		};
		// Rectangular Copy for contiguous storage
		// Actually copies as illife
		template<typename typelistT>
		struct rect_copy<typelistT, priv::using_contiguous_storage> {
			template<typename srcStorageT, typename destStorageT>
			static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
				std::cout << "Copying using illife (contiguous)" <<endl;
				meta::EFOR3<0, meta::Less, ct::length<typelistT>::value, +1, memcopy_channels>
					::exec(src, dest, r);
			}
			// Memory copy
			struct memcopy_channels {
				template<int i> struct Code {
					template<typename srcStorageT, typename destStorageT>
					static void exec(const srcStorageT& src, destStorageT& dest, const rect& r) {
						for(int y = 0; y < r.height; ++y) {
							memcpy(dest.template begin<i>(y), src.template begin<i>(y),
								r.width * sizeof(typename ct::type_at<typelistT, i>::type));
						}
					}
				};
			};
		};
		*/
	} // End of priv namespace

#if 0
	/** Rectangular Region
	 * Which presents data as illife_storage
	 */
	template<
		typename storageT,
		template<typename> class storageP = VVIS_DEFAULT_STORAGE_POLICY
	> class rect_region : public storageP<typename storageT::component_tl> {
	private:
		typedef storageP<typename storageT::component_tl> parent_type;
		typedef typename priv::fastest_storage<
			typename ct::cons_tl<storageT, rect_region<storageT, storageP> >::type
			>::type copy_storage_tag;
	public:
		typedef typename storageT::component_tl component_tl;
	public:
		/** Default constructor
		 * No storage and no data
		 */
		rect_region()
		: parent_type(), _pstorage(0) {
		}
		/** Copy constructor
		 * Points to the same storage. Makes a copy of the working data
		 */
		rect_region(const rect_region& r)
		: parent_type(r), _pstorage(r._pstorage), _rect(r._rect) {
    	}
		/// Create rectangular region that covers entire storage
		rect_region(storageT& s)
		: parent_type(), _pstorage(0) {
			admit(s);
			//cerr << "constructor storage" << endl;
		}
		/// Create rectangular region that covers region specified by rect
		rect_region(storageT& s, const rect& r)
		: parent_type(), _pstorage(0) {
			admit(s, r);
		}
		/** Destructor
		 * Will release storage
		 */
		~rect_region() {
			release();
		}
	public:
		/** Admit a storage to the region
		 * Upon admittance, the storage should not be touched
		 * Precondition: No storage is admitted
		 */
		void admit(storageT& storage) {
			assert(_pstorage == 0);
			admit(storage, rect(0, 0, storage.width(), storage.height()));
		}
		/** Admit part of the storage to the region
		 * Rect specifies region. Bottom and right are not part of region.
		 */
		void admit(storageT& storage, const rect& r) {
			assert(_pstorage == 0);
			_pstorage = &storage;
			_rect = r;
			resize(_rect.width, _rect.height);
			priv::rect_admit<component_tl, copy_storage_tag>::exec(*_pstorage, *this, _rect);
		}
		/** Releases the storage
		 */
		void release() {
			if(_pstorage == 0)
				return;
			// Copy data back
			priv::rect_release<component_tl, copy_storage_tag>::exec(*this, *_pstorage, _rect);
			// Reset state
			deallocate();
			_width = _height = 0;
			_rect.left = _rect.top = _rect.width = _rect.height = 0;
			_pstorage = 0;
		}
	public:
		// Begin & End inherited from storageP
	private:
		storageT* _pstorage;
		rect _rect;
	};
#endif
	
	/*! @struct aligned_policy
	 * @abstract
	 *   Tag representing aligned storage policies.
	 * @discussions
	 *   Used with rect_region
	 */
	struct aligned_policy;

	/*! @struct unaligned_policy
	 * @abstract
	 *   Tag representing unaligned storage policies.
	 * @discussion
	 *   Used with rect_region
	 */
	struct unaligned_policy;
		
	/*! @template rect_region
	 * @abstract
	 *   Rectangular region. Does not perform any copying.
	 * @discussion
	 *   This template is either an unaligned_contiguous_storage or an
	 *   unknown storage, or an illife storage with either types.
	 * @templatefield storageT Storage Type being accepted
	 * @templatefield alignmentP Alignment Policy (aligned_policy or unaligned_policy)
	 */
	template<
		typename storageT,
		typename alignmentP = unaligned_policy,
		typename specializedT = void> class rect_region {
	};
	
	//- rect_region<storageT, aligned_policy> ----------------------------------
	/** @template rect_region
	 * TODO: Fix black vertical band
	 * @abstract
	 *   Rect region for contiguous.
	 * @description
	 *   This rect_region returns an aligned storage
	 */
	template<
		typename storageT
	> class rect_region<
		storageT,
		aligned_policy,
		typename ct::enable_if<
			uses_contiguous_storage<storageT>::value
		>::type
	> : public contiguous_aligned_storage, public rectangular {
	public:
		/// Typelist containing types for components
		typedef typename storageT::component_tl component_tl;
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = storageT::prefetch_channel_count;
		
		typedef typename storageT::value_type value_type;
	public:
		rect_region() {
		}
		rect_region(const rect_region& rhs)
		: _storage(rhs._storage), _left(rhs._left), _width(rhs._width), _offset(rhs._offset) {
		}
		rect_region(storageT& storage, int left, int width)
		: _storage(storage), _left(left), _width(width) {
			_offset = (_storage.begin() + _left).scalar;
		}
	public:
		rect_region& operator=(const rect_region& rhs) {
			if(&rhs == this)
				return *this;
			_storage = rhs._storage;
			_left = rhs._left;
			_width = rhs._width;
			_offset = rhs._offset;
			return *this;
		}
	private:
		typedef typename storageT::iterator iterator_parent;
		typedef typename storageT::const_iterator const_iterator_parent;
	public:
		//- rect_region::iterator ----------------------------------------------
		class iterator : public iterator_parent {
		public:
			typedef typename iterator_parent::scalar_type scalar_type;
			typedef typename iterator_parent::vector_type vector_type;
		public:
			iterator(const iterator_parent& itr, vvm::offset_t offset)
			: iterator_parent(itr), _offset(offset) {
				this->scalar -= _offset;
			}
		public: // Accessor functions
			const scalar_type get_scalar() const {
				return reinterpret_cast<scalar_type*>(this->vector)[this->scalar + _offset];
			}
			void set_scalar(const scalar_type s) {
				reinterpret_cast<scalar_type*>(this->vector)[this->scalar + _offset] = s;
			}
			const vector_type get_vector() const {
				return vvm::load(*this->vector, *(this->vector + 1), _offset);
			}
			const vector_type get_vector(const vvm::offset_t shift) const {
				return vvm::load(*(this->vector + shift), *(this->vector + shift + 1), _offset);
			}
			void set_vector(const vector_type& v) {
				vvm::store_unaligned(v, this->vector, _offset);
			}
		private:
			vvm::offset_t _offset;
		};
		typedef iterator const_iterator;
	public:
		iterator begin() {
			return iterator(_storage.begin() + _left, _offset);
		}
		const_iterator begin() const {
			return const_iterator(_storage.begin() + _left, _offset);
		}
		iterator end() {
			return iterator(_storage.begin() + (_left + _width), _offset);
		}
		const_iterator end() const {
			return const_iterator(_storage.begin() + (_left + _width), _offset);
		}
	protected:
		storageT& _storage;
		int _left, _width;
		vvm::offset_t _offset;
	};
	template<
		typename storageT
	> struct unaligned_storage<rect_region<storageT, aligned_policy> > {
		typedef rect_region<storageT, unaligned_policy> type;
	};

	//- rect_region<storageT, unaligned_policy> --------------------------------
	/** @template rect_region
	 * TODO: Fix black vertical band
	 * @abstract
	 *   Rect region for contiguous.
	 * @description
	 *   This rect_region returns an unaligned storage
	 */
	template<
		typename storageT
	> class rect_region<
		storageT,
		unaligned_policy,
		typename ct::enable_if<
			uses_contiguous_storage<storageT>::value
		>::type
	> : public contiguous_unaligned_storage, public rectangular {
	public:
		/// Typelist containing types for components
		typedef typename storageT::component_tl component_tl;
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = storageT::prefetch_channel_count;
		
		typedef typename storageT::value_type value_type;
	public:
		rect_region() {
		}
		rect_region(const rect_region& rhs)
		: _storage(rhs._storage), _left(rhs._left), _width(rhs._width) {
		}
		rect_region(storageT& storage, int left, int width)
		: _storage(storage), _left(left), _width(width) {
		}
	public:
		rect_region& operator=(const rect_region& rhs) {
			if(&rhs == this)
				return *this;
			_storage = rhs._storage;
			_left = rhs._left;
			_width = rhs._width;
			return *this;
		}
	public:
		typedef typename storageT::iterator iterator;
		typedef typename storageT::const_iterator const_iterator;
	public:
		iterator begin() {
			return _storage.begin() + _left;
		}
		const_iterator begin() const {
			return _storage.begin() + _left;
		}
		iterator end() {
			return _storage.begin() + (_left + _width);
		}
		const_iterator end() const {
			return _storage.begin() + (_left + _width);
		}
	protected:
		storageT& _storage;
		int _left, _width;
	};
	template<
		typename storageT
	> struct aligned_storage<rect_region<storageT, unaligned_policy> > {
		typedef rect_region<storageT, aligned_policy> type;
	};

	//- rect_region<storageT, alignmentP> --------------------------------------
	/** @template rect_region
	 * @abstract
	 *   Rect region for contiguous
	 */
	template<
		typename storageT,
		typename alignmentP
	> class rect_region<
		storageT,
		alignmentP,
		typename ct::enable_if<
			uses_illife_storage<storageT>::value
		>::type
	> : public illife_storage, public rectangular {
	public:
		/// Typelist containing types for components
		typedef typename storageT::component_tl component_tl;
		/// Value type
		typedef rect_region<typename storageT::value_type, alignmentP> value_type;
		/// Number of prefetch channels needed to load all channels
		static const int prefetch_channel_count = value_type::prefetch_channel_count;
	public:
		rect_region(storageT& storage, int x, int y, int width, int height)
		: _storage(storage) {
			_data.reserve(height);
			for(int i = 0; i < height; ++i) {
				_data.push_back(value_type(_storage[y+i], x, width));
			}
		}
	public:
	//= Illife Storage Interface ===============================================
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
		storageT& _storage;
		std::vector<value_type> _data;
	};
	
#if 0
	//--------------------------------------------------------------------------
	// The following overrides are required for Visual C++ .NET 2003
	// It does not seem to detect the appropriate storage type of rect region,
	// even though it detects image correctly.
	template<
		typename storageT,
		template<typename> class storageP
	> struct uses_illife_storage<rect_region<storageT, storageP> > {
		static const bool value = uses_illife_storage<
			storageP<typename storageT::component_tl> >::value;
	};
	template<
		typename storageT,
		template<typename> class storageP
	> struct uses_contiguous_storage<rect_region<storageT, storageP> > {
		static const bool value = uses_contiguous_storage<
			storageP<typename storageT::component_tl> >::value;
	};
	//--------------------------------------------------------------------------
	// These metafunctions are duplicated in priv because if the class uses the
	// vvis versions, the user will be unable to use the vvis version on it
	namespace priv {
		template<
			typename storageT,
			template<typename> class storageP
		> struct uses_illife_storage<rect_region<storageT, storageP> > {
			static const bool value = uses_illife_storage<
				storageP<typename storageT::component_tl> >::value;
		};
		template<
			typename storageT,
			template<typename> class storageP
		> struct uses_contiguous_storage<rect_region<storageT, storageP> > {
			static const bool value = uses_contiguous_storage<
				storageP<typename storageT::component_tl> >::value;
		};
	} // End of priv namespace
#endif
} // End of vvis namespace

#endif
