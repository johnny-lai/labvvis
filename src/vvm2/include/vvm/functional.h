/*
 *  functional.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Fri Oct 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */
 
#ifndef _VVM_FUNCTIONAL_H
#define _VVM_FUNCTIONAL_H

namespace vvm {
	namespace functional {
		#define VVM_DEFINE_STD_FUNCTOR(CLASS, OP) \
			template<typename scalarT> struct CLASS \
			: public std::binary_function<vector<scalarT>, vector<scalarT>, \
			vector<typename scalar_traits<scalarT>::bool_type> > { \
				vector<typename scalar_traits<scalarT>::bool_type> \
				operator()(const vector<scalarT>& a, const vector<scalarT>& b) const { \
					return a OP b; \
				} \
			};
		VVM_DEFINE_STD_FUNCTOR(equal_to, ==);
		VVM_DEFINE_STD_FUNCTOR(not_equal_to, !=);
		VVM_DEFINE_STD_FUNCTOR(less, <);
		VVM_DEFINE_STD_FUNCTOR(less_equal, <=);
		VVM_DEFINE_STD_FUNCTOR(greater, >);
		VVM_DEFINE_STD_FUNCTOR(greater_equal, >=);
	} // End of functional namespace
} // End of vvm namespace

#endif