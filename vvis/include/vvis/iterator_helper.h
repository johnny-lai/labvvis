/*
 *  iterator_helper.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat Apr 3 2004.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_ITERATOR_HELPER_H
#define _VVIS_ITERATOR_HELPER_H

#include <vvis/image_base.h>

namespace vvis {
	namespace priv {
		template<typename T> class pointer_iterator {
		public:
			pointer_iterator(T* begin, int ox, int oy)
			: _begin(begin), _x(0), _ox(ox), _oy(oy) {
			}
			T& operator*() {
				return _begin[_x];
			}
			const T& operator*() const {
				return _begin[_x];
			}
			void operator+(const int x) {
				_x += x; 
			}
			const bool operator!=(const pointer_iterator<T>& rhs) {
				return _begin + _x != rhs._begin + rhs._x;
			}
			pointer_iterator<T>& operator++() {
				++_x;
				return *this;
			}
			image_base::coordinate_type x() const {
				return _ox + _x;
			}
			image_base::coordinate_type y() const {
				return _oy;
			}
		private:
			T* _begin;
			image_base::coordinate_type _x, _ox, _oy;
		};

		//= planar_iterator_helper =============================================
		template<typename storageT, typename TL, typename specializedT = void>
		class planar_iterator_helper {
		private:
			typedef typename ct::apply<TL, boost::add_pointer>::type ptr_tl;
			typedef typename ct::apply<TL, vvm::add_vector>::type vector_tl;
			typedef typename ct::apply<vector_tl, boost::add_pointer>::type vector_ptr_tl;
			struct do_convert {
				template<int i> struct Code {
					static void exec(ct::tuple<vector_ptr_tl>& ret, const ct::tuple<ptr_tl>& a) {
						ret.template get<i>() = reinterpret_cast<
							typename ct::type_at<vector_ptr_tl, i>::type>(a.template get<i>());
					}
					static void exec(ct::tuple<vector_ptr_tl>& ret, const ct::tuple<ptr_tl>& a, const int& offset) {
						ret.template get<i>() = reinterpret_cast<
							typename ct::type_at<vector_ptr_tl, i>::type>(a.template get<i>() + offset);
					}
				};
			};
		public:
			static const int channel_count = ct::length<TL>::value;
			typedef planar_iterator<TL> iterator;
			typedef planar_iterator<TL> const_iterator;
			
			inline static iterator begin(storageT& s) {
				iterator ret;
				ret.scalar = 0;
				meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1,
					do_convert>::exec(ret.vector, s._data);
				return ret;
			}
			inline static const_iterator begin(const storageT& s) {
				const_iterator ret;
				ret.scalar = 0;
				meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1,
					do_convert>::exec(ret.vector, s._data);
				return ret;
			}
			inline static iterator end(storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				const int left = total - right;
				iterator ret;
				ret.scalar = right;
				meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
					do_convert>::exec(ret.vector, s._data, left);
				return ret;
			}
			inline static const_iterator end(const storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				const int left = total - right;
				const_iterator ret;
				ret.scalar = right;
				meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
					do_convert>::exec(ret.vector, s._data, left);
				return ret;
			}
		};

		// Only for planar storage & 1 channel
		template<typename storageT, typename TL>
		class planar_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_contiguous_storage<storageT>::value == true &&
				ct::length<TL>::value == 1
			>::type
		> {
		private:
			typedef typename ct::type_at<TL, 0>::type T;
		public:
			typedef single_contiguous_iterator<T> iterator;
			typedef single_contiguous_iterator<T> const_iterator;
			
			inline static iterator begin(storageT& s) {
				return iterator(
					reinterpret_cast<typename iterator::vector_type*>(
						s._data.template get<0>()),
					0, s.left(), s.top());
			}
			inline static const_iterator begin(const storageT& s) {
				return const_iterator(
					reinterpret_cast<typename const_iterator::vector_type*>(
						s._data.template get<0>()),
					0, s.left(), s.top());
			}
			inline static iterator end(storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				return iterator(
					reinterpret_cast<typename iterator::vector_type*>(
						s._data.template get<0>() + total - right),
					right, s.right(), s.bottom());
			}
			inline static const_iterator end(const storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				return const_iterator(
					reinterpret_cast<typename const_iterator::vector_type*>(
						s._data.template get<0>() + total - right),
					right, s.right(), s.bottom());
			}
		};
	
		// Only for unknown storage & 1 channel
		template<typename storageT, typename TL>
		class planar_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_unknown_storage<storageT>::value == true &&
				ct::length<TL>::value == 1
			>::type
		> {
		private:
			typedef typename ct::type_at<TL, 0>::type T;
		public:
			//typedef T* iterator;
			//typedef const T* const_iterator;
			typedef pointer_iterator<T> iterator;
			typedef pointer_iterator<const T> const_iterator;

			inline static iterator begin(storageT& s) {
				return iterator(s.template begin<0>(), s.left(), s.top());
			}
			inline static const_iterator begin(const storageT& s) {
				return const_iterator(s.template begin<0>(), s.left(), s.top());
			}
			inline static iterator end(storageT& s) {
				return iterator(s.template end<0>(), s.right(), s.bottom());
			}
			inline static const_iterator end(const storageT& s) {
				return const_iterator(s.template end<0>(), s.right(), s.bottom());
			}
		};
		// Only for unknown storage & >1 channel
		template<typename storageT, typename TL>
		class planar_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_unknown_storage<storageT>::value == true &&
				(ct::length<TL>::value > 1)
			>::type
		> {
		private:
			static const int channel_count = ct::length<TL>::value;
			typedef typename ct::apply<TL, boost::add_pointer>::type ptr_tl;
			typedef typename ct::apply<
				typename ct::apply<TL, boost::add_const>::type,
				boost::add_pointer>::type const_ptr_tl;
		public:
			typedef ct::tuple<ptr_tl> iterator;
			typedef ct::tuple<const_ptr_tl> const_iterator;
			
			inline static iterator begin(storageT& s) {
				return s._data;
			}
			inline static const_iterator begin(const storageT& s) {
				return s._data;
			}
			inline static iterator end(storageT& s) {
				return s._data + s._width * s._height;
			}
			inline static const_iterator end(const storageT& s) {
				return s._data + s._width * s._height;
			}
		};


		//= chunky_iterator_helper ==============================================
		template<typename storageT, typename TL, typename specializedT = void>
		class chunky_iterator_helper {
		public:
			static const int channel_count = ct::length<TL>::value;
			typedef chunky_iterator<TL> iterator;
			typedef chunky_iterator<TL> const_iterator;
			
			inline static iterator begin(storageT& s) {
				return iterator(s._data, 0);
			}
			inline static const_iterator begin(const storageT& s) {
				return const_iterator(s._data, 0);
			}
			inline static iterator end(storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				const int left = total - right;
				return iterator(s._data + left, right);
			}
			inline static const_iterator end(const storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				const int left = total - right;
				return const_iterator(s._data + left, right);
			}
		};
		// Only for contiguous storage & 1 channel
		// Using planar iterator instead because planar
		template<typename storageT, typename TL>
		class chunky_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_contiguous_storage<storageT>::value == true &&
				ct::length<TL>::value == 1
			>::type
		> {
		private:
			typedef typename ct::type_at<TL, 0>::type T;
		public:
			typedef single_contiguous_iterator<T> iterator;
			typedef single_contiguous_iterator<T> const_iterator;
			
			inline static iterator begin(storageT& s) {
				return iterator(
					reinterpret_cast<typename iterator::vector_type*>(
						s._data),
					0, s.left(), s.top());
			}
			inline static const_iterator begin(const storageT& s) {
				return const_iterator(
					reinterpret_cast<typename const_iterator::vector_type*>(
						s._data),
					0, s.left(), s.top());
			}
			inline static iterator end(storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				return iterator(
					reinterpret_cast<typename iterator::vector_type*>(
						s._data + total / VVM_SCALAR_COUNT),
					right, s.right(), s.bottom());
			}
			inline static const_iterator end(const storageT& s) {
				const int total = s._width * s._height;
				const int right = total % VVM_SCALAR_COUNT;
				return const_iterator(
					reinterpret_cast<typename const_iterator::vector_type*>(
						s._data + total / VVM_SCALAR_COUNT),
					right, s.right(), s.bottom());
			}
		};
		// Only for unknown storage & 1 channel
		template<typename storageT, typename TL>
		class chunky_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_unknown_storage<storageT>::value == true &&
				ct::length<TL>::value == 1
			>::type
		> {
		private:
			typedef typename ct::type_at<TL, 0>::type T;
		public:
			//typedef T* iterator;
			//typedef const T* const_iterator;
			typedef pointer_iterator<T> iterator;
			typedef pointer_iterator<const T> const_iterator;
			
			inline static iterator begin(storageT& s) {
				return iterator(s.template begin<0>(), 0, 0);
			}
			inline static const_iterator begin(const storageT& s) {
				return const_iterator(s.template begin<0>(), 0, 0);
			}
			inline static iterator end(storageT& s) {
				return iterator(s.template end<0>(), s._width, s._height);
			}
			inline static const_iterator end(const storageT& s) {
				return const_iterator(s.template end<0>(), s._width, s._height);
			}
		};
		// Only for unknown storage & >1 channel
		template<typename storageT, typename TL>
		class chunky_iterator_helper<
			storageT, TL,
			typename ct::enable_if<
				uses_unknown_storage<storageT>::value == true &&
				(ct::length<TL>::value > 1)
			>::type
		> {
		private:
			static const int channel_count = ct::length<TL>::value;
			typedef typename ct::apply<TL, boost::add_pointer>::type ptr_tl;
			typedef typename ct::apply<
				typename ct::apply<TL, boost::add_const>::type,
				boost::add_pointer>::type const_ptr_tl;
		public:
			typedef typename storageT::chunk* iterator;
			typedef const typename storageT::chunk* const_iterator;
			
			inline static iterator begin(storageT& s) {
				return s._data;
			}
			inline static const_iterator begin(const storageT& s) {
				return s._data;
			}
			inline static iterator end(storageT& s) {
				return s._data + s._width * s._height;
			}
			inline static const_iterator end(const storageT& s) {
				return s._data + s._width * s._height;
			}
		};

	} // End of priv namespace
} // End of vvis namespace	

#endif
