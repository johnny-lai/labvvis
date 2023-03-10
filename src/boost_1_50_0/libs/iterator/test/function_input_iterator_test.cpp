// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/iterator/function_input_iterator.hpp>
#include <vector>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <iostream>

struct ones {
    typedef int result_type;
    result_type operator() () {
        return 1;
    }
};

int ones_function () {
    return 1;
}

using namespace std;

int main(int argc, char * argv[])
{
    // test the iterator with function objects
    ones ones_generator;
    vector<int> values(10);
    generate(values.begin(), values.end(), ones());
    
    vector<int> generated;
    copy(
        boost::make_function_input_iterator(ones_generator, 0),
        boost::make_function_input_iterator(ones_generator, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with function objects successful." << endl;

    // test the iterator with normal functions
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(&ones_function, 0),
        boost::make_function_input_iterator(&ones_function, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with pointer to function successful." << endl;

    // test the iterator with a reference to a function
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(ones_function, 0),
        boost::make_function_input_iterator(ones_function, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with reference to function successful." << endl;

    return 0;
}

