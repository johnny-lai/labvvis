/*
 *  traits.cpp
 *  vvm
 *
 *  Created by Johnny Lai on Wed Feb 05 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <common/type_name.h>
#include <vvm/vvm.h>

using namespace std;

template<typename normalT, typename signedT, typename unsignedT>
void report_specialized(char type_name[]) {
	cout << "         " << type_name << ": " << vvm::scalar_traits<normalT>::is_specialized << endl;
	cout << "  signed " << type_name << ": " << vvm::scalar_traits<signedT>::is_specialized << endl;
	cout << "unsigned " << type_name << ": " << vvm::scalar_traits<unsignedT>::is_specialized << endl;
	cout << endl;
}

int main() {
	cout << "Compliant VVM implementations MUST specialize all the following traits" << endl;
#	define SCALAR_TRAITS_IS_SPECIALIZED(T) vvm::scalar_traits<T>::is_specialized
	cout << "Fundamental Scalar Traits:" << endl;
	cout << "\t" << SCALAR_TRAITS_IS_SPECIALIZED(char) <<
		SCALAR_TRAITS_IS_SPECIALIZED(signed char) <<
		SCALAR_TRAITS_IS_SPECIALIZED(unsigned char) <<
		" " <<
		SCALAR_TRAITS_IS_SPECIALIZED(short int) <<
		SCALAR_TRAITS_IS_SPECIALIZED(unsigned short int) <<
		" " <<
		SCALAR_TRAITS_IS_SPECIALIZED(int) <<
		SCALAR_TRAITS_IS_SPECIALIZED(unsigned int) <<
		" " <<
		SCALAR_TRAITS_IS_SPECIALIZED(long) <<
		SCALAR_TRAITS_IS_SPECIALIZED(unsigned long) <<
		" " <<
		SCALAR_TRAITS_IS_SPECIALIZED(float) <<
		SCALAR_TRAITS_IS_SPECIALIZED(double) <<
		SCALAR_TRAITS_IS_SPECIALIZED(long double) <<
		endl;
#	define VPVECTOR_TRAITS_IS_SPECIALIZED(T) vvm::vpvector_traits<vvm::scalar_traits<T>::vpvector_type>::is_specialized
	cout << "Fundamental Vp::Vector Traits:" << endl;
	cout << "\t" << VPVECTOR_TRAITS_IS_SPECIALIZED(char) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(signed char) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned char) <<
		" " <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(short int) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned short int) <<
		" " <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(int) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned int) <<
		" " <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(long) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned long) <<
		" " <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(float) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(double) <<
		VPVECTOR_TRAITS_IS_SPECIALIZED(long double) <<
		endl;
#	define BOOL_SCALAR_TRAITS_IS_SPECIALIZED(T) vvm::scalar_traits<vvm::scalar_traits<T>::bool_type>::is_specialized
	cout << "Bool Scalar Traits:" << endl;
	cout << "\t" << BOOL_SCALAR_TRAITS_IS_SPECIALIZED(char) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(signed char) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(unsigned char) <<
		" " <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(short int) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(unsigned short int) <<
		" " <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(int) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(unsigned int) <<
		" " <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(long) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(unsigned long) <<
		" " <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(float) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(double) <<
		BOOL_SCALAR_TRAITS_IS_SPECIALIZED(long double) <<
		endl;
#	define BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(T) vvm::vpvector_traits<vvm::scalar_traits<vvm::scalar_traits<T>::bool_type>::vpvector_type>::is_specialized
	cout << "Bool VpVector Traits:" << endl;
	cout << "\t" << BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(char) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(signed char) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned char) <<
		" " <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(short int) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned short int) <<
		" " <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(int) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned int) <<
		" " <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(long) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(unsigned long) <<
		" " <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(float) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(double) <<
		BOOL_VPVECTOR_TRAITS_IS_SPECIALIZED(long double) <<
		endl;
#	define VECTOR_TRAITS_IS_SPECIALIZED(T) vvm::vector_traits<T>::is_specialized
	cout << "Vector Traits:" << endl;
	cout << "\t" << VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_char) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_schar) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_uchar) <<
		" " <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_short) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_ushort) <<
		" " <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_int) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_uint) <<
		" " <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_long) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_ulong) <<
		" " <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_float) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_double) <<
		VECTOR_TRAITS_IS_SPECIALIZED(vvm::vector_ldouble) <<
		endl;
#	define SCALAR_TO_VPVECTOR_TYPE_NAME(T) type_name(typeid(vvm::scalar_traits<T>::vpvector_type))
	cout << "Scalar to vp::vector mapping:" << endl;
	//cout << type_name(typeid(__vector unsigned char));
	/*
	cout << "\t" << SCALAR_TO_VPVECTOR_TYPE_NAME(char) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(signed char) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(unsigned char) << endl;
	cout << "\t" << SCALAR_TO_VPVECTOR_TYPE_NAME(short int) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(unsigned short int) << endl;
	cout << "\t" << SCALAR_TO_VPVECTOR_TYPE_NAME(int) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(unsigned int) << endl;
	cout << "\t" << SCALAR_TO_VPVECTOR_TYPE_NAME(long int) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(unsigned long int) << endl;
	cout << "\t" << SCALAR_TO_VPVECTOR_TYPE_NAME(float) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(double) << ", " <<
		SCALAR_TO_VPVECTOR_TYPE_NAME(long double) << endl;*/
#	define VPVECTOR_TO_SCALAR_TYPE_NAME(T) type_name(typeid(vvm::vpvector_traits<vvm::scalar_traits<T>::vpvector_type>::scalar_type))
	cout << "Vp::Vector to scalar mapping:" << endl;
	cout << "\t" << VPVECTOR_TO_SCALAR_TYPE_NAME(char) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(signed char) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(unsigned char) << endl;
	cout << "\t" << VPVECTOR_TO_SCALAR_TYPE_NAME(short int) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(unsigned short int) << endl;
	cout << "\t" << VPVECTOR_TO_SCALAR_TYPE_NAME(int) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(unsigned int) << endl;
	cout << "\t" << VPVECTOR_TO_SCALAR_TYPE_NAME(long int) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(unsigned long int) << endl;
	cout << "\t" << VPVECTOR_TO_SCALAR_TYPE_NAME(float) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(double) << ", " <<
		VPVECTOR_TO_SCALAR_TYPE_NAME(long double) << endl;
#	define INTEGER_TYPE_NAME(T) type_name(typeid(vvm::scalar_traits<T>::integer_type))
	cout << "Scalar to integer_type mapping:" << endl;
	cout << "\t" << INTEGER_TYPE_NAME(char) << ", " <<
		INTEGER_TYPE_NAME(signed char) << ", " <<
		INTEGER_TYPE_NAME(unsigned char) << endl;
	cout << "\t" << INTEGER_TYPE_NAME(short int) << ", " <<
		INTEGER_TYPE_NAME(unsigned short int) << endl;
	cout << "\t" << INTEGER_TYPE_NAME(int) << ", " <<
		INTEGER_TYPE_NAME(unsigned int) << endl;
	cout << "\t" << INTEGER_TYPE_NAME(long int) << ", " <<
		INTEGER_TYPE_NAME(unsigned long int) << endl;
	cout << "\t" << INTEGER_TYPE_NAME(float) << ", " <<
		INTEGER_TYPE_NAME(double) << ", " <<
		INTEGER_TYPE_NAME(long double) << endl;
#	define REAL_TYPE_NAME(T) type_name(typeid(vvm::scalar_traits<T>::real_type))
	cout << "Scalar to real_type mapping:" << endl;
	cout << "\t" << REAL_TYPE_NAME(char) << ", " <<
		REAL_TYPE_NAME(signed char) << ", " <<
		REAL_TYPE_NAME(unsigned char) << endl;
	cout << "\t" << REAL_TYPE_NAME(short int) << ", " <<
		REAL_TYPE_NAME(unsigned short int) << endl;
	cout << "\t" << REAL_TYPE_NAME(int) << ", " <<
		REAL_TYPE_NAME(unsigned int) << endl;
	cout << "\t" << REAL_TYPE_NAME(long int) << ", " <<
		REAL_TYPE_NAME(unsigned long int) << endl;
	cout << "\t" << REAL_TYPE_NAME(float) << ", " <<
		REAL_TYPE_NAME(double) << ", " <<
		REAL_TYPE_NAME(long double) << endl;
	return 0;
}