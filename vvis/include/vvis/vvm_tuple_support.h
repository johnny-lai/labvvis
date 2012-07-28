/*
 *  vvm_tuple_support.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sat April 15 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 *  Add some more functions to make using tuples of VVM vector easier
 *
 */

#ifndef _VVIS_VVM_TUPLE_SUPPORT_H
#define _VVIS_VVM_TUPLE_SUPPORT_H

#include <vvm/vvm.h>
#include <ct/tuple.h>

// Apply vvm::vector_cast to all elements in tuple
namespace vvm {
	/*
	//- vector_cast --------------------------------------------------------
	namespace priv {
		template<typename toTL, typename fromTL>
		struct do_vector_cast {
			template<int i> struct Code {
				static void exec(ct::tuple<toTL>& ret, const ct::tuple<fromTL>& a) {
					ret.template get<i>() = vvm::vector_cast<
						typename ct::type_at<toTL, i>::type>(a.template get<i>());
				}
			};
		};
	}
	
	template<typename toTL, typename fromTL>
	inline static ct::tuple<toTL> vector_cast(const ct::tuple<fromTL> &from) {
		ct::tuple<toTL> ret;
		meta::EFOR2<0, meta::Less, ct::length<fromTL>::value, +1,
			typename vvm::priv::do_vector_cast<toTL, fromTL> >::exec(ret, from);
		return ret;
	}
	*/
} // End of vvm namespace

#endif
