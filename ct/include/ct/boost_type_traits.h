/*
 *  boost_type_traits.h
 *  ct
 *
 *  Created by Bing-Chang Lai on Fri Sep 12 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 *  Boost type_traits uses the word "check" which also happens to be a macro.
 *  This is a wrapper around type_traits.hpp to allow it to co-exist with MacOS X
 *
 */

// check is a preprocessor macro that gets defined through vecLib.h
// It has to be undefed because it interferes with boost::type_traits
#undef check

#include <boost/type_traits.hpp>

