#include <iostream>
#include <common/type_name.h>
#include <ct/metafunction.h>

using namespace std;

template<typename T, template<typename> class U> void test() {
	cout << "  " << type_name(typeid(T)) << "  =>  "
		<< type_name(typeid(typename U<T>::type)) << endl;
}

int main(int argc, char* argv[]) {
	cout << "is char signed? " << std::numeric_limits<char>::is_signed << endl;
	cout << "sizeof(char)\t" << sizeof(char) << endl;
	cout << "sizeof(short)\t" << sizeof(short) << endl;
	cout << "sizeof(int)\t" << sizeof(int) << endl;
	cout << "sizeof(long)\t" << sizeof(long) << endl;
	cout << "sizeof(float)\t" << sizeof(float) << endl;
	cout << "sizeof(double)\t" << sizeof(double) << endl;
	cout << "sizeof(long double)\t" << sizeof(long double) << endl;
	cout << "sizeof(ct::null_type)\t" << sizeof(ct::null_type) << endl;
	cout << endl;
	
	cout << "promote" << endl;
	test<char, ct::promote>();
	test<signed char, ct::promote>();
	test<unsigned char, ct::promote>();
	test<short, ct::promote>();
	test<unsigned short, ct::promote>();
	test<int, ct::promote>();
	test<unsigned int, ct::promote>();
	test<long, ct::promote>();
	test<unsigned long, ct::promote>();
	test<float, ct::promote>();
	test<double, ct::promote>();
	test<long double, ct::promote>();
	test<ct::null_type, ct::promote>();
	test<CT_TYPELIST2(unsigned char, unsigned char), ct::promote>();
	test<CT_TYPELIST2(unsigned char, unsigned short), ct::promote>();
	test<CT_TYPELIST2(signed char, unsigned char), ct::promote>();
	test<CT_TYPELIST2(signed char, unsigned short), ct::promote>();
	cout << endl;
	
	cout << "promote_signed" << endl;
	test<char, ct::promote_signed>();
	test<signed char, ct::promote_signed>();
	test<unsigned char, ct::promote_signed>();
	test<short, ct::promote_signed>();
	test<unsigned short, ct::promote_signed>();
	test<int, ct::promote_signed>();
	test<unsigned int, ct::promote_signed>();
	test<long, ct::promote_signed>();
	test<unsigned long, ct::promote_signed>();
	test<float, ct::promote_signed>();
	test<double, ct::promote_signed>();
	test<long double, ct::promote_signed>();
	test<ct::null_type, ct::promote_signed>();
	test<CT_TYPELIST2(unsigned char, unsigned char), ct::promote_signed>();
	test<CT_TYPELIST2(unsigned char, unsigned short), ct::promote_signed>();
	test<CT_TYPELIST2(signed char, unsigned char), ct::promote_signed>();
	test<CT_TYPELIST2(signed char, unsigned short), ct::promote_signed>();
	cout << endl;
	
	cout << "largest" << endl;
	cout << "  ct::priv::unsigned_integer_tl\t"
		<< type_name(typeid(ct::largest<ct::priv::unsigned_integer_tl>::type))
		<< endl;
	cout << "  ct::priv::signed_integer_tl\t"
		<< type_name(typeid(ct::largest<ct::priv::signed_integer_tl>::type))
		<< endl;
	cout << "  ct::priv::real_tl\t"
		<< type_name(typeid(ct::largest<ct::priv::real_tl>::type))
		<< endl;
		
	return 0;
}