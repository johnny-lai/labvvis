//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Copyright (C) 2011 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <boost/thread/future.hpp>
// class packaged_task<R>

// packaged_task(packaged_task&) = delete;


#define BOOST_THREAD_VERSION 3
#include <boost/thread/future.hpp>
#include <boost/detail/lightweight_test.hpp>

class A
{
    long data_;

public:
    explicit A(long i) : data_(i) {}

    long operator()() const {return data_;}
    long operator()(long i, long j) const {return data_ + i + j;}
};


int main()
{
  {
    boost::packaged_task<double> p0(A(5));
    boost::packaged_task<double> p(p0);

  }

  return boost::report_errors();
}

void remove_unused_warning()
{
  //../../../boost/system/error_code.hpp:214:36: warning: ?boost::system::posix_category? defined but not used [-Wunused-variable]
  //../../../boost/system/error_code.hpp:215:36: warning: ?boost::system::errno_ecat? defined but not used [-Wunused-variable]
  //../../../boost/system/error_code.hpp:216:36: warning: ?boost::system::native_ecat? defined but not used [-Wunused-variable]

  (void)boost::system::posix_category;
  (void)boost::system::errno_ecat;
  (void)boost::system::native_ecat;
}
