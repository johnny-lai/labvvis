// (C) Copyright 2006-7 Anthony Williams
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <libs/thread/test/util.inl>
#include <libs/thread/test/shared_mutex_locking_thread.hpp>

#if defined BOOST_THREAD_USES_CHRONO

#define CHECK_LOCKED_VALUE_EQUAL(mutex_name,value,expected_value)    \
    {                                                                \
        boost::mutex::scoped_lock lock(mutex_name);                  \
        BOOST_CHECK_EQUAL(value,expected_value);                     \
    }


void test_timed_lock_shared_times_out_if_write_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;
    unsigned unblocked_count=0;
    boost::mutex::scoped_lock finish_lock(finish_mutex);
    boost::thread writer(simple_writing_thread(rw_mutex,finish_mutex,unblocked_mutex,unblocked_count));
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    CHECK_LOCKED_VALUE_EQUAL(unblocked_mutex,unblocked_count,1u);

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_shared_until(timeout);
    BOOST_CHECK((timeout-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_shared_for(wait_duration);
    BOOST_CHECK((timeout2-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    finish_lock.unlock();
    writer.join();
}

void test_timed_lock_shared_succeeds_if_no_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_shared_until(timeout);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_shared_for(wait_duration);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout2);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

}

void test_timed_lock_shared_succeeds_if_read_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;
    unsigned unblocked_count=0;
    boost::mutex::scoped_lock finish_lock(finish_mutex);
    boost::thread reader(simple_reading_thread(rw_mutex,finish_mutex,unblocked_mutex,unblocked_count));
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    CHECK_LOCKED_VALUE_EQUAL(unblocked_mutex,unblocked_count,1u);

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_shared_until(timeout);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_shared_for(wait_duration);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout2);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    finish_lock.unlock();
    reader.join();
}

void test_timed_lock_times_out_if_write_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;
    unsigned unblocked_count=0;
    boost::mutex::scoped_lock finish_lock(finish_mutex);
    boost::thread writer(simple_writing_thread(rw_mutex,finish_mutex,unblocked_mutex,unblocked_count));
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    CHECK_LOCKED_VALUE_EQUAL(unblocked_mutex,unblocked_count,1u);

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_until(timeout);
    BOOST_CHECK((timeout-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_for(wait_duration);
    BOOST_CHECK((timeout2-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    finish_lock.unlock();
    writer.join();
}

void test_timed_lock_succeeds_if_no_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_until(timeout);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_for(wait_duration);
    BOOST_CHECK(boost::chrono::steady_clock::now()<timeout2);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

}

void test_timed_lock_times_out_if_read_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;
    unsigned unblocked_count=0;
    boost::mutex::scoped_lock finish_lock(finish_mutex);
    boost::thread reader(simple_reading_thread(rw_mutex,finish_mutex,unblocked_mutex,unblocked_count));
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    CHECK_LOCKED_VALUE_EQUAL(unblocked_mutex,unblocked_count,1u);

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    boost::chrono::milliseconds const timeout_resolution(50);
    bool timed_lock_succeeded=rw_mutex.try_lock_until(timeout);
    BOOST_CHECK((timeout-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    boost::chrono::milliseconds const wait_duration(500);
    boost::chrono::steady_clock::time_point const timeout2=boost::chrono::steady_clock::now()+wait_duration;
    timed_lock_succeeded=rw_mutex.try_lock_for(wait_duration);
    BOOST_CHECK((timeout2-timeout_resolution)<boost::chrono::steady_clock::now());
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    finish_lock.unlock();
    reader.join();
}

void test_timed_lock_times_out_but_read_lock_succeeds_if_read_lock_held()
{
    boost::shared_mutex rw_mutex;
    boost::mutex finish_mutex;
    boost::mutex unblocked_mutex;
    unsigned unblocked_count=0;
    boost::mutex::scoped_lock finish_lock(finish_mutex);
    boost::thread reader(simple_reading_thread(rw_mutex,finish_mutex,unblocked_mutex,unblocked_count));
    boost::this_thread::sleep_for(boost::chrono::seconds(1));
    CHECK_LOCKED_VALUE_EQUAL(unblocked_mutex,unblocked_count,1u);

    boost::chrono::steady_clock::time_point const start=boost::chrono::steady_clock::now();
    boost::chrono::steady_clock::time_point const timeout=start+boost::chrono::milliseconds(500);
    bool timed_lock_succeeded=rw_mutex.try_lock_until(timeout);
    BOOST_CHECK(!timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock();
    }

    boost::chrono::milliseconds const wait_duration(500);
    timed_lock_succeeded=rw_mutex.try_lock_shared_for(wait_duration);
    BOOST_CHECK(timed_lock_succeeded);
    if(timed_lock_succeeded)
    {
        rw_mutex.unlock_shared();
    }

    finish_lock.unlock();
    reader.join();
}


boost::unit_test::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test::test_suite* test =
        BOOST_TEST_SUITE("Boost.Threads: shared_mutex test suite");

    test->add(BOOST_TEST_CASE(&test_timed_lock_shared_times_out_if_write_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_shared_succeeds_if_no_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_shared_succeeds_if_read_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_times_out_if_write_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_times_out_if_read_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_succeeds_if_no_lock_held));
    test->add(BOOST_TEST_CASE(&test_timed_lock_times_out_but_read_lock_succeeds_if_read_lock_held));

    return test;
}

#else
#error "Test not applicable: BOOST_THREAD_USES_CHRONO not defined for this platform as not supported"
#endif


void remove_unused_warning()
{

  //../../../boost/test/results_collector.hpp:40:13: warning: unused function 'first_failed_assertion' [-Wunused-function]
  //(void)first_failed_assertion;

  //../../../boost/test/tools/floating_point_comparison.hpp:304:25: warning: unused variable 'check_is_close' [-Wunused-variable]
  //../../../boost/test/tools/floating_point_comparison.hpp:326:25: warning: unused variable 'check_is_small' [-Wunused-variable]
  (void)boost::test_tools::check_is_close;
  (void)boost::test_tools::check_is_small;

}
