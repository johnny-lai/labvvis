/*
 *  emulation_math.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Jan 15 2004.
 *  Copyright (c) 2004 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_EMULATION_MATH_H
#define _VVM_EMULATION_MATH_H

#include <cmath>
#include <ct/enabler.h>
#include <ct/metafunction.h>
#include <ct/boost_type_traits.h>
#include <vvm/base_operation.h>

namespace vvm {
	namespace priv {
		//- abs ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct abs
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			abs<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::abs(a);
			}
		};
		//- acos ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct acos
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			acos<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::acos(a);
			}
		};
		//- asin ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct asin
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			asin<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::asin(a);
			}
		};
		//- atan ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct atan
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			atan<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::atan(a);
			}
		};
		//- atan2 --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct atan2
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			atan2<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a, const scalarT b) {
				ret = std::atan2(a, b);
			}
		};
		//- ceil ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct ceil
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			ceil<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::ceil(a);
			}
		};
		//- cos ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct cos
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			cos<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::cos(a);
			}
		};
		//- cosh ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct cosh
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			cosh<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::cosh(a);
			}
		};
		//- exp ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct exp
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			exp<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::exp(a);
			}
		};
		//- floor --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct floor
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			floor<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::floor(a);
			}
		};
		//- fmod ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct fmod
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			fmod<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a, const scalarT b) {
				ret = std::fmod(a, b);
			}
		};
		//- log ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct log
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			log<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::log(a);
			}
		};
		//- log10 --------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct log10
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			log10<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::log10(a);
			}
		};
		//- pow ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct pow
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			pow<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a, const scalarT b) {
				ret = std::pow(a, b);
			}
		};
		//- pow_int ------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct pow_int
		: public base_binary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			const vector<scalarT>&,
			pow_int<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a, const int b) {
				ret = std::pow(a, b);
			}
		};
		//- sin ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sin
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sin<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::sin(a);
			}
		};
		//- sinh ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sinh
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sinh<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::sinh(a);
			}
		};
		//- sqrt ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct sqrt
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			sqrt<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::sqrt(a);
			}
		};
		//- tan ----------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct tan
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			tan<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::tan(a);
			}
		};
		//- tanh ---------------------------------------------------------------
		template<typename scalarT, typename specializedT = void> struct tanh
		: public base_unary_operation<
			vector<scalarT>,
			const vector<scalarT>&,
			tanh<scalarT, specializedT>,
			scalar_iteration
		> {
			static void op(scalarT& ret, const scalarT& a) {
				ret = std::tanh(a);
			}
		};
	} // End of priv namespace
} // End of vvm namespace

#endif
