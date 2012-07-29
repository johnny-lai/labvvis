#include <iostream>
#include <vvis/impex.h>
#include <vvis/accessor.h>
#include <vvis/algorithm.h>
#include <vvis/vvis.h>

using namespace std;
using namespace vvm;
using namespace vvis;

struct double_pixel {
	template<typename T>
	typename ct::remove_reference<T>::type operator()(const T& v) {
		return v+v;
	}
};

int main(int argc, char* argv[]) {
	std::set_new_handler(0);	// Please throw bad_alloc on out of memory
	try {
		if(argc != 3) {
			cerr << "Expected: <image-file> <output-file>" << endl;
			return 1;
		}
		fimporter fi(argv[1]);
		if(!fi.good()) {
			cerr << "Unable to open " << argv[1] << endl;
			return 1;
		}
		cout << "     Codec: " << fi.codec() << endl;
		cout << "     Width: " << fi.width() << endl;
		cout << "    Height: " << fi.height() << endl;
		cout << "FrameCount: " << fi.frame_count() << endl;
		cout << "     Depth: " << fi.depth() << endl;
		rgb_image<uint8, contiguous_planar_storage> img;
		while(fi.good()) {
			cout << "Got image" << endl;
			cout << "     Width: " << fi.width() << endl;
			cout << "    Height: " << fi.height() << endl;
			fi >> img;
			if(fi.eof()) {
				break;
			}
		}
		fi.close();
	
		// Perform threshold operation
		vvis::transform(img, img, img, vvis::pixel_accessor(), plus_saturated<>());
	
		fexporter fe(argv[2], impex_base::tiff);
		fe << img;
		fe.close();
	} catch(...) {
		cerr << "Bad Allocation" << endl;
	}
	return 0;
}