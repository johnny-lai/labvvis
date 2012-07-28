/*
 *  operation.h
 *  Timing definitions for binary functions
 *
 *  Created by Bing-Chang Lai on Tues May 20 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */
#ifndef _VVM_TIMING_OPERATION_H
#define _VVM_TIMING_OPERATION_H

#include <iostream>
#include <vvm/vvm.h>
#include "util.h"

/** Function that actually performs the timing operation
 */
template<typename scalarT>
timingdiff_t time_vvm(const int op_count, const int count) {
	vvm::vector<scalarT>* a = new vvm::vector<scalarT>[count];
	vvm::vector<scalarT>* r = new vvm::vector<scalarT>[count];
	//fill_rand(reinterpret_cast<scalarT*>(a), count * VVM_SCALAR_COUNT);
	TIME_INIT;
	switch(op_count) {
	case 1: TIME_OP(r[i] = a[i] + a[i]); break;
	case 2: TIME_OP(r[i] = a[i] + a[i] + a[i]); break;
	case 3: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i]); break;
	case 4: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i] + a[i]); break;
	case 5: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i] + a[i] + a[i]); break;
	default: std::cerr << "Unsupported operation count of " << op_count << std::endl;
	};
	delete[] a;
	delete[] r;
	return TIME_RESULT;
}

template<typename scalarT>
timingdiff_t time_scalar(const int op_count, const int vvm_count) {
	const int count = vvm_count * vvm::vector_traits<vvm::vector<scalarT> >::scalar_count;
	scalarT* a = new scalarT[count];
	scalarT* r = new scalarT[count];
	//fill_rand(a, count);
	TIME_INIT;
	switch(op_count) {
	case 1: TIME_OP(r[i] = a[i] + a[i]); break;
	case 2: TIME_OP(r[i] = a[i] + a[i] + a[i]); break;
	case 3: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i]); break;
	case 4: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i] + a[i]); break;
	case 5: TIME_OP(r[i] = a[i] + a[i] + a[i] + a[i] + a[i] + a[i]); break;
	default: std::cerr << "Unsupported operation count of " << op_count << std::endl;
	};
	delete[] a;
	delete[] r;
	return TIME_RESULT;
}

template<typename scalarT>
timingdiff_t time_vpvector(const int op_count, const int vvm_count) {
#ifdef __ALTIVEC__
	const int count = vvm_count * vvm::vector_traits<vvm::vector<scalarT> >::vpvector_count;
	typedef typename vvm::vector_traits<vvm::vector<scalarT> >::vpvector_type vpvector_t;
	vpvector_t* a = new vpvector_t[count];
	vpvector_t* r = new vpvector_t[count];
	//fill_rand(reinterpret_cast<scalarT*>(a), count * vvm::vector_traits<vvm::vector<scalarT> >::vpvector_count);
	TIME_INIT;
	switch(op_count) {
	case 1: TIME_OP(r[i] = vec_add(a[i], a[i])); break;
	case 2: TIME_OP(r[i] = vec_add(vec_add(a[i], a[i]), a[i])); break;
	case 3: TIME_OP(r[i] = vec_add(vec_add(vec_add(a[i], a[i]), a[i]), a[i])); break;
	case 4: TIME_OP(r[i] = vec_add(vec_add(vec_add(vec_add(a[i], a[i]), a[i]), a[i]), a[i])); break;
	case 5: TIME_OP(r[i] = vec_add(vec_add(vec_add(vec_add(vec_add(a[i], a[i]), a[i]), a[i]), a[i]), a[i])); break;
	default: std::cerr << "Unsupported operation count of " << op_count << std::endl;
	};
	delete[] a;
	delete[] r;
	return TIME_RESULT;
#else
	return 0;
#endif
}

#endif
