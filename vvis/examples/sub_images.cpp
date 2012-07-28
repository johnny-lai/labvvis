#include <vvis/vvis.h>

int main(int argc, char *argv[]) {
	if(argc != 4) {
		std::cerr << argv[0] << " <input1> <input2> <output>" << std::endl;
		return 1;
	}
	// Open files
	vvis::fimporter in1(argv[1]);
	vvis::fimporter in2(argv[2]);
	if(!in1.good() || !in2.good()) {
		std::cerr << "Unable to open " << argv[1] <<
			" or " << argv[2] << std::endl;
		return 1;
	}
	// Check if image sizes are the same
	if(in1.width() != in2.width() || in1.height() != in2.height()) {
		std::cerr << "Unable to process because image sizes are not the same" << std::endl;
		return 1;
	}
	
	// Read in as gray scale images
	typedef vvis::image<vvis::uint8> image_t;
	image_t img1, img2, dest(in1.width(), in2.height());
	in1 >> img1;
	in2 >> img2;
	
	// Do saturated subtraction
	vvis::transform(img1, vvis::pixel_accessor<image_t>(),
		img2, vvis::pixel_accessor<image_t>(),
		dest, vvis::pixel_accessor<image_t>(),
		vvis::minus_saturated<>());
		
	// Write output
	vvis::fexporter out(argv[3]);
	if(!out.good()) {
		std::cerr << "Unable to open " << argv[3] << std::endl;
		return 1;
	}
	out << dest;
	
	return 0;
}