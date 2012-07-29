#ifndef _VVIS_FUNCTOR_H
#define _VVIS_FUNCTOR_H

#include <limits>
#include <map>
#include <cmath>
#include <functional>
#include <vvm/vvm.h>
#include <vvis/vvm_tuple_support.h>
#include <vvis/kernel.h>

#define _VVIS_INSERT_TUPLE_FUNCTION1() \
	template<typename TL> \
	ct::tuple<TL> operator()(const ct::tuple<TL>& a) const { \
		ct::tuple<TL> ret; \
		meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1, \
			priv::function_call>::exec(*this, ret, a); \
		return ret; \
	}
	
#define _VVIS_INSERT_BOOL_TUPLE_FUNCTION1() \
	template<typename TL> \
	ct::tuple<typename ct::apply<TL, vvm::to_bool>::type> operator()(const ct::tuple<TL>& a) const { \
		ct::tuple<typename ct::apply<TL, vvm::to_bool>::type> ret; \
		meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1, \
			priv::function_call>::exec(*this, ret, a); \
		return ret; \
	}
	
#define _VVIS_INSERT_TUPLE_FUNCTION2() \
	template<typename TL> \
	ct::tuple<TL> operator()(const ct::tuple<TL>& a, const ct::tuple<TL>& b) const { \
		ct::tuple<TL> ret; \
		meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1, \
			priv::function_call>::exec(*this, ret, a, b); \
		return ret; \
	}

#define _VVIS_INSERT_BOOL_TUPLE_FUNCTION2() \
	template<typename TL> \
	ct::tuple<typename ct::apply<TL, vvm::to_bool>::type> operator()(const ct::tuple<TL>& a, const ct::tuple<TL>& b) const { \
		ct::tuple<typename ct::apply<TL, vvm::to_bool>::type> ret; \
		meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1, \
			priv::function_call>::exec(*this, ret, a, b); \
		return ret; \
	}
	
namespace vvis
{
	namespace priv {
		/** Function Call
		 * Implements tuple code by calling self.operator()
		 */
		struct function_call {
			template<int i> struct Code {
				template<typename selfT, typename returnT, typename T>
				static void exec(const selfT& self, returnT& ret, const T& a) {
					ret.template get<i>() = self.operator()(a.template get<i>());
				}
				template<typename selfT, typename returnT, typename T>
				static void exec(const selfT& self, returnT& ret, const T& a, const T& b) {
					ret.template get<i>() = self.operator()(a.template get<i>(), b.template get<i>());
				}
			};
		};
	} // End of priv namespace
	
	/** Explicit functor
	 * Allows scalar and vvm::vector functors to be specified explicitly
	 */
	template<
		typename scalarF,
		typename vectorF
	> struct explicit_unary_functor {
		explicit_unary_functor(const scalarF& sf, const vectorF& vf)
		: _sf(sf), _vf(vf) {
		}
		typename scalarF::result_type
		operator()(const typename scalarF::argument_type& t) const {
			return _sf(t);
		}
		typename vectorF::result_type
		operator()(const typename vectorF::argument_type& t) const {
			return _vf(t);
		}
		/*
		template<typename T> typename scalarF::result_type
		operator()(const typename scalarF::first_argument_type& t1,
		const typename scalarF::second_argument_type& t2) const {
			return _sf(t1, t2);
		}
		template<typename T> typename vectorF::result_type
		operator()(const typename vectorF::first_argument_type& t1,
		const typename vectorF::second_argument_type& t2) const {
			return _vf(t1, t2);
		}
		*/
	private:
		const scalarF& _sf;
		const vectorF& _vf;
	};
	
	/** Creates explicit functor using a function template
	 * This function is provided so that types can be automatically
	 * deduced.
	 */
	template<
		typename scalarF,
		typename vectorF
	> explicit_unary_functor<scalarF, vectorF>
	make_explicit_unary_functor(const scalarF& sf, const vectorF& vf) {
		return explicit_unary_functor<scalarF, vectorF>(sf, vf);
	}
	
	/** Binary functor
	 * Wrapper for Standard C++ functors.
	 */
	template<template<typename> class functorT>
	struct binary_functor {
		template<typename T> typename functorT<T>::result_type
		operator()(const typename functorT<T>::first_argument_type& t1,
		const typename functorT<T>::second_argument_type& t2) const {
			functorT<T> f;
			return f(t1, t2);
		}
	};

	
	//==========================================================================
	// Standard C++ functors
	//--------------------------------------------------------------------------
	//- plus -------------------------------------------------------------------
	template<typename T = void> struct plus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a + b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a + b;
		}
	};
	template<> struct plus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a + b;
		}
	};
	//- minus ------------------------------------------------------------------
	template<typename T = void> struct minus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a - b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a - b;
		}
	};
	template<> struct minus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a - b;
		}
	};
	//- multiplies -------------------------------------------------------------
	template<typename T = void> struct multiplies
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a * b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a * b;
		}
	};
	template<> struct multiplies<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a * b;
		}
	};
	//- divides ----------------------------------------------------------------
	template<typename T = void> struct divides
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a / b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a / b;
		}
	};
	template<> struct divides<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a / b;
		}
	};
	//- modulus ----------------------------------------------------------------
	template<typename T = void> struct modulus
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a % b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a % b;
		}
	};
	template<> struct modulus<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a % b;
		}
	};
	//- negate -----------------------------------------------------------------
	template<typename T = void> struct negate
	: public std::unary_function<T, T> {
		T operator()(const T& a) const {
			return -a;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a) const {
			return -a;
		}
	};
	template<> struct negate<void> {
		template<typename T> T operator()(const T& a) const {
			return -a;
		}
	};
	
	//- equal_to ---------------------------------------------------------------
	template<typename T = void> struct equal_to
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a == b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a == b;
		}
	};
	template<typename TL> struct equal_to<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a == b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a == b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct equal_to<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a == b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a == b;
		}
	};
	//- not_equal_to -----------------------------------------------------------
	template<typename T = void> struct not_equal_to
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a != b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a != b;
		}
	};
	template<typename TL> struct not_equal_to<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a != b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a != b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct not_equal_to<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a != b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a != b;
		}
	};
	//- greater ----------------------------------------------------------------
	template<typename T = void> struct greater
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a > b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a > b;
		}
	};
	template<typename TL> struct greater<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a > b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a > b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct greater<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a > b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a > b;
		}
	};
	//- less -------------------------------------------------------------------
	template<typename T = void> struct less
	: public std::binary_function<T, T, typename vvm::to_bool<T>::type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a < b ? VVM_TRUE : VVM_FALSE;
		}
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a < b;
		}
	};
	template<typename TL> struct less<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a < b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a < b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct less<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a < b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a < b;
		}
	};
	//- greater_equal ----------------------------------------------------------
	template<typename T = void> struct greater_equal
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a >= b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a >= b;
		}
	};
	template<typename TL> struct greater_equal<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a >= b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a >= b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct greater_equal<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a >= b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a >= b;
		}
	};
	//- less_equal -------------------------------------------------------------
	template<typename T = void> struct less_equal
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a <= b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a <= b;
		}
	};
	template<typename TL> struct less_equal<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a <= b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a <= b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct less_equal<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a <= b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a <= b;
		}
	};

	//- logical_and ------------------------------------------------------------
	template<typename T = void> struct logical_and
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a && b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a && b;
		}
	};
	template<typename TL> struct logical_and<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a && b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a && b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct logical_and<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a && b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a && b;
		}
	};
	//- logical_or -------------------------------------------------------------
	template<typename T = void> struct logical_or
	: public std::binary_function<T, T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a, const T& b) const {
			return a || b ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a || b;
		}
	};
	template<typename TL> struct logical_or<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a || b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a || b;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct logical_or<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION2()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a, const T& b) const {
			return a || b ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a, const vvm::vector<T>& b) const {
			return a || b;
		}
	};
	//- logical_not ------------------------------------------------------------
	template<typename T = void> struct logical_not
	: public std::unary_function<T, typename vvm::scalar_traits<T>::bool_type> {
		typename vvm::scalar_traits<T>::bool_type
		operator()(const T& a) const {
			return !a ? VVM_TRUE : VVM_FALSE;
		}
		typename vvm::add_vector<typename vvm::scalar_traits<T>::bool_type>::type operator()(
		const typename vvm::add_vector<T>::type& a) const {
			return !a;
		}
	};
	template<typename TL> struct logical_not<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	typename vvm::to_bool<ct::tuple<TL> >::type> {
		typename vvm::to_bool<ct::tuple<TL> >::type operator()(
		const ct::tuple<TL>& a) const {
			typename vvm::to_bool<ct::tuple<TL> >::type ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
		typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a) const {
			typename vvm::add_vector<typename vvm::to_bool<ct::tuple<TL> >::type>::type ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a) const {
			return !a ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a) const {
			return !a;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct logical_not<void> {
		_VVIS_INSERT_BOOL_TUPLE_FUNCTION1()
		template<typename T>
		typename vvm::scalar_traits<T>::bool_type operator()(const T& a) const {
			return !a ? VVM_TRUE : VVM_FALSE;
		}
		template<typename T>
		vvm::vector<typename vvm::scalar_traits<T>::bool_type> operator()(
		const vvm::vector<T>& a) const {
			return !a;
		}
	};
	
	//==========================================================================
	// Standard C++ binders
	//--------------------------------------------------------------------------
	/** Binder 1st
	 * Functor where the first value of a binary functor is binded to an
	 * arbitrary value.
	 * Note that the template arguments to vvis::binder1st is different from
	 * std::binder1st. This is because VVIS functors in general do not know
	 * what type they are operating on until they are called.
	 * TODO: WHAT ABOUT RETURN TYPE?
	 */
	template<typename opT>
	class binder1st : public std::unary_function<typename opT::second_argument_type, typename opT::result_type> {
	public:
		binder1st(const opT& op, const typename opT::first_argument_type& value)
		: _op(op), _value(value), _vector_value(value) {
		}
		typename opT::result_type operator()(const typename opT::second_argument_type& a) const {
			return _op(_value, a);
		}
		typename vvm::add_vector<typename opT::result_type>::type operator()(
		const typename vvm::add_vector<typename opT::second_argument_type>::type& a) const {
			return _op(_vector_value, a);
		}
	protected:
		opT _op;
		typename opT::first_argument_type _value;
		typename vvm::add_vector<typename opT::first_argument_type>::type _vector_value;
	};
	template<typename opT, typename T>
	binder1st<opT> bind1st(const opT& op, const T& x) {
		return binder1st<opT>(op, x);
	}
	
	/** Binder 2nd
	 * Functor where the first value of a binary functor is binded to an
	 * arbitrary value.
	 * Note that the template arguments to vvis::binder1st is different from
	 * std::binder1st. This is because VVIS functors in general do not know
	 * what type they are operating on until they are called.
	 * TODO: WHAT ABOUT RETURN TYPE?
	 */
	template<typename opT>
	class binder2nd : public std::unary_function<typename opT::first_argument_type, typename opT::result_type> {
	public:
		binder2nd(const opT& op, const typename opT::second_argument_type& value)
		: _op(op), _value(value), _vector_value(value) {
		}
		typename opT::result_type operator()(const typename opT::first_argument_type& a) const {
			return _op(a, _value);
		}
		typename vvm::add_vector<typename opT::result_type>::type operator()(
		const typename vvm::add_vector<typename opT::first_argument_type>::type& a) const {
			return _op(a, _vector_value);
		}
	protected:
		opT _op;
		typename opT::second_argument_type _value;
		typename vvm::add_vector<typename opT::second_argument_type>::type _vector_value;
	};
	template<typename opT, typename T>
	binder2nd<opT> bind2nd(const opT& op, const T& x) {
		return binder2nd<opT>(op, x);
	}

	//==========================================================================
	// Additional functors
	//--------------------------------------------------------------------------
	//- plus_saturated ---------------------------------------------------------
	template<typename T = void> struct plus_saturated
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			typename ct::promote<T>::type r = a + b;
			return r < std::numeric_limits<T>::max() ?
				static_cast<T>(r) : std::numeric_limits<T>::max();
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return vvm::adds(a, b);
		}
	};
	template<typename TL> struct plus_saturated<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	ct::tuple<TL> > {
		ct::tuple<TL> operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			ct::tuple<TL> ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<ct::tuple<TL> >::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typedef typename ct::promote<T>::type promote_t;
			promote_t r = a + b;
			return r < std::numeric_limits<T>::max() ?
				static_cast<T>(r) : std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T>
		typename vvm::vector<T> operator()(
		const vvm::vector<T>& a,
		const vvm::vector<T>& b) const {
			return vvm::adds(a, b);
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct plus_saturated<void> {
		_VVIS_INSERT_TUPLE_FUNCTION2()
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typedef typename ct::promote<T>::type promote_t;
			promote_t r = a + b;
			return r < std::numeric_limits<T>::max() ?
				static_cast<T>(r) : std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T>
		typename vvm::vector<T> operator()(
		const vvm::vector<T>& a,
		const vvm::vector<T>& b) const {
			return vvm::adds(a, b);
		}
	};
	//- minus_saturated --------------------------------------------------------
	template<typename T = void> struct minus_saturated
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			long r = a - b;
			return r > std::numeric_limits<T>::min() ?
				r : std::numeric_limits<T>::min();
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return vvm::subs(a, b);
		}
	};
	template<typename TL> struct minus_saturated<ct::tuple<TL> >
	: public std::binary_function<ct::tuple<TL>, ct::tuple<TL>,
	ct::tuple<TL> > {
		ct::tuple<TL> operator()(
		const ct::tuple<TL>& a, const ct::tuple<TL>& b) const {
			ct::tuple<TL> ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
		typename vvm::add_vector<ct::tuple<TL> >::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a,
		const typename vvm::add_vector<ct::tuple<TL> >::type& b) const {
			typename vvm::add_vector<ct::tuple<TL> >::type ret;
			meta::EFOR4<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a, b);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typedef typename ct::promote<T>::type promote_t;
			long r = a - b;
			return r > std::numeric_limits<T>::min() ?
				r : std::numeric_limits<T>::min();
		}
		/// Vector implementation
		template<typename T>
		typename vvm::vector<T> operator()(
		const vvm::vector<T>& a,
		const vvm::vector<T>& b) const {
			return vvm::subs(a, b);
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct minus_saturated<void> {
		_VVIS_INSERT_TUPLE_FUNCTION2()
		/// Scalar implementation
		template<typename T> T operator()(const T& a, const T& b) const {
			typename ct::promote_signed<T>::type r = a - b;
			return r > std::numeric_limits<T>::min() ?
				r : std::numeric_limits<T>::min();
		}
		/// Vector implementation
		template<typename T>
		typename vvm::vector<T> operator()(
		const vvm::vector<T>& a,
		const vvm::vector<T>& b) const {
			return vvm::subs(a, b);
		}
	};
	//- bitwise_and ------------------------------------------------------------
	template<typename T = void> struct bitwise_and
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a & b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a & b;
		}
	};
	template<> struct bitwise_and<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a & b;
		}
	};
	//- bitwise_not ------------------------------------------------------------
	template<typename T = void> struct bitwise_not
	: public std::unary_function<T, T> {
		T operator()(const T& a) const {
			return ~a;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a) const {
			return ~a;
		}
	};
	template<> struct bitwise_not<void> {
		template<typename T> T operator()(const T& a) const {
			return ~a;
		}
	};
	//- bitwise_or -------------------------------------------------------------
	template<typename T = void> struct bitwise_or
	: public std::binary_function<T, T, T> {
		T operator()(const T& a, const T& b) const {
			return a | b;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& a,
		const typename vvm::add_vector<T>::type& b) const {
			return a | b;
		}
	};
	template<> struct bitwise_or<void> {
		template<typename T> T operator()(const T& a, const T& b) const {
			return a | b;
		}
	};
	//- constant ---------------------------------------------------------------
	template<typename T> struct constant {
		constant(const T a) : _scalar_constant(a), _vector_constant(a) {
		}
		T operator()(const T&) const {
			return _scalar_constant;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type&) const {
			return _vector_constant;
		}
	private:
		T _scalar_constant;
		typename vvm::add_vector<T>::type _vector_constant;
	};
	//- noop -------------------------------------------------------------------
	template<typename T = void> struct noop
		: public std::unary_function<T, T> {
			T operator()(const T& a) const {
				return a;
			}
			typename vvm::add_vector<T>::type operator()(
			const typename vvm::add_vector<T>::type& a) const {
				return a;
			}
		};
	template<> struct noop<void> {
		template<typename T> T operator()(const T& a) const {
			return a;
		}
	};	
	//- min_max ----------------------------------------------------------------
	template<typename T> class min_max {
	public:
		min_max() {
			_min = std::numeric_limits<T>::max();
			_max = std::numeric_limits<T>::min();
			_vmin = _min;
			_vmax = _max;
		}
		T operator()(const T& s) {
			if(s < _min)
				_min = s;
			else if(s > _max)
				_max = s;
			return s;
		}
		typename vvm::add_vector<T>::type operator()(
		const typename vvm::add_vector<T>::type& v) {
			_vmin = vvm::min(_vmin, v);
			_vmax = vvm::max(_vmax, v);
			return v;
		}
		const T min() {
			// Calculate min from both _min and _vmin
			for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
				if(_vmin.scalar(i) < _min)
					_min = _vmin.scalar(i);
			}
			return _min;
		}
		const T max() {
			// Calculate max from both _max and _vmax
			for(int i = 0; i < VVM_SCALAR_COUNT; ++i) {
				if(_vmax.scalar(i) > _max)
					_max = _vmax.scalar(i);
			}
			return _max;
		}
	private:
		T _min, _max;
		typename vvm::add_vector<T>::type _vmin, _vmax;
	};
	
	//= Discrete ===============================================================
	//- equalize ---------------------------------------------------------------
	/** Equalize
	 */
	template<typename T> struct equalize {
		equalize(const T min, const T max)
		: _smin(min), _smax(max), _vmin(min), _vmax(max), _vzero_val(0),
		_vmax_val(std::numeric_limits<T>::max()) {
			// Calculate m & c
			_sm = (std::numeric_limits<T>::max() - 1) / (F)(_smax - _smin);
			_sc = 1 - _sm * _smin;
			// Copy m & c to vvm::vector
			_vm = _sm;
			_vc = _sc;
		}
		T operator()(const T a) {
			if(a < _smin)
				return 0;
			else if(a > _smax)
				return std::numeric_limits<T>::max();
			else
				return _sm * a + _sc;
		}
		vvm::vector<T> operator()(const vvm::vector<T>& a) {
			typedef vvm::vector<typename vvm::scalar_traits<T>::bool_type> vector_bool_t;
			vvm::vector<T> ret;
			ret = vvm::madd(_vm, a, _vc);
			vector_bool_t min_mask = a < _vmin;
			vector_bool_t max_mask = a > _vmax;
			ret = vvm::select(min_mask, ret, _vzero_val);
			ret = vvm::select(max_mask, ret, _vmax_val);
			return ret;
		}
	private:
		T _smin, _smax;
		vvm::vector<T> _vmin, _vmax, _vm, _vc, _vzero_val, _vmax_val;
		typedef typename ct::promote_float<T>::type F;
		F _sm, _sc;

	};
	/** Equalize
	 */
	template<typename TL> struct equalize<ct::tuple<TL> > {
	private:
		typedef TL scalar_tl;
		typedef typename ct::apply<TL, vvm::add_vector>::type vector_tl;
	public:
		equalize(const ct::tuple<TL>& min, const ct::tuple<TL>& max) {
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				do_init>::exec(*this, min, max);
		}
		ct::tuple<scalar_tl> operator()(const ct::tuple<TL> a) {
			ct::tuple<scalar_tl> ret;
			meta::EFOR3<0, meta::Less, ct::length<scalar_tl>::value, +1,
				do_op>::exec(*this, ret, a);
			return ret;
		}
		ct::tuple<vector_tl> operator()(const typename vvm::add_vector<ct::tuple<TL> >::type& a) {
			ct::tuple<vector_tl> ret;
			meta::EFOR3<0, meta::Less, ct::length<vector_tl>::value, +1,
				do_op>::exec(*this, ret, a);
			return ret;
		}
	private:
		struct do_init {
			template<int i> struct Code {
				static void exec(equalize& self, const ct::tuple<TL>& min, const ct::tuple<TL>& max) {
					typedef typename ct::type_at<TL, i>::type scalar_type;
					self._smin.template get<i>() = min.template get<i>();
					self._smax.template get<i>() = max.template get<i>();
					self._vmin.template get<i>() = min.template get<i>();
					self._vmax.template get<i>() = max.template get<i>();
					self._vzero_val.template get<i>() = 0;
					self._vmax_val.template get<i>() = std::numeric_limits<scalar_type>::max();
					// Calculate m & c
					self._sm.template get<i>() = (std::numeric_limits<scalar_type>::max() - 1) /
						(self._smax.template get<i>() - self._smin.template get<i>());
					self._sc.template get<i>() = 1 - self._sm.template get<i>() * self._smin.template get<i>();
					// Copy m & c to vvm::vector
					self._vm.template get<i>() = self._sm.template get<i>();
					self._vc.template get<i>() = self._sc.template get<i>();
				}
			};
		};
		struct do_op {
			template<int i> struct Code {
				static void exec(equalize& self, ct::tuple<scalar_tl>& ret, const ct::tuple<TL>& a) {
					typedef typename ct::type_at<TL, i>::type scalar_type;
					if(a.template get<i>() < self._smin.template get<i>())
						ret.template get<i>() = 0;
					else if(a.template get<i>() > self._smax.template get<i>())
						ret.template get<i>() = std::numeric_limits<scalar_type>::max();
					else
						ret.template get<i>() = self._sm.template get<i>() * a.template get<i>() +
							self._sc.template get<i>();
				}
				static void exec(equalize& self, ct::tuple<vector_tl>& ret,
				const typename vvm::add_vector<ct::tuple<TL> >::type& a) {
					typedef typename ct::type_at<TL, i>::type scalar_type;
					typedef vvm::vector<typename vvm::scalar_traits<scalar_type>::bool_type> vector_bool_t;
					ret.template get<i>() = vvm::madd(
						self._vm.template get<i>(), 
						a.template get<i>(),
						self._vc.template get<i>());
					vector_bool_t min_mask = a.template get<i>() < self._vmin.template get<i>();
					vector_bool_t max_mask = a.template get<i>() > self._vmax.template get<i>();
					ret.template get<i>() = vvm::select(
						min_mask,
						ret.template get<i>(),
						self._vzero_val.template get<i>());
					ret.template get<i>() = vvm::select(
						max_mask,
						ret.template get<i>(),
						self._vmax_val.template get<i>());
				}
			};
		};
	private:
		ct::tuple<TL> _smin, _smax, _sm, _sc;
		typename vvm::add_vector<ct::tuple<TL> >::type _vmin, _vmax, _vm, _vc, _vzero_val, _vmax_val;
	};
	//- logarithmic ------------------------------------------------------------
	/** Natural Logarithm
	 */
	template<typename T = void> struct logarithmic {
	private:
		typedef typename ct::promote_float<T>::type promote_type;
	public:
		logarithmic()
		: _vmax_val(std::numeric_limits<T>::max()) {
		}
		/// Scalar implementation
		promote_type operator()(const T a) const {
			promote_type gx = (a / static_cast<promote_type>(std::numeric_limits<T>::max())) + 1;
			return std::log(static_cast<promote_type>(gx)) * std::numeric_limits<T>::max();
		}
		/// Vector implementation
		vvm::vector<promote_type> operator()(const vvm::vector<T>& a) const {
			vvm::vector<promote_type> gx = (vvm::vector_cast<vvm::vector<promote_type> >(a) / _vmax_val) +
				vvm::vector<promote_type>(1);
			return vvm::log(gx) * _vmax_val;
		}
	private:
		vvm::vector<promote_type> _vmax_val;
	};
	template<typename TL> struct logarithmic<ct::tuple<TL> > {
		ct::tuple<TL> operator()(const ct::tuple<TL>& a) const {
			ct::tuple<TL> ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
		typename vvm::add_vector<ct::tuple<TL> >::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a) const {
			typename vvm::add_vector<ct::tuple<TL> >::type ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
	public: // Should be private, but template friendship fails in GCC 3.1
		/// Scalar implementation
		template<typename T> T operator()(const T a) const {
			T gx = (a / std::numeric_limits<T>::max()) + 1;
			return std::log(static_cast<typename ct::promote_float<T>::type>(gx)) *
				std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T> vvm::vector<T> operator()(const vvm::vector<T>& a) const {
			const vvm::vector<T> max(std::numeric_limits<T>::max());
			vvm::vector<T> gx = (a / max) + vvm::vector<T>(1);
			return vvm::log(gx) * max;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct logarithmic<void> {
		/// Tuple implementation
		_VVIS_INSERT_TUPLE_FUNCTION1()
		/// Scalar implementation
		template<typename T> T operator()(const T a) const {
			T gx = (a / std::numeric_limits<T>::max()) + 1;
			return std::log(static_cast<typename ct::promote_float<T>::type>(gx)) *
				std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T> vvm::vector<T> operator()(const vvm::vector<T>& a) const {
			const vvm::vector<T> max(std::numeric_limits<T>::max());
			vvm::vector<T> gx = (a / max) + vvm::vector<T>(1);
			return vvm::log(gx) * max;
		}
	};
	//- exponential ------------------------------------------------------------
	/** Exponential
	 */
	template<typename T = void> struct exponential {
		exponential()
		: _v1(1), _vmax_val(std::numeric_limits<T>::max()) {
		}
		T operator()(const T a) {
			T gx = a / std::numeric_limits<T>::max();
			return std::exp(static_cast<typename ct::promote_float<T>::type>(gx)) * std::numeric_limits<T>::max();
		}
		vvm::vector<T> operator()(const vvm::vector<T>& a) {
			vvm::vector<T> gx = a / _vmax_val;
			return vvm::exp(gx) - _v1 * _vmax_val;
		}
	private:
		vvm::vector<T> _v1, _vmax_val;
	};
	template<typename TL> struct exponential<ct::tuple<TL> > {
		ct::tuple<TL> operator()(const ct::tuple<TL>& a) const {
			ct::tuple<TL> ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
		typename vvm::add_vector<ct::tuple<TL> >::type operator()(
		const typename vvm::add_vector<ct::tuple<TL> >::type& a) const {
			typename vvm::add_vector<ct::tuple<TL> >::type ret;
			meta::EFOR3<0, meta::Less, ct::length<TL>::value, +1,
				priv::function_call>::exec(*this, ret, a);
			return ret;
		}
	private:
		/// Scalar implementation
		template<typename T> T operator()(const T a) const {
			T gx = a / std::numeric_limits<T>::max();
			return std::exp(static_cast<typename ct::promote_float<T>::type>(gx)) * std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T> vvm::vector<T> operator()(const vvm::vector<T>& a) const {
			const vvm::vector<T> max(std::numeric_limits<T>::max());
			vvm::vector<T> gx = a / max;
			return vvm::exp(gx) - vvm::vector<T>(1) * max;
		}
		//template<int i> friend struct priv::function_call::Code;
	};
	template<> struct exponential<void> {
		/// Tuple implementation
		_VVIS_INSERT_TUPLE_FUNCTION1()
		/// Scalar implementation
		template<typename T> T operator()(const T a) const {
			T gx = a / std::numeric_limits<T>::max();
			return std::exp(static_cast<typename ct::promote_float<T>::type>(gx)) * std::numeric_limits<T>::max();
		}
		/// Vector implementation
		template<typename T> vvm::vector<T> operator()(const vvm::vector<T>& a) const {
			const vvm::vector<T> max(std::numeric_limits<T>::max());
			vvm::vector<T> gx = a / max;
			return vvm::exp(gx) - vvm::vector<T>(1) * max;
		}
	};
	//- rgb_to_cmyk ------------------------------------------------------------
	template<typename TL> struct rgb_to_cmyk {
		template<
			typename typelistU
		> ct::tuple<TL> operator()(const ct::tuple<typelistU>& v) {
			ct::tuple<TL> ret;
			ret.template get<0>() = v.template get<2>();
			ret.template get<1>() = v.template get<1>();
			ret.template get<2>() = v.template get<0>();
			ret.template get<3>() = 255;
			return ret;
		}
	};
	
	//- histogram --------------------------------------------------------------
	template<typename T, typename mapT = std::map<T, int> > struct histogram {
		typedef const mapT result_type;
		histogram() {
			_pcounts = new mapT();
			_delete_pcount = true;
		}
		histogram(mapT &counts)
		: _pcounts(&counts), _delete_pcount(false) {
		}
		~histogram() {
			if(_delete_pcount) {
				delete _pcounts;
			}
		}
		void operator()(const T p) {
			++((*_pcounts)[p]);
		}
		result_type &result() const {
			return *_pcounts;
		}
	private:
		bool _delete_pcount;
		mapT *_pcounts;
	};
	template<typename T, typename mapT> struct is_vectorised_functor<histogram<T, mapT> > {
		static const bool value = false;
	};
	//- count_if ---------------------------------------------------------------
	template<typename T, typename opT>
	struct count_if {
		count_if(const opT& op, const T& reference)
		: _op(op), _count(0), _reference(reference) {
		}
		T operator()(const T a) {
			if(_op(a, _reference)) {
				++_count;
			}
		}
		void reset() {
			_count = 0;
		}
		int count() const {
			return _count;
		}
	private:
		int _count;
		opT _op;
		const T& _reference;
	};
	template<typename T, typename opT> struct is_vectorised_functor<count_if<T, opT> > {
		static const bool value = false;
	};
	//==========================================================================
	// Convolution functors
	//--------------------------------------------------------------------------
	//- base_filter ------------------------------------------------------------
	/** Base Filter
	 */
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class base_filter {
	protected:
		typedef typename promoteP<CT_TYPELIST2(kernelT, T)>::type scalar_type;
		typedef vvm::vector<scalar_type> vector_type;
	public:
		typedef kernel2d<kernelT> kernel_type;
		base_filter() : _vector_kernel(0) {
		}
		base_filter(const base_filter& rhs)
		: _kernel(rhs._kernel) {
			allocate();
		}
		base_filter(kernel_type kernel)
		: _kernel(kernel) {
			allocate();
		}
		base_filter& operator=(const base_filter& rhs) {
			// Check if the same object
			if(&rhs == this)
				return *this;
			// Deallocate memory
			deallocate();
			// Then make copy
			_kernel = rhs._kernel;
			allocate();
			return *this;
		}
		const kernel_type kernel() const {
			return _kernel;
		}
	protected:
		// Destructor is protected so that derived classes do not have to be
		// virtual. In addition, it prevent base_filter from being used by
		// the user which is good.
		~base_filter() {
			deallocate();
		}
	private:
		void allocate() {
			_vector_kernel = new vector_type[_kernel.width()*_kernel.height()];
			typename kernel_type::const_iterator itr = _kernel.begin();
			for(int i = 0; itr != _kernel.end(); ++itr, ++i) {
				_vector_kernel[i] = *itr;
			}
			_vector_sum = _kernel.sum();
		}
		void deallocate() {
			if(_vector_kernel)
				delete[] _vector_kernel;
		}
	protected:
		kernel_type _kernel;
		vector_type _vector_sum;
		vector_type* _vector_kernel;
	};

	namespace priv {
		// Required for GCC 3.3. Does not handle
		// template<typename T> struct do_promote {
		//   typedef typename ct::promote<CT_TYPELIST2(T, kernelT)>::type type;
		// };
		// typedef typename ct::apply<TL, do_promote>::type scalar_tl;
		template<
			typename kernelT,
			typename TL,
			template<typename> class promoteP
		> struct apply_promote_with_kernel {
			typedef ct::typelist<
				typename promoteP<CT_TYPELIST2(typename TL::head, kernelT)>::type,
				//typename ct::promote<CT_TYPELIST2(typename TL::head, kernelT)>::type,
				typename apply_promote_with_kernel<kernelT, typename TL::tail, promoteP>::type
				> type;
		};
		template<typename kernelT, template<typename> class promoteP>
		struct apply_promote_with_kernel<kernelT, ct::null_type, promoteP> {
			typedef ct::null_type type;
		};
	} // End of priv namespace
	
	/// Base Filter for tuple types
	template<
		typename kernelT,
		typename TL,
		template<typename> class promoteP
	> class base_filter<kernelT, ct::tuple<TL>, promoteP> {
	protected:
		typedef typename priv::apply_promote_with_kernel<kernelT, TL, promoteP>::type scalar_tl;
		typedef typename ct::apply<scalar_tl, vvm::add_vector>::type vector_tl;
	private:
		typedef typename ct::no_duplicates<scalar_tl>::type scalar_kernel_tl;
		typedef typename ct::no_duplicates<vector_tl>::type vector_kernel_tl;
	public:
		typedef kernel2d<kernelT> kernel_type;
		base_filter() : _vector_kernel(0) {
		}
		base_filter(const base_filter& rhs)
		: _kernel(rhs._kernel) {
			allocate();
		}
		base_filter(kernel_type kernel)
		: _kernel(kernel) {
			allocate();
		}
		~base_filter() {
			deallocate();
		}
	public:
		const kernel_type kernel() const {
			return _kernel;
		}
	protected:
		// Retrieves the kernel for the type T
		template<typename T> 
		vvm::vector<T>* vector_kernel() const {
			const int ki = ct::index_of<scalar_kernel_tl, T>::value;
			return _vector_kernel.template get<ki>();
		}
		// Retrieves the kernel sum for the type T
		template<typename T> 
		vvm::vector<T> vector_sum() const {
			const int ki = ct::index_of<scalar_kernel_tl, T>::value;
			return _vector_sum.template get<ki>();
		}
	private:
		void allocate() {
			meta::EFOR1<0, meta::Less, ct::length<vector_kernel_tl>::value, +1,
				do_allocate>::exec(*this);
		}
		void deallocate() {
			meta::EFOR1<0, meta::Less, ct::length<vector_kernel_tl>::value, +1,
				do_deallocate>::exec(*this);
		}	
	private:
		struct do_allocate {
			template<int i> struct Code {
				static void exec(base_filter& self) {
					typedef typename ct::type_at<vector_kernel_tl, i>::type vector_type;
					typedef typename ct::type_at<scalar_kernel_tl, i>::type scalar_type;
					self._vector_kernel.template get<i>() = new vector_type[
							self._kernel.width()*self._kernel.height()];
					typename kernel_type::const_iterator itr = self._kernel.begin();
					for(int j = 0; itr != self._kernel.end(); ++itr, ++j) {
						self._vector_kernel.template get<i>()[j] = *itr;
					}
					self._vector_sum.template get<i>() = self._kernel.sum();
				}
			};
		};
		struct do_deallocate {
			template<int i> struct Code {
				static void exec(base_filter& self) {
					if(self._vector_kernel.template get<i>())
						delete[] self._vector_kernel.template get<i>();
				}
			};
		};
	private:
		kernel_type _kernel;
		ct::tuple<typename ct::apply<
			vector_kernel_tl, boost::add_pointer>::type> _vector_kernel;
		ct::tuple<vector_kernel_tl> _vector_sum;
	};
	
	//- linear_filter ----------------------------------------------------------
	/** Linear Filter
	 * Scalar kernel not implemented because can get value from vector kernel
	 * -> Allows object to be smaller
	 * -> Scalar code would not be used much anyway
	 */
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class linear_filter : public base_filter<kernelT, T, promoteP> {
	private:
		typedef base_filter<kernelT, T, promoteP> parent_type;
		typedef typename parent_type::scalar_type scalar_type;
		typedef typename parent_type::vector_type vector_type;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		linear_filter() : parent_type() {
		}
		linear_filter(const linear_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
		_vector_result(rhs._vector_result), _i(rhs._i) {
		}
		linear_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~linear_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
			_vector_result = 0;
		}
		const scalar_type scalar_result() const {
			// TODO: Restrict result to range
			return _scalar_result / this->_kernel.sum();
		}
		const vector_type vector_result() const {
			// TODO: Restrict result to range
			return _vector_result / this->_vector_sum;
		}
		void accumulate(const T& v) {
			_scalar_result += v * this->_vector_kernel[_i].scalar(0);
			++_i;
		}
		void accumulate(const vvm::vector<T>& v) {
			_vector_result = madd(vvm::vector_cast<vector_type>(v),
				this->_vector_kernel[_i],
				_vector_result);
			++_i;
		}
	private:
		scalar_type _scalar_result;
		vector_type _vector_result;
		int _i;
	};

	//- linear_filter ----------------------------------------------------------
	/** Linear Filter
	 * Scalar kernel not implemented because can get value from vector kernel
	 * -> Allows object to be smaller
	 * -> Scalar code would not be used much anyway
	 */
	template<
		typename kernelT,
		typename TL,
		template<typename> class promoteP
	> class linear_filter<kernelT, ct::tuple<TL>, promoteP> : public base_filter<kernelT, ct::tuple<TL>, promoteP> {
	private:
		typedef base_filter<kernelT, ct::tuple<TL>, promoteP> parent_type;
		typedef typename parent_type::scalar_tl scalar_tl;
		typedef typename parent_type::vector_tl vector_tl;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		linear_filter() : parent_type() {
		}
		linear_filter(const linear_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
		_vector_result(rhs._vector_result), _i(rhs._i) {
		}
		linear_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~linear_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
			_vector_result = 0;
		}
		const ct::tuple<scalar_tl> scalar_result() const {
			ct::tuple<scalar_tl> ret;
			meta::EFOR2<0, meta::Less, ct::length<scalar_tl>::value, +1, do_result>::exec(*this, ret);
			return ret;
		}
		const ct::tuple<vector_tl> vector_result() const {
			ct::tuple<vector_tl> ret;
			meta::EFOR2<0, meta::Less, ct::length<vector_tl>::value, +1, do_result>::exec(*this, ret);
			return ret;
		}
		void accumulate(const ct::tuple<TL>& v) {
			meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1, do_accumulate>::exec(*this, v);
			++_i;
		}
		void accumulate(const typename vvm::add_vector<ct::tuple<TL> >::type& v) {
			meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1, do_accumulate>::exec(*this, v);
			++_i;
		}
	private:
		struct do_accumulate {
			template<int i> struct Code {
				static void exec(linear_filter& self, const ct::tuple<TL>& v) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					self._scalar_result += v.template get<i>() *
						self.template vector_kernel<scalar_type>()[self._i].scalar(0);
				}
				static void exec(linear_filter& self, const typename vvm::add_vector<ct::tuple<TL> >::type& v) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					typedef typename ct::type_at<vector_tl, i>::type vector_type;
					self._vector_result.template get<i>() = madd(
						vvm::vector_cast<vector_type>(v.template get<i>()),
						self.template vector_kernel<scalar_type>()[self._i],
						self._vector_result.template get<i>());
				}
			};
		};
		struct do_result {
			template<int i> struct Code {
				// TODO: Restrict result to range
				static void exec(const linear_filter& self, ct::tuple<scalar_tl>& result) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					result.template get<i>() = self._scalar_result.template get<i>() /
						self.template vector_sum<scalar_type>().scalar(0);
				}
				static void exec(const linear_filter& self, ct::tuple<vector_tl>& result) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					result.template get<i>() = self._vector_result.template get<i>() /
						self.template vector_sum<scalar_type>();
				}
			};
		};
	private:
		ct::tuple<scalar_tl> _scalar_result;
		ct::tuple<vector_tl> _vector_result;
		int _i;
	};

	//- max_filter ----------------------------------------------------------
	/** Filter that returns the largest value
	 * Scalar kernel not implemented because can get value from vector kernel
	 * -> Allows object to be smaller
	 * -> Scalar code would not be used much anyway
	 */ 
	template<
		typename kernelT,
		typename T,
		template<typename> class promoteP = ct::promote
	> class max_filter : public base_filter<kernelT, T, promoteP> {
	private:
		typedef base_filter<kernelT, T, promoteP> parent_type;
		typedef typename parent_type::scalar_type scalar_type;
		typedef typename parent_type::vector_type vector_type;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		max_filter() : parent_type() {
		}
		max_filter(const max_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
		_vector_result(rhs._vector_result), _i(rhs._i) {
		}
		max_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~max_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
			_vector_result = 0;
		}
		const scalar_type scalar_result() const {
			return _scalar_result;
		}
		const vector_type vector_result() const {
			return _vector_result;
		}
		void accumulate(const T& v) {
			scalar_type val = v * this->_vector_kernel[_i].scalar(0);
			if(val > _scalar_result)
				_scalar_result = val;
			++_i;
		}
		void accumulate(const vvm::vector<T>& v) {
			vector_type val = vvm::vector_cast<vector_type>(v) * this->_vector_kernel[_i];
			_vector_result = vvm::select(val > _vector_result, val, _vector_result);
			++_i;
		}
	private:
		scalar_type _scalar_result;
		vector_type _vector_result;
		int _i;
	};
	
	template<
		typename kernelT,
		typename TL,
		template<typename> class promoteP
	> class max_filter<kernelT, ct::tuple<TL>, promoteP> : public base_filter<kernelT, ct::tuple<TL>, promoteP> {
	private:
		typedef base_filter<kernelT, ct::tuple<TL>, promoteP> parent_type;
		typedef typename parent_type::scalar_tl scalar_tl;
		typedef typename parent_type::vector_tl vector_tl;
	public:
		typedef typename parent_type::kernel_type kernel_type;
		max_filter() : parent_type() {
		}
		max_filter(const max_filter& rhs)
		: parent_type(rhs), _scalar_result(rhs._scalar_result),
		_vector_result(rhs._vector_result), _i(rhs._i) {
		}
		max_filter(kernel_type kernel)
		: parent_type(kernel) {
		}
		~max_filter() {
		}
	public:
		void reset() {
			_i = 0;
			_scalar_result = 0;
			_vector_result = 0;
		}
		const ct::tuple<scalar_tl> scalar_result() const {
			return _scalar_result;
		}
		const ct::tuple<vector_tl> vector_result() const {
			return _vector_result;
		}
		void accumulate(const ct::tuple<TL>& v) {
			meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1, do_accumulate>::exec(*this, v);
			++_i;
		}
		void accumulate(const typename vvm::add_vector<ct::tuple<TL> >::type& v) {
			meta::EFOR2<0, meta::Less, ct::length<TL>::value, +1, do_accumulate>::exec(*this, v);
			++_i;
		}
	private:
		struct do_accumulate {
			template<int i> struct Code {
				static void exec(max_filter& self, const ct::tuple<TL>& v) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					scalar_type val =  v.template get<i>() *
						self.template vector_kernel<scalar_type>()[self._i].scalar(0);
					if(val > self._scalar_result.template get<i>())
						self._scalar_result.template get<i>() = val;
				}
				static void exec(max_filter& self, const typename vvm::add_vector<ct::tuple<TL> >::type& v) {
					typedef typename ct::type_at<scalar_tl, i>::type scalar_type;
					typedef typename ct::type_at<vector_tl, i>::type vector_type;
					// Multiply with kernel
					vector_type val = vvm::vector_cast<vector_type>(v.template get<i>()) *
						self.template vector_kernel<scalar_type>()[self._i];
					// Select largest
					self._vector_result.template get<i>() = vvm::select(
						val > self._vector_result.template get<i>(),
						val,
						self._vector_result.template get<i>());
				}
			};
		};
	private:
		ct::tuple<scalar_tl> _scalar_result;
		ct::tuple<vector_tl> _vector_result;
		int _i;
	};
	
	
	//==========================================================================
	// Hough Transform
	//--------------------------------------------------------------------------
	struct hough_line {
		hough_line(double ar, int aq, int acount)
		: r(ar), q(aq), count(acount) {
		}
		double r;
		int q;
		int count;
	};

	template<typename accessorT> class hough {
		typedef typename accessorT::scalar_type scalar_type;
		typedef typename accessorT::vector_type vector_type;
		typedef std::map<int, int> rq_t;
		static const double PI = 3.14159265;
	public:
		typedef hough_line value_type;
	public:
		hough() {
		}
		~hough() {
		}
		
		void operator()(const scalar_type& p) {
			if(p.value) {
				for(int q = 0; q < 180; q += 1) {
					double rad = q*M_PI/180.0;
					// Keep up to 2 decimals
					int r = round((p.x*cos(rad) + p.y*sin(rad)) * 100);
					++_accumulator[q][r];
				}
			}
		}

		// TODO: Hough Transform is not vectorised
		// void operator()(const vector_type& v);
		
		std::vector<value_type > result(const int threshold) {
			std::vector<value_type > ret;
			for(int q = 0; q < 180; ++q) {
				rq_t &rq = _accumulator[q];
				for(rq_t::const_iterator i = rq.begin(); i != rq.end(); ++i) {
					if(i->second >= threshold) {
						std::clog << "[vvis] Line r=" << (i->first / 100.0) << ", q=" << q << std::endl; 
						ret.push_back(value_type(i->first / 100.0, q, i->second));
					}
				}
			}
			return ret;
		}
	private:
		rq_t _accumulator[180];
	};

	// Hough Transform is not vectorised
	template<typename imageT> struct is_vectorised_functor<hough<imageT> > {
		static const bool value = false;
	};
} // End of vvis namespace

#endif
