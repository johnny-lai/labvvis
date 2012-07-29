#include <iostream>
#include <ct/tuple.h>
#include <common/timing.h>

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

int main(int argc, char* argv[]) {
	const int count = 320*240*30;
	
	int *a, *b, *c;
	a = new int[count];
	b = new int[count];
	c = new int[count];
	TIME("scalar_loop", scalar_loop(a, b, c, count));

	ct::tuple<ct::cons_tl_repeat<const int*, 3>::type> t;
	t.get<0>() = a;
	t.get<1>() = b;
	t.get<2>() = c;
	TIME("tuple_loop", tuple_loop(t, count));
	TIME("tuple_loop2", tuple_loop2(t, count));
	TIME("tuple_loop3", tuple_loop3(a, b, c, count));
		
	boost::tuple<int*, int*, int*> bt;
	bt.get<0>() = a;
	bt.get<1>() = b;
	bt.get<2>() = c;
	TIME("boost_tuple_loop", boost_tuple_loop(bt, count));
	
	ct::tuple<ct::cons_tl_repeat<int, 3>::type> t3;
	t3.get<0>() = 1;
	t3.get<1>() = 2;
	t3.get<2>() = 3;
	ct::tuple<ct::cons_tl_repeat<int*, 3>::type> t2;
	t2.get<0>() = a;
	t2.get<1>() = b;
	t2.get<2>() = c;
	cout << a[0] << ", " << b[0] << ", " << c[0] << endl;
	++(*t2);
	//t3 = t3;
	*t2 = t3;
	cout << a[0] << ", " << b[0] << ", " << c[0] << endl;
	
	-t3;
	
	delete[] a;
	delete[] b;
	delete[] c;

	return 0;
}