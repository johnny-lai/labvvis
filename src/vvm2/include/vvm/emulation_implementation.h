/*
 *  emulation_implementation.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Oct 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_EMULATION_IMPLEMENTATION_H
#define _VVM_EMULATION_IMPLEMENTATION_H

#include <ct/enabler.h>
#include <ct/metafunction.h>
#include <ct/boost_type_traits.h>
#include <vvm/base_operation.h>

// Include Math functions
#include <vvm/emulation_math.h>

namespace vvm {
	namespace priv {
		template<typename scalarT, typename specializedT = void> struct pre_dec;
		template<typename scalarT, typename specializedT = void> struct pre_inc;
		//- add ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct add
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			add<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a + b;
			}
		};
		//- adds ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct adds
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			adds<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				typename ct::promote<scalarT>::type r = a + b;
				ret = r < std::numeric_limits<scalarT>::max() ?
					r : std::numeric_limits<scalarT>::max();
			}
		};
		//- add_assign ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct add_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			add_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a += b;
			}
		};
		//- all_eq -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_eq
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_eq<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && (a == b);
			}
		};
		//- all_ge -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_ge
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ge<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && (a >= b);
			}
		};
		//- all_gt -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_gt
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_gt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && (a > b);
			}
		};
		//- all_le -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_le
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_le<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && (a <= b);
			}
		};
		//- all_lt -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_lt
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_lt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && (a < b);
			}
		};
		//- all_ne -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_ne
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ne<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && !(a == b);
			}
		};
		//- all_nge ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_nge
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nge<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && !(a >= b);
			}
		};
		//- all_ngt ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_ngt
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_ngt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && !(a > b);
			}
		};
		//- all_nle ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_nle
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nle<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && !(a <= b);
			}
		};
		//- all_nlt ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct all_nlt
		: public base_binary_predicate<
			true,
			const vector<scalarT>&,
			const vector<scalarT>&,
			all_nlt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret && !(a < b);
			}
		};
		//- any_eq -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_eq
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_eq<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || (a == b);
			}
		};
		//- any_ge -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_ge
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ge<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || (a >= b);
			}
		};
		//- any_gt -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_gt
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_gt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || (a > b);
			}
		};
		//- any_le -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_le
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_le<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || (a <= b);
			}
		};
		//- any_lt -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_lt
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_lt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || (a < b);
			}
		};
		//- any_ne -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_ne
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ne<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || !(a == b);
			}
		};
		//- any_nge ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_nge
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nge<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || !(a >= b);
			}
		};
		//- any_ngt ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_ngt
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_ngt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || !(a > b);
			}
		};
		//- any_nle ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_nle
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nle<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || !(a <= b);
			}
		};
		//- any_nlt ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct any_nlt
		: public base_binary_predicate<
			false,
			const vector<scalarT>&,
			const vector<scalarT>&,
			any_nlt<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(bool& ret, const scalarT& a, const scalarT& b) {
				ret = ret || !(a < b);
			}
		};
		//- bitwise_and --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_and
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_and<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a & b;
			}
		};
		//- bitwise_and_assign -------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_and_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_and_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a &= b;
			}
		};
		//- bitwise_or ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_or
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_or<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a | b;
			}
		};
		//- bitwise_or_assign --------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_or_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_or_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a |= b;
			}
		};
		//- bitwise_xor --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_xor
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			bitwise_xor<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a ^ b;
			}
		};
		//- bitwise_xor_assign -------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct bitwise_xor_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			bitwise_xor_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a ^= b;
			}
		};
		//- constant -----------------------------------------------------------
		template<typename scalarT, int value, typename specializedT = void>
		struct constant {
			inline static const vector<scalarT> exec() {
				return vector<scalarT>(value);
			}
		};
		//- div ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct div
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			div<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a / b;
			}
		};
		//- div_assign ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct div_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			div_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a /= b;
			}
		};
		//- equal --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct equal
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			equal<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a == b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- greater ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct greater
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			greater<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a > b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- greater_equal ------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct greater_equal
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			greater_equal<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a >= b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- less ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct less
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			less<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a < b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- less_equal ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct less_equal
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			less_equal<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a <= b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- load ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct load {
			inline static const vector<scalarT>
			exec(const scalarT* const p) {
				return *(reinterpret_cast<const vector<scalarT>* const>(p));
			}
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				//_VVM_WARN("emulation::load");

				return *(reinterpret_cast<const vector<scalarT>*>
					(reinterpret_cast<const uint8*>(p) + offset));
				/*
				vector<scalarT> ret;
				const scalarT* base = reinterpret_cast<const scalarT*>(
					reinterpret_cast<int_address_t>(p) + offset);
				for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
					ret.scalar(i) = base[i];
				}
				return ret;
				*/
			}
		};
		//- load2 --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct load2 {
			inline static const vector<scalarT>
			exec(const vector<scalarT> a, const vector<scalarT> b, const offset_t offset) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				// Check if result is either a or b
				switch(offset) {
				case 0: return a;
				case step: return b;
				}
				// Result is partially a and partially b
				const int left = step - offset;
				const int right = offset;
				vector<scalarT> ret;
				for(int i = 0; i < left; ++i)
					ret.scalar(i) = a.scalar(offset + i);
				for(int i = 0; i < right; ++i)
					ret.scalar(left + i) = b.scalar(i);
				return ret;
			}
		};
		//- logical_and --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct logical_and
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			logical_and<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = (a && b) ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- logical_not --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct logical_not
		: public base_unary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			logical_not<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a) {
				ret = !a ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- logical_or ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct logical_or
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			logical_or<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = (a || b) ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- lvsl ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct lvsl {
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				vector<scalarT> ret;
				const scalarT* base = reinterpret_cast<const scalarT*>(
					reinterpret_cast<int_address_t>(p) + offset);
				for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
					ret.scalar(i) = (offset + i) % (2 * VVM_SCALAR_COUNT);
				}
				return ret;
			}
		};
		//- lvsr ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct lvsr {
			inline static const vector<scalarT>
			exec(const scalarT* const p, const offset_t offset) {
				vector<scalarT> ret;
				const scalarT* base = reinterpret_cast<const scalarT*>(
					reinterpret_cast<int_address_t>(p) + offset);
				const scalarT start = VVM_SCALAR_COUNT - offset;
				for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
					ret.scalar(i) = (start + i) % (2 * VVM_SCALAR_COUNT);
				}
				return ret;
			}
		};
		//- madd ---------------------------------------------------------------
		// TODO: Faster version that uses add<>, mul<> templates
		template<typename scalarT, typename specializedT = void> struct madd
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			madd<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b, const scalarT& c) {
				ret = a * b + c;
			}
		};
		//- max ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct max
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			max<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a > b ? a : b;
			}
		};
		//- mergeh -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mergeh {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& b) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				vector<scalarT> ret;
				for(int i = 0; i < step; i += 2) {
					ret.scalar(i) = a.scalar(i / 2);
				}
				for(int i = 1; i < step; i += 2) {
					ret.scalar(i) = b.scalar(i / 2);
				}
				return ret;
			}
		};
		//- mergel -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mergel {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& b) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				const int a_start = step % 2;
				const int b_start = (step+1) % 2;
				vector<scalarT> ret;
				for(int i = a_start; i < step; i += 2) {
					ret.scalar(i) = a.scalar(i / 2 + (step / 2));
				}
				for(int i = b_start; i < step; i += 2) {
					ret.scalar(i) = b.scalar(i / 2 + (step / 2));
				}
				return ret;
			}
		};
		//- min ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct min 
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			min<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a < b ? a : b;
			}
		};
		//- mod ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mod 
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mod<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a % b;
			}
		};
		//- mod_assign ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mod_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			mod_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a %= b;
			}
		};
		//- mul ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mul 
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			mul<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a * b;
			}
		};
		//- mul_assign ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct mul_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			mul_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a *= b;
			}
		};
		//- negate -------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct negate
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			negate<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a) {
				ret = -a;
			}
		};
		//- nmsub --------------------------------------------------------------
		// TODO: Faster version that uses negate<>, sub<>, mul<> templates
		template<typename scalarT, typename specializedT = void> struct nmsub
		: public base_ternary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			const vector<scalarT>&,
			nmsub<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b, const scalarT& c) {
				ret = -(a * b - c);
			}
		};
		//- not_equal ----------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct not_equal
		: public base_binary_operation<
			vector<typename scalar_traits<scalarT>::bool_type>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			not_equal<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename scalar_traits<scalarT>::bool_type& ret, const scalarT& a, const scalarT& b) {
				ret = a != b ? VVM_TRUE : VVM_FALSE;
			}
		};
		//- ones_complement ----------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct ones_complement
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			ones_complement<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a) {
				ret = ~a;
			}
		};
		//- pack ---------------------------------------------------------------
		template<typename returnT, typename scalarT, typename specializedT = void> struct pack
		: public base_unary_operation<
			vector<returnT>,
			const vector<scalarT>&,
			pack<returnT, scalarT, specializedT>,
			scalar_iteration
		> {
			inline static const returnT op(const scalarT& a) {
				return a & returnT(~0);
			}
		};
		//- perm ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct perm {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& b, const vector<scalarT>& c) {
				vector<scalarT> ret;
				for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
					const scalarT index = c.scalar(i);
					if(index < VVM_SCALAR_COUNT)
						ret.scalar(i) = a.scalar(index);
					else
						ret.scalar(i) = b.scalar(index - VVM_SCALAR_COUNT);
				}
				return ret;
			}
		};
		//- positive -----------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct positive
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			positive<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a) {
				ret = a;
			}
		};
		//- post_dec -----------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct post_dec {
			inline static vector<scalarT>
			exec(vector<scalarT>& a) {
				vector<scalarT> ret = a;
				priv::pre_dec<scalarT>::exec(a);
				return ret;
			}
		};
		//- post_inc -----------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct post_inc {
			inline static vector<scalarT>
			exec(vector<scalarT>& a) {
				vector<scalarT> ret = a;
				priv::pre_inc<scalarT>::exec(a);
				return ret;
			}
		};
		//- prefetch -----------------------------------------------------------
		template<int channel, typename usage, typename scalarT, typename specializedT = void>
		struct prefetch {
			template<typename T>
			inline static void exec(T* const addr, const int count) {
			}
		};
		//- pre_dec ------------------------------------------------------------
		template<typename scalarT, typename specializedT> struct pre_dec {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				meta::EFOR1<0, meta::Less, vector_traits<vector<scalarT> >::scalar_count, +1,
					operation>::exec(a);
				return a;
			}
			struct operation {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret) {
						--ret.scalar(i);
					}
				};
			};
		};
		//- pre_inc ------------------------------------------------------------
		template<typename scalarT, typename specializedT> struct pre_inc {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a) {
				meta::EFOR1<0, meta::Less, vector_traits<vector<scalarT> >::scalar_count, +1,
					operation>::exec(a);
				return a;
			}
			struct operation {
				template<int i> struct Code {
					inline static void exec(vector<scalarT>& ret) {
						++ret.scalar(i);
					}
				};
			};
		};
		//- rotate_left --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct rotate_leftt {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const offset_t shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				vector<scalarT> ret;
				for(int i = 0; i < shift; ++i) {
					ret.scalar(i) = a.scalar(i + shift);
				}
				for(int i = shift; i < step; ++i) {
					ret.scalar(i) = a.scalar(i - shift);
				}
				return ret;
			}
		};
		//- select -------------------------------------------------------------
		namespace priv {
			// For non-floats
			template<typename scalarT, typename specializedT = void> struct do_select {
				inline static const vector<scalarT>
				exec(const vector<typename scalar_traits<scalarT>::bool_type>& mask,
				const vector<scalarT>& a, const vector<scalarT>& b) {
					vector<scalarT> ret;
					for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
						ret.scalar(i) = (~mask.scalar(i) & a.scalar(i)) |
							(mask.scalar(i) & b.scalar(i));
					}
					return ret;
				}
			};
			// For floats
			template<typename scalarT> struct do_select<
				scalarT,
				typename ct::enable_if<
					ct::contains<real_types, scalarT>::value
				>::type	
			> {
				inline static const vector<scalarT>
				exec(const vector<typename scalar_traits<scalarT>::bool_type>& mask,
				const vector<scalarT>& a, const vector<scalarT>& b) {
					typedef typename scalar_traits<scalarT>::integer_type int_t;
					vector<scalarT> ret;
					for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i) {
						int_t temp = (~mask.scalar(i) & *reinterpret_cast<const int_t*>(&a.scalar(i))) |
							(mask.scalar(i) & *reinterpret_cast<const int_t*>(&b.scalar(i)));
						ret.scalar(i) = *reinterpret_cast<scalarT*>(&temp);
					}
					return ret;
				}
			};
		} // End of priv namespace
		template<typename scalarT, typename specializedT = void> struct select {
			inline static const vector<scalarT>
			exec(const vector<typename scalar_traits<scalarT>::bool_type>& mask,
			const vector<scalarT>& a, const vector<scalarT>& b) {
				return priv::do_select<scalarT>::exec(mask, a, b);
			}
		};
		//- shift_left ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_left {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const unsigned char shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				const unsigned short rshift = (sizeof(scalarT)*8) - shift;
				const scalarT& mask = ~0 << rshift;
				vector<scalarT> ret;
				for(int i = 0; i < step - 1; ++i) {
					ret.scalar(i) = (a.scalar(i) << shift) | 
						((a.scalar(i+1) & mask) >> rshift);
				}
				ret.scalar(step - 1) = a.scalar(step - 1) << shift;
				return ret;
			}
		};
		//- shift_left_assign --------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_left_assign {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a, const unsigned char shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				const unsigned short rshift = (sizeof(scalarT)*8) - shift;
				const scalarT& mask = ~0 << rshift;
				// Walk from 0 to step-2 so that there is no need to make a copy of a
				for(int i = 0; i < step - 1; ++i) {
					a.scalar(i) = (a.scalar(i) << shift) | 
						((a.scalar(i+1) & mask) >> rshift);
				}
				a.scalar(step - 1) = a.scalar(step - 1) << shift;
				return a;
			}
		};
		//- shift_left_elements ------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_left_elements {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				vector<scalarT> ret;
				for(int i = 0; i < step; ++i) {
					ret.scalar(i) = a.scalar(i) << shift.scalar(i);
				}
				return ret;
			}
		};
		//- shift_left_elements_assign -----------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_left_elements_assign {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a, const vector<scalarT>& shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				for(int i = 0; i < step; ++i) {
					a.scalar(i) = a.scalar(i) << shift.scalar(i);
				}
				return a;
			}
		};
		//- shift_right --------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_right {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const unsigned char shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				const unsigned short lshift = (sizeof(scalarT)*8) - shift;
				const scalarT& mask = ~0 >> lshift;
				vector<scalarT> ret;
				for(int i = 1; i < step; ++i) {
					ret.scalar(i) = (a.scalar(i) >> shift) |
						((a.scalar(i - 1) & mask) << lshift);
				}
				ret.scalar(0) = a.scalar(0) >> shift;
				return ret;
			}
		};
		//- shift_right_assign -------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_right_assign {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a, const unsigned char shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				const unsigned short lshift = (sizeof(scalarT)*8) - shift;
				const scalarT& mask = ~0 >> lshift;
				// Walk from step-1 to 1 so that there is no need to make a copy of a
				for(int i = step-1; i >= 1; --i) {
					a.scalar(i) = (a.scalar(i) >> shift) |
						((a.scalar(i - 1) & mask) << lshift);
				}
				a.scalar(0) = a.scalar(0) >> shift;
				return a;
			}
		};
		//- shift_right_elements -----------------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_right_elements {
			inline static const vector<scalarT>
			exec(const vector<scalarT>& a, const vector<scalarT>& shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				vector<scalarT> ret;
				for(int i = 0; i < step; ++i) {
					ret.scalar(i) = a.scalar(i) >> shift.scalar(i);
				}
				return ret;
			}
		};
		//- shift_right_elements_assign ----------------------------------------
		template<typename scalarT, typename specializedT = void> struct shift_right_elements_assign {
			inline static vector<scalarT>&
			exec(vector<scalarT>& a, const vector<scalarT>& shift) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				for(int i = 0; i < step; ++i) {
					a.scalar(i) = a.scalar(i) >> shift.scalar(i);
				}
				return a;
			}
		};
		//- splat --------------------------------------------------------------
		// No default for specializedT because forward declared in vector.h
		template<typename scalarT, typename specializedT> struct splat {
			inline static void
			exec(vector<scalarT>& dest, const scalarT& a) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				for(int i = 0; i < step; ++i) {
					dest.scalar(i) = a;
				}
			}
		};
		//- store --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct store {
			inline static void
			exec(const vector<scalarT> a, scalarT* const p) {
				memcpy(
					reinterpret_cast<void* const>(p),
					&a,
					sizeof(vector<scalarT>));
			}
			inline static void
			exec(const vector<scalarT> a, scalarT* const p, const offset_t offset) {
				memcpy(
					reinterpret_cast<void* const>(reinterpret_cast<uint8* const>(p) + offset),
					&a,
					sizeof(vector<scalarT>));
			}
		};
		//- store2 --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct store_unaligned {
			inline static void
			exec(const vector<scalarT> a, scalarT* const p, const offset_t offset) {
				const int step = vector_traits<vector<scalarT> >::scalar_count;
				// Store unaligned
				for(int i = 0; i < step; ++i)
					p[i] = a.scalar(i);
			}
		};
		//- sub ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sub
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			sub<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				ret = a - b;
			}
		};
		//- subs ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct subs
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			subs<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& ret, const scalarT& a, const scalarT& b) {
				typename ct::promote_signed<scalarT>::type r = a - b;
				ret = r > std::numeric_limits<scalarT>::min() ?
					r : std::numeric_limits<scalarT>::min();
			}
		};
		//- sub_assign ---------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sub_assign
		: public base_assign_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sub_assign<scalarT, specializedT>,
			scalar_iteration
		> {
			inline static void op(scalarT& a, const scalarT& b) {
				a -= b;
			}
		};
		//- sum ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sum {
			inline static typename ct::promote<scalarT>::type
			exec(const vvm::vector<scalarT>& a) {
				typename ct::promote<scalarT>::type r = 0;
				meta::EFOR2<0, meta::Less, vvm::vector_traits<vvm::vector<scalarT> >::scalar_count,
					+1, do_sum>::exec(r, a);
				return r;
			}
		private:
			struct do_sum {
				template<int i> struct Code {
					inline static void
					exec(typename ct::promote<scalarT>::type& r, const vvm::vector<scalarT>& v) {
						r += v.scalar(i);
					}
				};
			};
		};
		//- vector_cast --------------------------------------------------------
		template<typename toVectorT, typename fromVectorT, typename specializedT = void>
		struct vector_cast
		: public base_unary_operation<
			toVectorT,
			const fromVectorT&,
			vector_cast<toVectorT, fromVectorT, specializedT>,
			scalar_iteration
		> {
			inline static void
			op(typename vvm::vector_traits<toVectorT>::scalar_type& ret, const typename vvm::vector_traits<fromVectorT>::scalar_type a) {
				ret = static_cast<typename vvm::vector_traits<toVectorT>::scalar_type>(a);
			}
		};  
		// Specialization for vector_casts requiring no casting
		template<typename toVectorT, typename fromVectorT>
		struct vector_cast<toVectorT, fromVectorT,
			typename ct::enable_if<
				boost::is_same<toVectorT, fromVectorT>::value
			>::type
		> {
			inline static toVectorT exec(const fromVectorT& a) {
				return a;
			}
		};
		// Specialization for vector_casts for ct::tuples
		template<typename toVectorTL, typename fromVectorTL>
		struct vector_cast<ct::tuple<toVectorTL>, ct::tuple<fromVectorTL> > {
			inline static ct::tuple<toVectorTL> exec(const ct::tuple<fromVectorTL>& a) {
				ct::tuple<toVectorTL> ret;
				meta::EFOR2<0, meta::Less, ct::length<toVectorTL>::value, +1,
					do_vector_cast>::exec(ret, a);
				return ret;
			}
		private:
			struct do_vector_cast {
				template<int i> struct Code {
					inline static void exec(ct::tuple<toVectorTL>& ret, const ct::tuple<fromVectorTL>& a) {
						typedef typename ct::type_at<toVectorTL, i>::type to_vector_type;
						typedef typename ct::type_at<fromVectorTL, i>::type from_vector_type;
						ret.template get<i>() = vector_cast<
							to_vector_type,
							from_vector_type>::exec(a.template get<i>());
					}
				};
			};
		};
	} // End of priv namespace
} // End of vvm namespace

#endif
