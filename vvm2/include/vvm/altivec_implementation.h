/*
 *  altivec_implementation.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Sat Aug 23 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_ALTIVEC_IMPLEMENTATION_H
#define _VVM_ALTIVEC_IMPLEMENTATION_H

#include <ct/enabler.h>
#include <ct/boost_type_traits.h>
#include <vvm/altivec_types.h>
#include <vvm/base_operation.h>
#include <vvm/vecLib_wrappers.h>

// Include math functions
#include <vvm/altivec_math.h>

namespace vvm {
	namespace priv {
		namespace priv {
			template<typename V> V splat(const typename vpvector_traits<V>::scalar_type s) {
                V result = vec_lde(0, &s);
                __vector unsigned char rotate_mask = vec_lvsl(0, &s);
                result= vec_perm(result, result, rotate_mask);
                return vec_splat(result, 0);
			}
		} // End of priv namespace
		//- add ----------------------------------------------------------------
		template<typename scalarT>
		class add<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			add<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_add(a, b);
			}
		};
		//- adds ---------------------------------------------------------------
		template<typename scalarT>
		class adds<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			adds<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_adds(a, b);
			}
		};
		//- add_assign ---------------------------------------------------------
		template<typename scalarT>
		class add_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			add_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				a = vec_add(a, b);
			}
		};
		//- all_eq -------------------------------------------------------------
		template<typename scalarT>
		class all_eq<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_eq<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_eq(a, b);
			}
		};
		//- all_ge -------------------------------------------------------------
		template<typename scalarT>
		class all_ge<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_ge(a, b);
			}
		};
		//- all_gt -------------------------------------------------------------
		template<typename scalarT>
		class all_gt<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_gt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_gt(a, b);
			}
		};
		//- all_le -------------------------------------------------------------
		template<typename scalarT>
		class all_le<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_le<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_le(a, b);
			}
		};
		//- all_lt -------------------------------------------------------------
		template<typename scalarT>
		class all_lt<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_lt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_lt(a, b);
			}
		};
		//- all_ne -------------------------------------------------------------
		template<typename scalarT>
		class all_ne<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ne<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_ne(a, b);
			}
		};
		//- all_nge ------------------------------------------------------------
		template<typename scalarT>
		class all_nge<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a >= b) == (a < b)
				ret = ret && vec_all_lt(a, b);
			}
		};
		template<typename scalarT>
		class all_nge<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_nge(a, b);
			}
		};
		//- all_ngt ------------------------------------------------------------
		template<typename scalarT>
		class all_ngt<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ngt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a > b) == a <= b
				ret = ret && vec_all_le(a, b);
			}
		};
		template<typename scalarT>
		class all_ngt<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ngt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_ngt(a, b);
			}
		};
		//- all_nle ------------------------------------------------------------
		template<typename scalarT>
		class all_nle<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nle<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a <= b) == a > b
				ret = ret && vec_all_gt(a, b);
			}
		};
		template<typename scalarT>
		class all_nle<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nle<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_nle(a, b);
			}
		};
		//- all_nlt ------------------------------------------------------------
		template<typename scalarT>
		class all_nlt<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nlt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a < b) == a >= b
				ret = ret && vec_all_ge(a, b);
			}
		};
		template<typename scalarT>
		class all_nlt<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nlt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret && vec_all_nlt(a, b);
			}
		};
		//- any_eq -------------------------------------------------------------
		template<typename scalarT>
		class any_eq<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_eq<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_eq(a, b);
			}
		};
		//- any_ge -------------------------------------------------------------
		template<typename scalarT>
		class any_ge<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_ge(a, b);
			}
		};
		//- any_gt -------------------------------------------------------------
		template<typename scalarT>
		class any_gt<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_gt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_gt(a, b);
			}
		};
		//- any_le -------------------------------------------------------------
		template<typename scalarT>
		class any_le<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_le<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_le(a, b);
			}
		};
		//- any_lt -------------------------------------------------------------
		template<typename scalarT>
		class any_lt<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_lt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_lt(a, b);
			}
		};
		//- any_ne -------------------------------------------------------------
		template<typename scalarT>
		class any_ne<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ne<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_ne(a, b);
			}
		};
		//- any_nge ------------------------------------------------------------
		template<typename scalarT>
		class any_nge<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a >= b) == a < b
				ret = ret || vec_any_lt(a, b);
			}
		};
		template<typename scalarT>
		class any_nge<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nge<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_nge(a, b);
			}
		};
		//- any_ngt ------------------------------------------------------------
		template<typename scalarT>
		class any_ngt<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ngt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a > b) == a <= b
				ret = ret || vec_any_le(a, b);
			}
		};
		template<typename scalarT>
		class any_ngt<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ngt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_ngt(a, b);
			}
		};
		//- any_nle ------------------------------------------------------------
		template<typename scalarT>
		class any_nle<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nle<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a <= b) = a > b
				ret = ret || vec_any_gt(a, b);
			}
		};
		template<typename scalarT>
		class any_nle<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nle<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_nle(a, b);
			}
		};
		//- any_nlt ------------------------------------------------------------
		template<typename scalarT>
		class any_nlt<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nlt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				// !(a < b) == a >= b
				ret = ret || vec_any_ge(a, b);
			}
		};
		template<typename scalarT>
		class any_nlt<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nlt<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(bool& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = ret || vec_any_nlt(a, b);
			}
		};
		//- bitwise_and --------------------------------------------------------
		template<typename scalarT>
		class bitwise_and<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_and<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_and(a, b);
			}
		};
		//- bitwise_and_assign -------------------------------------------------
		template<typename scalarT>
		class bitwise_and_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_and_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				a = vec_and(a, b);
			}
		};
		//- bitwise_or ---------------------------------------------------------
		template<typename scalarT>
		class bitwise_or<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_or<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_or(a, b);
			}
		};
		//- bitwise_or_assign --------------------------------------------------
		template<typename scalarT>
		class bitwise_or_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_or_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				a = vec_or(a, b);
			}
		};
		//- bitwise_xor --------------------------------------------------------
		template<typename scalarT>
		class bitwise_xor<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_xor<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_xor(a, b);
			}
		};
		//- bitwise_xor_assign -------------------------------------------------
		template<typename scalarT>
		class bitwise_xor_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_xor_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				a = vec_xor(a, b);
			}
		};
		//- constant -----------------------------------------------------------
		namespace priv {
			template<typename scalarT, int value, typename specializedT = void>
			struct vpvector_constant {
				static const bool is_specialized = false;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					typename scalar_traits<scalarT>::vpvector_type ret = {value};
					return vec_splat(ret, 0);
				}
			};
			// Specialize for __vector signed char for values -16 to 15
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector signed char>::value
					&& value >= -16 && value < 16
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_s8(value);
				}
			};
			// Specialize for __vector unsigned char for values 0 to 32
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector unsigned char>::value
					&& value >= 0 && value < 32
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_u8(value);
				}
			};
			// Specialize for __vector signed short for values -16 to 15
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector signed short>::value
					&& value >= -16 && value < 16
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_s16(value);
				}
			};
			// Specialize for __vector unsigned short for values 0 to 32
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector unsigned short>::value
					&& value >= 0 && value < 32
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_u16(value);
				}
			};
			// Specialize for __vector signed int for values -16 to 15
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector signed int>::value
					&& value >= -16 && value < 16
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_s32(value);
				}
			};
			// Specialize for __vector unsigned int for values 0 to 32
			template<typename scalarT, int value>
			struct vpvector_constant<scalarT, value,
				typename ct::enable_if<
					boost::is_same<
						typename scalar_traits<scalarT>::vpvector_type,
						__vector unsigned int>::value
					&& value >= 0 && value < 32
				>::type
			> {
				static const bool is_specialized = true;
				static typename scalar_traits<scalarT>::vpvector_type exec() {
					return vec_splat_u32(value);
				}
			};
		} // End of priv namespace
		template<typename scalarT, int value>
		struct constant<scalarT, value, 
			typename ct::enable_if<
				priv::vpvector_constant<scalarT, value>::is_specialized
			>::type
		> {
			inline static const vector<scalarT>
			exec() {
				vector<scalarT> ret;
				meta::EFOR1<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					do_constant>::exec(ret);
				return ret;
			}
			struct do_constant {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret) {
						ret.vpvector(i) = priv::vpvector_constant<scalarT, value>::exec();
					}
				};
			};
		};
		//- div ----------------------------------------------------------------
		template<typename scalarT>
		class div<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			div<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vecLib_div(a, b, 0);
			}
		};
		template<typename scalarT>
		class div<scalarT, _VVM_VPVECTOR_IS(__vector float)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			div<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vdivf(a, b);
			}
		};
		//- div_assign ---------------------------------------------------------
		template<typename scalarT>
		class div_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			div_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				div<scalarT>::op(a, a, b);
			}
		};
		//- equal --------------------------------------------------------------
		template<typename scalarT>
		class equal<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			equal<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_cmpeq(a, b);
			}
		};
		//- greater ------------------------------------------------------------
		template<typename scalarT>
		class greater<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			greater<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_cmpgt(a, b);
			}
		};
		//- greater_equal ------------------------------------------------------
		template<typename scalarT>
		class greater_equal<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			greater_equal<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_or(vec_cmpgt(a, b), vec_cmpeq(a, b));
			}
		};
		template<typename scalarT>
		class greater_equal<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			greater_equal<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_cmpge(a, b);
			}
		};
		//- less ---------------------------------------------------------------
		template<typename scalarT>
		class less<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			less<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_cmplt(a, b);
			}
		};
		//- less_equal ---------------------------------------------------------
		template<typename scalarT>
		class less_equal<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			less_equal<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret,const vpvector_t& a, const vpvector_t& b) {
				ret = vec_or(vec_cmplt(a, b), vec_cmpeq(a, b));
			}
		};
		template<typename scalarT>
		class less_equal<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			less_equal<scalarT, void>
		> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_cmple(a, b);
			}
		};
		//- load ---------------------------------------------------------------
		template<typename scalarT>
		class load<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static const vector<scalarT>
			exec(const scalarT* const p) {
				vector<scalarT> ret;
				meta::EFOR2<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, reinterpret_cast<const vpvector_t* const>(p));
				return ret;
			}
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				vector<scalarT> ret;
				meta::EFOR3<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, reinterpret_cast<const vpvector_t* const>(p), offset);
				return ret;
			}
			struct operation {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret, const vpvector_t* const p, const offset_t offset) {
						ret.vpvector(i) = vec_ld(offset + 16*i, p);
					}
					static void exec(vector<scalarT>& ret, const vpvector_t* const p) {
						ret.vpvector(i) = vec_ld(16*i, p);
					}
				};
			};
		};
		//- load2 --------------------------------------------------------------
		template<typename scalarT>
		class load2<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static const vector<scalarT>
			exec(const vector<scalarT> a, const vector<scalarT> b, const offset_t offset) {
				vector<scalarT> ret;
				__vector unsigned char lvsl_mask = vec_lvsl(offset, (unsigned char*)0);
				meta::EFOR4<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, a, b, lvsl_mask);
				return ret;
			}
			struct operation {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret, const vector<scalarT>& a,
					const vector<scalarT>& b, const __vector unsigned char& mask) {
						ret.vpvector(i) = vec_perm(a.vpvector(i), b.vpvector(i), mask);
						//ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 0);
						// Only 16 possibilities accepted
						//assert(offset >= 0 && offset < 16);
						/*
						switch(offset) {
						case 0: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 0); break;
						case 1: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 1); break;
						case 2: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 2); break;
						case 3: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 3); break;
						case 4: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 4); break;
						case 5: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 5); break;
						case 6: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 6); break;
						case 7: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 7); break;
						case 8: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 8); break;
						case 9: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 9); break;
						case 10: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 10); break;
						case 11: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 11); break;
						case 12: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 12); break;
						case 13: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 13); break;
						case 14: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 14); break;
						case 15: ret.vpvector(i) = vec_sld(a.vpvector(i), b.vpvector(i), 15); break;
						}
						*/
					}
				};
			};
		};
		//- logical_and --------------------------------------------------------
		template<typename scalarT>
		struct logical_and<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			logical_and<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
		public:
			static void op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_and(
					vec_cmpgt(a, priv::vpvector_constant<scalarT, 0>::exec()),
					vec_cmpgt(b, priv::vpvector_constant<scalarT, 0>::exec()));
			}
		};
		//- logical_not --------------------------------------------------------
		template<typename scalarT>
		struct logical_not<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_unary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			logical_not<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
		public:
			static void op(bool_vpvector_t& ret, const vpvector_t& a) {
				ret = vec_cmpeq(a, priv::vpvector_constant<scalarT, 0>::exec());
			}
		};
		//- logical_or ---------------------------------------------------------
		template<typename scalarT>
		struct logical_or<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			logical_or<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
		public:
			static void op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_or(
					vec_cmpgt(a, priv::vpvector_constant<scalarT, 0>::exec()),
					vec_cmpgt(b, priv::vpvector_constant<scalarT, 0>::exec()));
			}
		};
		//- lvsl ---------------------------------------------------------------
		template<typename scalarT>
		class lvsl<scalarT, _VVM_VPVECTOR_IS(__vector unsigned char)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				vector<scalarT> ret;
				meta::EFOR3<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, p, offset);
				return ret;
			}
			struct operation {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret, const scalarT* const p, const offset_t offset) {
						ret.vpvector(i) = vec_lvsl(offset, p);
					}
				};
			};
		};
		//- lvsr ---------------------------------------------------------------
		template<typename scalarT>
		class lvsr<scalarT, _VVM_VPVECTOR_IS(__vector unsigned char)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				vector<scalarT> ret;
				meta::EFOR3<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, p, offset);
				return ret;
			}
			struct operation {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret, const scalarT* const p, const offset_t offset) {
						ret.vpvector(i) = vec_lvsr(offset, p);
					}
				};
			};
		};
		//- madd ---------------------------------------------------------------
		template<typename scalarT>
		class madd<scalarT, _VVM_VPVECTOR_IN(altivec_mladd_types)>
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			madd<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b, const vpvector_t& c) {
				ret = vec_mladd(a, b, c);
			}
		};
		template<typename scalarT>
		class madd<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			madd<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b, const vpvector_t& c) {
				ret = vec_madd(a, b, c);
			}
		};
		//- max ----------------------------------------------------------------
		template<typename scalarT>
		class max<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			max<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_max(a, b);
			}
		};
		//- mergeh -------------------------------------------------------------
		template<typename scalarT>
		class mergeh<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mergeh<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_mergeh(a, b);
			}
		};
		//- mergel -------------------------------------------------------------
		template<typename scalarT>
		class mergel<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mergel<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_mergel(a, b);
			}
		};
		//- min ----------------------------------------------------------------
		template<typename scalarT>
		class min<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			min<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_min(a, b);
			}
		};
		//- mod ----------------------------------------------------------------
		template<typename scalarT>
		class mod<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mod<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				vecLib_div(a, b, &ret);
			}
		};
		//- mod_assign ---------------------------------------------------------
		template<typename scalarT>
		class mod_assign<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			mod_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				mod<scalarT>::op(a, a, b);
			}
		};
		//- mul ----------------------------------------------------------------
		template<typename scalarT>
		class mul<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mul<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vecLib_mul(a, b);
			}
		};
		//TODO: mul for float
		//- mul_assign ---------------------------------------------------------
		template<typename scalarT>
		class mul_assign<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			mul_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				mul<scalarT>::op(a, a, b);
			}
		};
		//- negate -------------------------------------------------------------
		// TODO:
		//- nmsub --------------------------------------------------------------
		template<typename scalarT>
		class nmsub<scalarT, _VVM_VPVECTOR_IN(altivec_float_types)>
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			nmsub<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b, const vpvector_t& c) {
				ret = vec_nmsub(a, b, c);
			}
		};
		//- not_equal ----------------------------------------------------------
		template<typename scalarT>
		class not_equal<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			not_equal<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
		public:
			static void
			op(bool_vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				bool_vpvector_t eq = vec_cmpeq(a, b);
				ret = vec_nor(eq, eq); // Invert answer
			}
		};
		//- ones_complement ----------------------------------------------------
		template<typename scalarT>
		struct ones_complement<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			ones_complement<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& ret, const vpvector_t& a) {
				// a NOR a = ~a
				ret = vec_nor(a, a);
			}
		};
		//- perm ---------------------------------------------------------------
		template<typename scalarT>
		class perm<scalarT, _VVM_VPVECTOR_IS(__vector unsigned char)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& b, const vector<scalarT>& c) {
				vector<scalarT> ret;
				meta::EFOR4<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(ret, a, b, c);
				return ret;
			}
			struct operation {
				template<int i> struct Code {
					static void exec(vector<scalarT>& ret, const vector<scalarT>& a,
					const vector<scalarT>& b, const vector<scalarT>& c) {
						ret.vpvector(i) = vec_perm(a.vpvector(i), b.vpvector(i), c.vpvector(i));
					}
				};
			};
		};
		//- positive -----------------------------------------------------------
		//- post_dec -----------------------------------------------------------
		// No need to override since emulation post_dec uses pre_dec
		//- post_inc -----------------------------------------------------------
		// No need to override since emulation post_inc uses pre_inc
		//- prefetch -----------------------------------------------------------
		template<int channel, typename usage, typename scalarT>
		struct prefetch<channel, usage, scalarT,
			typename ct::enable_if<
				// Channel is supported
				(channel >= 0 && channel <= 3) &&
				// Read only usage
				boost::is_same<usage, read_only>::value &&
				// Is an AltiVec enabled type
				ct::contains<
					altivec_types,
					typename scalar_traits<scalarT>::vpvector_type
				>::value
			>::type
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			template<typename T>
			inline static void exec(T* const addr, const int count) {
				vec_dst(reinterpret_cast<vpvector_t* const>(addr),
					((sizeof(vpvector_t)*8*VVM_SCALAR_COUNT/vpvector_traits<vpvector_t>::scalar_count) << 24) | (1 << 16) | (count & 0xffff),
					channel);
			}
		};
		template<int channel, typename usage, typename scalarT>
		struct prefetch<channel, usage, scalarT,
			typename ct::enable_if<
				// Channel is supported
				(channel >= 0 && channel <= 3) &&
				// Read only transient usage
				boost::is_same<usage, read_only_transient>::value &&
				// Is an AltiVec enabled type
				ct::contains<
					altivec_types,
					typename scalar_traits<scalarT>::vpvector_type
				>::value
			>::type
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			template<typename T>
			inline static void exec(T* const addr, const int count) {
				vec_dstt(reinterpret_cast<vpvector_t* const>(addr),
					((sizeof(vpvector_t)*8*VVM_SCALAR_COUNT/vpvector_traits<vpvector_t>::scalar_count) << 24) | (1 << 16) | (count & 0xffff),
					channel);
			}
		};
		template<int channel, typename usage, typename scalarT>
		struct prefetch<channel, usage, scalarT,
			typename ct::enable_if<
				// Channel is supported
				(channel >= 0 && channel <= 3) &&
				// Read write usage
				boost::is_same<usage, read_write>::value &&
				// Is an AltiVec enabled type
				ct::contains<
					altivec_types,
					typename scalar_traits<scalarT>::vpvector_type
				>::value
			>::type
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			template<typename T>
			inline static void exec(T* const addr, const int count) {
				vec_dstst(reinterpret_cast<vpvector_t* const>(addr),
					((sizeof(vpvector_t)*8*VVM_SCALAR_COUNT/vpvector_traits<vpvector_t>::scalar_count) << 24) | (1 << 16) | (count & 0xffff),
					channel);
			}
		};
		template<int channel, typename usage, typename scalarT>
		struct prefetch<channel, usage, scalarT,
			typename ct::enable_if<
				// Channel is supported
				(channel >= 0 && channel <= 3) &&
				// Read write transient usage
				boost::is_same<usage, read_write_transient>::value &&
				// Is an AltiVec enabled type
				ct::contains<
					altivec_types,
					typename scalar_traits<scalarT>::vpvector_type
				>::value
			>::type
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			template<typename T>
			inline static void exec(T* const addr, const int count) {
				vec_dststt(reinterpret_cast<vpvector_t* const>(addr),
					((sizeof(vpvector_t)*8*VVM_SCALAR_COUNT/vpvector_traits<vpvector_t>::scalar_count) << 24) | (1 << 16) | (count & 0xffff),
					channel);
			}
		};
		//- pre_dec ------------------------------------------------------------
		template<typename scalarT>
		struct pre_dec<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				meta::EFOR1<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(a);
				return a;
			}
			struct operation {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret) {
						ret.vpvector(i) = vec_sub(
							ret.vpvector(i),
							priv::vpvector_constant<scalarT, 1>::exec());
					}
				};
			};
		};
		/*
		template<typename scalarT>
		struct pre_dec<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				a = sub<scalarT>::exec(a, constant<scalarT, 1>::exec());
				return a;
			}
		};
		*/
		//- pre_inc ------------------------------------------------------------
		template<typename scalarT>
		struct pre_inc<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				meta::EFOR1<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(a);
				return a;
			}
			struct operation {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret) {
						ret.vpvector(i) = vec_add(
							ret.vpvector(i),
							priv::vpvector_constant<scalarT, 1>::exec());
					}
				};
			};
		};
		/*
		template<typename scalarT>
		struct pre_inc<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				a = add<scalarT>::exec(a, constant<scalarT, 1>::exec());
				return a;
			}
		};
		*/
		//- select -------------------------------------------------------------
		template<typename scalarT>
		class select<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<typename scalar_traits<scalarT>::bool_type>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			select<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const bool_vpvector_t a, const vpvector_t& b, const vpvector_t& c) {
				ret = vec_sel(b, c, a);
			}
		};
		//- shift_left ---------------------------------------------------------
		template<typename scalarT>
		class shift_left<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			struct do_shift_left {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret, const vector<scalarT>& a,
					const __vector unsigned char& vshift, const __vector unsigned char& vrshift,
					const __vector unsigned char& vselect) {
						ret.vpvector(i) = vec_sel(
							vec_srl(vec_sro(a.vpvector(i+1), vrshift), vrshift),
							vec_sll(vec_slo(a.vpvector(i), vshift), vshift),
							(bool_vpvector_t)vselect);
					}
				};
			};
		public:
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const unsigned char shift) {
				__vector unsigned char vshift = priv::splat<__vector unsigned char>(shift);
				__vector unsigned char vrshift = priv::splat<__vector unsigned char>((sizeof(scalarT)*8) - shift);
				__vector unsigned char vselect = vec_splat_u8(-1);  // 0xff
				vselect = vec_sll(vec_slo(vselect, vshift), vshift);
				
				vector<scalarT> ret;
				meta::EFOR5<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count - 1, +1,
					do_shift_left>::exec(ret, a, vshift, vrshift, vselect);
				ret.vpvector(vector_traits<vector<scalarT> >::vpvector_count - 1) =
					vec_sll(vec_slo(a.vpvector(vector_traits<vector<scalarT> >::vpvector_count - 1), vshift), vshift);
				return ret;
			}
		};
		//- shift_left_assign --------------------------------------------------
		template<typename scalarT>
		class shift_left_assign<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			shift_left_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				shift_left<scalarT>::op(a, a, b);
			}
		};
		//- shift_left_elements ------------------------------------------------
		template<typename scalarT>
		class shift_left_elements<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			shift_left_elements<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_sl(a, b);
			}
		};
		//- shift_left_elements_assign -----------------------------------------
		template<typename scalarT>
		class shift_left_elements_assign<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			shift_left_elements_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				shift_left_elements<scalarT>::op(a, a, b);
			}
		};
		//- shift_right --------------------------------------------------------
		template<typename scalarT>
		class shift_right<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)> {
			typedef typename scalar_traits<
				typename scalar_traits<scalarT>::bool_type>::vpvector_type bool_vpvector_t;
			struct do_shift_right {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret, const vector<scalarT>& a,
					const __vector unsigned char& vshift, const __vector unsigned char& vlshift,
					const __vector unsigned char& vselect) {
						ret.vpvector(i) = vec_sel(
							vec_sll(vec_slo(a.vpvector(i-1), vlshift), vlshift),
							vec_srl(vec_sro(a.vpvector(i), vshift), vshift),
							(bool_vpvector_t)vselect);
					}
				};
			};
		public:
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const unsigned char shift) {
				__vector unsigned char vshift = priv::splat<__vector unsigned char>(shift);
				__vector unsigned char vlshift = priv::splat<__vector unsigned char>((sizeof(scalarT)*8) - shift);
				__vector unsigned char vselect = vec_splat_u8(-1);  // 0xff
				vselect = vec_srl(vec_sro(vselect, vshift), vshift);
				
				vector<scalarT> ret;
				meta::EFOR5<1, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					do_shift_right>::exec(ret, a, vshift, vlshift, vselect);
				ret.vpvector(0) = vec_srl(vec_sro(a.vpvector(0), vshift), vshift);
				return ret;
			}
		};
		//- shift_right_assign--------------------------------------------------
		template<typename scalarT>
		class shift_right_assign<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			shift_right_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				shift_right<scalarT>::op(a, a, b);
			}
		};
		//- shift_right_elements -----------------------------------------------
		template<typename scalarT>
		class shift_right_elements<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			shift_right_elements<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_sr(a, b);
			}
		};
		//- shift_right_elements_assign ----------------------------------------
		template<typename scalarT>
		class shift_right_elements_assign<scalarT, _VVM_VPVECTOR_IN(altivec_shift_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			shift_right_elements_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				shift_right_elements<scalarT>::op(a, a, b);
			}
		};
		//- splat --------------------------------------------------------------
		template<typename scalarT>
		class splat<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			struct do_splat {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& dest) {
						dest.vpvector(i) = dest.vpvector(0);
					}
				};
			};
		public:
			inline static void
			exec(vector<scalarT>& dest, const scalarT a) {
				dest.scalar(0) = a;
				dest.vpvector(0) = vec_splat(dest.vpvector(0), 0);
				meta::EFOR1<1, meta::Less, vector_traits<vector<scalarT> >::vpvector_count,
					+1, do_splat>::exec(dest);
			}
		};
		//- store --------------------------------------------------------------
		template<typename scalarT>
		class store<scalarT, _VVM_VPVECTOR_IN(altivec_types)> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			template<typename T> inline static void
			exec(const vector<scalarT>& a, T* const p) {
				meta::EFOR2<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(a, reinterpret_cast<vpvector_t* const>(p));
			}
			template<typename T> inline static void
			exec(const vector<scalarT>& a, T* const p, const offset_t offset) {
				meta::EFOR3<0, meta::Less, vector_traits<vector<scalarT> >::vpvector_count, +1,
					operation>::exec(a, reinterpret_cast<vpvector_t* const>(p), offset);
			}
			struct operation {
				template<int i> struct Code {
					static void exec(const vector<scalarT>& a, vpvector_t* p) {
						vec_st(a.vpvector(i), 0, p);
					}
					static void exec(const vector<scalarT>& a, vpvector_t* p, const offset_t offset) {
						vec_st(a.vpvector(i), offset, p);
					}
				};
			};
		};
		//- store2 -------------------------------------------------------------
		//- sub ----------------------------------------------------------------
		template<typename scalarT>
		class sub<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			sub<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_sub(a, b);
			}
		};
		//- subs ---------------------------------------------------------------
		template<typename scalarT>
		class subs<scalarT, _VVM_VPVECTOR_IN(altivec_integer_types)>
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			subs<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			inline static void
			op(vpvector_t& ret, const vpvector_t& a, const vpvector_t& b) {
				ret = vec_subs(a, b);
			}
		};
		//- sub_assign ---------------------------------------------------------
		template<typename scalarT>
		class sub_assign<scalarT, _VVM_VPVECTOR_IN(altivec_types)>
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sub_assign<scalarT, void>
		> {
			typedef typename scalar_traits<scalarT>::vpvector_type vpvector_t;
		public:
			static void op(vpvector_t& a, const vpvector_t& b) {
				a = vec_sub(a, b);
			}
		};
		//- vector_cast --------------------------------------------------------
		// For conversions using vec_unpack
		// __vector signed char  -> __vector signed short
		// __vector bool char    -> __vector bool short
		// __vector signed short -> __vector signed int
		// __vector bool short   -> __vector bool int
#define _VVM_DEFINE_VECTOR_CAST_UNPACK(FROM, TO) \
		template<typename toScalarT, typename fromScalarT> \
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>, \
			typename ct::enable_if< \
				boost::is_same< \
					typename scalar_traits<fromScalarT>::vpvector_type, \
					FROM>::value && \
				boost::is_same< \
					typename scalar_traits<toScalarT>::vpvector_type, \
					TO>::value \
			>::type \
		> { \
			inline static vector<toScalarT> \
			exec(const vector<fromScalarT>& a) { \
				vector<toScalarT> ret; \
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<fromScalarT> >::vpvector_count, \
					+1, do_unpack>::exec(ret, a); \
				return ret; \
			} \
		private: \
			struct do_unpack { \
				template<int i> struct Code { \
					inline static void \
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) { \
						ret.vpvector(i*2) = vec_unpackh(a.vpvector(i)); \
						ret.vpvector(i*2+1) = vec_unpackl(a.vpvector(i)); \
					} \
				}; \
			}; \
		}
		_VVM_DEFINE_VECTOR_CAST_UNPACK(__vector signed char, __vector signed short);
		_VVM_DEFINE_VECTOR_CAST_UNPACK(__vector bool char, __vector bool short);
		_VVM_DEFINE_VECTOR_CAST_UNPACK(__vector signed short, __vector signed int);
		_VVM_DEFINE_VECTOR_CAST_UNPACK(__vector bool short, __vector bool int);
#undef _VVM_DEFINE_VECTOR_CAST_UNPACK

		// For conversions using vec_pack
		// __vector signed short   -> __vector signed char
		// __vector unsigned short -> __vector unsigned char
		// __vector bool short     -> __vector bool char
		// __vector signed int     -> __vector signed short
		// __vector unsigned int   -> __vector unsigned short
		// __vector bool int       -> __vector bool short
#define _VVM_DEFINE_VECTOR_CAST_PACK(FROM, TO) \
		template<typename toScalarT, typename fromScalarT> \
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>, \
			typename ct::enable_if< \
				boost::is_same< \
					typename scalar_traits<fromScalarT>::vpvector_type, \
					FROM>::value && \
				boost::is_same< \
					typename scalar_traits<toScalarT>::vpvector_type, \
					TO>::value \
			>::type \
		> { \
			inline static vector<toScalarT> \
			exec(const vector<fromScalarT>& a) { \
				vector<toScalarT> ret; \
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<toScalarT> >::vpvector_count, \
					+1, do_pack>::exec(ret, a); \
				return ret; \
			} \
		private: \
			struct do_pack { \
				template<int i> struct Code { \
					inline static void \
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) { \
						ret.vpvector(i) = vec_pack(a.vpvector(2*i), a.vpvector(2*i+1)); \
					} \
				}; \
			}; \
		}
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector signed short, __vector signed char);
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector unsigned short, __vector unsigned char);
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector bool short, __vector bool char);
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector signed int, __vector signed short);
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector unsigned int, __vector unsigned short);
		_VVM_DEFINE_VECTOR_CAST_PACK(__vector bool int, __vector bool short);
#undef _VVM_DEFINE_VECTOR_CAST_PACK


		// (vector signed int, vector unsigned int) -> vector float
		template<typename toScalarT, typename fromScalarT>
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>,
			typename ct::enable_if<
				ct::contains<
					CT_TYPELIST2(__vector unsigned int, __vector signed int),
					typename scalar_traits<fromScalarT>::vpvector_type>::value &&
				boost::is_same<
					typename scalar_traits<toScalarT>::vpvector_type,
					__vector float>::value
			>::type
		> {
			inline static vector<toScalarT>
			exec(const vector<fromScalarT>& a) {
				vector<toScalarT> ret;
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<toScalarT> >::vpvector_count,
					+1, do_ctf>::exec(ret, a);
				return ret;
			}
		private:
			struct do_ctf {
				template<int i> struct Code {
					inline static void
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) {
						ret.vpvector(i) = vec_ctf(a.vpvector(i), 0);
					}
				};
			};
		};
		// vector float -> vector signed int
		template<typename toScalarT, typename fromScalarT>
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>,
			typename ct::enable_if<
				boost::is_same<
					typename scalar_traits<fromScalarT>::vpvector_type,
					__vector float>::value &&
				boost::is_same<
					typename scalar_traits<toScalarT>::vpvector_type,
					__vector signed int>::value
			>::type
		> {
			inline static vector<toScalarT>
			exec(const vector<fromScalarT>& a) {
				vector<toScalarT> ret;
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<toScalarT> >::vpvector_count,
					+1, do_cts>::exec(ret, a);
				return ret;
			}
		private:
			struct do_cts {
				template<int i> struct Code {
					inline static void
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) {
						ret.vpvector(i) = vec_cts(a.vpvector(i), 0);
					}
				};
			};
		};
		// vector float -> vector unsigned int
		template<typename toScalarT, typename fromScalarT>
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>,
			typename ct::enable_if<
				boost::is_same<
					typename scalar_traits<fromScalarT>::vpvector_type,
					__vector float>::value &&
				boost::is_same<
					typename scalar_traits<toScalarT>::vpvector_type,
					__vector unsigned int>::value
			>::type
		> {
			inline static vector<toScalarT>
			exec(const vector<fromScalarT>& a) {
				vector<toScalarT> ret;
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<toScalarT> >::vpvector_count,
					+1, do_ctu>::exec(ret, a);
				return ret;
			}
		private:
			struct do_ctu {
				template<int i> struct Code {
					inline static void
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) {
						ret.vpvector(i) = vec_ctu(a.vpvector(i), 0);
					}
				};
			};
		};
#define _VVM_DEFINE_VECTOR_CAST_PACK_PACK(FROM, TO) \
		template<typename toScalarT, typename fromScalarT> \
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>, \
			typename ct::enable_if< \
				boost::is_same< \
					typename scalar_traits<fromScalarT>::vpvector_type, \
					FROM>::value && \
				boost::is_same< \
					typename scalar_traits<toScalarT>::vpvector_type, \
					TO>::value \
			>::type \
		> { \
			inline static vector<toScalarT> \
			exec(const vector<fromScalarT>& a) { \
				vector<toScalarT> ret; \
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<toScalarT> >::vpvector_count, \
					+1, do_pack>::exec(ret, a); \
				return ret; \
			} \
		private: \
			struct do_pack { \
				template<int i> struct Code { \
					inline static void \
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) { \
						ret.vpvector(i) = vec_pack( \
								vec_pack(a.vpvector(4*i), a.vpvector(4*i+1)), \
								vec_pack(a.vpvector(4*i+2), a.vpvector(4*i+3))); \
					} \
				}; \
			}; \
		};
		_VVM_DEFINE_VECTOR_CAST_PACK_PACK(__vector signed int, __vector signed char);
		_VVM_DEFINE_VECTOR_CAST_PACK_PACK(__vector unsigned int, __vector unsigned char);
		_VVM_DEFINE_VECTOR_CAST_PACK_PACK(__vector bool int, __vector bool char);
#undef _VVM_DEFINE_VECTOR_CAST_PACK_PACK

#define _VVM_DEFINE_VECTOR_CAST_UNPACK_UNPACK(FROM, IMMEDIATE, TO) \
		template<typename toScalarT, typename fromScalarT> \
		struct vector_cast<vector<toScalarT>, vector<fromScalarT>, \
			typename ct::enable_if< \
				boost::is_same< \
					typename scalar_traits<fromScalarT>::vpvector_type, \
					FROM>::value && \
				boost::is_same< \
					typename scalar_traits<toScalarT>::vpvector_type, \
					TO>::value \
			>::type \
		> { \
			inline static vector<toScalarT> \
			exec(const vector<fromScalarT>& a) { \
				vector<toScalarT> ret; \
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vector<fromScalarT> >::vpvector_count, \
					+1, do_unpack>::exec(ret, a); \
				return ret; \
			} \
		private: \
			struct do_unpack { \
				template<int i> struct Code { \
					inline static void \
					exec(vector<toScalarT>& ret, const vector<fromScalarT>& a) { \
						IMMEDIATE h = vec_unpackh(a.vpvector(i)); \
						IMMEDIATE l = vec_unpackl(a.vpvector(i)); \
						ret.vpvector(i*4) = vec_unpackh(h); \
						ret.vpvector(i*4+1) = vec_unpackl(h); \
						ret.vpvector(i*4+2) = vec_unpackh(l); \
						ret.vpvector(i*4+3) = vec_unpackl(l); \
					} \
				}; \
			}; \
		}
		_VVM_DEFINE_VECTOR_CAST_UNPACK_UNPACK(__vector signed char, __vector signed short, __vector signed int);
		_VVM_DEFINE_VECTOR_CAST_UNPACK_UNPACK(__vector bool char, __vector bool short, __vector bool int);
#undef _VVM_DEFINE_VECTOR_CAST_UNPACK_UNPACK
	
	} // End of priv namespace
	//= AltiVec Only ================================================================
#ifdef VVM_ALTIVEC
	template<typename T>
	inline const offset_t uoffset(T* const p) {
		// AltiVec is 16-byte aligned
		return reinterpret_cast<int_address_t>(p) & 0xf;
	}
	/** Returns an aligned pointer
	 * Aligned pointer is the largest value less than or equal to p+offset.
	 * In other words result is either p+offset, or an aligned pointer smaller
	 * than p+offset.
	 */
	template<typename T>
	inline T* align_prev(T* p, const offset_t offset = 0) {
		// Shift down 4 bits, then up 4 bits again to ensure the least significant
		// 4 bits are 0
		return reinterpret_cast<T*>(((reinterpret_cast<int_address_t>(p) + offset) >> 4) << 4);
	}
	/** Returns an aligned pointer
	 * Aligned pointer is the largest value greater than or equal to p+offset.
	 * In other words result is either p+offset, or an aligned pointer larger
	 * than p+offset.
	 */
	template<typename T>
	inline T* align_next(T* p, const offset_t offset = 0) {
		// Shift down 4 bits, then up 4 bits again to ensure the least significant
		// 4 bits are 0
		T* ret = p + offset;
		return uoffset(ret) == 0 ?
			ret :
			reinterpret_cast<T*>(((reinterpret_cast<int_address_t>(ret) + 16) >> 4) << 4);
	}
#endif
} // End of vvm namespace

#endif
