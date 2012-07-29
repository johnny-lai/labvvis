/*
 *  accessor.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat May 31 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

/*! @header vvis/accessor.h
 * @abstract
 *   Include this file for VVIS accessors.
 */
#ifndef _VVIS_ACCESSOR_H
#define _VVIS_ACCESSOR_H

#include <vvm/vvm.h>
#include <vvis/vvm_tuple_support.h>
#include <vvis/metafunction.h>
#include <vvis/storage.h>
#include <vvis/iterator.h>
#include <vvis/image_base.h>

namespace vvis {
	/*! @struct no_prefetch
	 * @abstract
	 *   Policy class to tell accessors not to prefetch.
	 */
	struct no_prefetch {
		/*! @var prefetch_channel_count
		 * @abstract
		 *   Number of prefetch channels used per prefetch
		 */
		static const int prefetch_channel_count = 0;
	};

	/*! @template prefetch
	 * @abstract
	 *   Policy class to tell accessors to prefetch.
	 * @templatefield usageT Prefetch usage. Defaults to VVIS_DEFAULT_PREFETCH_USAGE.
	 * @templatefield prefetchCount Approximate number of bytes to prefetch. Defaults
	 *                      to VVIS_DEFAULT_PREFETCH_COUNT
	 */
	template<
		typename usageT = VVIS_DEFAULT_PREFETCH_USAGE,
		int prefetchCount = VVIS_DEFAULT_PREFETCH_COUNT
	> struct prefetch {
		/// Number of prefetch channels used per prefetch
		static const int prefetch_channel_count = 1;
	};

	namespace priv {
		// component_accessor_prefetch_helper
		// Eliminates one level of indirection to help non-prefetched code
		// run faster
		template<
			typename storageT,
			int component,
			typename prefetchP
		> struct component_accessor_prefetch_helper;

		template<
			typename storageT,
			int i
		> struct component_accessor_prefetch_helper<storageT, i, no_prefetch> {
			template<int channel, typename iteratorT>
			void prefetch_read(const iteratorT& p) const {
			}
			template<int channel, typename iteratorT>
			void prefetch_write(const iteratorT& p) const {
			}
		};
		
		template<
			typename storageT,
			int i,
			typename usageT,
			int prefetchCount
		> struct component_accessor_prefetch_helper<storageT, i, prefetch<usageT, prefetchCount> > {
			/// Prefetch a single channel for read-only
			template<int channel>
			inline void prefetch_read(const typename storageT::const_iterator& p) const {
				p.template prefetch_component<i, channel, vvm::read_only, prefetchCount>();
			}
			/// Prefetch a single channel for read/write
			template<int channel>
			inline void prefetch_write(const typename storageT::iterator& p) const {
				p.template prefetch_component<i, channel, vvm::read_write, prefetchCount>();
			}
		};
	} // End of priv namespace

	/*! @template component_accessor
	 * @abstract
	 *   Accessor that provides access to a single component in an image.
	 * @templatefield i Component in storage that will be accessed
	 * @templatefield prefetchP Prefetch Policy. Use either vvis::no_prefetch or vvis::prefetch.
	 */
	template<
		typename storageT,
		int i,
		typename prefetchP = VVIS_DEFAULT_PREFETCH_POLICY,
		typename specializedT = void
	> struct component_accessor
	: public priv::component_accessor_prefetch_helper<
		typename priv::effective_storage<storageT>::type, i, prefetchP> {
	private:
		typedef typename priv::effective_storage<storageT>::type effective_storage;
		typedef typename effective_storage::iterator iterator;
		typedef typename effective_storage::const_iterator const_iterator;
	public:
		/// Type returned by get_scalar
		typedef typename ct::type_at<
			typename effective_storage::component_tl, i>::type scalar_type;
		/// Type returned by get_vector
		typedef typename vvm::add_vector<scalar_type>::type vector_type;

		/// Number of prefetch channels used
		static const int prefetch_channel_count = prefetchP::prefetch_channel_count;
		
		/** Writes scalar \c v at iterator \c p.
		 * \param v Scalar that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		inline void set_scalar(const scalar_type& s, iterator& p) const {
			(*p.vector).template get<i>().scalar(p.scalar) = s;
		}
		/** Returns scalar from iterator \c p.
		 * \param p Iterator to read from.
		 */
		inline const scalar_type get_scalar(const const_iterator& p) const {
			return (*p.vector).template get<i>().scalar(p.scalar);
		}
		
		/** Writes vvm::vector \c v at iterator \c p.
		 * \param v Vvm::vector that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		inline void set_vector(const vector_type& v, iterator& p) const {
			(*p.vector).template get<i>() = v;
		}
		/** Writes vvm::vector \c v at iterator \c p with vvm::vector_cast.
		 * \param v Vvm::vector that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		template<typename valueT>
		inline void set_vector(const valueT& v, iterator& p) const {
			(*p.vector).template get<i>() = vvm::vector_cast<vector_type>(v);
		}
		/** Returns vvm::vector from iterator \c p.
		 * \param p Iterator to read from.
		 */
		inline const vector_type
		get_vector(const const_iterator& p) const {
			return (*p.vector).template get<i>();
		}
		/** Returns vvm::vector from iterator \c p.
		 * \param p Iterator to read from.
		 * \param shift Number of vvm::vectors ahead
		 */
		inline const vector_type
		get_vector(const const_iterator& p, const vvm::offset_t shift) const {
			return (*(p.vector + shift)).template get<i>();
		}

		/** Returns vvm::vector from two other vvm::vectors
		 * This is used to perform unaligned loading.
		 * \param a First vvm::vector.
		 * \param b Second vvm::vector.
		 * \param shift Number of scalars offset from the left of \c a.
		 */
		inline const vector_type
		get_vector(const vector_type& a, const vector_type& b,
		const vvm::offset_t shift = 0) const {
			return vvm::load(a, b, shift);
		}
	};
	/** \class component_accessor "accessor.h" <vvis/accessor.h>
	 * Accessor that provides access to a single channel in an image.
	 * This version only applies to unknown storages.
	 * \param storageT Storage Type that accessor is accesssing
	 * \param prefetchP Prefetch Policy. Use either vvis::no_prefetch or vvis::prefetch.
	 */
	template<
		typename storageT,
		int i,
		typename prefetchP
	> struct component_accessor<
		storageT, i, prefetchP,
		typename ct::enable_if<
			uses_unknown_storage<
				typename priv::effective_storage<storageT>::type
			>::value
		>::type
	> {
	public:
		typedef typename priv::effective_storage<storageT>::type effective_storage;
		typedef typename effective_storage::iterator iterator;
		typedef typename effective_storage::const_iterator const_iterator;
	public:
		/// Type returned by get_scalar
		typedef typename ct::type_at<
			typename effective_storage::component_tl,
			i>::type scalar_type;

		/// Number of prefetch channels used
		static const int prefetch_channel_count =
			prefetchP::prefetch_channel_count * storageT::prefetch_channel_count;

		/** Writes scalar \c v at iterator \c p.
		 * \param v Scalar that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		void set_scalar(const scalar_type& s, iterator p) const {
			(*p).template get<i>() = s;
		}
		/** Returns scalar from iterator \c p.
		 * \param p Iterator to read from.
		 */
		const scalar_type get_scalar(const_iterator p) const {
			return (*p).template get<i>();
		}
	};
	
	
	//- pixel_accessor ---------------------------------------------------------
	namespace priv {
		/*
		// In general, the effective storage type is the storage type
		template<typename storageT, typename specializedT = void>
		struct effective_storage {
			typedef storageT type;
		};
		// For Illife, effective type is the value_type
		template<typename storageT>
		struct effective_storage<
			storageT,
			typename ct::enable_if<
				uses_illife_storage<storageT>::value
			>::type
		> {
			typedef typename effective_storage<
				typename storageT::value_type>::type type;
		};
		*/

		// pixel_accessor_prefetch_helper
		// Eliminates one level of indirection to help non-prefetched code
		// run faster
		template<
			typename storageT,
			typename prefetchP
		> struct pixel_accessor_prefetch_helper;

		template<
			typename storageT
		> struct pixel_accessor_prefetch_helper<storageT, no_prefetch> {
			template<int channel>
			inline void prefetch_read(const typename storageT::const_iterator& p) const {
			}
			template<int channel>
			inline void prefetch_write(const typename storageT::iterator& p) const {
			}
		};

		template<
			typename storageT,
			typename usageT,
			int prefetchCount
		> struct pixel_accessor_prefetch_helper<storageT, prefetch<usageT, prefetchCount> > {
			/// Prefetch all channels
			template<int channel>
			inline void prefetch_read(const typename storageT::const_iterator& p) const {
				p.template prefetch_all<channel, vvm::read_only, prefetchCount>();
			}
			/// Prefetch all channels
			template<int channel>
			inline void prefetch_write(const typename storageT::iterator& p) const {
				p.template prefetch_all<channel, vvm::read_write, prefetchCount>();
			}
		};

	} // End of priv namespace

	/** \class pixel_accessor "accessor.h" <vvis/accessor.h>
	 * Accessor that provides access to pixels in an image.
	 * This is the general version.
	 * \param storageT Storage Type that accessor is accesssing
	 * \param prefetchP Prefetch Policy. Use either vvis::no_prefetch or vvis::prefetch.
	 * \param specializedT For class enabling.
	 */
	template<
		typename storageT,
		typename prefetchP = VVIS_DEFAULT_PREFETCH_POLICY,
		typename specializedT = void
	> struct pixel_accessor
	: public priv::pixel_accessor_prefetch_helper<
		typename priv::effective_storage<storageT>::type,
		prefetchP
	> {
	protected:
		typedef typename priv::effective_storage<storageT>::type effective_storage;
		typedef typename effective_storage::iterator iterator;
		typedef typename effective_storage::const_iterator const_iterator;
	public:
		/// Type returned by get_scalar
		typedef typename effective_storage::value_type scalar_type;
		/// Type returned by get_vector
		typedef typename vvm::add_vector<typename effective_storage::value_type
			>::type vector_type;

		/// Number of prefetch channels used
		static const int prefetch_channel_count =
			prefetchP::prefetch_channel_count * storageT::prefetch_channel_count;

		/** Writes scalar \c v at iterator \c p.
		 * \param v Scalar that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		inline void set_scalar(const typename iterator::scalar_type& s, iterator& p) const {
			p.set_scalar(s);
		}
		/** Returns scalar from iterator \c p.
		 * \param p Iterator to read from.
		 */
		inline const typename const_iterator::scalar_type get_scalar(const const_iterator& p) const {
			return p.get_scalar();
		}

		/** Writes vvm::vector \c v at iterator \c p.
		 * \param v Vvm::vector that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		inline void set_vector(const typename iterator::vector_type& v, iterator& p) const {
			p.set_vector(v);
		}
		/** Writes vvm::vector \c v at iterator \c p with vvm::vector_cast.
		 * \param v Vvm::vector that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		template<typename valueT>
		inline void set_vector(const valueT& v, iterator& p) const {
			p.set_vector(vvm::vector_cast<typename iterator::vector_type>(v));
		}
		/** Returns vvm::vector from iterator \c p.
		 * \param p Iterator to read from.
		 */
		inline const typename const_iterator::vector_type
		get_vector(const const_iterator& p) const {
			return p.get_vector();
		}
		/** Returns vvm::vector from iterator \c p.
		 * \param p Iterator to read from.
		 * \param shift Number of vvm::vectors ahead
		 */
		inline const typename const_iterator::vector_type
		get_vector(const const_iterator& p, const vvm::offset_t shift) const {
			return p.get_vector(shift);
		}

		/** Returns vvm::vector from two other vvm::vectors
		 * This is used to perform unaligned loading.
		 * \param a First vvm::vector.
		 * \param b Second vvm::vector.
		 * \param shift Number of scalars offset from the left of \c a.
		 */
		inline const typename const_iterator::vector_type
		get_vector(const typename const_iterator::vector_type& a,
		const typename const_iterator::vector_type& b, const vvm::offset_t shift = 0) const {
			return vvm::load(a, b, shift);
		}
	};

	/** \class pixel_accessor "accessor.h" <vvis/accessor.h>
	 * Accessor that provides access to pixels in an image.
	 * This version only applies to unknown storages.
	 * \param storageT Storage Type that accessor is accesssing
	 * \param prefetchP Prefetch Policy. Use either vvis::no_prefetch or vvis::prefetch.
	 */
	template<
		typename storageT,
		typename prefetchP
	> struct pixel_accessor<
		storageT, prefetchP,
		typename ct::enable_if<
			uses_unknown_storage<
				typename priv::effective_storage<storageT>::type
			>::value
		>::type
	> {
	public:
		typedef typename priv::effective_storage<storageT>::type effective_storage;
		typedef typename effective_storage::iterator iterator;
		typedef typename effective_storage::const_iterator const_iterator;
	public:
		/// Type returned by get_scalar
		typedef typename effective_storage::value_type scalar_type;

		/** Writes scalar \c v at iterator \c p.
		 * \param v Scalar that is to be written.
		 * \param p Iterator to write \c v to.
		 */
		void set_scalar(const scalar_type& s, iterator p) const {
			*p = s;
		}
		/** Returns scalar from iterator \c p.
		 * \param p Iterator to read from.
		 */
		const scalar_type get_scalar(const_iterator p) const {
			return *p;
		}
	};
	
	
	//= Pixel Accessor for contiguous storages =================================
	
	/** Pixel Accessor
	 * Accessor for contiguous storages that have more than one channel
	 */
	template<
		typename storageT,
		typename prefetchP
	> struct pixel_accessor<
		storageT, prefetchP,
		typename ct::enable_if<
			uses_contiguous_storage<
				typename priv::effective_storage<storageT>::type
			>::value == true &&
			ct::length<
				typename priv::effective_storage<storageT>::type::component_tl
			>::value >= 2
		>::type
	> : public priv::pixel_accessor_prefetch_helper<
		typename priv::effective_storage<storageT>::type,
		prefetchP
	> {
	private:
		typedef typename priv::effective_storage<storageT>::type effective_storage;
		typedef typename effective_storage::iterator iterator;
		typedef typename effective_storage::const_iterator const_iterator;
		struct do_store_scalar {
			template<int i> struct Code {
				static void exec(const typename iterator::scalar_type& s, iterator& p) {
					(*p.vector).template get<i>().scalar(p.scalar) = s.template get<i>();
				}
			};
		};
		struct do_load_scalar {
			template<int i> struct Code {
				static void exec(const const_iterator& p, typename const_iterator::scalar_type& ret) {
					ret.template get<i>() = (*p.vector).template get<i>().scalar(p.scalar);
				}
			};
		};
	public:
		typedef typename effective_storage::value_type scalar_type;
		typedef typename vvm::add_vector<typename effective_storage::value_type
			>::type vector_type;

		/// Number of prefetch channels used
		static const int prefetch_channel_count =
			prefetchP::prefetch_channel_count * storageT::prefetch_channel_count;

		/// Sets a scalar s at position p
		inline void set_scalar(const typename iterator::scalar_type& s, iterator& p) const {
			meta::EFOR2<0, meta::Less, ct::length<typename effective_storage::component_tl>::value, +1,
				do_store_scalar>::exec(s, p);
		}
		/// Returns a scalar from position p
		inline const typename const_iterator::scalar_type get_scalar(const const_iterator& p) const {
			typename const_iterator::scalar_type ret;
			meta::EFOR2<0, meta::Less, ct::length<typename effective_storage::component_tl>::value, +1,
				do_load_scalar>::exec(p, ret);
			return ret;
		}

		/// Sets a vector at position p
		inline void set_vector(const typename iterator::vector_type& v, iterator& p) const {
			p.set_vector(v);
		}
		/// Sets a vector at position p
		template<typename valueT>
		inline void set_vector(const valueT& v, iterator& p) const {
			p.set_vector(vvm::vector_cast<typename iterator::vector_type>(v));
		}

		/// Returns a vector from position p
		inline const typename const_iterator::vector_type
		get_vector(const const_iterator& p) const {
			return p.get_vector();
		}
		inline const typename const_iterator::vector_type
		get_vector(const const_iterator& p, const vvm::offset_t shift) const {
			return p.get_vector(shift);
		}

		/// Returns a vector from position p
		template<typename TL>
		inline const ct::tuple<TL>
		get_vector(const ct::tuple<TL>& a, const ct::tuple<TL>& b, const vvm::offset_t shift = 0) const {
			ct::tuple<TL> ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::vvm_load_shift_element>::exec(a, b, ret, shift);
			return ret;
		}
	};


	//
	template<typename accessorT> class xy_accessor : public accessorT {
	public:
		typedef image_base::coordinate_type coordinate_type;
		struct scalar_type {
			scalar_type() {
			}
			scalar_type(const typename accessorT::scalar_type s,
			const coordinate_type ax, const coordinate_type ay)
			: value(s), x(ax), y(ay) {
			}
			typename accessorT::scalar_type value;
			const coordinate_type x;
			const coordinate_type y;
		};
		struct vector_type {
			vector_type() {
			}
			vector_type(const typename accessorT::vector_type v,
			const coordinate_type ax, const coordinate_type ay)
			: value(v), x(ax), y(ay) {
				x += vvm::vector_cast<vvm::vector<coordinate_type> >(
					vvm::lvsl(reinterpret_cast<typename accessorT::vector_type *>(0), 0));
			}
			typename accessorT::vector_type value;
			vvm::vector<coordinate_type> x;
			vvm::vector<coordinate_type> y;

		};
	public:
		/** Writes scalar \c v at iterator \c p.
		 * \param v Scalar that is to be written.
		 * \param p Iterator to write \c v to (Ignored).
		 */
		void set_scalar(const scalar_type& s, typename accessorT::iterator p) const {
			// s.xy ignored
			accessorT::set_scalar(s.value, p);
		}
		/** Returns scalar from iterator \c p.
		 * \param p Iterator to read from.
		 */
		const scalar_type get_scalar(typename accessorT::const_iterator p) const {
			return scalar_type(accessorT::get_scalar(p), p.x(), p.y());
		}
		
		/// Sets a vector at position p
		inline void set_vector(const vector_type& v, typename accessorT::iterator& p) const {
			// v.xy ignored
			p.set_vector(v.value);
		}
		/// Sets a vector at position p
		template<typename valueT>
		inline void set_vector(const valueT& v, typename accessorT::iterator& p) const {
			// v.xy ignored
			p.set_vector(
				vvm::vector_cast<typename accessorT::vector_type>(v));
		}

		/// Returns a vector from position p
		inline const vector_type
		get_vector(typename accessorT::const_iterator& p) const {
			return vector_type(p.get_vector(), p.x(), p.y());
		}
		inline const vector_type
		get_vector(typename accessorT::const_iterator& p, const vvm::offset_t shift) const {
			return vector_type(p.get_vector(shift), p.xy(shift));
		}
	};
	
	
	template<typename storageT>
	class acc {
    public:
		typedef typename priv::effective_storage<storageT>::type storage;
		typedef typename storage::iterator iterator;
	public:
		/// Type returned by get_scalar
		typedef typename storage::value_type scalar_type;
		/// Type returned by get_vector
		typedef typename vvm::add_vector<typename storage::value_type
			>::type vector_type;

    public:
		void set_scalar(const scalar_type v, storage& s, const iterator p) const {
			s[p] = v;
		}
		void get_scalar(const storage& s, const iterator p) const {
			return s[p];
		}
		
		void set_vector(const vector_type v, storage& s, const iterator p) const {
			s.vector(p) = v;
		}
		void get_vector(const vector_type v, storage& s, const iterator p) const {
			return s.vector(p);
		}
	};
} // End of vvis namespace

#endif

