/*
 *  scalar_implementation.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Aug 22 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  Emulation mode is just as fast as the scalar implementation. So no need
 *  to overload anything.
 *
 */

#ifndef _VVM_SCALAR_IMPLEMENTATION_H
#define _VVM_SCALAR_IMPLEMENTATION_H

#include <ct/enabler.h>
#include <boost/type_traits.hpp>

// Include math functions
#include <vvm/scalar_math.h>

namespace vvm {
	namespace priv {
		//- add ----------------------------------------------------------------
		//- add_assign ---------------------------------------------------------
		//- all_eq -------------------------------------------------------------
		//- all_ge -------------------------------------------------------------
		//- all_gt -------------------------------------------------------------
		//- all_le -------------------------------------------------------------
		//- all_lt -------------------------------------------------------------
		//- all_ne -------------------------------------------------------------
		//- all_nge ------------------------------------------------------------
		//- all_ngt ------------------------------------------------------------
		//- all_nle ------------------------------------------------------------
		//- all_nlt ------------------------------------------------------------
		//- any_eq -------------------------------------------------------------
		//- any_ge -------------------------------------------------------------
		//- any_gt -------------------------------------------------------------
		//- any_le -------------------------------------------------------------
		//- any_lt -------------------------------------------------------------
		//- any_ne -------------------------------------------------------------
		//- any_nge ------------------------------------------------------------
		//- any_ngt ------------------------------------------------------------
		//- any_nle ------------------------------------------------------------
		//- any_nlt ------------------------------------------------------------
		//- bitwise_and --------------------------------------------------------
		//- bitwise_and_assign -------------------------------------------------
		//- bitwise_or ---------------------------------------------------------
		//- bitwise_or_assign --------------------------------------------------
		//- bitwise_xor --------------------------------------------------------
		//- bitwise_xor_assign -------------------------------------------------
		//- div ----------------------------------------------------------------
		//- div_assign ---------------------------------------------------------
		//- equal --------------------------------------------------------------
		//- greater ------------------------------------------------------------
		//- greater_equal ------------------------------------------------------
		//- less ---------------------------------------------------------------
		//- less_equal ---------------------------------------------------------
		//- load ---------------------------------------------------------------
		//- load2 --------------------------------------------------------------
		//- logical_and --------------------------------------------------------
		//- logical_not --------------------------------------------------------
		//- logical_or ---------------------------------------------------------
		//- madd ---------------------------------------------------------------
		//- mod ----------------------------------------------------------------
		//- mod_assign ---------------------------------------------------------
		//- mul ----------------------------------------------------------------
		//- mul_assign ---------------------------------------------------------
		//- negate -------------------------------------------------------------
		//- nmsub --------------------------------------------------------------
		//- not_equal ----------------------------------------------------------
		//- ones_complement ----------------------------------------------------
		//- positive -----------------------------------------------------------
		//- post_dec -----------------------------------------------------------
		//- post_inc -----------------------------------------------------------
		//- pre_dec ------------------------------------------------------------
		//- pre_inc ------------------------------------------------------------
		//- splat --------------------------------------------------------------
		//- store --------------------------------------------------------------
		//- store2 -------------------------------------------------------------
		//- sub ----------------------------------------------------------------
		//- sub_assign ---------------------------------------------------------
	} // End of priv namespace
	//= Scalar Only ============================================================
#ifdef VVM_SCALAR
	template<typename T>
	inline const offset_t uoffset(T* const p) {
		// In scalar mode, always aligned
		return 0;
	}
	template<typename T>
	inline T* align_prev(T* p, const offset_t offset) {
		return p + offset;
	}
	template<typename T>
	inline T* align_next(T* p, const offset_t offset) {
		return p + offset;
	}
#endif
} // End of vvm namespace

#endif
