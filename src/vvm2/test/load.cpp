//#define VVM_USER_SCALAR_COUNT 16

#include <iostream>
#include <common/type_name.h>
#include <vvm/vvm.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	typedef vector<typename scalar_traits<scalarT>::bool_type> vector_bool_type;
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	const int step = vvm::vector_traits<vvm::vector<scalarT> >::scalar_count + 5;
	scalarT* pscalar = new scalarT[step];
	for(int i = 0; i < step; ++i) {
		pscalar[i] = i * 10 + 10;
	}
	/*
	BOOST_CHECK(load(pscalar) == 10);
	BOOST_CHECK(load(pscalar, 1) == 10);
	BOOST_CHECK(load(pscalar, 2) == 10);
	BOOST_CHECK(load(pscalar, 3) == 10);
	BOOST_CHECK(load(pscalar, 4) == 10);
	*/
	cout << "0: " << load(pscalar) << endl;
	cout << "1: " << load(pscalar, 1) << endl;
	cout << "2: " << load(pscalar, 2) << endl;
	cout << "3: " << load(pscalar, 3) << endl;
	cout << "4: " << load(pscalar, 4) << endl;
	delete[] pscalar;
}

int main() {
	test<char>();
	test<signed char>();
	test<unsigned char>();
	test<short int>();
	test<unsigned short int>();
	test<int>();
	test<unsigned int>();
	test<long int>();
	test<unsigned long int>();
	test<float>();
	test<double>();
	test<long double>();
	return 0;
}