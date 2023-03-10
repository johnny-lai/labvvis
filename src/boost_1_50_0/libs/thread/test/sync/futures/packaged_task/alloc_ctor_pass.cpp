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

// template <class F, class Allocator>
//     explicit packaged_task(allocator_arg_t, const Allocator& a, F&& f);


#define BOOST_THREAD_VERSION 3

#include <boost/thread/future.hpp>
#include <boost/detail/lightweight_test.hpp>
#if defined BOOST_THREAD_PROVIDES_FUTURE_CTOR_ALLOCATORS
#include <libs/thread/test/sync/futures/test_allocator.hpp>

double fct()
{
  return 5.0;
}
long lfct()
{
  return 5;
}

class A
{
  long data_;

public:
  BOOST_THREAD_COPYABLE_AND_MOVABLE(A)
  static int n_moves;
  static int n_copies;

  explicit A(long i) : data_(i)
  {
  }
  A(BOOST_THREAD_RV_REF(A) a) : data_(BOOST_THREAD_RV(a).data_)
  {
    ++n_moves; BOOST_THREAD_RV(a).data_ = -1;
  }
  A(const A& a) : data_(a.data_)
  {
    ++n_copies;
  }
  ~A()
  {
  }

  long operator()() const
  { return data_;}
  long operator()(long i, long j) const
  { return data_ + i + j;}
};

int A::n_moves = 0;
int A::n_copies = 0;

int main()
{
  {
    boost::packaged_task<double> p(boost::allocator_arg,
        test_allocator<A>(), BOOST_THREAD_MAKE_RV_REF(A(5)));
    BOOST_TEST(test_alloc_base::count > 0);
    BOOST_TEST(p.valid());
    boost::future<double> f = BOOST_THREAD_MAKE_RV_REF(p.get_future());
    //p(3, 'a');
    p();
    BOOST_TEST(f.get() == 5.0);
  }
  BOOST_TEST(A::n_copies == 0);
  BOOST_TEST(A::n_moves > 0);
  BOOST_TEST(test_alloc_base::count == 0);
  A::n_copies = 0;
  A::n_copies = 0;
  {
    A a(5);
    boost::packaged_task<double> p(boost::allocator_arg,
        test_allocator<A>(), a);
    BOOST_TEST(test_alloc_base::count > 0);
    BOOST_TEST(p.valid());
    boost::future<double> f = BOOST_THREAD_MAKE_RV_REF(p.get_future());
    //p(3, 'a');
    p();
    BOOST_TEST(f.get() == 5.0);
  }
  BOOST_TEST(A::n_copies > 0);
  BOOST_TEST(A::n_moves > 0);
  BOOST_TEST(test_alloc_base::count == 0);
  A::n_copies = 0;
  A::n_copies = 0;
  {
    const A a(5);
    boost::packaged_task<double> p(boost::allocator_arg,
        test_allocator<A>(), a);
    BOOST_TEST(test_alloc_base::count > 0);
    BOOST_TEST(p.valid());
    boost::future<double> f = BOOST_THREAD_MAKE_RV_REF(p.get_future());
    //p(3, 'a');
    p();
    BOOST_TEST(f.get() == 5.0);
  }
  BOOST_TEST(A::n_copies > 0);
  BOOST_TEST(A::n_moves > 0);
  BOOST_TEST(test_alloc_base::count == 0);
  {
    boost::packaged_task<double> p(boost::allocator_arg,
        test_allocator<A>(), fct);
    BOOST_TEST(test_alloc_base::count > 0);
    BOOST_TEST(p.valid());
    boost::future<double> f = BOOST_THREAD_MAKE_RV_REF(p.get_future());
    //p(3, 'a');
    p();
    BOOST_TEST(f.get() == 5.0);
  }
  {
    boost::packaged_task<double> p(boost::allocator_arg,
        test_allocator<A>(), &lfct);
    BOOST_TEST(test_alloc_base::count > 0);
    BOOST_TEST(p.valid());
    boost::future<double> f = BOOST_THREAD_MAKE_RV_REF(p.get_future());
    //p(3, 'a');
    p();
    BOOST_TEST(f.get() == 5.0);
  }

  return boost::report_errors();
}

#else
int main()
{
  return boost::report_errors();
}
#endif

