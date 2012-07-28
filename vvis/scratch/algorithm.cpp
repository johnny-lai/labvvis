//#define VVM_USER_SCALAR_COUNT 16

#include <iostream>
#include <functional>
#include <vvm/vvm.h>
#include <vvm/functional.h>
#include <vvis/image.h>
#include <vvis/impex.h>
#include <vvis/region.h>
#include <vvis/algorithm.h>
#include <vvis/accessor.h>
#include <vvis/functor.h>

using namespace vvis;
using namespace vvm;
using namespace std;

void output(vvm::vector<uint8> v) {
	cout << v << " ";
}

vvm::vector<uint8> double_f(vvm::vector<uint8> v) {
	return --v;
}

struct func {
	template<typename T>
	typename ct::remove_reference<T>::type operator()(const T& v) {
		return v + v;
	}
};

template<typename typelistT>
struct func43 {
	template<
		typename typelistU
	> ct::tuple<typelistT> operator()(const ct::tuple<typelistU>& v) {
		ct::tuple<typelistT> ret;
		ret.template get<0>() = v.template get<2>();
		ret.template get<1>() = v.template get<1>();
		ret.template get<2>() = v.template get<0>();
		ret.template get<3>() = 255;
		return ret;
	}
};

template<typename scalarT>
struct threshold {
	threshold(const scalarT t)
	: _sthreshold(t), _vthreshold(t) {
	}
	scalarT operator()(const scalarT& v) {
		return v > _sthreshold ? 0 : std::numeric_limits<scalarT>::max();
	}
	vvm::vector<typename vvm::scalar_traits<scalarT>::bool_type> operator()(const vvm::vector<scalarT>& v) {
		return v > _vthreshold;
	}
private:
	const scalarT _sthreshold;
	const vvm::vector<scalarT> _vthreshold;
};

struct print_out {
	template<typename T>
	void operator()(const T& v) {
		cout << v << " ";
	}
};

int main(int argc, char* argv[]) {
	/*if(argc != 2) {
		cerr << "Expected: <image-file>" << endl;
		return 1;
	}
	fimporter fi(argv[1]);
	*/
	/*fimporter fi("/Users/bclai/thesis/pictures/5x5.bmp");
	cout << "     Codec: " << fi.codec() << endl;
	cout << "     Width: " << fi.width() << endl;
	cout << "    Height: " << fi.height() << endl;
	cout << "FrameCount: " << fi.frame_count() << endl;
	cout << "     Depth: " << fi.depth() << endl;
	image<unsigned char> img(fi.width(), fi.height());
	while(fi.good()) {
		fi >> img;
		if(fi.eof()) {
			break;
		}
		//for_each(make_rect_region(img), output);
		//rect_region<rgb_image<uint8> > r(img);
		//for_each(r, pixel_accessor(), output);
		//r.release();
		cout << "Before:" << endl << img << endl;
		vvis::transform(img, img, vector_accessor(), double_f);
		cout << "After:" << endl << img << endl;
		vvis::for_each(img, vector_accessor(), output);
		vvis::transform(img, img, img, vector_accessor(), std::minus<vector_uint8>());
		cout << "After2:" << endl << img << endl;
	}
	*/
	image<uint8, illife_chunky_storage> img(20, 5);
	for(int y = 0; y < img.height(); ++y) {
		for(int x = 0; x < img.width(); ++x) {
			img.pixel(x, y) = x;
		}
	}
	image<uint16, illife_chunky_storage> fimg(20, 5);
	cout << "Before:" << endl << img << endl;
	vvis::transform(img, img, vector_accessor(), func());
	cout << "After:" << endl << img << endl;
	
	rgb_image<short, illife_planar_storage> img2(5, 5);
	for(int y = 0; y < img2.height(); ++y) {
		for(int x = 0; x < img2.width(); ++x) {
			img2.pixel(x, y).get<0>() = x;
			img2.pixel(x, y).get<1>() = x+1;
			img2.pixel(x, y).get<2>() = x+2;
		}
	}
	cout << endl << "img2: " << img2 << endl;
	vvis::for_each(img2, vector_accessor(), print_out());
	vvis::transform(img2, img2, vector_accessor(), func());
	cout << img2 << endl;

	cmyk_image<short, illife_planar_storage> img3(5, 5);
	for(int y = 0; y < img3.height(); ++y) {
		for(int x = 0; x < img3.width(); ++x) {
			img3.pixel(x, y).get<0>() = x;
			img3.pixel(x, y).get<1>() = x+1;
			img3.pixel(x, y).get<2>() = x+2;
			img3.pixel(x, y).get<3>() = x+3;
		}
	}
	cout << "img3: " << img3 << endl;
	vvis::transform(img2, img3, vector_accessor(), func43<
		ct::cons_tl_repeat<vvm::vector<short>, 4>::type>());
	cout << "img3: " << img3 << endl;

	short mask[] = 
		{ 1, -2,  1,
		 -2,  5, -2,
		  1,  5,  1};

	//vvis::convolute(img2, img2, vector_accessor(), mask);
	
	cout << "img: " << img << endl;
	//vvis::transform(img, img, vector_accessor(), threshold<unsigned char>(10));
	
	vvis::transform(img, fimg, vector_accessor(),
		vvis::make_explicit_unary_functor(
			bind2nd(std::less<uint8>(), 20),
			bind2nd(vvm::functional::less<uint8>(), vvm::vector<uint8>(20))
		));
	cout << "img: " << img << endl;
	cout << "fimg: " << fimg << endl;
		
	vvis::transform(img, img, fimg, vector_accessor(), vvis::equal_to());
	cout << "img: " << img << endl;
	cout << "fimg: " << fimg << endl;
	
	return 0;
}
