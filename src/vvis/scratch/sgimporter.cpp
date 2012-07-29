#include <iostream>
#include <vvis/vvis.h>
#include <vvis/impex.h>
#include <vvis/conversion.h>

using namespace std;
using namespace vvm;
using namespace vvis;

int counter = 0;
rgb_image<uint8> gImage(320, 240);
float average_fps;
long frame_count = 0;
/*
OSErr callback(sgimporter<async_capture>& sgi, TimeValue time, void*) {
	sgi >> gImage;
	//sgi.stop();
	//cout << image.red(0, 0) << ", " << image.green(0, 0) << ", " << image.blue(0, 0) << endl;
	++frame_count;
	//average_fps = ((float)frame_count * (float)sgi.time_scale()) / (float)time;
	return noErr;
}
*/
void callback2(sgimporter<async_capture>& sgi, const TimeValue time, void*) {
	sgi >> gImage;
	//sgi.stop();
	//cout << image.red(0, 0) << ", " << image.green(0, 0) << ", " << image.blue(0, 0) << endl;
	++frame_count;
	average_fps = ((float)frame_count * (float)sgi.time_scale()) / (float)time;
	//return noErr;
}

int main(int argc, char* argv[]) {
	cout << is_image<rgb_image<uint8> >::value << endl;
	for(int y = 0; y < gImage.height(); ++y) {
		for(int x = 0; x < gImage.width(); ++x) {
			gImage.red(x, y) = 0;
			gImage.green(x, y) = 0;
			gImage.blue(x, y) = 0;
		}
	}

	sgimporter<async_capture> sgi;
	sgi.open(sgsettings(320, 240));
	if(sgi.good()) {
		sgi.start(10 * 60, callback2, 0);
		cout << "async time_scale " << sgi.time_scale() << endl;
		sgi.wait_for();
		if(!sgi.eof()) {
			cerr << "An error occured\n";
		} else {
			// Write last image to disk
			fexporter fe("out.tiff", impex_base::tiff);
			fe << gImage;
			fe.close();
		}
		
		// sgi.good, sgi.bad, egi.fail, sgi.is_open, sgi.is_ready
		// eof triggered when
		// 1. time has expired
		// 2. record stopped for some reason.
		// props has size, asyncrhonous etc
		// are default settings dependant on the component?
		// width,height dependanton component?
		// sgsettings is a union?
		// policies:
		// 1. capture policy
		// default is synchronous

		//sgsettings s = sgi.default_settings();
		//s.width = 640;
		//s.height = 480;
		//sgi.open(component, s);
		//for(int i = 0; i < 10; ++i) {
		//	sgi >> f;
		//}
		//sgi.close();
		
		//sgimporter<synchronous>
		//sgimporter<asynchronous>
		cout << "Average FPS: " << frame_count << ", " << average_fps << endl;
	} else {
		cerr << "Unable to open sequence grabber\n";
	}
	sgi.close();
	
	sgsettings s;
	sgimporter<sync_capture> sync_sgi(s);//(sgsettings());
	//sync_sgi.open(sgsettings());
	if(!sync_sgi.good()) {
		cerr << "Failed to open" << endl;
	} else {
		for(int i = 0; i < 3; ++i) {
			cout << i << endl;
			sync_sgi >> gImage;
			// Write last image to disk
			char filename[255];
			sprintf(filename, "sync_%d.tiff", i);
			fexporter fe(filename, impex_base::tiff);
			fe << gImage;
			fe.close();
		}
	}
	sync_sgi.close();
	
	return 0;
}