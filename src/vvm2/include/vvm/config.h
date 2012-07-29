/*
 *  vvm.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Thu Aug 21 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_CONFIG_H
#define _VVM_CONFIG_H

// Vector Processor Information
#if !defined VVM_USER_NO_VECTOR_PROCESSOR && defined __ALTIVEC__
	#define VVM_VECTOR_PROCESSOR 1
	#define VVM_ALTIVEC
	#define VVM_PREFERRED_SCALAR_COUNT 16
	#define VVM_INFO_VECTOR_PROCESSOR "AltiVec"
	//#warning vvm2: AltiVec active
#else
	#define VVM_SCALAR
	#define VVM_PREFERRED_SCALAR_COUNT 1
	#define VVM_INFO_VECTOR_PROCESSOR "None"
	//#warning vvm2: No vector processor detected
#endif

//==============================================================================
// Turn on warnings?
//------------------------------------------------------------------------------
#ifdef VVM_WARNINGS
	#define _VVM_WARN(MESSAGE) { std::clog << MESSAGE << std::endl; }
#else
	#define _VVM_WARN(MESSAGE)
#endif

//==============================================================================
// Set VVM_SCALAR_COUNT
//------------------------------------------------------------------------------
// Accept VVM_USER_SCALAR_COUNT only if it is a multiple of VVM_PREFERRED_SCALAR_COUNT
#if defined VVM_USER_SCALAR_COUNT && VVM_USER_SCALAR_COUNT % VVM_PREFERRED_SCALAR_COUNT == 0
	//#warning vvm2: Scalar count overriden by user-defined value of VVM_USER_SCALAR_COUNT
	#define VVM_SCALAR_COUNT VVM_USER_SCALAR_COUNT
#elif defined VVM_USER_SCALAR_COUNT && VVM_USER_SCALAR_COUNT % VVM_PREFERRED_SCALAR_COUNT != 0
	#error In this VVM implementation, only VVM_USER_SCALAR_COUNT of values divisible by VVM_PREFERRED_SCALAR_COUNT are allowed
#else
	#define VVM_SCALAR_COUNT VVM_PREFERRED_SCALAR_COUNT
#endif

//===================================================================================
// Implementation Information
//-----------------------------------------------------------------------------------
#define VVM_INFO_IMPLEMENTATION "VVM Reference Implementation"
#define VVM_INFO_AUTHOR "Bing-Chang Lai, Phillip John McKerrow"
#define VVM_INFO_COMPANY "University of Wollongong"
#define VVM_INFO_VERSION "0.1"
#define VVM_INFO_BUILD 1

#endif