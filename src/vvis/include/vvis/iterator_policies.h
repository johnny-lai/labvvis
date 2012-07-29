/*
 *  iterator_policies.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on 3/01/05.
 *  Copyright 2005 Bing-Chang Lai. All rights reserved.
 *
 */
 
/*! @header vvis/iterator_policies.h
 * @abstract
 *   Include this file for VVIS iterator policies.
 */
#ifndef _VVIS_ITERATOR_POLICIES_H
#define _VVIS_ITERATOR_POLICIES_H

namespace vvis {
	/* Not used
	class aligned_accessor_policy {
	public:
		const scalar_type get_scalar() const {
			return (*vector).scalar(scalar);
		}
		void set_scalar(const scalar_type s) {
			(*vector).scalar(scalar) = s;
		}
		const vector_type get_vector() const {
			return *vector;
		}
		const vector_type get_vector(const vvm::offset_t shift) const {
			return *(vector + shift);
		}
		void set_vector(const vector_type& v) {
			*vector = v;
		}
	}
	*/
} // End of vvis namespace

#endif