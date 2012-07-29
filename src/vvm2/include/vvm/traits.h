/*
 *  traits.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Aug 21 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_TRAITS_H
#define _VVM_TRAITS_H

#include <vvm/forward.h>
#include <vvm/types.h>
#include <vvm/vector.h>

namespace vvm {
	/// Scalar Traits
	template<typename scalarT> struct scalar_traits {
		static const bool is_specialized = false;
		typedef ct::null_type vpvector_type;
		typedef ct::null_type bool_type;
		typedef ct::null_type integer_type;
		typedef ct::null_type float_type;
	};
	
	/// Vp::vector Traits
	template<typename scalarT> struct vpvector_traits {
		static const bool is_specialized = false;
		typedef ct::null_type scalar_type;
		typedef ct::null_type bool_type;
		enum { scalar_count = 0 };
	};
		
	/// Vector Traits
	template<typename scalarT> struct vector_traits {
		static const bool is_specialized = false;
		typedef ct::null_type scalar_type;
		typedef ct::null_type vpvector_type;
		typedef ct::null_type bool_type;
		typedef ct::null_type integer_type;
		typedef ct::null_type float_type;
		enum {
			scalar_count = 0,
			vpvector_count = 0,
			step = 0
		};
	};

	// vpvector_traits
	// Always defined regardless of active vector processor implementation
	namespace priv {
		/** Base vp::vector traits
		 * Creates a specialized vpvector_traits for types that have no vector processor
		 * support.
		 */
		template<typename scalarT> struct scalar_vpvector_traits {
			static const bool is_specialized = true;
			typedef scalarT scalar_type;
			typedef typename find_scalar<boolean, sizeof(scalarT)>::type bool_type;
			enum { scalar_count = 1 };
		};
	} // End of priv namespace
	template<> struct vpvector_traits<char> : public priv::scalar_vpvector_traits<char> {};
	template<> struct vpvector_traits<signed char> : public priv::scalar_vpvector_traits<signed char> {};
	template<> struct vpvector_traits<unsigned char> : public priv::scalar_vpvector_traits<unsigned char> {};
	template<> struct vpvector_traits<short int> : public priv::scalar_vpvector_traits<short int> {};
	template<> struct vpvector_traits<unsigned short int> : public priv::scalar_vpvector_traits<unsigned short int> {};
	template<> struct vpvector_traits<int> : public priv::scalar_vpvector_traits<int> {};
	template<> struct vpvector_traits<unsigned int> : public priv::scalar_vpvector_traits<unsigned int> {};
	template<> struct vpvector_traits<long int> : public priv::scalar_vpvector_traits<long int> {};
	template<> struct vpvector_traits<unsigned long int> : public priv::scalar_vpvector_traits<unsigned long int> {};
	template<> struct vpvector_traits<float> : public priv::scalar_vpvector_traits<float> {};
	template<> struct vpvector_traits<double> : public priv::scalar_vpvector_traits<double> {};
	template<> struct vpvector_traits<long double> : public priv::scalar_vpvector_traits<long double> {};
	template<> struct vpvector_traits<bchar> : public priv::scalar_vpvector_traits<bchar> {};
	template<> struct vpvector_traits<bshort> : public priv::scalar_vpvector_traits<bshort> {};
	template<> struct vpvector_traits<bint> : public priv::scalar_vpvector_traits<bint> {};
	template<> struct vpvector_traits<blong> : public priv::scalar_vpvector_traits<blong> {};
	template<> struct vpvector_traits<bfloat> : public priv::scalar_vpvector_traits<bfloat> {};
	template<> struct vpvector_traits<bdouble> : public priv::scalar_vpvector_traits<bdouble> {};
	template<> struct vpvector_traits<bldouble> : public priv::scalar_vpvector_traits<bldouble> {};

} // End of vvm namespace

// Load traits for the current vector processor implementation
#ifdef VVM_ALTIVEC
	#include <vvm/altivec_traits.h>
#else
	#include <vvm/scalar_traits.h>
#endif

#endif
