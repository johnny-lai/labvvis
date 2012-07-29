/*
 *  altivec_math.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Jan 15 2004.
 *  Copyright (c) 2004 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_ALTIVEC_MATH_H
#define _VVM_ALTIVEC_MATH_H

#include <vecLib/vecLib.h>

namespace vvm {
	namespace priv {
		//- abs ----------------------------------------------------------------
		template<typename scalarT>
		class abs<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			abs<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vec_abs(a);
			}
		};
		//- acos ---------------------------------------------------------------
		template<typename scalarT>
		class acos<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			acos<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vacosf(a);
			}
		};
		//- asin ---------------------------------------------------------------
		template<typename scalarT>
		class asin<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			asin<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vasinf(a);
			}
		};
		//- atan ---------------------------------------------------------------
		template<typename scalarT>
		class atan<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			atan<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vatanf(a);
			}
		};
		//- atan2 --------------------------------------------------------------
		template<typename scalarT>
		class atan2<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			atan2<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t b) {
				ret = vatan2f(a, b);
			}
		};
		//- ceil ---------------------------------------------------------------
		template<typename scalarT>
		class ceil<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			ceil<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vec_ceil(a);
			}
		};
		//- cos ----------------------------------------------------------------
		template<typename scalarT>
		class cos<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			cos<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vcosf(a);
			}
		};
		//- cosh ---------------------------------------------------------------
		template<typename scalarT>
		class cosh<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			cosh<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vcoshf(a);
			}
		};
		//- exp ----------------------------------------------------------------
		template<typename scalarT>
		class exp<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			exp<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vexpf(a);
			}
		};
		//- floor --------------------------------------------------------------
		template<typename scalarT>
		class floor<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			floor<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vec_floor(a);
			}
		};
		//- fmod ---------------------------------------------------------------
		template<typename scalarT>
		class fmod<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			fmod<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t b) {
				ret = vmodf(a, b);
			}
		};
		//- log ----------------------------------------------------------------
		template<typename scalarT>
		class log<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			log<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vlogf(a);
			}
		};
		//- log10 --------------------------------------------------------------
		template<typename scalarT>
		class log10<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			log10<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vlog10f(a);
			}
		};
		//- pow ----------------------------------------------------------------
		template<typename scalarT>
		class pow<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			pow<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t b) {
				ret = vpowf(a, b);
			}
		};
		//- pow_int ------------------------------------------------------------
		template<typename scalarT>
		class pow_int<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<int>&,
			pow_int<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<int>::vpvector_type int_vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const int_vpvector_t b) {
				ret = vipowf(a, b);
			}
		};
		//- sin ----------------------------------------------------------------
		template<typename scalarT>
		class sin<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sin<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vsinf(a);
			}
		};
		//- sinh ---------------------------------------------------------------
		template<typename scalarT>
		class sinh<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sinh<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vsinhf(a);
			}
		};
		//- sqrt ---------------------------------------------------------------
		template<typename scalarT>
		class sqrt<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sqrt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vsqrtf(a);
			}
		};
		//- tan ----------------------------------------------------------------
		template<typename scalarT>
		class tan<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			tan<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vtanf(a);
			}
		};
		//- tanh ---------------------------------------------------------------	
		template<typename scalarT>
		class tanh<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			tanh<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a) {
				ret = vtanhf(a);
			}
		};
	} // End of priv namespace
} // End of vvm namespace

#endif
