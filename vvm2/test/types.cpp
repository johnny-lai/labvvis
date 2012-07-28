// Checks that vectors of different basic types can be created
#include <iostream>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <vvm/vvm.h>

using namespace std;

int main(int argc, char *argv[])
{
	#define OP(T, C) \
	{ \
		vvm::Vector<T, C> v(BOOST_PP_ENUM_PARAMS(C, BOOST_PP_EMPTY())); \
		cout << BOOST_PP_STRINGIZE(T) << "(" << vvm::Vector<T>::scalarCount << ", " << vvm::Vector<T>::vectorCount << "): "; \
		cout << v << endl; \
	}
	OP(unsigned char, VVM_SCALAR_COUNT);
	OP(signed char, VVM_SCALAR_COUNT);
	OP(unsigned short, VVM_SCALAR_COUNT);
	OP(signed short, VVM_SCALAR_COUNT);
	OP(unsigned int, VVM_SCALAR_COUNT);
	OP(signed int, VVM_SCALAR_COUNT);
//	OP(unsigned long, VVM_SCALAR_COUNT);
//	OP(signed long, VVM_SCALAR_COUNT);
	OP(float, VVM_SCALAR_COUNT);
//	OP(double, VVM_SCALAR_COUNT);
}
