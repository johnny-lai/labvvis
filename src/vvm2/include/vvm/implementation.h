/*
 *  implementation.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Aug 22 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

/*! @header vvm/vvm.h
 * @abstract
 *   Include this file to gain access to most VVM functions
 * @discussion
 *   This file includes most of VVM functions. To gain access to
 *   VVM's functors, include vvm/functional.h instead.
 *
 *   This VVM implementation comes with an AltiVec vector processor
 *   implementation. To find out how this AltiVec vector processor
 *   implementation maps from VVM functions to AltiVec instructions, see
 *   <a href="XXX">AltiVec Mapping</a>.
 */

#ifndef _VVM_IMPLEMENTATION_H
#define _VVM_IMPLEMENTATION_H

#include <iostream>
#include <ct/enabler.h>
#include <ct/metafunction.h>
#include <ct/meta_meta.h>
#include <ct/tuple.h>
#include <vvm/types.h>
#include <vvm/vector.h>
#include <vvm/traits.h>

namespace vvm {
	//==========================================================================
	// I/O functions
	//--------------------------------------------------------------------------
	template<typename scalarT> std::istream& operator>>(std::istream& is, vector<scalarT>& v)  {
		for(int i = 0; i < vector_traits<vector<scalarT> >::scalar_count; ++i)
			is >> v.scalar(i);
		return is;
	}
	namespace priv_char_output {
		// Enable this version only if T is a char type
		template<typename scalarT>
		typename ct::enable_if<
			ct::contains<priv::char_types, scalarT>::value,
			std::ostream&>::type
		operator<<(std::ostream& os, const vector<scalarT>& v) {
			os << static_cast<short>(v.scalar(0));
			for(int i = 1; i < vector_traits<vector<scalarT> >::scalar_count; ++i)
				os << " " << static_cast<short>(v.scalar(i));
			return os;
		}
	} // End of priv_char_output
	namespace priv_general_output {
		// Use this version of output if scalarT is not a char type
		template<typename scalarT>
		typename ct::disable_if<
			ct::contains<priv::char_types, scalarT>::value,
			std::ostream&>::type
		operator<<(std::ostream& os, const vector<scalarT>& v) {
			os << v.scalar(0);
			for(int i = 1; i < vector_traits<vector<scalarT> >::scalar_count; ++i)
				os << " " << v.scalar(i);
			return os;
		}
	} // End of priv_general_output
	using priv_char_output::operator<<;
	using priv_general_output::operator<<;
} // End of vvm namespace

namespace vvm {
	//= Implementation-Dependent ===============================================
	// These are just prototypes and are defined before implementations so that
	// defaults are picked up by GCC
	/*! @function uoffset
	 * @abstract
	 *   Returns number of bytes after closest aligned position.
	 * @param p Pointer to calculate offset from
	 * @discussion
	 *   The closest aligned position before p is uoffset(p) bytes before.
	 *   Instead of using uoffset(p) to derive an aligned pointer from p,
	 *   use align_prev and align_next functions instead. If uoffset(p) returns
	 *   0, then p is aligned.
	 */
	template<typename scalarT>
	inline const offset_t uoffset(scalarT* const p);
	/*! @function align_prev
	 * @abstract
	 *   Returns the first aligned pointer before or equal to the specified
	 *   location.
	 * @param p Pointer to derive aligned pointer from.
	 * @param offset Number of bytes to offset p by.
	 * @discussion
	 *   Aligned pointer is the largest value less than or equal to p+offset.
	 *   In other words result is either p+offset, or an aligned pointer smaller
	 *   than p+offset.
	 */
	template<typename T>
	inline T* align_prev(T* p, const offset_t offset = 0);
	/*! @function align_next
	 * @abstract
	 *   Returns the first aligned pointer after or equal to the specified
	 *   location.
	 * @param p Pointer to derive aligned pointer from.
	 * @param offset Number of bytes to offet p by.
	 * @discussion
	 *   Aligned pointer is the largest value greater than or equal to p+offset.
	 *   In other words result is either p+offset, or an aligned pointer larger
	 *   than p+offset.
	 */
	template<typename T>
	inline T* align_next(T* p, const offset_t offset = 0);
} // End of vvm namespace

//==============================================================================
// Include appropriate implementations
//------------------------------------------------------------------------------
// Emulation Implementation always on
#include <vvm/emulation_implementation.h>

// Add other vector processor implementations here
#if defined VVM_ALTIVEC
	#include <vvm/altivec_implementation.h>
#elif defined VVM_SCALAR
	#include <vvm/scalar_implementation.h>
#endif

//==============================================================================
// VVM Specification functions
//------------------------------------------------------------------------------

namespace vvm {
	//= Memory =================================================================
	/*! @function load
	 * @abstract
	 *   Loads a vector from memory.
	 * @param p Pointer to load a vvm::vector from.
	 * @discussion
	 *   Please note that this function only returns valid values for aligned
	 *   addresses. For unaligned addresses, the result is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT> load(const scalarT* const p) {
		return priv::load<scalarT>::exec(p);
	}
	template<typename scalarT>
	inline const vector<scalarT> load(const vector<scalarT>* const p) {
		return priv::load<scalarT>::exec(reinterpret_cast<const scalarT* const>(p));
	}
	/*! @function load
	 * @abstract
	 *  Loads a vector from memory. The exact address loaded is given by
	 *  p+offset.
	 * @param p Pointer to load a vvm::vector from.
	 * @param offset Offset in bytes.
	 * @discussion
	 *   Please note that this function only returns valid values when
	 *   p+offset is aligned. For unaligned addresses, the result is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT> load(const scalarT* const p, const offset_t offset) {
		return priv::load<scalarT>::exec(p, offset);
	}
	template<typename scalarT>
	inline const vector<scalarT> load(const vector<scalarT>* const p, const offset_t offset) {
		return priv::load<scalarT>::exec(reinterpret_cast<const scalarT* const>(p), offset);
	}
	/*! @function load
	 * @abstract
	 *   Load a vvm::vector from two vvm::vectors. Used for unaligned loading.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @param offset Index of first scalar of the resultant vvm::vector.
	 *   0 refers to the first scalar of the first vvm::vector a.
	 *   VVM_SCALAR_COUNT refers to the first scalar of the second vvm::vector b.
	 * @discussion
	 *   Use this function to perform unaligned loading.
	 *   Loads a vector from two vectors. The vector loaded is determined by offset
	 *   which is the number of scalars from the left vector.
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	load(const vector<scalarT>& a, const vector<scalarT>& b, const offset_t offset) {
		return priv::load2<scalarT>::exec(a, b, offset);
	}
	/*! @function store
	 * @abstract
	 *   Stores a vvm::vector.
	 * @param a Vvm::vector to store.
	 * @param p Pointer to location that a should be written to.
	 * @discussion
	 *   This function is only valid when p is aligned.
	 */
	template<typename scalarT>
	inline void store(const vector<scalarT>& a, scalarT* const p) {
		priv::store<scalarT>::exec(a, p);
	}
	template<typename scalarT>
	inline void store(const vector<scalarT>& a, vector<scalarT>* const p) {
		priv::store<scalarT>::exec(a, reinterpret_cast<scalarT* const>(p));
	}
	/*! @function store
	 * @abstract
	 *   Stores a vvm::vector.
	 * @param a Vvm::vector to store.
	 * @param p Pointer to location that a should be written to.
	 * @param offset Offset to p in number of bytes.
	 * @discussion
	 *   This function is only valid when p+offset is aligned.
	 */
	template<typename scalarT>
	inline void store(const vector<scalarT>& a, scalarT* const p, const offset_t offset) {
		priv::store<scalarT>::exec(a, p, offset);
	}
	template<typename scalarT>
	inline void store(const vector<scalarT>& a, vector<scalarT>* const p, const offset_t offset) {
		priv::store<scalarT>::exec(a, reinterpret_cast<scalarT* const>(p), offset);
	}
	/*! @function store
	 * @abstract
	 *   Store from a vvm::vector, derived from two vvm::vectors, to memory.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @param p Pointer to location to store the resultant vvm::vector to.
	 * @param offset Index of first scalar of the resultant vvm::vector.
	 *   0 refers to the first scalar of the first vvm::vector a.
	 *   VVM_SCALAR_COUNT refers to the first scalar of the second vvm::vector b.
	 * @discussion
	 *   Stores a vvm::vector, that is derives from two other vvm::vectors.
	 *   This function is used to make unaligned store easier. The vvm::vector
	 *   written is determined by offset which is the number of scalars from the left vector
	 */
	template<typename scalarT>
	inline void store_unaligned(const vector<scalarT>& a, scalarT* const p, const offset_t offset) {
		priv::store_unaligned<scalarT>::exec(a, p, offset);
	}
	template<typename scalarT>
	inline void store_unaligned(const vector<scalarT>& a, vector<scalarT>* const p, const offset_t offset) {
		priv::store_unaligned<scalarT>::exec(a, reinterpret_cast<scalarT* const>(p), offset);
	}
	/*! @function prefetch
	 * @abstract
	 *   Starts prefetching.
	 * @templatefield channel The prefetch channel to use.
	 * @templatefield usage
	 *   A hint describing usage pattern of data being prefetched.
	 *   read_only: Use this when the program expects to read only many times.
	 *   read_write: Use this when the program expects to read and write many times.
	 *   read_only_transient: Use this when the program expected to read only
	 *     once.
	 *   read_write_transient: Use this when the program expected to read and
	 *     write many times.
	 * @param addr Approximate memory location to prefetch to the cache.
	 * @param count Approximate number of bytes to prefetch.
	 */
	template<int channel, typename usage, typename T>
	inline typename ct::enable_if<
		ct::contains<priv::all_usage_types, usage>::value,
  		void>::type
	prefetch(T* const addr, const int count) {
		typedef typename boost::mpl::if_c<
			vector_traits<T>::is_specialized,
			typename vector_traits<T>::scalar_type,    // T is a vector
			T    // T is not a vector
			>::type scalar_type;
		priv::prefetch<channel, usage, scalar_type>::exec(addr, count);
	}
	//= Arithmetic =============================================================
	/*! @function operator+
	 * @abstract
	 *   Adds two vvm::vectors.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @disccussion
	 *   Adds two vvm::vectors and returns the result.
	 *   On overflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline static const vector<scalarT>
	operator+(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::add<scalarT>::exec(a, b);
	}
	/*! @function adds
	 * @abstract
	 *   Saturated Addition.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @discussion
	 *   Adds two vvm::vectors (saturated) and returns the result.
	 *   On overflow, the return value is the largest possible value.
	 */
	template<typename scalarT>
	inline static const vector<scalarT>
	adds(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::adds<scalarT>::exec(a, b);
	}
	/*! @function operator-
	 * @abstract 
	 *   Subtracts one vvm::vector from another.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @discussion
	 *   Subtracts b from a and returns the result.
	 *   On underflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline static const vector<scalarT>
	operator-(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::sub<scalarT>::exec(a, b);
	}
	/*! @function subs
	 * @abstract 
	 *   Saturated Subtraction
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @discussion
	 *   Subtracts b from a and returns the result.
	 *   On underflow, the return value is smallest possible value.
	 */
	template<typename scalarT>
	inline static const vector<scalarT>
	subs(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::subs<scalarT>::exec(a, b);
	}
	/*! @function operator*
	 * @abstract
	 *   Multiples two vvm::vectors.
	 * @param a First vvm::vector.
	 * @param b Second vvm::vector.
	 * @discussion
	 *   Multiplies a and b and returns the result.
	 *   On overflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	operator*(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mul<scalarT>::exec(a, b);
	}
	/*! @function operator/
	 * @abstract
	 *   Division
	 * @param a Dividend
	 * @param b Divisor
	 * @discussion
	 *   Return result of a / b. On overflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	operator/(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::div<scalarT>::exec(a, b);
	}
	/*! @function operator%
	 * @abstract
	 *   Modulus
	 * @param a Dividend
	 * @param b Divisor
	 * @discussion
	 *   Return result of a % b. This operation is only defined for integers.
	 */
	template<typename scalarT>
	inline typename ct::enable_if<boost::is_integral<scalarT>::value, const vector<scalarT> >::type
	operator%(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mod<scalarT>::exec(a, b);
	}
	/*! @function operator++
	 * @abstract
	 *   Pre-increment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns ++a. On overflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator++(vector<scalarT>& a) {
		return priv::pre_inc<scalarT>::exec(a);
	}
	/*! @function operator++
	 * @abstract
	 *   Post-increment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a++. On overflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator++(vector<scalarT>& a, int) {
		return priv::post_inc<scalarT>::exec(a);
	}
	/*! @function operator--
	 * @abstract
	 *   Pre-decrement
	 * @param a Variable to increment
	 * @discussion
	 *   Returns --a. On underflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator--(vector<scalarT>& a) {
		return priv::pre_dec<scalarT>::exec(a);
	}
	/*! @function operator--
	 * @abstract
	 *   Post-decrement
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a--. On underflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator--(vector<scalarT>& a, int) {
		return priv::post_dec<scalarT>::exec(a);
	}
	/*! @function operator+=
	 * @abstract
	 *   Addition/assignment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a += b. On overflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator+=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::add_assign<scalarT>::exec(a, b);
	}
	/*! @function operator-=
	 * @abstract
	 *   Subtraction/assignment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a -= b. On underflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator-=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::sub_assign<scalarT>::exec(a, b);
	}
	/*! @function operator*=
	 * @abstract
	 *   Multiplication/assignment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a *= b. On overflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator*=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mul_assign<scalarT>::exec(a, b);
	}
	/*! @function operator/=
	 * @abstract
	 *   Division/assignment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a /= b. On overflow, the result is undefined.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator/=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::div_assign<scalarT>::exec(a, b);
	}
	/*! @function operator%=
	 * @abstract
	 *   Modulus/assignment
	 * @param a Variable to increment
	 * @discussion
	 *   Returns a %= b. This operation is only defined for integers.
	 */
	template<typename scalarT>
	inline typename ct::enable_if<boost::is_integral<scalarT>::value, vector<scalarT>&>::type
	operator%=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mod_assign<scalarT>::exec(a, b);
	}
	/*! Negate operator
	 * Returns -a
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	operator-(const vector<scalarT>& a) {
		return priv::negate<scalarT>::exec(a);
	}
	/*! Unary + operator
	 * Returns +a
	 */
	template<typename scalarT>
	inline const vector<scalarT>&
	operator+(const vector<scalarT>& a) {
		return priv::positive<scalarT>::exec(a);
	}
	/*! Multiply and Add
	 * On overflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	madd(const vector<scalarT>& a, const vector<scalarT>& b, const vector<scalarT>& c) {
		return priv::madd<scalarT>::exec(a, b, c);
	}
	/*! Negate Multipy and Subtract
	 * On overflow, the return value is undefined.
	 */
	template<typename scalarT>
	inline const vector<scalarT>
	nmsub(const vector<scalarT>& a, const vector<scalarT>& b, const vector<scalarT>& c) {
		return priv::nmsub<scalarT>::exec(a, b, c);
	}
	//= Bitwise ================================================================
	/*! One's complement
	 * Returns ~a
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator~(const vector<scalarT>& a) {
		return priv::ones_complement<scalarT>::exec(a);
	}
	/*! Bitwise Exclusive OR
	 * Returns a^b
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator^(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_xor<scalarT>::exec(a, b);
	}
	/*! Bitwise AND
	 * Returns a&b
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator&(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_and<scalarT>::exec(a, b);
	}
	/*! Bitwise OR
	 * Returns a|b
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator|(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_or<scalarT>::exec(a, b);
	}
	/*! Bitwise Exclusive OR and assignment
	 * Returns a^=b
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator^=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_xor_assign<scalarT>::exec(a, b);
	}
	/*! Bitwise AND and assignment
	 * Returns a&=b
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator&=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_and_assign<scalarT>::exec(a, b);
	}
	/*! Bitwise OR and assignment
	 * Returns a|=b
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator|=(vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::bitwise_or_assign<scalarT>::exec(a, b);
	}
	//= Shift ==================================================================
	/*! Shift Left Entire Vector
	 * Returns a << shift where shift refers to the number of bits to shift left
	 * shift cannot be negative and is undefined if it is larger than the size of
	 * the vector.
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator<<(const vector<scalarT>& a, const unsigned char shift) {
		return priv::shift_left<scalarT>::exec(a, shift);
	}
	/*! Shift Left Entire Vector and Assignment
	 * Returns a << shift where shift refers to the number of bits to shift left
	 * shift cannot be negative and is undefined if it is larger than the size of
	 * the vector.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator<<=(vector<scalarT>& a, const unsigned char shift) {
		return priv::shift_left_assign<scalarT>::exec(a, shift);
	}
	/*! Shift Right Entire Vector
	 * Returns a >> shift where shift refers to the number of bits to shift right
	 * shift cannot be negative and is undefined if it is larger than the size of
	 * the vector.
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator>>(const vector<scalarT>& a, const unsigned char shift) {
		return priv::shift_right<scalarT>::exec(a, shift);
	}
	/*! Shift Right Entire Vector and Assignment
	 * Returns a >> shift where shift refers to the number of bits to shift right
	 * shift cannot be negative and is undefined if it is larger than the size of
	 * the vector.
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator>>=(vector<scalarT>& a, const unsigned char shift) {
		return priv::shift_right_assign<scalarT>::exec(a, shift);
	}
	/*! Shift Left Elements in Vector
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator<<(const vector<scalarT>& a, const vector<scalarT>& shift) {
		return priv::shift_left_elements<scalarT>::exec(a, shift);
	}
	/*! Shift Left Elements in Vector and Assignment
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator<<=(vector<scalarT>& a, const vector<scalarT>& shift) {
		return priv::shift_left_elements_assign<scalarT>::exec(a, shift);
	}
	/*! Shift Right Elements in Vector
	 */
	template<typename scalarT>
	inline vector<scalarT>
	operator>>(const vector<scalarT>& a, const vector<scalarT>& shift) {
		return priv::shift_right_elements<scalarT>::exec(a, shift);
	}
	/*! Shift Right Elements in Vector and Assignment
	 */
	template<typename scalarT>
	inline vector<scalarT>&
	operator>>=(vector<scalarT>& a, const vector<scalarT>& shift) {
		return priv::shift_right_elements_assign<scalarT>::exec(a, shift);
	}
	//= Logical ================================================================
	/*! Logical Not
	 * Returns !a
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator!(const vector<scalarT>& a) {
		return priv::logical_not<scalarT>::exec(a);
	}
	/*! Logical Inclusive OR
	 * Returns a||b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator||(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::logical_or<scalarT>::exec(a, b);
	}
	/*! Logical AND
	 * Returns a&&b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator&&(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::logical_and<scalarT>::exec(a, b);
	}
	//= Comparison =============================================================
	/*! Logical Less Than operator
	 * Returns a<b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator<(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::less<scalarT>::exec(a, b);
	}
	/*! Logical Greater Than operator
	 * Returns a>b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator>(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::greater<scalarT>::exec(a, b);
	}
	/*! Logical Less Than Or Equals To operator
	 * Returns a<=b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator<=(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::less_equal<scalarT>::exec(a, b);
	}
	/*! Logical Greater Than Or Equals To operator
	 * Returns a>=b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator>=(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::greater_equal<scalarT>::exec(a, b);
	}
	/*! Equalilty operator
	 * Returns a==b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator==(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::equal<scalarT>::exec(a, b);
	}
	/*! Inequalilty operator
	 * Returns a!=b
	 */
	template<typename scalarT>
	inline vector<typename scalar_traits<scalarT>::bool_type>
	operator!=(const vector<scalarT>& a, const vector<scalarT>&b) {
		return priv::not_equal<scalarT>::exec(a, b);
	}
	//= Constant ===============================================================
	// TODO: Keep constant?
	template<typename scalarT, int value>
	vector<scalarT> constant() {
		return priv::constant<scalarT, value>::exec();
	}
	//= Vector =================================================================
	/*
	/// Rotate Left
	template<typename scalarT>
	inline vector<scalarT> rotate_left(const vector<scalarT>& a, const offset_t shift) {
		return priv::rotate_left<scalarT>::exec(a, shift);
	}
	/// Rotate Right
	template<typename scalarT>
	inline vector<scalarT> rotate_right(const vector<scalarT>& a, const offset_t shift) {
		return priv::rotate_right<scalarT>:exec(a, shift);
	}
	*/
	/*! Vector Load for Left Shift
	 * Equivalent to vec_lvsl
	 */
	template<typename scalarT>
	inline vector<scalarT> lvsl(const scalarT* const p, const offset_t offset = 0) {
		return priv::lvsl<scalarT>::exec(p, offset);
	}
	template<typename scalarT>
	inline vector<scalarT> lvsl(const vector<scalarT>* const p, const offset_t offset = 0) {
		return priv::lvsl<scalarT>::exec(reinterpret_cast<const scalarT* const>(p), offset);
	}
	/*! Vector Load for Right Shift
	 * Equivalent to vec_lvsr
	 */
	template<typename scalarT>
	inline vector<scalarT> lvsr(const scalarT* const p, const offset_t offset = 0) {
		return priv::lvsr<scalarT>::exec(p, offset);
	}
	template<typename scalarT>
	inline vector<scalarT> lvsr(const vector<scalarT>* const p, const offset_t offset = 0) {
		return priv::lvsr<scalarT>::exec(reinterpret_cast<const scalarT* const>(p), offset);
	}
	/*! Permutation
	 */
	template<typename scalarT>
	inline vector<scalarT> perm(const vector<scalarT>& a, const vector<scalarT>& b, const vector<scalarT>& c) {
		return priv::perm<scalarT>::exec(a, b, c);
	}
	/*! Select
	 * Each bit is used to decide which bit the output vector should use.
	 * 0 selects the first vector, 1 selects the second
	 */
	template<typename scalarT>
	inline vector<scalarT> select(const vector<typename scalar_traits<scalarT>::bool_type>& mask,
	const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::select<scalarT>::exec(mask, a, b);
	}	
	/*! Merge High
	 * Merges more significant scalars of a and b
	 */
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::integer_types, scalarT>::value, const vector<scalarT> >::type
	mergeh(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mergeh<scalarT>::exec(a, b);
	}
	/*! Merge Low
	 * Merges less signficiant scalars of a and b
	 */
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::integer_types, scalarT>::value, const vector<scalarT> >::type
	mergel(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::mergel<scalarT>::exec(a, b);
	}
	/*! Vector Minimum
	 * Returns vector with smaller of a and b
	 */
	template<typename scalarT>
	vector<scalarT> min(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::min<scalarT>::exec(a, b);
	}
	/*! Vector Maximum
	 * Returns vector with larger of a and b
	 */
	template<typename scalarT>
	vector<scalarT> max(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::max<scalarT>::exec(a, b);
	}
	/*! Sum
	 * Returns the sum of all scalars in the vector
	 */
	template<typename scalarT>
	typename ct::promote<scalarT>::type sum(const vector<scalarT>& a) {
		return priv::sum<scalarT>::exec(a);
	}

	//= Predicates =============================================================
	/*! All Equal Predicate
	 * Returns true if all a == b is all true
	 */
	template<typename scalarT>
	bool all_eq(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_eq<scalarT>::exec(a, b);
	}
	/*! All Greater Than or Equal Predicate
	 * Returns true if all a >= b is all true
	 */
	template<typename scalarT>
	bool all_ge(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_ge<scalarT>::exec(a, b);
	}
	/*! All Greater Than Predicate
	 * Returns true if all a > b is all true
	 */
	template<typename scalarT>
	bool all_gt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_gt<scalarT>::exec(a, b);
	}
	/*! All Less Than or Equal Predicate
	 * Returns true if all a >= b is all true
	 */
	template<typename scalarT>
	bool all_le(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_le<scalarT>::exec(a, b);
	}
	/*! All Less Than Predicate
	 * Returns true if all a > b is all true
	 */
	template<typename scalarT>
	bool all_lt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_lt<scalarT>::exec(a, b);
	}
	/*! All Not Equal Predicate
	 * Returns true if all a == b is all false
	 */
	template<typename scalarT>
	bool all_ne(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_ne<scalarT>::exec(a, b);
	}
	/*! All Not Greater Than or Equal Predicate
	 * Returns true if all a >= b is all false
	 */
	template<typename scalarT>
	bool all_nge(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_nge<scalarT>::exec(a, b);
	}
	/*! All Not Greater Than Predicate
	 * Returns true if all a > b is all false
	 */
	template<typename scalarT>
	bool all_ngt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_ngt<scalarT>::exec(a, b);
	}
	/*! All Not Less Than or Equal Predicate
	 * Returns true if all a >= b is all false
	 */
	template<typename scalarT>
	bool all_nle(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_nle<scalarT>::exec(a, b);
	}
	/*! All Not Less Than Predicate
	 * Returns true if all a > b is all false
	 */
	template<typename scalarT>
	bool all_nlt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::all_nlt<scalarT>::exec(a, b);
	}
	/*! Any Equal
	 * Returns true if any a == b is true
	 */
	template<typename scalarT>
	bool any_eq(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_eq<scalarT>::exec(a, b);
	}
	/*! Any Greater Than or Equal
	 * Returns true if any a >= b is true
	 */
	template<typename scalarT>
	bool any_ge(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_ge<scalarT>::exec(a, b);
	}
	/*! Any Greater Than
	 * Returns true if any a > b is true
	 */
	template<typename scalarT>
	bool any_gt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_gt<scalarT>::exec(a, b);
	}
	/*! Any Less Than or Equal
	 * Returns true if any a == b is true
	 */
	template<typename scalarT>
	bool any_le(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_le<scalarT>::exec(a, b);
	}
	/*! Any Less Than
	 * Returns true if any a == b is true
	 */
	template<typename scalarT>
	bool any_lt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_lt<scalarT>::exec(a, b);
	}
	/*! Any Not Equal
	 * Returns true if any !(a == b) is true
	 */
	template<typename scalarT>
	bool any_ne(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_ne<scalarT>::exec(a, b);
	}
	/*! Any Not Greater Than or Equal
	 * Returns true if any !(a >= b) is true
	 */
	template<typename scalarT>
	bool any_nge(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_nge<scalarT>::exec(a, b);
	}
	/*! Any Not Greater Than
	 * Returns true if any !(a > b) is true
	 */
	template<typename scalarT>
	bool any_ngt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_ngt<scalarT>::exec(a, b);
	}
	/*! Any Not Less Than or Equal
	 * Returns true if any !(a <= b) is true
	 */
	template<typename scalarT>
	bool any_nle(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_nle<scalarT>::exec(a, b);
	}
	/*! Any Not Less Than
	 * Returns true if any !(a < b) is true
	 */
	template<typename scalarT>
	bool any_nlt(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::any_nlt<scalarT>::exec(a, b);
	}
	//= Casting ================================================================
	template<typename toVectorT, typename fromVectorT>
	toVectorT vector_cast(const fromVectorT& from) {
		return priv::vector_cast<toVectorT, fromVectorT>::exec(from);
	}

	//==========================================================================
	// VVM Math functions
	//--------------------------------------------------------------------------
	/// Absolute value
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	abs(const vector<scalarT>& a) {
		return priv::abs<scalarT>::exec(a);
	}
	/// ArcCosine
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	acos(const vector<scalarT>& a) {
		return priv::acos<scalarT>::exec(a);
	}
	/// ArcSine
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	asin(const vector<scalarT>& a) {
		return priv::asin<scalarT>::exec(a);
	}
	/// ArcTangent
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	atan(const vector<scalarT>& a) {
		return priv::atan<scalarT>::exec(a);
	}
	/// ArcTangent2
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	atan2(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::atan2<scalarT>::exec(a, b);
	}
	/// Ceiling
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	ceil(const vector<scalarT>& a) {
		return priv::ceil<scalarT>::exec(a);
	}
	/// Cosine
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	cos(const vector<scalarT>& a) {
		return priv::cos<scalarT>::exec(a);
	}
	/// Cosh
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	cosh(const vector<scalarT>& a) {
		return priv::cosh<scalarT>::exec(a);
	}
	/// Exponential
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	exp(const vector<scalarT>& a) {
		return priv::exp<scalarT>::exec(a);
	}
	/// Floor
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	floor(const vector<scalarT>& a) {
		return priv::floor<scalarT>::exec(a);
	}
	/// Float Modulus
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	fmod(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::fmod<scalarT>::exec(a, b);
	}
	/// Natural Logarithm
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	log(const vector<scalarT>& a) {
		return priv::log<scalarT>::exec(a);
	}
	/// Base 10 Logarithm
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	log10(const vector<scalarT>& a) {
		return priv::log10<scalarT>::exec(a);
	}
	/// Power
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	pow(const vector<scalarT>& a, const vector<scalarT>& b) {
		return priv::pow<scalarT>::exec(a, b);
	}
	/// Power by Integer
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	pow(const vector<scalarT>& a, const vector_int& b) {
		return priv::pow_int<scalarT>::exec(a, b);
	}
	/// Sine
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	sin(const vector<scalarT>& a) {
		return priv::sin<scalarT>::exec(a);
	}
	/// Sinh
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	sinh(const vector<scalarT>& a) {
		return priv::sinh<scalarT>::exec(a);
	}
	/// Square Root
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	sqrt(const vector<scalarT>& a) {
		return priv::sqrt<scalarT>::exec(a);
	}
	/// Tangent
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	tan(const vector<scalarT>& a) {
		return priv::tan<scalarT>::exec(a);
	}
	/// Tanh
	template<typename scalarT>
	typename ct::enable_if<ct::contains<priv::real_types, scalarT>::value, vector<scalarT> >::type
	tanh(const vector<scalarT>& a) {
		return priv::tanh<scalarT>::exec(a);
	}
} // End of vvm namespace


#endif
