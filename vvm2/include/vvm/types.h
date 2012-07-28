/*
 *  types.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Aug 22 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_TYPES_H
#define _VVM_TYPES_H

#include <ct/boost_type_traits.h>
#include <ct/metafunction.h>
#include <ct/typelist.h>

#define VVM_TRUE ~0
#define VVM_FALSE 0

namespace vvm {
	//===============================================================================
	// Boolean for base types
	//-------------------------------------------------------------------------------
	// Note the use of _priv as opposed to priv
	// This is to prevent GCC from failing when
	//   using namespace vvm;
	//   all_eq(vector<bchar>(), vector<bchar>());
	// GCC seems to look for vvm::priv::all_eq instead of vvm::all_eq if
	// bchar belongs to priv
	namespace _priv {
		/** Wrapper class
		 * Used to allow "True Typedefs"
		 */
		template<typename scalarT, int discriminator> class wrapper {
			scalarT _value;
		public:
			wrapper() {}
			wrapper(const scalarT value) : _value(value) {
			}
			operator scalarT() const {
				return _value;
			}
		};
	} // End of priv namespace
	typedef _priv::wrapper<char, 0> bchar;
	typedef _priv::wrapper<short int, 1> bshort;
	typedef _priv::wrapper<int, 2> bint;
	typedef _priv::wrapper<long int, 3> blong;
	typedef _priv::wrapper<int, 4> bfloat;
	typedef _priv::wrapper<long long, 5> bdouble;
	typedef _priv::wrapper<long long, 6> bldouble;

	//===============================================================================
	// Locating the appropriate scalar
	//-------------------------------------------------------------------------------
	namespace priv {
		// Enumeration of type types
		enum { signed_int, unsigned_int, real, boolean, user_defined };
		// Use a typelist to store list of scalar types
		// This list os then searched to perform vp::vector to scalar matching
		typedef ct::cons_tl<
			char, signed char, unsigned char,
			short int, unsigned short int,
			int, unsigned int,
			long int, unsigned long int,
			float, double, long double,
			long long, unsigned long long,
			bchar, bshort, bint, blong, bfloat, bdouble, bldouble
		>::type fundamental_types;
		typedef ct::keep_only<fundamental_types, boost::is_integral>::type integer_types;
		typedef ct::keep_only<integer_types, ct::is_signed>::type signed_integer_types;
		typedef ct::keep_only<integer_types, ct::logical_not<ct::is_signed>::func>::type unsigned_integer_types;
		typedef ct::keep_only<fundamental_types, boost::is_float>::type real_types;
		typedef ct::cons_tl<
			bchar, bshort, bint, blong, bfloat, bdouble, bldouble
		>::type boolean_types;
		typedef ct::cons_tl<
			char, signed char, unsigned char, bchar
		>::type char_types;
			
		/// Find scalar type that matches type and size
		template<int kind, int size> struct find_scalar;
		template<int size> struct find_scalar<priv::signed_int, size> {
			typedef typename ct::find_first<
				signed_integer_types,
				ct::has_sizeof<size>::template func, int>::type type;
		};
		template<int size> struct find_scalar<priv::unsigned_int, size> {
			typedef typename ct::find_first<
				unsigned_integer_types,
				ct::has_sizeof<size>::template func, unsigned int>::type type;
		};
		template<int size> struct find_scalar<priv::real, size> {
			typedef typename ct::find_first<
				real_types,
				ct::has_sizeof<size>::template func, float>::type type;
		};
		template<int size> struct find_scalar<priv::boolean, size> {
			typedef typename ct::find_first<
				boolean_types,
				ct::has_sizeof<size>::template func>::type type;
		};

		template<typename T> struct get_type {
			enum {
			};
		};
	} // End of priv namespace
	
	//===============================================================================
	// Scalar types with explicit sizes
	//-------------------------------------------------------------------------------
	typedef priv::find_scalar<priv::signed_int,   1>::type sint8;
	typedef priv::find_scalar<priv::unsigned_int, 1>::type uint8;
	typedef priv::find_scalar<priv::boolean,      1>::type bint8;
	typedef priv::find_scalar<priv::signed_int,   2>::type sint16;
	typedef priv::find_scalar<priv::unsigned_int, 2>::type uint16;
	typedef priv::find_scalar<priv::boolean,      2>::type bint16;
	typedef priv::find_scalar<priv::signed_int,   4>::type sint32;
	typedef priv::find_scalar<priv::unsigned_int, 4>::type uint32;
	typedef priv::find_scalar<priv::boolean,      4>::type bint32;
	
	/// Type for offsets
	typedef int offset_t;
	
	/// Type for converting address to integers
	typedef priv::find_scalar<priv::unsigned_int, sizeof(int*)>::type int_address_t;
	
	//===============================================================================
	// Prefetch
	//-------------------------------------------------------------------------------
	struct read_only {};
	struct read_write {};
	struct read_only_transient {};
	struct read_write_transient{};
	
	namespace priv {
		/// Typelist containing all usage types
		typedef ct::cons_tl<read_only, read_write,
			read_only_transient, read_write_transient>::type all_usage_types;
	} // End of priv namespace
	
} // End of vvm namespace

#endif
