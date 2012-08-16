/*
 *  typelist.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Sat Jul 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  Contains code derived from
 *    The Loki Library
 *    Copyright (c) 2001 by Andrei Alexandrescu
 *    Which accompanied the book:
 *      Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
 *      Patterns Applied". Copyright (c) 2001. Addison-Wesley.
 *    Copyright notice from The Loki Library:
 *      ////////////////////////////////////////////////////////////////////////////////
 *      // The Loki Library
 *      // Copyright (c) 2001 by Andrei Alexandrescu
 *      // This code accompanies the book:
 *      // Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
 *      //     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
 *      // Permission to use, copy, modify, distribute and sell this software for any 
 *      //     purpose is hereby granted without fee, provided that the above copyright 
 *      //     notice appear in all copies and that both that copyright notice and this 
 *      //     permission notice appear in supporting documentation.
 *      // The author or Addison-Wesley Longman make no representations about the 
 *      //     suitability of this software for any purpose. It is provided "as is" 
 *      //     without express or implied warranty.
 *      ////////////////////////////////////////////////////////////////////////////////
 *    It has been reimplemented to produce a more consistent naming style. In addition,
 *    reimplementation allows the author to freely add/modify structures without having
 *    to call it Loki (and thereby possibly polluting the original).
 *
 */

#ifndef _CT_TYPELIST_H
#define _CT_TYPELIST_H

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/tuple/tuple.hpp>

namespace ct {
	/** Typelist
	 */
	template<typename headT, typename tailT> struct typelist {
		typedef headT head;
		typedef tailT tail;
	};
	
	/** Typelist terminator.
	 * Use this type to terminate the typelist
	 * Using boost::null_type instead of creating own for tuple
	 */
	using boost::tuples::null_type;
	
	//====================================================================================
	// Typelist constructors
	// Allows up to 25 parameters.
	//------------------------------------------------------------------------------------
	template<
		typename T1,
		typename T2 = null_type,
		typename T3 = null_type,
		typename T4 = null_type,
		typename T5 = null_type,
		typename T6 = null_type,
		typename T7 = null_type,
		typename T8 = null_type,
		typename T9 = null_type,
		typename T10 = null_type,
		typename T11 = null_type,
		typename T12 = null_type,
		typename T13 = null_type,
		typename T14 = null_type,
		typename T15 = null_type,
		typename T16 = null_type,
		typename T17 = null_type,
		typename T18 = null_type,
		typename T19 = null_type,
		typename T20 = null_type,
		typename T21 = null_type,
		typename T22 = null_type,
		typename T23 = null_type,
		typename T24 = null_type,
		typename T25 = null_type
	> struct cons_tl {
		typedef typelist<T1, typelist<T2, typelist<T3, typelist<T4, typelist<T5,
			typelist<T6, typelist<T7, typelist<T8, typelist<T9, typelist<T10,
			typelist<T11, typelist<T12, typelist<T13, typelist<T14, typelist<T15,
			typelist<T16, typelist<T17, typelist<T18, typelist<T19, typelist<T20,
			typelist<T21, typelist<T22, typelist<T23, typelist<T24, typelist<T25, null_type
			> > > > >
			> > > > >
			> > > > >
			> > > > >
			> > > > > type;
	};
	template<typename T1>
	struct cons_tl<T1> {
		typedef typelist<T1, null_type> type;
	};
	template<typename T1, typename T2>
	struct cons_tl<T1, T2> {
		typedef typelist<T1, typelist<T2, null_type> > type;
	};
	template<typename T1, typename T2, typename T3>
	struct cons_tl<T1, T2, T3> {
		typedef typelist<T1, typename cons_tl<T2, T3>::type> type;
	};
	template<typename T1, typename T2, typename T3, typename T4>
	struct cons_tl<T1, T2, T3, T4> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4>::type> type;
	};
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	struct cons_tl<T1, T2, T3, T4, T5> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6
	> struct cons_tl<T1, T2, T3, T4, T5, T6> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19, typename T20
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19, typename T20,
		typename T21
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19, typename T20,
		typename T21, typename T22
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19, typename T20,
		typename T21, typename T22, typename T23
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>::type> type;
	};
	template<
		typename T1, typename T2, typename T3, typename T4, typename T5,
		typename T6, typename T7, typename T8, typename T9, typename T10,
		typename T11, typename T12, typename T13, typename T14, typename T15,
		typename T16, typename T17, typename T18, typename T19, typename T20,
		typename T21, typename T22, typename T23, typename T24
	> struct cons_tl<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> {
		typedef typelist<T1, typename cons_tl<T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>::type> type;
	};

	//- cons_tl_repeat -------------------------------------------------------------------
	template<typename T, int count> struct cons_tl_repeat {
		typedef typelist<T, typename cons_tl_repeat<T, count-1>::type> type;
	};
	template<typename T> struct cons_tl_repeat<T, 0> {
		typedef null_type type;
	};

	//- Constructor Macros -----------------------------------------------------
	#define CT_TYPELIST1(T1) ct::typelist<T1, ct::null_type> 
	#define CT_TYPELIST2(T1, T2) ct::typelist<T1, CT_TYPELIST1(T2) > 
	#define CT_TYPELIST3(T1, T2, T3) ct::typelist<T1, CT_TYPELIST2(T2, T3) > 
	#define CT_TYPELIST4(T1, T2, T3, T4) ct::typelist<T1, CT_TYPELIST3(T2, T3, T4) > 
	#define CT_TYPELIST5(T1, T2, T3, T4, T5) ct::typelist<T1, CT_TYPELIST4(T2, T3, T4, T5) > 
	
	//====================================================================================
	// Metafunctions
	//------------------------------------------------------------------------------------
	/** Returns the type at a given index in a typelist
	 * This code is derived from Loki::TypeAt. The names were changed to better match
	 * CT's style.
	 */
	template<class typelistT, unsigned int index> struct type_at;
	template<class headT, class tailT>
	struct type_at<typelist<headT, tailT>, 0> {
		typedef headT type;
	};
	template<class headT, class tailT, unsigned int i>
	struct type_at<typelist<headT, tailT>, i> {
		typedef typename type_at<tailT, i - 1>::type type;
	};
	
	/** Returns the type at a given index in a typelist, or null_type if invalid
	 * This code is derived from Loki::TypeAtNonStrict. The names were changed to better
	 * match CT's style.
	 */
	template<typename typelistT, unsigned int index, typename defaultT = null_type>
	struct type_at_non_strict {
		typedef defaultT type;
	};
	template<typename headT, class tailT, typename defaultT>
	struct type_at_non_strict<typelist<headT, tailT>, 0, defaultT> {
		typedef headT type;
	};
	template<typename headT, class tailT, unsigned int i, typename defaultT>
	struct type_at_non_strict<typelist<headT, tailT>, i, defaultT> {
		typedef typename 
			type_at_non_strict<tailT, i-1, defaultT>::type type;
	};
	
	/** Returns the index of a given type in a typelist
	 * Returns -1 if not found.
	 * This code is dervied from Loki::IndexOf. The names were changed to better match
	 * CT's style.
	 */
	template<typename typelistT, typename T> struct index_of;
	template<typename T> struct index_of<null_type, T> {
		static const int value = -1;
	};
	template<typename T, typename tailT> struct index_of<typelist<T, tailT>, T> {
		static const int value = 0;
	};
	template<typename headT, typename tailT, typename T> struct index_of<typelist<headT, tailT>, T> {
	private:
		static const int temp = index_of<tailT, T>::value;
	public:
		static const int value = (temp == -1 ? -1 : 1 + temp);
	};
	
	/** Locates first type that satisfies exprT
	 * If type is not found in the typelist returns notFoundT
	 * notFoundT defaults to null_type
	 */
	template<typename typelistT, template<typename> class exprT, typename notFoundT = null_type>
	struct find_first;
	template<typename headT, typename tailT, template<typename> class exprT, typename notFoundT>
	struct find_first<typelist<headT, tailT>, exprT, notFoundT> {
		typedef typename boost::mpl::if_c<
			exprT<headT>::value,
			headT,
			typename find_first<tailT, exprT, notFoundT>::type
			>::type type;
	};
	template<template<typename> class exprT, typename notFoundT>
	struct find_first<null_type, exprT, notFoundT> {
		typedef notFoundT type;
	};

	/** Returns the number of elements in a typelist
	 * This code is dervied from Loki::Length. The names were changed to better match
	 * CT's style.
	 */
	template<typename typelistT> struct length;
	template<> struct length<null_type> {
		static const int value = 0;
	};
	template<typename T, typename U> struct length< typelist<T, U> > {
		static const int value = 1 + length<U>::value;
	};
	
	/** Returns a typelist that is TL followed by T
	 * This code is dervied from Loki::Append. The names were changed to better match
	 * CT's style.
	 */
	template<typename TL, typename T> struct append;
	template<> struct append<null_type, null_type> {
		typedef null_type type;
	};
	template<typename T> struct append<null_type, T> {
		typedef typename cons_tl<T>::type type;
	};
	template<typename headT, typename tailT>
	struct append<null_type, typelist<headT, tailT> > {
		typedef typelist<headT, tailT> type;
	};
	template<typename headT, typename tailT, typename T>
	struct append<typelist<headT, tailT>, T> {
		typedef typelist<headT, 
			typename append<tailT, T>::type> type;
	};
	
	/** Returns a typelist that is TL without the first occurrence of T
	 * This code is derived from Loki::Erase. The name was changed to better match
	 * CT's style.
	 */
	template<typename TL, typename T> struct erase;
	template<typename T>
	struct erase<null_type, T> {
		typedef null_type type;
	};
	template<typename T, class tailT>
	struct erase<typelist<T, tailT>, T> {
		typedef tailT type;
	};
	template<typename headT, typename tailT, typename T>
	struct erase<typelist<headT, tailT>, T> {
		typedef typelist<headT,
			typename erase<tailT, T>::type>	type;
	};

	/** Returns a typelist that is TL without any occurrence of T
	 * This code is derived from Loki::EraseAll. The name was changed to better
	 * match CT's style.
	 */
	template<typename TL, typename T> struct erase_all;
	template<typename T> struct erase_all<null_type, T> {
		typedef null_type type;
	};
	template<typename T, typename tailT>
	struct erase_all<typelist<T, tailT>, T> {
		// Go all the way down the list removing the type
		typedef typename erase_all<tailT, T>::type type;
	};
	template<typename headT, typename tailT, class T>
	struct erase_all<typelist<headT, tailT>, T> {
		// Go all the way down the list removing the type
		typedef typelist<headT, 
			typename erase_all<tailT, T>::type> type;
	};

	/** Returns a typelist that is TL without any duplicates
	 * This code is derived from Loki::no_duplicates. The name was changed to
	 * better match CT's style.
	 */
	template<typename TL> struct no_duplicates;
	template<> struct no_duplicates<null_type> {
		typedef null_type type;
	};
	template <typename headT, typename tailT>
	struct no_duplicates< typelist<headT, tailT> >
	{
	private:
		typedef typename no_duplicates<tailT>::type tl1;
		typedef typename erase<tl1, headT>::type tl2;
	public:
		typedef typelist<headT, tl2> type;
	};

	/** Applies operation to the typelist
	 */
	template<typename typelistT, template<typename> class operationT>
	struct apply {
		typedef typelist<
			typename operationT<typename typelistT::head>::type,
			typename apply<typename typelistT::tail, operationT>::type
			> type;
	};
	template<template<typename> class operationT>
	struct apply<null_type, operationT> {
		typedef null_type type;
	};
	
	/** Keeps only types for which exprT returns true
	 */
	template<typename typelistT, template<typename> class exprT> struct keep_only;
	// keep_only specialization for a non-empty typelist
	template<typename headT, typename tailT, template<typename> class exprT>
	struct keep_only<typelist<headT, tailT>, exprT> {
		typedef typename boost::mpl::if_c<
			exprT<headT>::value,
			typelist<headT, typename keep_only<tailT, exprT>::type>,
			typename keep_only<tailT, exprT>::type
			>::type type;
	};
	// keep_only specialization for empty typelist
	template<template<typename> class exprT>
	struct keep_only<null_type, exprT> {
		typedef null_type type;
	};
	
	/** Checks if a type is in the typelist
	 * Returns true if the type was found, otherwise false
	 */
	template<typename typelistT, typename T> struct contains {
		static const int value = index_of<typelistT, T>::value != -1;
	};
	
	/** Applies expression to the typelist, ORing results
	 * Returns true if any is true
	 */
	template<typename typelistT, template<typename> class exprT> struct any {
		static const int
			value = exprT<typename typelistT::head>::value ||
				any<typename typelistT::tail, exprT>::value;
	};
	template<template<typename T> class exprT> struct any<null_type, exprT> {
		static const int value = false;
	};
	
	/** Applies expression to the typelist, ANDing results
	 * Returns true only if all is true
	 */
	template<typename typelistT, template<typename T> class exprT> struct all {
		static const int
			value = exprT<typename typelistT::head>::value &&
				all<typename typelistT::tail, exprT>::value;
	};
	template<template<typename T> class exprT> struct all<null_type, exprT> {
		static const int value = true;
	};
} // End of vvis namespace

#endif
