/*
 *  metafunction.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Fri Aug 22 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

#ifndef _CT_METAFUNCTION_H
#define _CT_METAFUNCTION_H

#include <limits>
#include <ct/typelist.h>
#include <ct/boost_type_traits.h>

namespace ct {
	namespace priv {
		// Signed integer types
		typedef ct::cons_tl<
			signed char,
			signed short int,
			signed int,
			signed long int>::type signed_integer_tl;
		// Unsigned integer types
		typedef ct::cons_tl<
			unsigned char,
			unsigned short int,
			unsigned int,
			unsigned long int>::type unsigned_integer_tl;
		// Real types
		typedef ct::cons_tl<float, double, long double>::type real_tl;
	} // End of priv namespace
	
	//- is_signed --------------------------------------------------------------
	/// Checks if a type is signed
	template<typename T> struct is_signed {
		enum { value = std::numeric_limits<T>::is_signed };
	};
	
	//- is_unsigned ------------------------------------------------------------
	/// Checks if a type is unsigned
	template<typename T> struct is_unsigned {
		enum { value = !std::numeric_limits<T>::is_signed };
	};
		
	//- has_sizeof -------------------------------------------------------------
	/// Checks if a type is a specific size
	template<int size> struct has_sizeof {
		template<typename T> struct func {
			enum { value = (sizeof(T) == size) };
		};
	};
	
	//- has_larger_sizeof ------------------------------------------------------
	/// Checks if a type is larger than a specific size
	template<int size> struct has_larger_sizeof {
		template<typename T> struct func {
			enum { value = (sizeof(T) > size) };
		};
	};
	
	//- logical_not ------------------------------------------------------------
	/// Inverts return value of supplied metafunction
	template<template<typename> class exprT> struct logical_not {
		template<typename T> struct func {
			enum { value = !exprT<T>::value };
		};
	};
	
	//- remove_all -------------------------------------------------------------
	/// Removes all pointers, references, const, etc
	template<typename T> struct remove_all {
		typedef typename boost::remove_pointer<
			typename boost::remove_const<
				typename boost::remove_reference<
					typename boost::remove_volatile<T>::type
					>::type
				>::type
			>::type type;
	};
	
	//- largest ----------------------------------------------------------------
	template<typename typelistT> struct largest;
	template<> struct largest<null_type> {
		typedef null_type type;
	};
	template<typename headT, typename tailT> struct largest<ct::typelist<headT, tailT> > {
	private:
		typedef typename largest<tailT>::type tail_largest;
	public:
		typedef typename boost::mpl::if_c<
			sizeof(headT) >= sizeof(tail_largest),
			headT,
			tail_largest>::type type;
	};
	
	//- promote ----------------------------------------------------------------
	namespace priv {
		// This is required for GCC 3.3 to compile. It did not like
		// find_first<tl, has_larger_sizeof<sizeof(T)>::func>::type
		// The problem was sizeof(T). If replaced with 4, it would compile
		template<typename typelistT, typename T> struct find_larger;
		template<typename headT, typename tailT, typename T>
		struct find_larger<typelist<headT, tailT>, T> {
			typedef typename boost::mpl::if_c<
				(sizeof(headT) > sizeof(T)),
				headT,
				typename find_larger<tailT, T>::type
				>::type type;
		};
		template<typename T> struct find_larger<null_type, T> {
			typedef null_type type;
		};
	} // End of priv namespace

	/// Returns a larger type than T
	template<typename T> struct promote {
	public:
		typedef typename boost::mpl::if_c<
			boost::is_integral<T>::value,  // Is T an integer?
			typename boost::mpl::if_c<
				std::numeric_limits<T>::is_signed,   // Is T a signed integer?
				priv::signed_integer_tl,  // T is a signed integer
				priv::unsigned_integer_tl // T is an unsigned integer
				>::type,
			typename boost::mpl::if_c<
				boost::is_float<T>::value,	// Is T a real?
				priv::real_tl,	// T is a real
				null_type		// T is unknown -- cannot promote
				>::type
			>::type result_tl;
		typedef typename priv::find_larger<	
			result_tl,  // Typelist to look for type in
			T			// Look for first type larger than T
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			T,				// If NOT, then use T
			promoted_type	// Otherwise use larger_type
			>::type type;
	};
	/** Returns a larger type than all the types in the typelist
	 * If(all types are integers)
	 *   If(all types are unsigned)
	 *     Resulting Type is an unsigned integer
	 *   Else
	 *     Resulting Type is a signed integer
	 *   End If
	 * Else If(all types are arithmetic types)
	 *   Resulting Type is a real
	 * Else
	 *   Resulting Type is unknown (so just return largest type in typelist)
	 * End If
	 */
	template<typename headT, typename tailT>
	struct promote<ct::typelist<headT, tailT> > {
	private:
		typedef ct::typelist<headT, tailT> tl;
		typedef typename boost::mpl::if_c<
			all<tl, boost::is_integral>::value,
			typename boost::mpl::if_c<				// All types are integers
				all<tl, is_unsigned>::value,
				priv::unsigned_integer_tl,			// Result is an unsigned integer
				priv::signed_integer_tl				// Result is a signed integer
			>::type,
			typename boost::mpl::if_c<
				all<tl, boost::is_arithmetic>::value,
				priv::real_tl,	// Result is a real
				tl				// Result is unknown -- just use original typelist
				>::type
			>::type result_tl;
		typedef typename largest<tl>::type largest_type;
		typedef typename priv::find_larger<	
			result_tl,
			largest_type
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			largest_type,	// If NOT, then use largest_type
			promoted_type	// Otherwise use promoted_type
			>::type type;
	};
	//- promote_signed ---------------------------------------------------------
	/// Returns a larger signed type than T
	template<typename T> struct promote_signed {
	private:
		typedef typename boost::mpl::if_c<
			boost::is_integral<T>::value,  // Is T an integer?
			priv::signed_integer_tl,  // T is a signed integer
			typename boost::mpl::if_c<
				boost::is_float<T>::value,	// Is T a real?
				priv::real_tl,	// T is a real
				null_type		// T is unknown -- cannot promote
				>::type
			>::type result_tl;
		typedef typename largest<result_tl>::type largest_valid_type;
		typedef typename priv::find_larger<result_tl, T
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			typename boost::mpl::if_c<		// If NOT, then check if T is unsigned
				is_unsigned<T>::value,
				largest_valid_type,		// Answer cannot be unsigned, so return largest valid type
				T						// T is signed and larger/as large
			>::type,
			promoted_type		// Otherwise use promoted_type
			>::type type;
	};
	template<typename headT, typename tailT>
	struct promote_signed<ct::typelist<headT, tailT> > {
	private:
		typedef ct::typelist<headT, tailT> tl;
		typedef typename boost::mpl::if_c<
			all<tl, boost::is_integral>::value,
			priv::signed_integer_tl,			// Result is a signed integer
			typename boost::mpl::if_c<
				all<tl, boost::is_arithmetic>::value,
				priv::real_tl,	// Result is a real
				tl				// Result is unknown -- just use original typelist
				>::type
			>::type result_tl;
		typedef typename largest<result_tl>::type largest_valid_type;
		typedef typename largest<tl>::type largest_type;
		typedef typename priv::find_larger<result_tl, largest_type
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			typename boost::mpl::if_c<		// If NOT, then check if T is unsigned
				is_unsigned<largest_type>::value,
				largest_valid_type,		// Answer cannot be unsigned, so return largest valid type
				largest_type			// T is signed and larger/as large
				>::type,
			promoted_type	// Otherwise use promoted_type
			>::type type;
	};
	//- promote_float ----------------------------------------------------------
	/// Returns a larger float type than T
	template<typename T> struct promote_float {
	private:
		typedef typename largest<priv::real_tl>::type largest_valid_type;
		typedef typename priv::find_larger<priv::real_tl, T
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			typename boost::mpl::if_c<		// If NOT, then check if T is unsigned
				boost::is_float<T>::value,
				largest_valid_type,		// Answer cannot be non-float, so return largest valid type
				T						// T is float and larger/as large
			>::type,
			promoted_type		// Otherwise use promoted_type
			>::type type;
	};
	template<typename headT, typename tailT>
	struct promote_float<ct::typelist<headT, tailT> > {
	private:
		typedef ct::typelist<headT, tailT> tl;
		typedef typename largest<priv::real_tl>::type largest_valid_type;
		typedef typename largest<tl>::type largest_type;
		typedef typename priv::find_larger<priv::real_tl, largest_type
			>::type promoted_type;
	public:
		typedef typename boost::mpl::if_c<
			boost::is_same<promoted_type, null_type>::value,	// Was a larger type found?
			typename boost::mpl::if_c<		// If NOT, then check if T is unsigned
				boost::is_float<largest_type>::value,
				largest_valid_type,		// Answer cannot be non-float, so return largest valid type
				largest_type			// T is float and larger/as large
				>::type,
			promoted_type	// Otherwise use promoted_type
			>::type type;
	};
	
	//- promote_if_necessary ---------------------------------------------------
	/** Promote only if necessary
	 * For single types, always returns the same type.
	 * For typelists, 
	 *   If not all types have the same signedness
	 *     Return promoted type
	 *   Else
	 *     Return the largest type in the typelist
	 */
	template<typename T> struct promote_if_necessary {
		typedef T type;
	};
	/** Returns a larger type than all the types in the typelist
	 * If(all types are integers)
	 *   If(all types are unsigned)
	 *     Resulting Type is an unsigned integer
	 *   Else
	 *     Resulting Type is a signed integer
	 *   End If
	 * Else If(all types are arithmetic types)
	 *   Resulting Type is a real
	 * Else
	 *   Resulting Type is unknown (so just return largest type in typelist)
	 * End If
	 */
	template<typename headT, typename tailT>
	struct promote_if_necessary<ct::typelist<headT, tailT> > {
	private:
		typedef ct::typelist<headT, tailT> tl;
		typedef typename largest<tl>::type largest_type;
		typedef typename promote<tl>::type promoted_type;
		static const bool dont_use_promoted_type =
			(ct::all<tl, boost::is_integral>::value &&
				(ct::all<tl, is_signed>::value ||
				 ct::all<tl, is_unsigned>::value)) ||
			(ct::all<tl, boost::is_float>::value &&
				(ct::all<tl, is_signed>::value ||
				 ct::all<tl, is_unsigned>::value));
	public:
		typedef typename boost::mpl::if_c<
			dont_use_promoted_type,
			largest_type,
			promoted_type
			>::type type;
	};
} // End of ct namespace

#endif

