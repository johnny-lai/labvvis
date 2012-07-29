#include <iostream>
#include <vvis/image.h>
#include <vvis/region.h>
#include <vvis/algorithm.h>
#include <vvis/accessor.h>
#include <vvis/functor.h>
#include <common/type_name.h>

using namespace std;
using namespace vvis;

struct f {
	template<typename T> T operator()(const T t) {
		return t + T(1);
	}
};

struct output {
	template<typename T> void operator()(const T t) {
		cout <<  t << " ";
	}
};

#define STORAGE contiguous_chunky_storage

int main(int argc, char* argv[]) {
	const int i = 5;
	int& ir(i);
	base_image<ct::cons_tl<int, int, int>::type> b(10, 10);
	b.component<1>(1, 1) = 25;
	cout << b.component<1>(1, 1) << endl;
	
	base_image<ct::cons_tl<int, int, int>::type> c(b);
	cout << c.component<1>(1, 1) << endl;

	rgb_image<int, STORAGE> rgb1(10, 10);
	rgb1.red(1, 1) = 15;
	cout << rgb1.red(1, 1) << endl;
	
	rgb_image<int, STORAGE> rgb2(rgb1);
	cout << rgb2.red(1, 1) << endl;
	
	cout << rgb2.pixel(1, 1).get<0>() << endl;
	
	rgb2.pixel(1, 1).get<0>() = 12;
	cout << "in: " << rgb2.pixel(1, 1).get<0>() << endl;

	rgb2.pixel(2, 2) = rgb1.pixel(1, 1);
	//base_image<ct::cons_tl<int, int, int>::type, illife_storage> bc1(10, 10);
	//base_image<ct::cons_tl<int, int, int>::type> bc2(bc1);
	//cout << rgb3.red(1, 1) << endl;
	//cout << vvis::uses_contiguous_storage<rect_region<rgb_image<int, illife_storage>, contiguous_storage> >::value << endl;
	//cout << uses_contiguous_storage<rect_region<rgb_image<int, illife_storage>, contiguous_storage> >::value << endl;
	
	//cout << uses_illife_storage<rect_region<rgb_image<int, illife_storage>, contiguous_storage> >::value << endl;
	cout << "IN:\n" << rgb2 << endl;

	rect_region<rgb_image<int, STORAGE>, illife_storage> r(rgb2);
	vvis::transform(r, r, vector_accessor(), f());
	vvis::transform(r, r, r, vector_accessor(), binary_functor<std::plus>());
	vvis::transform(r, rgb1, r, vector_accessor(),
		make_explicit_functor(std::plus<int>(), std::plus<vvm::vector<int> >()));
	r.release();
	
	vvis::for_each(rgb2, vector_accessor(), output());
	cout << "OUT2:\n" << rgb2 << endl;
	
	return 0;
}