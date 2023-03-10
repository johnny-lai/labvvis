//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//  Adaptation to Boost of the libcxx
//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/ratio/ratio.hpp>
#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif
#define BOOST_RATIO_INTMAX_T_MAX (0x7FFFFFFFFFFFFFFFLL)

void test()
{
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R1;
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, 1> R1;
    typedef boost::ratio<1, -1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<-BOOST_RATIO_INTMAX_T_MAX, 1> R1;
    typedef boost::ratio<BOOST_RATIO_INTMAX_T_MAX, 1> R2;
    BOOST_RATIO_STATIC_ASSERT((boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
    {
    typedef boost::ratio<1, BOOST_RATIO_INTMAX_T_MAX> R1;
    typedef boost::ratio<1, -BOOST_RATIO_INTMAX_T_MAX> R2;
    BOOST_RATIO_STATIC_ASSERT((!boost::ratio_less_equal<R1, R2>::value), NOTHING, ());
    }
}
