/*
 *  empty_type.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Sun Aug 17 2003.
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

#ifndef _CT_EMPTY_TYPE_H
#define _CT_EMPTY_TYPE_H

namespace ct {
	/** empty_type
	 * Used as a class that doesn't hold anything
	 * Useful as a strawman class
	 * From Loki::EmptyType
	 */
    class empty_type {
	};
} // End of ct namespace

#endif
