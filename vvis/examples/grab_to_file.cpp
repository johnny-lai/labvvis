#include <iostream>
#include <vvis/vvis.h>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cerr << "Expected: <output-file.tiff>" << std::endl;
		return 1;
	}
	const int width = 640;
	const int height = 480;
	const char* file = argv[1];
	
	// Prepare Sequence Grabber
	vvis::sgimporter<vvis::sync_capture> sgi(vvis::sgsettings(width, height));
	if(!sgi.good()) {
		std::cerr << "Unable to open sequence grabber" << std::endl;
		return 1;
	}

	// Get an image
	vvis::image<vvis::uint8> img;
	sgi >> img;

	vvis::fexporter fe(file);
	if(!fe.good()) {
		std::cerr << "Unable to write to " << file << std::endl;
		return 1;
	}
	fe << img;
	
	return 0;
}