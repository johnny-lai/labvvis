/*
 *  base_operation.h
 *  vvm
 *
 *  Created by Bing-Chang Lai on Wed Oct 01 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#ifndef _VVM_BASE_OPERATION_H
#define _VVM_BASE_OPERATION_H

// Used to specialize operations.
#define _VVM_VPVECTOR_IN(TL) \
	typename ct::enable_if< \
		ct::contains< \
			TL, \
			typename scalar_traits<scalarT>::vpvector_type \
		>::value \
	>::type

#define _VVM_VPVECTOR_IS(T) \
	typename ct::enable_if< \
		boost::is_same<typename scalar_traits<scalarT>::vpvector_type, T>::value \
	>::type

namespace vvm {
	namespace priv {
		//- scalar_iteration ---------------------------------------------------
		// Scalar Iteration
		template<typename vectorOutT> struct scalar_iteration {
			enum { count = vector_traits<vectorOutT>::scalar_count };
			//- unary_expr -----------------------------------------------------
			template<
				typename vectorInT,
				typename operationT
			> struct unary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorInT a) {
						operationT::op(ret.scalar(i), a.scalar(i));
					}
				};
			};
			//- binary_expr ----------------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename operationT
			> struct binary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b) {
						operationT::op(ret.scalar(i), a.scalar(i), b.scalar(i));
					}
				};
			};
			//- ternary_expr ---------------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename vectorIn3T,
				typename operationT
			> struct ternary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b,
					vectorIn3T c) {
						operationT::op(ret.scalar(i), a.scalar(i), b.scalar(i), c.scalar(i));
					}
				};
			};
			//- assign_expr ----------------------------------------------------
			template<
				typename vectorInT,
				typename operationT
			> struct assign_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorInT a) {
						operationT::op(ret.scalar(i), a.scalar(i));
					}
				};
			};
			//- binary_predicate -----------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename operationT
			> struct binary_predicate {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b) {
						operationT::op(ret, a.scalar(i), b.scalar(i));
					}
				};
			};
		};
		//- vpvector_iteration -------------------------------------------------
		// Vp::vector Iteration
		template<typename vectorOutT> struct vpvector_iteration {
			enum { count = vector_traits<vectorOutT>::vpvector_count };
			//- unary_expr ----------------------------------------------------
			template<
				typename vectorInT,
				typename operationT
			> struct unary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorInT a) {
						operationT::op(ret.vpvector(i), a.vpvector(i));
					}
				};
			};
			//- binary_expr ----------------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename operationT
			> struct binary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b) {
						operationT::op(ret.vpvector(i), a.vpvector(i), b.vpvector(i));
					}
				};
			};
			//- ternary_expr ---------------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename vectorIn3T,
				typename operationT
			> struct ternary_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b,
					vectorIn3T c) {
						operationT::op(ret.vpvector(i), a.vpvector(i), b.vpvector(i), c.vpvector(i));
					}
				};
			};
			//- assign_expr ----------------------------------------------------
			template<
				typename vectorInT,
				typename operationT
			> struct assign_expr {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorInT a) {
						operationT::op(ret.vpvector(i), a.vpvector(i));
					}
				};
			};
			//- binary_predicate -----------------------------------------------
			template<
				typename vectorIn1T,
				typename vectorIn2T,
				typename operationT
			> struct binary_predicate {
				template<int i> struct Code {
					inline static void
					exec(vectorOutT& ret, vectorIn1T a, vectorIn2T b) {
						operationT::op(ret, a.vpvector(i), b.vpvector(i));
					}
				};
			};
		};
		//- base_unary_operation -----------------------------------------------
		template<
			typename vectorOutT,
			typename vectorInT,
			typename operationT,
			template<typename> class iterationP = vpvector_iteration
		> struct base_unary_operation {
			inline static const vectorOutT
			exec(vectorInT a) {
				vectorOutT ret;
				meta::EFOR2<0, meta::Less, iterationP<vectorOutT>::count, +1,
					typename iterationP<vectorOutT>::template unary_expr<vectorInT, operationT>
					>::exec(ret, a);
				return ret;
			}
		};
		//- base_binary_operation ----------------------------------------------
		template<
			typename vectorOutT,
			typename vectorIn1T,
			typename vectorIn2T,
			typename operationT,
			template<typename> class iterationP = vpvector_iteration
		> struct base_binary_operation {
			inline static const vectorOutT
			exec(vectorIn1T a, vectorIn2T b) {
				vectorOutT ret;
				meta::EFOR3<0, meta::Less, iterationP<vectorOutT>::count, +1,
					typename iterationP<vectorOutT>::
						template binary_expr<vectorIn1T, vectorIn2T, operationT>
					>::exec(ret, a, b);
				return ret;
			}
		};
		//- base_ternary_operation ---------------------------------------------
		template<
			typename vectorOutT,
			typename vectorIn1T,
			typename vectorIn2T,
			typename vectorIn3T,
			typename operationT,
			template<typename> class iterationP = vpvector_iteration
		> struct base_ternary_operation {
			inline static const vectorOutT
			exec(vectorIn1T a, vectorIn2T b, vectorIn3T c) {
				vectorOutT ret;
				meta::EFOR4<0, meta::Less, iterationP<vectorOutT>::count, +1,
					typename iterationP<vectorOutT>::
						template ternary_expr<vectorIn1T, vectorIn2T, vectorIn3T, operationT>
					>::exec(ret, a, b, c);
				return ret;
			}
		};
		//- base_assign_operation ----------------------------------------------
		template<
			typename vectorOutT,
			typename vectorInT,
			typename operationT,
			template<typename> class iterationP = vpvector_iteration
		> struct base_assign_operation {
			inline static vectorOutT&
			exec(vectorOutT& ret, vectorInT a) {
				meta::EFOR2<0, meta::Less, iterationP<vectorOutT>::count, +1,
					typename iterationP<vectorOutT>::
						template assign_expr<vectorInT, operationT>
					>::exec(ret, a);
				return ret;
			}
		};
		//- base_binary_predicate ----------------------------------------------
		template<
			bool init,
			typename vectorIn1T,
			typename vectorIn2T,
			typename operationT,
			template<typename> class iterationP = vpvector_iteration
		> struct base_binary_predicate {
			inline static const bool
			exec(vectorIn1T a, vectorIn2T b) {
				bool ret = init;
				meta::EFOR3<0, meta::Less,
					iterationP<
						typename ct::remove_all<vectorIn1T>::type>::count, +1,
					typename iterationP<bool>::
						template binary_predicate<vectorIn1T, vectorIn2T, operationT>
					>::exec(ret, a, b);
				return ret;
			}
		};
	} // End of priv namespace
} // End of vvm namespace

#endif
