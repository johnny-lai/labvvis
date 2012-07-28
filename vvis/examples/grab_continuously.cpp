#include <iostream>
#include <common/timing.h>
#include <vvis/impex.h>

using namespace std;
using namespace vvm;
using namespace vvis;

int main(int argc, char* argv[]) {
	if(argc != 5) {
		cerr << "Expected: <width> <height> <max_frames> <output-file.tiff>" << endl;
		cerr << "Set <max_frames> to zero to capture forever" << endl;
		return 1;
	}
	const int width = atoi(argv[1]);
	const int height = atoi(argv[2]);
	const int max_frames = atoi(argv[3]);
	const char* file = argv[4];
	rgb_image<uint8, illife_planar_storage> image(width, height);
	
	sgimporter<sync_capture> sync_sgi(sgsettings(width, height));
	if(sync_sgi.good()) {
		timing_t start, end;
		start = timenow();
		for(int i = 0; max_frames == 0 || i < max_frames; ++i) {
			sync_sgi >> image;
			fexporter fe(file);
			if(fe.good()) {
				fe << image;
				fe.close();
			} else {
				cerr << "Failed to write to " << file << endl;
				return 1;
			}
		}
		end = timenow();
		double time_elapsed = timediff(end, start) / 1000000.0;
		cerr << "Capture Rate was " << (max_frames / time_elapsed) << endl;
	} else {
		cerr << "Unable to open sequence grabber";
	}
	sync_sgi.close();
	return 0;
}