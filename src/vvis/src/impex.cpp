/*
 *  impex.cpp
 *  vvis
 *
 *  Created by Bing-Chang Lai on Tue June 17 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#include <algorithm>
#include <string>
#include <vvis/macros.h>
#include <vvis/impex.h>

namespace vvis {
	//- atstart/atexit ---------------------------------------------------------
	namespace priv {
		atstart_impex _dummy;
		atstart_impex::atstart_impex() {
#ifdef _WIN32
			InitializeQTML(0 );
#endif
			EnterMovies();
		}
		void atexit_impex() {
			ExitMovies();
#ifdef _WIN32
			TerminateQTML();
#endif
		}
	} // End of priv namespace

	//- impex_base -------------------------------------------------------------
	impex_base::impex_base() {
	}
	impex_base::~impex_base() {
		for(std::list<call_pair>::iterator i = _call_pairs.begin(); i != _call_pairs.end(); ++i) {
			i->first(erase_event, *this, i->second);
		}
	}
	void impex_base::register_callback(event_callback fn, int index) {
		_call_pairs.push_front(call_pair(fn, index));
	}
	
	//- base_impex -------------------------------------------------------------
	base_impex::base_impex() : _ready(false), _rdstate(badbit), _exceptions(goodbit) {
	}
	base_impex::~base_impex() {
	}
	base_impex::operator void*() const {
		return fail() ? 0 : reinterpret_cast<void*>(1);
	}
	bool base_impex::operator!() const {
		return fail();
	}
	impex_base::iostate base_impex::rdstate() const {
		return _rdstate;
	}
	void base_impex::clear(const impex_base::iostate state) {
		if(rdstate() & exceptions()) {
			throw failure("clear failed");
		}
		_rdstate = ready() ? state : state | badbit;
	}
	void base_impex::setstate(const impex_base::iostate state) {
		clear(rdstate() | state);
	}
	bool base_impex::good() const {
		return rdstate() == 0;
	}
	bool base_impex::bad() const {
		return rdstate() & eofbit;
	}
	bool base_impex::fail() const {
		return (rdstate() & failbit) || (rdstate() & badbit);
	}
	bool base_impex::eof() const {
		return rdstate() & eofbit;
	}
	impex_base::iostate base_impex::exceptions() const {
		return _exceptions;
	}
	void base_impex::exceptions(iostate except) {
		_exceptions = except;
	}
	bool base_impex::ready() const {
		return _ready;
	}
	void base_impex::ready(bool is_ready) {
		_ready = is_ready;
	}
	
	//- fimporter --------------------------------------------------------------
	fimporter::fimporter() : base_impex(), _gi(0), _desc(0) {
	}
	fimporter::fimporter(const char filename[], const openmode mode) : base_impex(), _gi(0), _desc(0) {
		open(filename, mode);
	}
	fimporter::~fimporter() {
		close();
	}
	bool fimporter::is_open() const {
		return _is_open;
	}
	void fimporter::open(const char filename[], const openmode mode) {
		FSSpec fsspec;
		_VVIS_CHECK_OSERR(make_fsspec(fsspec, filename),
			setstate(badbit); return);
		_VVIS_CHECK_OSERR(GetGraphicsImporterForFile(&fsspec, &_gi),
			setstate(badbit); return);
		_VVIS_CHECK_OSERR(GraphicsImportGetImageCount(_gi, &_frame_count),
			setstate(badbit); return);
		_current_index = 1;
		_VVIS_CHECK_OSERR(GraphicsImportSetImageIndex(_gi, _current_index),
			setstate(badbit); return);
		_VVIS_CHECK_OSERR(GraphicsImportGetImageDescription(_gi, &_desc),
			setstate(badbit); return);
		// Use default matrix, clip, source rect etc
		RgnHandle clip_region = NewRgn();
		if(GraphicsImportGetDefaultClip(_gi, &clip_region) == noErr) {
			// Clipping region found
			_VVIS_WARN("Setting clip region");
			GraphicsImportSetClip(_gi, clip_region);
		}
		MatrixRecord matrix;
		if(GraphicsImportGetDefaultMatrix(_gi, &matrix) == noErr) {
			_VVIS_WARN("Setting matrix");
			GraphicsImportSetMatrix(_gi, &matrix);
		}
		long graphics_code;
		RGBColor op_color;
		if(GraphicsImportGetDefaultGraphicsMode(_gi, &graphics_code, &op_color) == noErr) {
			_VVIS_WARN("Setting graphics mode");
			GraphicsImportSetGraphicsMode(_gi, graphics_code, &op_color);
		}
		Rect source_rect;
		if(GraphicsImportGetDefaultSourceRect(_gi, &source_rect) == noErr) {
			_VVIS_WARN("Setting source rect");
			GraphicsImportSetSourceRect(_gi, &source_rect);
		}
		_is_open = true;
		ready(true);
		clear();
	}
	void fimporter::close() {
		_is_open = false;
		DisposeHandle((Handle)_desc);
		CloseComponent(_gi);
		_gi = 0;
		_desc = 0;
		_current_index = 0;
		ready(false);
		clear();
	}
	const int fimporter::width() const {
		return (*_desc)->width;
	}
	const int fimporter::height() const {
		return (*_desc)->height;
	}
	const unsigned long fimporter::frame_count() const {
		return _frame_count;
	}
	const short fimporter::depth() const {
		return (*_desc)->depth;
	}
	impex_base::codec_type fimporter::codec() const {
		return static_cast<impex_base::codec_type>((*_desc)->cType);
	}	
	
	//- fexporter --------------------------------------------------------------
	fexporter::fexporter() : base_impex(), _ge(0) {
	}
	fexporter::fexporter(const char* filename, openmode mode) : base_impex(), _ge(0) {
		open(filename, mode);
	}
	fexporter::fexporter(const char* filename, const codec_type ft, openmode mode) : base_impex(), _ge(0) {
		open(filename, ft, mode);
	}
	fexporter::~fexporter() {
		close();
	}
	bool fexporter::is_open() const {
		return _is_open;
	}
	void fexporter::open(const char* filename, openmode mode) {
		// Get the extension
		std::string f(filename);
		std::string::size_type dot_pos = f.find_last_of('.');
		if(dot_pos == std::string::npos) {
			// No dot found. So can't guess type
			setstate(badbit);
		} else {
			codec_type ft;
			std::string extension = f.substr(dot_pos + 1);
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
			if(extension == "tif" || extension == "tiff") {
				ft = impex_base::tiff;
			} else if(extension == "png") {
				ft = impex_base::png;
			} else if(extension == "gif") {
				ft = impex_base::gif;
			} else if(extension == "pic" || extension == "pct" || extension == "pict") {
				ft = impex_base::pict;
			} else if(extension == "psd") {
				ft = impex_base::psd;
			} else if(extension == "sgi") {
				ft = impex_base::sgi;
			} else if(extension == "bmp") {
				ft = impex_base::bmp;
			} else if(extension == "jpg" || extension == "jpeg") {
				ft = impex_base::jpeg;
			} else if(extension == "jfif") {
				ft = impex_base::jfif;
			} else if(extension == "mov") {
				ft = impex_base::mov;
			} else if(extension == "avi") {
				ft = impex_base::avi;
			} else if(extension == "dvc") {
				ft = impex_base::dvc;
			} else if(extension == "mp4") {
				ft = impex_base::mp4;
			} else {
				std::clog << "vvis: Unknown extension " << extension << std::endl;
				// Unknown type
				setstate(badbit);
				return;
			}
			open(filename, ft, mode);
		}
	}
	void fexporter::open(const char* filename, const codec_type ft, openmode mode) {
		// Use ofstream to open the file so that it exists
		std::ofstream ofs(filename, mode);
		ofs.close();
		// Get a FSSpec to the file
		_VVIS_CHECK_OSERR(make_fsspec(_fss, filename),
			setstate(badbit); return);
		// Open a Graphics Exporter Component
		_VVIS_CHECK_OSERR(OpenADefaultComponent(GraphicsExporterComponentType, ft, &_ge),
			setstate(badbit); return);
		_is_open = true;
		ready(true);
		clear();
	}
	void fexporter::close() {
		_is_open = false;
		CloseComponent(_ge);
		_ge = 0;
	}
#ifndef VVIS_USER_DISABLE_SEQUENCE_GRABBER
	//- sgsettings -------------------------------------------------------------
	sgsettings::sgsettings() : width(0), height(0), pixel_format(0) {
	}
	sgsettings::sgsettings(const int w, const int h, const OSType pf) 
	: width(w), height(h), pixel_format(pf) {
	}
#endif
} // End of vvis namespace