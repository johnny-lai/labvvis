/*
 *  iterator.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Mon Dec 15 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_ITERATOR_H
#define _VVIS_ITERATOR_H

#include <ct/tuple.h>
#include <vvm/vvm.h>
#include <vvis/vvm_tuple_support.h>
#include <vvis/metafunction.h>

namespace vvis {
	//= planar_iterator ========================================================
	/** \class planar_iterator "iterator.h" <vvis/iterator.h>
	 * General iterator for contiguous planar storages.
	 * Always uses a tuple to keep the pointers to memory.
	 * Use single_contiguous_iterator if there is only one channel instead.
	 * \param TL Typelist of channels in image. This is equivalent to
	 *           \c component_tl in storages.
	 */
	template<typename TL> class planar_iterator {
	private:
		enum { channel_count = ct::length<TL>::value };
		typedef typename ct::apply<TL, boost::add_pointer>::type ptr_tl;
		typedef typename ct::apply<TL, vvm::add_vector>::type vector_tl;
		typedef typename ct::apply<vector_tl, boost::add_pointer>::type vector_ptr_tl;
		
		template<int channel, typename usageT, int prefetchCount>
		struct do_prefetch_component {
			template<int i> struct Code {
				static void exec(const planar_iterator& itr) {
					vvm::prefetch<channel+i, usageT>(
						reinterpret_cast<typename ct::type_at<vector_ptr_tl, i>::type>(itr.vector.template get<i>()), prefetchCount);
				}
			};
		};
	public:
		/// The resultant type of A - B where A and B are planar_iterators
		typedef int difference_type;
		typedef typename ct::tuple<vector_tl> vector_type;
		typedef typename ct::tuple<TL> scalar_type;
	public:
		planar_iterator() {
		}
		planar_iterator(const ct::tuple<vector_ptr_tl>& v, int s)
		: scalar(s), vector(v) {
		}
	public:
		/// Move forward by a pixel step
		planar_iterator& operator++() {
			++scalar;
			normalize();
			return *this;
		}
		/// Move forward by a pixel step
		planar_iterator& operator++(int) {
			planar_iterator ret(*this);
			++scalar;
			normalize();
			return ret;
		}
		/// Move forward by step \c pixel steps
		planar_iterator& operator+=(const int step) {
			scalar += step;
			normalize();
			return *this;
		}
		/// Move forward by \c step pixel steps
		planar_iterator operator+(const int step) {
			planar_iterator ret(*this);
			ret.scalar += step;
			ret.normalize();
			return ret;
		}
		/// Move backward by a pixel step
		planar_iterator& operator--() {
			--scalar;
			normalize();
			return *this;
		}
		/// Move backward by a pixel step
		planar_iterator& operator--(int) {
			planar_iterator ret(*this);
			--scalar;
			normalize();
			return ret;
		}
		/// Move backward by \c step pixel steps
		planar_iterator& operator-=(const int step) {
			scalar -= step;
			normalize();
			return *this;
		}
		/// Move backward by \c step pixel steps
		planar_iterator operator-(const int step) {
			planar_iterator ret(*this);
			ret.scalar -= step;
			ret.normalize();
			return ret;
		}
		/// Return number of pixel steps between \c this and \c rhs
		difference_type operator-(const planar_iterator& rhs) const {
			return (vector.template get<0>() - rhs.vector.template get<0>()) * VVM_SCALAR_COUNT +
			   	(scalar - rhs.scalar);
		}
	public:
		/// Check if both .vector and .scalar components are not equal
		bool operator!=(const planar_iterator& rhs) const {
			return vector != rhs.vector || scalar != rhs.scalar;
		}
	public:
		/** Prefetch all channels. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c pixel_accessor.
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int channel, typename usageT, int prefetchCount>
		void prefetch_all() const {
			meta::EFOR1<0, meta::Less, channel_count, +1,
				do_prefetch_component<channel, usageT, prefetchCount> >::exec(*this);
		}
		/** Prefetch a single channel. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c component_accessor.
		 * \param component Refers to channel in storage that we want to prefetch
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int component, int channel, typename usageT, int prefetchCount>
		void prefetch_component() const {
			vvm::prefetch<channel, usageT>(vector.template get<component>(), prefetchCount);
		}
	public:
		const scalar_type get_scalar() const {
			return (*vector).scalar(scalar);
		}
		void set_scalar(const scalar_type s) {
			(*vector).scalar(scalar) = s;
		}
		const vector_type get_vector() const {
			return *vector;
		}
		const vector_type get_vector(const vvm::offset_t shift) const {
			return *(vector + shift);
		}
		void set_vector(const vector_type& v) {
			*vector = v;
		}
	public:
		/// Scalar component
		int scalar;
		/// Vvm::vector component
		ct::tuple<vector_ptr_tl> vector;
	private:	
		// Keep 0 <= scalar < VVM_SCALAR_COUNT
		void normalize() {
			if(scalar >= VVM_SCALAR_COUNT) {
				vector += scalar / VVM_SCALAR_COUNT;
				scalar %= VVM_SCALAR_COUNT;
			} else if(scalar < 0) {
				vector -= (-scalar / VVM_SCALAR_COUNT) + 1;
				scalar = VVM_SCALAR_COUNT - (-scalar % VVM_SCALAR_COUNT);
			}
		}
	};
	
	//= chunky_iterator ========================================================
	/** \class chunky_iterator "iterator.h" <vvis/iterator.h>
	 * General iterator for contiguous chunky storages.
	 * Use single_contiguous_iterator if there is only one channel instead.
	 * \param TL Typelist of channels in image. This is equivalent to
	 *           \c component_tl in storages.
	 */
	template<typename TL> class chunky_iterator {
	private:
		typedef chunky_iterator<TL> self_type;
		// get aligned uses this
		enum { channel_count = ct::length<TL>::value };
		typedef typename ct::apply<TL, vvm::add_vector>::type vector_tl;
		typedef ct::tuple<vector_tl> chunk;
		struct move_to {
			template<int i> struct Code {
				static void exec(chunky_iterator& itr, const int new_ci, const int new_si) {
					// Move to new location
					itr.scalar.template get<i>() = reinterpret_cast<typename ct::type_at<TL, i>::type*>(
						reinterpret_cast<char*>(itr._begin.template get<i>()) + 
						new_ci * sizeof(chunk) +
						new_si * sizeof(typename ct::type_at<TL, i>::type));
				}
			};
		};
		typedef typename ct::apply<TL, boost::add_pointer>::type ptr_tl;
		typedef typename ct::apply<TL, boost::add_reference>::type ref_tl;
		typedef ct::tuple<ref_tl> scalar_ref_type;
		typedef ct::tuple<ptr_tl> scalar_ptr_type;
	public:
		/// The resultant type of A - B where A and B are chunky_iterators
		typedef int difference_type;
		typedef ct::tuple<typename ct::apply<TL, vvm::add_vector>::type> vector_type;
		typedef ct::tuple<TL> scalar_type;
	public:
		chunky_iterator() {
		}
		chunky_iterator(chunk* v, const int s)
		: scalar(s), vector(v) {
		};
	public:
		/// Move forward by a pixel step
		chunky_iterator& operator++() {
			++scalar;
			normalize();
			return *this;
		}
		/// Move forward by a pixel step
		chunky_iterator& operator++(int) {
			chunky_iterator ret(*this);
			++scalar;
			normalize();
			return ret;
		}
		/// Move forward by \c step pixel steps
		chunky_iterator& operator+=(const int step) {
			scalar += step;
			normalize();
			return *this;
		}
		/// Move forward by \c step pixel steps
		chunky_iterator operator+(const int step) {
			chunky_iterator ret(*this);
			ret.scalar += step;
			ret.normalize();
			return ret;
		}
		/// Move backward by a pixel step
		chunky_iterator& operator--() {
			--scalar;
			return *this;
		}
		/// Move backward by a pixel step
		chunky_iterator& operator--(int) {
			chunky_iterator ret(*this);
			--scalar;
			normalize();
			return ret;
		}
		/// Move backward by \c step pixel steps
		chunky_iterator& operator-=(const int step) {
			scalar -= step;
			normalize();
			return *this;
		}
		/// Move backward by \c step pixel steps
		chunky_iterator operator-(const int step) {
			chunky_iterator ret(*this);
			ret.scalar -= step;
			ret.normalize();
			return ret;
		}
		/// Return number of pixel steps between \c this and \c rhs
		difference_type operator-(const chunky_iterator& rhs) {
			return (vector - rhs.vector) * VVM_SCALAR_COUNT +
				(scalar - rhs.scalar);
		}
	public:
		/// Check if both .vector and .scalar components are not equal
		bool operator!=(const chunky_iterator& rhs) const {
			return vector != rhs.vector || scalar != rhs.scalar;
		}
	public:
		/** Prefetch all channels. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c pixel_accessor.
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int channel, typename usageT, int prefetchCount>
		void prefetch_all() const {
			vvm::prefetch<channel, usageT>(vector, prefetchCount);
		}
		/** Prefetch a single channel. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c component_accessor.
		 * \param component Refers to channel in storage that we want to prefetch
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int component, int channel, typename usageT, int prefetchCount>
		void prefetch_component() const {
			vvm::prefetch<channel, usageT>(vector, prefetchCount);
		}
	public:
		const scalar_type get_scalar() const {
			return (*vector).scalar(scalar);
		}
		void set_scalar(const scalar_type s) {
			(*vector).scalar(scalar) = s;
		}
		const vector_type get_vector() const {
			return *vector;
		}
		const vector_type get_vector(const vvm::offset_t shift) const {
			return *(vector + shift);
		}
		void set_vector(const vector_type& v) {
			*vector = v;
		}
	public:
		/// Scalar component
		int scalar;
		/// Vvm::vector component
		chunk* vector;
	private:
		// Keep 0 <= scalar < VVM_SCALAR_COUNT
		void normalize() {
			if(scalar >= VVM_SCALAR_COUNT) {
				vector += scalar / VVM_SCALAR_COUNT;
				scalar %= VVM_SCALAR_COUNT;
			} else if(scalar < 0) {
				vector -= (-scalar / VVM_SCALAR_COUNT) + 1;
				scalar = VVM_SCALAR_COUNT - (-scalar % VVM_SCALAR_COUNT);
			}
		}
	};
	
	/** \class single_contiguous_iterator "iterator.h" <vvis/iterator.h>
	 * Iterator for contiguous storages with a single channel.
	 * When there is a single channel, chunky and planar storages are
	 * equivalent. Therefore, when there is a single channel, they can use
	 * the same iterator.
	 * \param TL Typelist of channels in image. This is equivalent to
	 *           \c component_tl in storages.
	 */
	template<typename T>
	class single_contiguous_iterator {
	public:
		typedef vvm::vector<T> vector_type;
		typedef T scalar_type;
		/// The resultant type of A - B where A and B are planar_iterators
		typedef int difference_type;
	public:	
		single_contiguous_iterator() : scalar(0) {
		}
		single_contiguous_iterator(vector_type* begin, int s, const int ox, const int oy)
		: scalar(s), vector(begin), _begin(begin), _ox(ox), _oy(oy) {
		}
	public:
		/// Move forward by a pixel step
		single_contiguous_iterator& operator++() {
			++scalar;
			normalize();
			return *this;
		}
		/// Move forward by a pixel step
		single_contiguous_iterator& operator++(int) {
			single_contiguous_iterator ret(*this);
			++scalar;
			normalize();
			return ret;
		}
		/// Move forward by \c step pixel steps
		single_contiguous_iterator& operator+=(const int step) {
			scalar += step;
			normalize();
			return *this;
		}
		/// Move forward by \c step pixel steps
		single_contiguous_iterator operator+(const int step) {
			single_contiguous_iterator ret(*this);
			ret.scalar += step;
			ret.normalize();
			return ret;
		}
		/// Move backward by a pixel step
		single_contiguous_iterator& operator--() {
			--scalar;
			normalize();
			return *this;
		}
		/// Move backward by a pixel step
		single_contiguous_iterator& operator--(int) {
			single_contiguous_iterator ret(*this);
			--scalar;
			normalize();
			return ret;
		}
		/// Move backward by \c step pixel steps
		single_contiguous_iterator& operator-=(const int step) {
			scalar -= step;
			normalize();
			return *this;
		}
		/// Move backward by \c step pixel steps
		single_contiguous_iterator operator-(const int step) {
			single_contiguous_iterator ret(*this);
			ret.scalar -= step;
			ret.normalize();
			return ret;
		}
		/// Return number of pixel steps between \c this and \c rhs
		difference_type operator-(const single_contiguous_iterator& rhs) const {
			return ((vector - rhs.vector) * VVM_SCALAR_COUNT) +
				(scalar - rhs.scalar);
		}
	public:
		/// Check if both .vector and .scalar components are not equal
		bool operator!=(const single_contiguous_iterator& rhs) const {
			return vector != rhs.vector || scalar != rhs.scalar;
		}
	public:
		/** Prefetch all channels. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c pixel_accessor.
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int channel, typename usageT, int prefetchCount>
		void prefetch_all() const {
			vvm::prefetch<channel, usageT>(
					reinterpret_cast<scalar_type*>(vector), prefetchCount);
		}
		/** Prefetch a single channel. Called by an accessor.
		 * This function prefetch a single channel from the current iterator
		 * location. This function is part of the interface required by
		 * \c component_accessor.
		 * \param component Refers to channel in storage that we want to prefetch
		 * \param channel Refers to the prefetch channel
		 * \param usageT Refers to how the data is expected to be used.
		 *               See VVM for more information.
		 * \param prefetchCount Approximate number of byes to prefetch
		 */
		template<int component, int channel, typename usageT, int prefetchCount>
		void prefetch_component() const {
			vvm::prefetch<channel, usageT>(
					reinterpret_cast<scalar_type*>(vector), prefetchCount);
		}
	public:
		const scalar_type get_scalar() const {
			return (*vector).scalar(scalar);
		}
		void set_scalar(const scalar_type s) {
			(*vector).scalar(scalar) = s;
		}
		const vector_type get_vector() const {
			return *vector;
		}
		const vector_type get_vector(const vvm::offset_t shift) const {
			return vector[shift];
		}
		void set_vector(const vector_type& v) {
			*vector = v;
		}
	public:
		image_base::coordinate_type x() const {
			return _ox + ((vector - _begin) * VVM_SCALAR_COUNT) + scalar;
		}
		image_base::coordinate_type y() const {
			return _oy;
		}
	public:
		int scalar;
		vector_type* vector;
	private:
		vector_type* _begin;
		image_base::coordinate_type _ox, _oy;
	private:
		// Keep 0 <= scalar < VVM_SCALAR_COUNT
		void normalize() {
			if(scalar >= VVM_SCALAR_COUNT) {
				vector += scalar / VVM_SCALAR_COUNT;
				scalar %= VVM_SCALAR_COUNT;
			} else if(scalar < 0) {
				vector -= (-scalar / VVM_SCALAR_COUNT) + 1;
				scalar = VVM_SCALAR_COUNT - (-scalar % VVM_SCALAR_COUNT);
			}
		}
	};
} // End of vvis namespace

#endif
