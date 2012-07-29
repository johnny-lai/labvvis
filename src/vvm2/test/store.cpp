//#define VVM_USER_SCALAR_COUNT 16

#include <iostream>
#include <common/type_name.h>
#include <vvm/vvm.h>

using namespace std;
using namespace vvm;

template<typename scalarT> void test() {
	typedef vector<typename scalar_traits<scalarT>::bool_type> vector_bool_type;
	cout << "Testing " << type_name(typeid(scalarT)) << endl;
	vector<scalarT> v;
	for(int i = 0; i < vvm::vector_traits<vvm::vector<scalarT> >::scalar_count; ++i) {
		v.scalar(i) = i * 10 + 10;
	}
	scalarT* pscalar = new scalarT[vvm::vector_traits<vvm::vector<scalarT> >::scalar_count];
	store(v, pscalar);
	for(int i = 0; i < vvm::vector_traits<vvm::vector<scalarT> >::scalar_count; ++i) {
		cout << (int)pscalar[i] << " ";
	}
	cout << endl;
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