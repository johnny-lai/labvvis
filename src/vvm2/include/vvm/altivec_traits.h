/*
 *  base_traits.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Sat Aug 23 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_base_TRAITS_H
#define _VVM_base_TRAITS_H

#include <ct/typelist.h>
#include <ct/metafunction.h>

namespace vvm {
	//====================================================================================
	// Locating the appropriate AltiVec vp::vector
	//------------------------------------------------------------------------------------
	namespace priv {
		// Returns vp::vector
		// If there is no AltiVec type, return the scalar type
		template<int kind, int scalar_size> struct find_vpvector {
			typedef typename find_scalar<kind, scalar_size>::type type;
		};
		template<> struct find_vpvector<signed_int, 1>   { typedef __vector signed char type; };
		template<> struct find_vpvector<unsigned_int, 1> { typedef __vector unsigned char type; };
		template<> struct find_vpvector<boolean, 1>      { typedef __vector bool char type; };
		template<> struct find_vpvector<signed_int, 2>   { typedef __vector signed short type; };
		template<> struct find_vpvector<unsigned_int, 2> { typedef __vector unsigned short type; };
		template<> struct find_vpvector<boolean, 2>      { typedef __vector bool short type; };
		template<> struct find_vpvector<signed_int, 4>   { typedef __vector signed int type; };
		template<> struct find_vpvector<unsigned_int, 4> { typedef __vector unsigned int type; };
		template<> struct find_vpvector<boolean, 4>      { typedef __vector bool int type; };
		template<> struct find_vpvector<real, 4>         { typedef __vector float type; };

		/** Base scalar traits
		 * Creates a specialized scalar_traits for types that have no vector processor
		 * support.
		 */
		template<int kind, typename scalarT> struct base_scalar_traits {
		private:
			typedef typename find_scalar<boolean, sizeof(scalarT)>::type __bool_scalar;
			typedef typename boost::mpl::if_c<
				boost::is_same<__bool_scalar, ct::null_type>::value,
				scalarT, // Not found
				__bool_scalar>::type _bool_scalar;
			typedef typename find_scalar<signed_int, sizeof(scalarT)>::type _integer_scalar;
			typedef typename find_scalar<real, sizeof(scalarT)>::type _real_scalar;
		public:
			static const bool is_specialized = true;
			typedef typename find_vpvector<kind, sizeof(scalarT)>::type vpvector_type;
			typedef _bool_scalar bool_type;
			typedef typename boost::mpl::if_c<
				std::numeric_limits<scalarT>::is_integer,
				scalarT,
				_integer_scalar>::type integer_type;
			typedef typename boost::mpl::if_c<
				boost::is_float<scalarT>::value,
				scalarT,
				_real_scalar>::type float_type;
		};
	
		/** Base vp::vector traits
		 * Creates a specialized vpvector_traits for types that have AltiVec 
		 * vector processor support.
		 */
		template<int kind, int scalar_size> struct base_vpvector_traits {
			static const bool is_specialized = true;
			typedef typename find_scalar<kind, scalar_size>::type scalar_type;
			typedef typename find_vpvector<boolean, scalar_size>::type bool_type;
			enum { scalar_count = 16 / scalar_size };
		};
			
		/** Base vector traits
		 * Creates a specialized vector_traits for types that have no vector processor
		 * support.
		 */
		template<typename scalarT> struct base_vector_traits {
			static const bool is_specialized = true;
			typedef scalarT scalar_type;
			typedef typename scalar_traits<scalar_type>::vpvector_type vpvector_type;
			typedef vector<typename scalar_traits<scalar_type>::bool_type> bool_type;
			typedef vector<typename scalar_traits<scalar_type>::integer_type> integer_type;
			typedef vector<typename scalar_traits<scalar_type>::float_type> float_type;
			enum {
				scalar_count = VVM_SCALAR_COUNT,
				vpvector_count = VVM_SCALAR_COUNT / vpvector_traits<vpvector_type>::scalar_count,
				step = vpvector_traits<vpvector_type>::scalar_count
			};
		};
	} // End of priv namespace

	// scalar_traits
	template<> struct scalar_traits<char> : public priv::base_scalar_traits<std::numeric_limits<char>::is_signed ? priv::signed_int : priv::unsigned_int, char> {};
	template<> struct scalar_traits<signed char> : public priv::base_scalar_traits<priv::signed_int, signed char> {};
	template<> struct scalar_traits<unsigned char> : public priv::base_scalar_traits<priv::unsigned_int, unsigned char> {};
	template<> struct scalar_traits<short int> : public priv::base_scalar_traits<priv::signed_int, short int> {};
	template<> struct scalar_traits<unsigned short int> : public priv::base_scalar_traits<priv::unsigned_int, unsigned short int> {};
	template<> struct scalar_traits<int> : public priv::base_scalar_traits<priv::signed_int, int> {};
	template<> struct scalar_traits<unsigned int> : public priv::base_scalar_traits<priv::unsigned_int, unsigned int> {};
	template<> struct scalar_traits<long int> : public priv::base_scalar_traits<priv::signed_int, long int> {};
	template<> struct scalar_traits<unsigned long int> : public priv::base_scalar_traits<priv::unsigned_int, unsigned long int> {};
	template<> struct scalar_traits<float> : public priv::base_scalar_traits<priv::real, float> {};
	template<> struct scalar_traits<double> : public priv::base_scalar_traits<priv::real, double> {};
	template<> struct scalar_traits<long double> : public priv::base_scalar_traits<priv::real, long double> {};
	template<> struct scalar_traits<bchar> : public priv::base_scalar_traits<priv::boolean, bchar> {};
	template<> struct scalar_traits<bshort> : public priv::base_scalar_traits<priv::boolean, bshort> {};
	template<> struct scalar_traits<bint> : public priv::base_scalar_traits<priv::boolean, bint> {};
	template<> struct scalar_traits<blong> : public priv::base_scalar_traits<priv::boolean, blong> {};
	template<> struct scalar_traits<bfloat> : public priv::base_scalar_traits<priv::boolean, bfloat> {};
	template<> struct scalar_traits<bdouble> : public priv::base_scalar_traits<priv::boolean, bdouble> {};
	template<> struct scalar_traits<bldouble> : public priv::base_scalar_traits<priv::boolean, bldouble> {};
	
	// vpvector_traits
	template<> struct vpvector_traits<__vector signed char>    : public priv::base_vpvector_traits<priv::signed_int, 1> {};
	template<> struct vpvector_traits<__vector unsigned char>  : public priv::base_vpvector_traits<priv::unsigned_int, 1> {};
	template<> struct vpvector_traits<__vector bool char>      : public priv::base_vpvector_traits<priv::boolean, 1> {};
	template<> struct vpvector_traits<__vector signed short>   : public priv::base_vpvector_traits<priv::signed_int, 2> {};
	template<> struct vpvector_traits<__vector unsigned short> : public priv::base_vpvector_traits<priv::unsigned_int, 2> {};
	template<> struct vpvector_traits<__vector bool short>     : public priv::base_vpvector_traits<priv::boolean, 2> {};
	template<> struct vpvector_traits<__vector signed int>     : public priv::base_vpvector_traits<priv::signed_int, 4> {};
	template<> struct vpvector_traits<__vector unsigned int>   : public priv::base_vpvector_traits<priv::unsigned_int, 4> {};
	template<> struct vpvector_traits<__vector bool int>       : public priv::base_vpvector_traits<priv::boolean, 4> {};
	template<> struct vpvector_traits<__vector float>          : public priv::base_vpvector_traits<priv::real, 4> {};

	// vector_traits
	template<> struct vector_traits<vector_char> : public priv::base_vector_traits<char> {};
	template<> struct vector_traits<vector_uchar> : public priv::base_vector_traits<unsigned char> {};
	template<> struct vector_traits<vector_schar> : public priv::base_vector_traits<signed char> {};
	template<> struct vector_traits<vector_short> : public priv::base_vector_traits<short> {};
	template<> struct vector_traits<vector_ushort> : public priv::base_vector_traits<unsigned short> {};
	template<> struct vector_traits<vector_int> : public priv::base_vector_traits<int> {};
	template<> struct vector_traits<vector_uint> : public priv::base_vector_traits<unsigned int> {};
	template<> struct vector_traits<vector_long> : public priv::base_vector_traits<long> {};
	template<> struct vector_traits<vector_ulong> : public priv::base_vector_traits<unsigned long> {};
	template<> struct vector_traits<vector_float> : public priv::base_vector_traits<float> {};
	template<> struct vector_traits<vector_double> : public priv::base_vector_traits<double> {};
	template<> struct vector_traits<vector_ldouble> : public priv::base_vector_traits<long double> {};
	template<> struct vector_traits<vector_bchar> : public priv::base_vector_traits<bchar> {};
	template<> struct vector_traits<vector_bshort> : public priv::base_vector_traits<bshort> {};
	template<> struct vector_traits<vector_bint> : public priv::base_vector_traits<bint> {};
	template<> struct vector_traits<vector_blong> : public priv::base_vector_traits<blong> {};
	template<> struct vector_traits<vector_bfloat> : public priv::base_vector_traits<bfloat> {};
	template<> struct vector_traits<vector_bdouble> : public priv::base_vector_traits<bdouble> {};
	template<> struct vector_traits<vector_bldouble> : public priv::base_vector_traits<bldouble> {};
} // End of vvm namespace

#endif
