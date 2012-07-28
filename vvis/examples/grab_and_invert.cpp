#include <vvis/vvis.h>

int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << argv[0] << " <output>" << std::endl;
		return 1;
	}
	
	// Prepare sequence grabber to grab 640x480
	vvis::sgimporter<vvis::sync_capture> sgi(vvis::sgsettings(640, 480));
	if(!sgi.good()) {
		std::cerr << "Unable to connect to sequence grabber" << std::endl;
		return 1;
	}
	
	// Get an image
	typedef vvis::image<vvis::uint8> image_t;
	image_t img;
	sgi >> img;
		
	// Do invert
	vvis::transform(img, vvis::pixel_accessor<image_t>(),
		img, vvis::pixel_accessor<image_t>(),
		vvis::bind1st(vvis::minus<image_t::pixel_type>(),
			std::numeric_limits<vvis::uint8>::max()));
			
	// Write output
	vvis::fexporter out(argv[1]);
	if(!out.good()) {
		std::cerr << "Unable to open " << argv[1] << std::endl;
		return 1;
	}
	out << img;
	
	return 0;
}