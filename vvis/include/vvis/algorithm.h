/*
 *  algorithm.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Aug 03 2003.
 *  Copyright (c) 2003 Bing-Chang Lai. All rights reserved.
 *
 */

#ifndef VVIS_ALGORITHM_H
#define VVIS_ALGORITHM_H

#include <vvm/vvm.h>
#include <vvis/vvm_tuple_support.h>
#include <vvis/storage.h>

namespace vvis {
	//= convolute ==============================================================
	namespace priv_unknown_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT, 
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == false ||
			(boost::is_same<
				typename priv::select_illife_unknown<CT_TYPELIST2(storageInT, storageOutT)>::type,
				priv::using_unknown_storage
			>::value &&
			is_rectangular<storageInT>::value &&
			is_rectangular<storageOutT>::value)
		>::type convolute(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			// Get Kernel Width and Height
			const int kernel_width = f.kernel().width();
			const int kernel_height = f.kernel().height();
			const int height = in.size() - f.kernel().height() + 1;
			// Work across Rows
			for(int y = 0; y < height; ++y) {
				// Get beginning and end of row
				typename storageInT::value_type::const_iterator pi = in[y].begin();
				typename storageInT::value_type::const_iterator end = in[y].end() - (kernel_width - 1);
				typename storageOutT::value_type::iterator po = out[y].begin();
				// Apply functor
				for(int i = 0; pi != end; ++i, ++pi, ++po) {
					f.reset();
					for(int ky = 0; ky < kernel_height; ++ky) {
						// Move to appropriate position
						typename storageInT::value_type::const_iterator itr = in[y + ky].begin() + i;
						for(int kx = 0; kx < kernel_width; ++kx) {
							f.accumulate(ia.get_scalar(itr + kx));
						}
					}
					oa.set_scalar(f.scalar_result(), po);
				}
			}
		}
	} // End of priv_unknown_storage namespace
	namespace priv_contiguous_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT, 
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == true &&
			(boost::is_same<
				typename priv::select_illife_unknown<CT_TYPELIST2(storageInT, storageOutT)>::type,
				priv::using_contiguous_storage
			>::value &&
			is_rectangular<storageInT>::value &&
			is_rectangular<storageOutT>::value)
		>::type convolute(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			const int kernel_width = f.kernel().width();
			const int kernel_height = f.kernel().height();
			const int height = in.size() - f.kernel().height() + 1;
			const int req_vcount = kernel_width / VVM_SCALAR_COUNT + 2;
			typename accessorInT::vector_type vcache[req_vcount];
			typename storageInT::value_type::const_iterator pi[kernel_height];
			// Work across Rows
			for(int y = 0; y < height; ++y) {
				// Get beginning and end
				for(int i = 0; i < kernel_height; ++i) {
					pi[i] = in[y + i].begin();
				}
				typename storageInT::value_type::const_iterator end = in[y].end() - (kernel_width - 1);
				typename storageOutT::value_type::iterator po = out[y].begin();
				// Apply functor
				for(; pi[0].vector != end.vector; ++po.vector) {
					ia.template prefetch_read<0>(pi[0]);
					oa.template prefetch_write<accessorInT::prefetch_channel_count>(po);
					f.reset();
					for(int ky = 0; ky < kernel_height; ++ky) {
						// Load vectors
						for(int i = 0; i < req_vcount; ++i) {
							vcache[i] = ia.get_vector(pi[ky], i);
						}
						// Now accumulate
						f.accumulate(vcache[0]);
						for(int kx = 1; kx < kernel_width; ++kx) {
							f.accumulate(
								ia.get_vector(vcache[kx / VVM_SCALAR_COUNT],
									vcache[kx / VVM_SCALAR_COUNT + 1],
									kx % VVM_SCALAR_COUNT));
						}
						// Advance pi
						++pi[ky].vector;
					}
					oa.set_vector(f.vector_result(), po);
				}
				// Apply functor using the Scalar Processor
				for(; pi[0].scalar != end.scalar; ++po.scalar) {
					f.reset();
					for(int ky = 0; ky < kernel_height; ++ky) {
						for(int kx = 0; kx < kernel_width; ++kx) {
							f.accumulate(ia.get_scalar(pi[ky] + kx));
						}
						// Advance pi
						++pi[ky].scalar;
					}
					oa.set_scalar(f.scalar_result(), po);
				}
			}
		}
	} // End of priv_contiguous_storage namespace
	using priv_unknown_storage::convolute;
	using priv_contiguous_storage::convolute;

	//= for_each ===============================================================
	namespace priv_unknown_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename functorT>
		typename ct::enable_if<
			(is_vectorised_functor<functorT>::value == false &&
			!boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST1(storageInT)>::type,
				priv::using_illife_storage
			>::value) ||
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST1(storageInT)>::type,
				priv::using_unknown_storage
			>::value
		>::type for_each(const storageInT& in, const accessorInT& ia, functorT& f) {
			typename storageInT::const_iterator end = in.end();
			typename storageInT::const_iterator pi = in.begin();
			for(; pi != end; ++pi) {
				f(ia.get_scalar(pi));
			}
		}
	} // End of priv_unknown_storage
	using priv_unknown_storage::for_each;
	//- for_each<using_contiguous_storage> ---------------------------------
	namespace priv_contiguous_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == true &&
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST1(storageInT)>::type,
				priv::using_contiguous_storage
			>::value
		>::type for_each(const storageInT& in, const accessorInT& ia, functorT& f) {
			// Calculate aligned end position
			typename storageInT::const_iterator pi = in.begin();
			typename storageInT::const_iterator end = in.end();
			// Process front using the vector processor
			for(; pi.vector != end.vector; ++pi.vector) {
				ia.template prefetch_read<0>(pi);
				// Perform operation
				f(ia.get_vector(pi));
			}
			// Process rest using the scalar processor
			for(; pi.scalar != end.scalar; ++pi.scalar) {
				f(ia.get_scalar(pi));
			}
		}
	} // End of priv_contiguous_storage namespace
	using priv_contiguous_storage::for_each;
	//- for_each<using_illife_storage> -------------------------------------
	namespace priv_illife_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename functorT>
		typename ct::enable_if<
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST1(storageInT)>::type,
				priv::using_illife_storage
			>::value
		>::type for_each(const storageInT& in, const accessorInT& ia, functorT& f) {
			typename storageInT::const_iterator i = in.begin();
			typename storageInT::const_iterator end = in.end();
			for(; i != end; ++i) {
				vvis::for_each(*i, ia, f);
			}
		}
	} // End of priv_illife_storage namespace
	/** For Each Algorithm
	 * Selects, by default,
	 *   no_edges for images,
	 *   scalar_edges for regions,
	 *   prefetch
	 */
	using priv_illife_storage::for_each;

	//= transform =================================================================
	// Select if either storageInT or storageOutT are unknown storages
	namespace priv_unknown_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT,
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == false &&
			ct::length<
				typename ct::keep_only<
					CT_TYPELIST2(storageInT, storageOutT),
					uses_unknown_storage
				>::type
			>::value >= 1
		>::type transform(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageOutT::iterator po = out.begin();
			typename storageInT::const_iterator end = in.end();
			typename storageInT::const_iterator pi = in.begin();
			for(; pi != end; ++pi, ++po) {
				oa.set_scalar(f(ia.get_scalar(pi)), po);
			}
		}
	} // End of priv_unknown_storage
	using priv_unknown_storage::transform;
	//- transform - contiguous_aligned_storage ---------------------------------
	// Select if both storageInT and storageOutT are either
	// contiguous_unaligned_storages or contiguous_aligned_storages
	namespace priv_contiguous_aligned_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT,
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == true &&
			(ct::length<
				typename ct::keep_only<
					CT_TYPELIST2(storageInT, storageOutT),
					uses_contiguous_aligned_storage
				>::type
			>::value +
			ct::length<
				typename ct::keep_only<
					CT_TYPELIST2(storageInT, storageOutT),
					uses_contiguous_unaligned_storage
				>::type
			>::value) == 2
		>::type transform(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageOutT::iterator po = out.begin();
			typename storageInT::const_iterator pi = in.begin();
			typename storageInT::const_iterator end = in.end();
			for(; pi.vector != end.vector; ++pi.vector, ++po.vector) {
				ia.template prefetch_read<0>(pi);
				oa.template prefetch_write<accessorInT::prefetch_channel_count>(po);
				oa.set_vector(f(ia.get_vector(pi)), po);
			}
			for(; pi.scalar != end.scalar; ++pi.scalar, ++po.scalar) {
				oa.set_scalar(f(ia.get_scalar(pi)), po);
			}
		}
	} // End of priv_contiguous_aligned_storage
	using priv_contiguous_aligned_storage::transform;
	//- transform - contiguous_unaligned_storage -------------------------------
	// Select if both storageInT and storageOutT are contiguous_unaligned_storages
	namespace priv_contiguous_unaligned_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT,
			typename functorT>
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == true && 
			ct::length<
				typename ct::keep_only<
					CT_TYPELIST2(storageInT, storageOutT),
					uses_contiguous_unaligned_storage
				>::type
			>::value == 2
		>::type transform(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			std::cerr << "unaligned" << std::endl;
			typename storageOutT::iterator po = out.begin();
			typename storageInT::const_iterator pi = in.begin();
			typename storageInT::const_iterator end = in.end();
			typename accessorInT::vector_type iv;
			typename accessorOutT::vector_type ov;
			
			// Load location = input offset +
			//   (element count - output offset)
			// % step required because want values
			//   0 to element count - 1
			int ioffset = pi.scalar;
			int ooffset = po.scalar;
			int roffset = (VVM_SCALAR_COUNT - ooffset) % VVM_SCALAR_COUNT;
			if(roffset + ioffset > VVM_SCALAR_COUNT)
				++pi.vector;
			// Do Initial load
			typename accessorInT::vector_type iv_xtr = ia.get_vector(pi);
			// Do front with scalar processor
			for(; po.scalar != 0; ++pi, ++po)
				oa.set_scalar(f(ia.get_scalar(pi)), po);
			// Do middle with vector processor
			for(; pi.vector != end.vector; ++pi.vector, ++po.vector)
			{
				// Load unaligned
				iv = iv_xtr;
				iv_xtr = ia.get_vector(pi);
				iv = ia.get_vector(iv, iv_xtr, pi.scalar);
				// Do operation
				ov = f(iv);
				// Write aligned
				vvm::store(ov, po);
			}
			// Do end with scalar processor
			for(; pi.scalar != end.scalar; ++pi.scalar, ++po.scalar)
				oa.set_scalar(f(ia.get_scalar(pi)), po);
		}
	} // End of priv_contiguous_unaligned_storage
	//- transform<using_illife_storage> ------------------------------------
	// Select if both storageIntT and storageOutT are illife_storages
	namespace priv_illife_storage {
		template<
			typename storageInT,
			typename accessorInT, 
			typename storageOutT,
			typename accessorOutT,
			typename functorT>
		typename ct::enable_if<
			ct::length<
				typename ct::keep_only<
					CT_TYPELIST2(storageInT, storageOutT),
					uses_illife_storage
				>::type
			>::value == 2
		>::type transform(const storageInT& in, const accessorInT& ia, storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageInT::const_iterator i = in.begin();
			typename storageInT::const_iterator end = in.end();
			typename storageOutT::iterator o = out.begin();
			for(; i != end; ++i, ++o) {
				vvis::transform(*i, ia, *o, oa, f);
			}
		}
	} // End of priv_illife_storage
	using priv_illife_storage::transform;

	//= transform2 =============================================================
	//- transform2<using_unknown_storage> --------------------------------------
	namespace priv_unknown_storage {
		template<
			typename storageIn1T,
			typename accessorIn1T,
			typename storageIn2T,
			typename accessorIn2T,
			typename storageOutT,
			typename accessorOutT,
			typename functorT> 
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == false ||
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST3(storageIn1T, storageIn2T, storageOutT)>::type,
				priv::using_unknown_storage
			>::value
		>::type transform(const storageIn1T& in1, const accessorIn1T& ia1, 
		                  const storageIn2T& in2, const accessorIn2T& ia2,
		                  storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageIn1T::const_iterator pi1 = in1.begin();
			typename storageIn1T::const_iterator pi1_end = in1.end();
			typename storageIn2T::const_iterator pi2 = in2.begin();
			typename storageOutT::iterator po = out.begin();
			// Process using the scalar processor
			for(; pi1 != pi1_end; ++pi1, ++pi2, ++po) {
				oa.set_scalar(f(ia1.get_scalar(pi1), ia2.get_scalar(pi2)), po);
			}
		}
	} // End of priv_unknown_storage namespace
	using priv_unknown_storage::transform;
	//- transform2<using_contiguous_storage> -----------------------------------
	namespace priv_contiguous_storage {
		template<
			typename storageIn1T,
			typename accessorIn1T,
			typename storageIn2T,
			typename accessorIn2T,
			typename storageOutT,
			typename accessorOutT,
			typename functorT> 
		typename ct::enable_if<
			is_vectorised_functor<functorT>::value == true &&
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST3(storageIn1T, storageIn2T, storageOutT)>::type,
				priv::using_contiguous_storage
			>::value
		>::type transform(const storageIn1T& in1, const accessorIn1T& ia1,
		                  const storageIn2T& in2, const accessorIn2T& ia2,
		                  storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageIn1T::iterator pi1 = in1.begin();
			typename storageIn2T::iterator pi2 = in2.begin();
			typename storageIn1T::iterator end1 = in1.end();
			typename storageOutT::iterator po = out.begin();
			// Process front using the vector processor
			for(; pi1.vector != end1.vector; ++pi1.vector, ++pi2.vector, ++po.vector) {
				// Prefetch
				ia1.template prefetch_read<0>(pi1);
				ia2.template prefetch_read<accessorIn1T::prefetch_channel_count>(pi2);
				oa.template prefetch_write<accessorIn1T::prefetch_channel_count +
					accessorIn2T::prefetch_channel_count>(po);
				// Perform operation
				oa.set_vector(f(ia1.get_vector(pi1), ia2.get_vector(pi2)), po);
			}
			// Process rest using the scalar processor
			for(; pi1.scalar != end1.scalar; ++pi1.scalar, ++pi2.scalar, ++po.scalar) {
				oa.set_scalar(f(ia1.get_scalar(pi1), ia2.get_scalar(pi2)), po);
			}
		}
	} // End of priv_contiguous_storage
	using priv_contiguous_storage::transform;
	//- transform2<using_illife_storage> ---------------------------------------
	namespace priv_illife_storage {
		template<
			typename storageIn1T,
			typename accessorIn1T,
			typename storageIn2T,
			typename accessorIn2T,
			typename storageOutT,
			typename accessorOutT,
			typename functorT> 
		typename ct::enable_if<
			boost::is_same<
				typename priv::fastest_storage<CT_TYPELIST3(storageIn1T, storageIn2T, storageOutT)>::type,
				priv::using_illife_storage
			>::value
		>::type transform(const storageIn1T& in1, const accessorIn1T& ia1,
		                  const storageIn2T& in2, const accessorIn2T& ia2,
		                  storageOutT& out, const accessorOutT& oa, functorT f) {
			typename storageIn1T::const_iterator i1 = in1.begin();
			typename storageIn1T::const_iterator i1_end = in1.end();
			typename storageIn2T::const_iterator i2 = in2.begin();
			typename storageOutT::iterator o = out.begin();
			for(; i1 != i1_end; ++i1, ++i2, ++o) {
				vvis::transform(*i1, ia1, *i2, ia2, *o, oa, f);
			}
		}
	} // End of priv_illife_storage namespace
	using priv_illife_storage::transform;
} // End of vvis namespace

#endif

