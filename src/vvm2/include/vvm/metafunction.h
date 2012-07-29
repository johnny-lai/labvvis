/*
 *  metafunction.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Sun Sep 28 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_METAFUNCTION_H
#define _VVM_METAFUNCTION_H

#include <ct/typelist.h>
#include <ct/tuple.h>
#include <vvm/forward.h>

namespace vvm {
	/// Returns the vvm::vector equivalent of type T
	template<typename T> struct add_vector {
		typedef vvm::vector<T> type;
	};
	template<typename typelistT> struct add_vector<ct::tuple<typelistT> > {
		typedef ct::tuple<
			typename ct::apply<typelistT, vvm::add_vector>::type> type;
	};
	
	/** Removes the vvm::vector portion of T
	 * If T is not a vvm::vector, it returns T
	 */
	template<typename T> struct remove_vector {
		typedef T type;
	};
	template<typename T> struct remove_vector<vector<T> > {
		typedef T type;
	};

	/// Returns bool_type for type T
	template<typename T> struct to_bool {
		typedef typename scalar_traits<T>::bool_type type;
	};
	template<typename T> struct to_bool<vector<T> > {
		typedef typename vector_traits<vector<T> >::bool_type type;
	};
	template<typename typelistT> struct to_bool<ct::tuple<typelistT> > {
		typedef ct::tuple<
			typename ct::apply<typelistT, vvm::to_bool>::type> type;
	};
	
} // End of vvm namespace

#endif
