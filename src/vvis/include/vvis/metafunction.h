/*
 *  metafunction.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Sun Aug 31 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVIS_METAFUNCTION_H
#define _VVIS_METAFUNCTION_H

#include <ct/boost_type_traits.h>
#include <ct/metafunction.h>
#include <vvm/vvm.h>
#include <vvis/image_base.h>
#include <vvis/shape.h>

namespace vvis {
	/** Returns true if T is an image.
	 */
	template<typename T> struct is_image {
		enum { value = boost::is_base_and_derived<image_base, T>::value };
	};
	
	//==========================================================================
	// Unknown Storage
	//--------------------------------------------------------------------------
	/** Inherit from this class to say that you are implementing the unknown
	 * storage interface.
	 */
	class unknown_storage {};

	//==========================================================================
	// Contiguous Storage
	//--------------------------------------------------------------------------
	/** Inherit from this class to say that you are implementing the contiguous
	 * storage interface.
	 */
	class contiguous_storage {};

	/** Inherit from this class to say that you are implementing the contiguous
	 * storage interface, but begin must be aligned
	 */
	class contiguous_aligned_storage : public contiguous_storage {};

	/** Inherit from this class to say that you are implementing the contiguous
	 * storage interface, but begin might be unaligned
	 */
	class contiguous_unaligned_storage : public contiguous_storage {};
	
	//==========================================================================
	// Illife Storage
	//--------------------------------------------------------------------------
	/** Inherit from this class to say that you are implementing the illife
	 * storage interface.
	 */
	class illife_storage {};


	//==========================================================================
	// Storage conversions
	//--------------------------------------------------------------------------
	template<typename storageT> struct aligned_storage {
		typedef storageT type;
	};
	template<typename storageT> struct unaligned_storage {
		typedef storageT type;
	};

	//==========================================================================
	// Functor
	//--------------------------------------------------------------------------
	template<typename functorT> struct is_vectorised_functor {
		static const bool value = true;
	};
	
	//------------------------------------------------------------------------------------
	/// Metafunction to check if an image uses illife storage
	template<typename imageT> struct uses_illife_storage {
		static const bool value = boost::is_base_and_derived<
			illife_storage, imageT>::value;
	};
	
	/// Metafunction to check if an image uses contiguous storage
	template<typename imageT> struct uses_contiguous_storage {
#if defined _VVIS_TREAT_CONTIGUOUS_AS_UNKNOWN
		static const bool value = false;
#else
		static const bool value = boost::is_base_and_derived<
			contiguous_storage, imageT>::value;
#endif
	};
	
	/// Metafunction to check if an image uses contiguous unaligned storage
	template<typename imageT> struct uses_contiguous_aligned_storage {
#if defined _VVIS_TREAT_CONTIGUOUS_AS_UNKNOWN
		static const bool value = false;
#else
		static const bool value = boost::is_base_and_derived<
			contiguous_aligned_storage, imageT>::value;
#endif
	};
	
	/// Metafunction to check if an image uses contiguous unaligned storage
	template<typename imageT> struct uses_contiguous_unaligned_storage {
#if defined _VVIS_TREAT_CONTIGUOUS_AS_UNKNOWN
		static const bool value = false;
#else
		static const bool value = boost::is_base_and_derived<
			contiguous_unaligned_storage, imageT>::value;
#endif
	};
	
	/** Metafunction to check if an image uses unknown storage
	 * Unknown storages refers to storages that VVIS does not know about.
	 * In this case, VVIS takes the safe route by assuming that there is no
	 * relationship between the different elements - ie. it regards it as
	 * disjointed.
	 */
	template<typename imageT> struct uses_unknown_storage {
		static const bool value = !uses_contiguous_storage<imageT>::value &&
			!uses_illife_storage<imageT>::value;
	};
	
	/** Metafunction to check is a storage is rectangular
	 */
	template<typename storageT>	struct is_rectangular {
		static const bool value = boost::is_base_and_derived<
			rectangular, storageT>::value;
	};

	//------------------------------------------------------------------------------------
	// These metafunctions are duplicated in priv because if the class uses the
	// vvis versions, the user will be unable to use the vvis version on it
	namespace priv {
		/// Metafunction to check if an image uses illife storage
		template<typename imageT> struct uses_illife_storage {
            static const bool value = false;
            /*
			static const bool value = boost::is_base_and_derived<
				vvis::illife_storage, imageT>::value;
                */
		};
		
		/// Metafunction to check if an image uses contiguous storage
		template<typename imageT> struct uses_contiguous_storage {
#if defined _VVIS_TREAT_CONTIGUOUS_AS_UNKNOWN
			static const bool value = false;
#else
			static const bool value = boost::is_base_and_derived<
				contiguous_storage, imageT>::value;
#endif
		};
		/** Metafunction to check if an image uses unknown storage
		* Unknown storages refers to storages that VVIS does not know about.
		* In this case, VVIS takes the safe route by assuming that there is no
		* relationship between the different elements - ie. it regards it as
		* disjointed.
		*/
		template<typename imageT> struct uses_unknown_storage {
			static const bool value = !uses_contiguous_storage<imageT>::value &&
				!uses_illife_storage<imageT>::value;
		};
	} // End of priv namespace
	
		
	namespace priv {
		//-vvm_align_prev ------------------------------------------------------
		struct vvm_align_prev {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(const T& src, U& dest) {
					dest.template get<i>() = vvm::align_prev(src.template get<i>());
				}
			};
		};
		//-vvm_align_next ------------------------------------------------------
		struct vvm_align_next {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(const T& src, U& dest) {
					dest.template get<i>() = vvm::align_next(src.template get<i>());
				}
			};
		};
		//-vvm_load -----------------------------------------------------------
		struct vvm_load {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(const T& src, U& dest, vvm::offset_t offset = 0) {
					dest.template get<i>() = vvm::load(src.template get<i>(), offset);
				}
				template<typename T, typename U>
				static void exec(const T& a, const T& b, U& dest, vvm::offset_t shift) {
					dest.template get<i>() = vvm::load(a.template get<i>(), b.template get<i>(), shift);
				}
			};
		};
		//-vvm_load_shift_element ----------------------------------------------
		struct vvm_load_shift_element {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(const T& src, U& dest, vvm::offset_t offset = 0) {
					dest.template get<i>() = vvm::load(src.template get<i>() + offset);
				}
				template<typename T, typename U>
				static void exec(const T& a, const T& b, U& dest, vvm::offset_t shift) {
					dest.template get<i>() = vvm::load(a.template get<i>(), b.template get<i>(), shift);
				}
			};
		};
		//- vvm_store ----------------------------------------------------------
		struct vvm_store {
			template<int i> struct Code {
				template<typename T, typename U>
				static void exec(const T& src, U& dest) {
					vvm::store(src.template get<i>(), dest.template get<i>(), 0);
				}
			};
		};
	} // End of priv namespace

} // End of vvis namespace

#endif
