
#include <iostream>
#include <ct/tuple.h>
#include "common/timing.h"

using namespace std;

void scalar_loop(const int* a, const int* b, const int* c, const int count) {
	for(int i = 0; i < count; ++i) {
		a = 0;
		b = 0;
		c = 0;
		++a;
		++b;
		++c;
	}
}

void tuple_loop(ct::tuple<ct::cons_tl_repeat<const int*, 3>::type> t, const int count) {
	for(int i = 0; i < count; ++i) {
		t.get<0>() = 0;
		t.get<1>() = 0;
		t.get<2>() = 0;
		++t.get<0>();
		++t.get<1>();
		++t.get<2>();
	}
}

void tuple_loop2(ct::tuple<ct::cons_tl_repeat<const int*, 3>::type> t, const int count) {
	for(int i = 0; i < count; ++i) {
		t.get<0>() = 0;
		t.get<1>() = 0;
		t.get<2>() = 0;
		++t;
	}
}

void tuple_loop3(const int* a, const int* b, const int* c, const int count) {
	ct::tuple<ct::cons_tl_repeat<const int*, 3>::type> t(a, b, c);
	for(int i = 0; i < count; ++i) {
		t.get<0>() = 0;
		t.get<1>() = 0;
		t.get<2>() = 0;
		++t;
	}
}

void boost_tuple_loop(boost::tuple<int*, int*,int*> t, const int count) {
	for(int i = 0; i < count; ++i) {
		t.get<0>() = 0;
		t.get<1>() = 0;
		t.get<2>() = 0;
		++t.get<0>();
		++t.get<1>();
		++t.get<2>();
	}
}


struct rgb_op {
	template<typename T> inline typename ct::remove_reference<T>::type operator()(const T& in) {
		//return typename ct::remove_reference<T>::type(in);
		return in;
	}
};

struct A {};

int main(int argc, char* argv[]) {
	const int count = 320*240*30;
	
	int *a, *b, *c;
	a = new int[count];
	b = new int[count];
	c = new int[count];

	ct::tuple<CT_TYPELIST3(const int, const int, const int)> c1(1, 2, 3);
	ct::tuple<CT_TYPELIST3(const int, const int, const int)> c2(10, 20, 30);
	
	cout << "Testing operators ..." << endl;
	ct::tuple<CT_TYPELIST3(int, int, int)> to1(c1), to2(c2);
	cout << "to1, to2 = " << to1 << ", " << to2 << endl;
	cout << "to1 + to2 = " << to1 + to2 << endl;
	cout << "to1 - to2 = " << to1 - to2 << endl;
	cout << "to1 * to2 = " << to1 * to2 << endl;
	cout << "to1 / to2 = " << to1 / to2 << endl;
	cout << "to1 += to2, to1 = " << (to1 += to2) << ", " << to1 << endl;
	cout << "to1 -= to2, to1 = " << (to1 -= to2) << ", " << to1 << endl;
	cout << "to1 *= to2, to1 = " << (to1 *= to2) << ", " << to1 << endl;
	cout << "to1 /= to2, to1 = " << (to1 /= to2) << ", " << to1 << endl;
	

	cout << "Testing pointers ..." << endl;
	ct::tuple<CT_TYPELIST3(int*, int*, int*)> tp1(a, b, c);
	*tp1 = c1;
	++tp1;  // Should now advance to [1]
	*tp1 = c2;
	cout << "a[0], b[0], c[0] = " << a[0] << ", " << b[0] << ", " << c[0] << endl;
	cout << "a[1], b[1], c[1] = " << a[1] << ", " << b[1] << ", " << c[1] << endl;

	cout << "Testing references ..." << endl;
	int x = 10, y = 20, z = 30;
	ct::tuple<CT_TYPELIST3(int&, int&, int&)> tr1(x, y, z);
	--tr1;
	cout << "x, y, z = " << x << ", " << y << ", " << z << endl;
	++tr1;
	cout << "x, y, z = " << x << ", " << y << ", " << z << endl;
	ct::remove_reference<ct::tuple<CT_TYPELIST3(int&, int&, int&)> >::type tdr1 = tr1 + tr1;
	cout << "tdr1 = " << tdr1 << endl;
	cout << "x, y, z = " << x << ", " << y << ", " << z << endl;
	tr1 = tdr1;
	cout << "x, y, z = " << x << ", " << y << ", " << z << endl;
	*tp1 = tdr1;
	cout << "a[0], b[0], c[0] = " << a[0] << ", " << b[0] << ", " << c[0] << endl;
	cout << "a[1], b[1], c[1] = " << a[1] << ", " << b[1] << ", " << c[1] << endl;
	
	// Free memory
	delete[] a;
	delete[] b;
	delete[] c;
	
	ct::tuple<CT_TYPELIST2(int, A)> xy;
	
	return 0;
}