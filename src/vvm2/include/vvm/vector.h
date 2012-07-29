/*
 *  vector.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Aug 22 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_VECTOR_H
#define _VVM_VECTOR_H

#include <vvm/forward.h>
#include <vvm/types.h>
#include <vvm/traits.h>

namespace vvm {
	// Forward declaration of splat
	namespace priv {
		template<typename scalarT, typename specializedT = void> struct splat;
	} // End of priv namespace
	/// The VVM vector
	template<typename scalarT> class vector {
	private:
		typedef scalarT scalar_type;
		typedef typename scalar_traits<scalar_type>::vpvector_type vpvector_type;
		enum {
			scalar_count = vector_traits<vector<scalar_type> >::scalar_count,
			vpvector_count = vector_traits<vector<scalar_type> >::vpvector_count
		};
	public:
		inline vector() {}
		// Do not provide an implementation of copy constructor to help
		// compiler enregister class
		// vector(const vector)
		inline vector(const scalar_type a) {
			priv::splat<scalar_type>::exec(*this, a);
		}
/*	public:
		inline vector& operator=(const vector& a) {
			if(this == &a)
				return *this;
			// Copy using memcpy
			memcpy(reinterpret_cast<void*>(_vpvector),
				reinterpret_cast<const void*>(a._vpvector),
				sizeof(vpvector_type)*vpvector_count);
			return *this;
		}*/
		inline vector& operator=(const scalar_type a) {
			priv::splat<scalar_type>::exec(*this, a);
			return *this;
		}
	public:
		inline vpvector_type& vpvector(const int i) {
			return _vpvector[i];
		}
		inline const vpvector_type& vpvector(const int i) const {
			return _vpvector[i];
		}
		inline scalar_type& scalar(const int i) {
			return (reinterpret_cast<scalarT*>(_vpvector))[i];
		}
		inline const scalar_type& scalar(const int i) const {
			return (reinterpret_cast<const scalarT*>(_vpvector))[i];
		}
	private:
		vpvector_type _vpvector[vpvector_count];
	};
	
	// Typedefs for VVM vectors
	typedef vector<char> vector_char;
	typedef vector<unsigned char> vector_uchar;
	typedef vector<signed char> vector_schar;
	typedef vector<short int> vector_short;
	typedef vector<unsigned short int> vector_ushort;
	typedef vector<int> vector_int;
	typedef vector<unsigned int> vector_uint;
	typedef vector<long> vector_long;
	typedef vector<unsigned long> vector_ulong;
	typedef vector<float> vector_float;
	typedef vector<double> vector_double;
	typedef vector<long double> vector_ldouble;
	typedef vector<bchar> vector_bchar;
	typedef vector<bshort> vector_bshort;
	typedef vector<bint> vector_bint;
	typedef vector<blong> vector_blong;
	typedef vector<bfloat> vector_bfloat;
	typedef vector<bdouble> vector_bdouble;
	typedef vector<bldouble> vector_bldouble;
	typedef vector<sint8> vector_sint8;
	typedef vector<uint8> vector_uint8;
	typedef vector<bint8> vector_bint8;
	typedef vector<sint16> vector_sint16;
	typedef vector<uint16> vector_uint16;
	typedef vector<bint16> vector_bint16;
	typedef vector<sint32> vector_sint32;
	typedef vector<uint32> vector_uint32;
	typedef vector<bint32> vector_bint32;
} // End of vvm namespace

#endif
