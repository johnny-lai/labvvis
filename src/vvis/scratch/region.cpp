//#define VVM_USER_SCALAR_COUNT 16

#include <iostream>
#include <vvis/types.h>
#include <vvis/image.h>
#include <vvis/region.h>

template<typename T> class A {
public:
	template<int channel> T test() {
		return channel;
	}
};

using namespace std;
using namespace vvis;

int main(int argc, char* argv[]) {
	rgb_image<uint8> ri1(16, 16);
	for(int y = 0, i = 0; y < ri1.height(); ++y) {
		for(int x = 0; x < ri1.width(); ++x, ++i) {
			ri1.red(x, y) = i;
			ri1.green(x, y) = i+1;
			ri1.blue(x, y) = i+2;
		}
	}

	typedef ct::cons_tl<rgb_image<uint8> >::type rgb1;
	rect_region<rgb1>::type r = make_rect_region(ri1);

	for(;!r.at_end(); ++r) {
		cerr << r.get<0, 0>() << "," << r.get<0, 1>() << "," << r.get<0, 2>() << endl;
//		cerr << (int)r.get<0, 0>() << "," << (int)r.get<0, 1>() << "," << (int)r.get<0, 2>() << endl;
	}
	cout << "complete" << endl;
	
	return 0;
}