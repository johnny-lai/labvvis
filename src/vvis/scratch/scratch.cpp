#include <vvis/vvis.h>

#define VERSION 3

int main(int argc, char *argv[]) {
	if(argc != 3) {
		std::cerr << argv[0] << " <input> <output>" << std::endl;
		return 1;
	}
	
	// Prepare sequence grabber to grab 640x480
	//#define USE_SEQUENCE_GRABBER
#ifdef USE_SEQUENCE_GRABBER
	vvis::sgimporter<vvis::sync_capture> sgi(vvis::sgsettings(640, 480));
	if(!sgi.good()) {
		std::cerr << "Unable to connect to sequence grabber" << std::endl;
		return 1;
	}
#else	
	vvis::fimporter sgi(argv[1]);
	if(!sgi.is_open()) {
		std::cerr << "Unable to open " << argv[1] << std::endl;
		return 1;
	}	
#endif

	// Get an image
	typedef vvis::rgb_image<vvis::uint8> image_t;
	//typedef vvis::image<vvis::uint8, vvis::contiguous_planar_storage> image_t;
	//typedef vvis::image<vvis::sint16, vvis::illife_planar_storage> image_t;
	image_t img;
	sgi >> img;
	std::cout << "Image Size: " << img.width() << " x " << img.height() << std::endl;
	/*
	image_t::pixel_type* h = &img.pixel(0, 0);
	
	const image_t &cimg = img;
	const image_t::pixel_type* i = &cimg.pixel(0, 0);
	*/
	// Version
	//#define VERSION 4
	
	// Do invert
#if VERSION==1
	// Do Region
	std::cout << "Region inversion" << std::endl;
	typedef vvis::rect_region<image_t> region_t;
	region_t r(img, 50, 50, img.width()-100, img.height()-100);

	vvis::transform(r, vvis::pixel_accessor<region_t>(),
		r, vvis::pixel_accessor<region_t>(),
		vvis::bind1st(vvis::minus<image_t::pixel_type>(),
			std::numeric_limits<image_t::pixel_type>::max()));
#elif VERSION==2
	// Do Component Accessor
	// Requires RGB image
	std::cout << "Component accessor inversion" << std::endl;
	vvis::transform(img, vvis::component_accessor<image_t, 0>(),
		img, vvis::component_accessor<image_t, 0>(),
	//	vvis::bind1st(vvis::minus<vvis::uint8>(),
	//		std::numeric_limits<vvis::uint8>::max()));
	//vvis::transform(img, vvis::component_accessor<image_t, 0>(),
	//				img, vvis::component_accessor<image_t, 0>(),
					vvis::bind2nd(vvis::greater<vvis::uint8>(), 128));
#elif VERSION==3
	// Do Pixel Accessor
	std::cout << "Pixel accessor inversion" << std::endl;
	vvis::transform(img, vvis::pixel_accessor<image_t>(),
		img, vvis::pixel_accessor<image_t>(),
		vvis::bind1st(vvis::minus<image_t::pixel_type>(),
			std::numeric_limits<image_t::pixel_type>::max()));
#elif VERSION==4
	// Test histogram
	std::cout << "Histogram" << std::endl;
	typedef vvis::histogram<image_t::pixel_type> histogram_t;
	histogram_t h;
	vvis::for_each(img, vvis::pixel_accessor<image_t>(), h);
	histogram_t::result_type &r = h.result();
	std::cout << "Result: " << r.size() << std::endl;
	for(histogram_t::result_type::const_iterator p = r.begin(); p != r.end(); ++p) {
		std::cout << p->first << " : " << p->second << std::endl;
	}
#else
	// No inversion
	std::cout << "No inversion done" << std::endl;
#endif

	// Write output
	vvis::fexporter out(argv[2]);
	if(!out.good()) {
		std::cerr << "Unable to write " << argv[2] << std::endl;
		return 1;
	}
	out << img;
	
	return 0;
}