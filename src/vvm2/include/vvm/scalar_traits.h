/*
 *  scalar_traits.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Sat Aug 23 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_SCALAR_TRAITS_H
#define _VVM_SCALAR_TRAITS_H

namespace vvm {
	namespace priv {
		/** Base scalar traits
		 * Creates a specialized scalar_traits for types that have no vector processor
		 * support.
		 */
		template<typename scalarT> struct base_scalar_traits {
		private:
			typedef typename find_scalar<boolean, sizeof(scalarT)>::type _bool_scalar;
			typedef typename find_scalar<signed_int, sizeof(scalarT)>::type _integer_scalar;
			typedef typename find_scalar<real, sizeof(scalarT)>::type _real_scalar;
		public:
			static const bool is_specialized = true;
			typedef scalarT vpvector_type;
			typedef typename find_scalar<boolean, sizeof(scalarT)>::type bool_type;
			typedef typename boost::mpl::if_c<
				std::numeric_limits<scalarT>::is_integer,
				scalarT,
				typename boost::mpl::if_c<
					boost::is_same<_integer_scalar, ct::null_type>::value,
					int,
					_integer_scalar>::type
				>::type integer_type;
			typedef typename boost::mpl::if_c<
				boost::is_float<scalarT>::value,
				scalarT,
				typename boost::mpl::if_c<
					boost::is_same<_real_scalar, ct::null_type>::value,
					float,
					_real_scalar>::type
				>::type float_type;
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
	template<> struct scalar_traits<char> : public priv::base_scalar_traits<char> {};
	template<> struct scalar_traits<signed char> : public priv::base_scalar_traits<signed char> {};
	template<> struct scalar_traits<unsigned char> : public priv::base_scalar_traits<unsigned char> {};
	template<> struct scalar_traits<short int> : public priv::base_scalar_traits<short int> {};
	template<> struct scalar_traits<unsigned short int> : public priv::base_scalar_traits<unsigned short int> {};
	template<> struct scalar_traits<int> : public priv::base_scalar_traits<int> {};
	template<> struct scalar_traits<unsigned int> : public priv::base_scalar_traits<unsigned int> {};
	template<> struct scalar_traits<long int> : public priv::base_scalar_traits<long int> {};
	template<> struct scalar_traits<unsigned long int> : public priv::base_scalar_traits<unsigned long int> {};
	template<> struct scalar_traits<float> : public priv::base_scalar_traits<float> {};
	template<> struct scalar_traits<double> : public priv::base_scalar_traits<double> {};
	template<> struct scalar_traits<long double> : public priv::base_scalar_traits<long double> {};
	template<> struct scalar_traits<bchar> : public priv::base_scalar_traits<bchar> {};
	template<> struct scalar_traits<bshort> : public priv::base_scalar_traits<bshort> {};
	template<> struct scalar_traits<bint> : public priv::base_scalar_traits<bint> {};
	template<> struct scalar_traits<blong> : public priv::base_scalar_traits<blong> {};
	template<> struct scalar_traits<bfloat> : public priv::base_scalar_traits<bfloat> {};
	template<> struct scalar_traits<bdouble> : public priv::base_scalar_traits<bdouble> {};
	template<> struct scalar_traits<bldouble> : public priv::base_scalar_traits<bldouble> {};
	
	// vpvector_traits
	// Scalar vpvector_traits are always defined.
	// So are found in traits.h
	
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
