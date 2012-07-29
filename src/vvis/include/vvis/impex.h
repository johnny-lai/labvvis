/*
 *  impex.h
 *  vvis
 *
 *  Created by Bing-Chang Lai on Fri Jun 13 2003.
 *  Copyright (c) 2003 University of Wollongong. All rights reserved.
 *
 */

#if !defined _VVIS_IMPEX_H && !defined VVIS_USER_DISABLE_IMPEX
#define _VVIS_IMPEX_H

#include <list>
#include <iostream>
#include <fstream>
#include <boost/config.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <ct/enabler.h>
#include <vvis/macros.h>
#include <vvis/image.h>
#include <vvis/conversion.h>
#include <vvis/metafunction.h>
#include <vvis/quicktime.h>

namespace vvis {
	namespace priv {
		// Used to execute at startup without user intervention
		struct atstart_impex {
			atstart_impex();
		};
		extern atstart_impex _dummy;
		// This function is registered to be called when the program exits
		void atexit_impex();
	} // End of priv namespace

	//- impex_base -----------------------------------------------------------------------	
	/** \class impex_base "impex.h" <vvis/impex.h>
	 * All Import/Export classes inherit from this class.
	 */
	class impex_base {
	public:
		typedef std::ios_base::failure failure;
		
		typedef std::ios_base::iostate iostate;
		static const iostate badbit = std::ios_base::badbit;
		static const iostate eofbit = std::ios_base::eofbit;
		static const iostate failbit = std::ios_base::failbit;
		static const iostate goodbit = std::ios_base::goodbit;
		
		typedef std::ios_base::openmode openmode;
		static const openmode app = std::ios_base::app;
		static const openmode ate = std::ios_base::ate;
		static const openmode in = std::ios_base::in;
		static const openmode out = std::ios_base::out;
		static const openmode trunc = std::ios_base::trunc;
		
		/// Import/Output File Types
		enum codec_type {
			png = kQTFileTypePNG,
			gif = kQTFileTypeGIF,
			tiff = kQTFileTypeTIFF,
			pict = kQTFileTypePicture,
			psd = kQTFileTypePhotoShop,
			sgi = kQTFileTypeSGIImage,
			bmp = kQTFileTypeBMP,
			jpeg = kQTFileTypeJPEG,
			jfif = kQTFileTypeJFIF,
			mov = kQTFileTypeMovie,
			avi = kQTFileTypeAVI,
			dvc = kQTFileTypeDVC,
			mp4 = kQTFileTypeMP4
		};
	
		enum event { erase_event };
		typedef void (*event_callback)(event, impex_base&, int index);
	public:
		/// impex_base destructor
		virtual ~impex_base();
		/// Register a callback function for an event
		void register_callback(event_callback fn, int index);
	protected:
		/// impex_base default constructor
		impex_base();
	private:
		typedef std::pair<event_callback, int> call_pair;
		std::list<call_pair> _call_pairs;
	};
	
	//- base_impex -----------------------------------------------------------------------
	/** \class base_impex "impex.h" <vvis/impex.h>
	 * Base Implementation for Import/Exporters
	 * Corresponds to base_io.
	 */
	class base_impex : public impex_base {
	public:
		base_impex();
		virtual ~base_impex();
	public:
		operator void*() const;
		bool operator!() const;
		iostate rdstate() const;
		void clear(const iostate state = goodbit);
		void setstate(const iostate state);
		bool good() const;
		bool bad() const;
		bool fail() const;
		bool eof() const;
		iostate exceptions() const;
		void exceptions(iostate except);
		bool ready() const;
		void ready(bool is_ready);
	protected:
		bool _ready;
		iostate _rdstate;
		iostate _exceptions;
	};
	
	//- fimporter ------------------------------------------------------------------------
	/** \class fimporter "impex.h" <vvis/impex.h>
	 * File Importer.
	 * Use this class to read image files.
	 */
	class fimporter : public base_impex {
	public:
		fimporter();
		explicit fimporter(const char filename[], const openmode mode = in);
		virtual ~fimporter();
	public:
		bool is_open() const;
		void open(const char filename[], const openmode mode = in);
		void close();
	public:
		const int width() const;
		const int height() const;
		const unsigned long frame_count() const;
		const short depth() const;
		codec_type codec() const;
	public:
		template<typename imageT>
		typename ct::enable_if<is_image<imageT>::value, fimporter&>::type
		operator>>(imageT& image) {
			// Check if EOF
			if(_current_index > frame_count()) {
				setstate(eofbit);
				return *this;
			}
			Rect r;
			GWorldPtr gworld;
			// Import
			const int w = width();
			const int h = height();
			MacSetRect(&r, 0, 0, width(), height());
			_VVIS_CHECK_OSERR(
				QTNewGWorld(&gworld, k32ARGBPixelFormat, &r, 0, NULL, 0),
				setstate(badbit); goto cleanup);
			_VVIS_CHECK_OSERR(
				GraphicsImportSetGWorld(_gi, gworld, 0),
				setstate(badbit); goto cleanup);
			GraphicsImportDraw(_gi);
			// Convert to rgb_image<scalarT>
			// Resize image if required
			image.resize(w, h);
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_image(image, gworld, w, h);
			// Prepare for next frame
			++_current_index;
			_VVIS_CHECK_OSERR(GraphicsImportSetImageIndex(_gi, _current_index), setstate(badbit); goto cleanup);
			DisposeHandle((Handle)_desc);
			_VVIS_CHECK_OSERR(GraphicsImportGetImageDescription(_gi, &_desc), setstate(badbit); goto cleanup);
		cleanup:
			//TODO: Figure out why DisposeGWorld(gworld) causes problems in AltiVec
			DisposeGWorld(gworld);
			return *this;
		}
	protected:
		unsigned long _current_index;
		unsigned long _frame_count;
		GraphicsImportComponent _gi;
		ImageDescriptionHandle _desc;
		bool _is_open;
	};
	
	//- fexporter ------------------------------------------------------------------------
	/** \class fexporter "impex.h" <vvis/impex.h>
	 * File Exporter.
	 * Use this class to write image files.
	 */
	class fexporter : public base_impex {
	public:
		fexporter();
		explicit fexporter(const char* filename, openmode mode = out);
		explicit fexporter(const char* filename, const codec_type ft, openmode mode = out);
		virtual ~fexporter();
	public:
		bool is_open() const;
		void open(const char* filename, openmode mode = out);
		void open(const char* filename, const codec_type ft, openmode mode = out);
		void close();
	public:
		template<typename imageT>
		typename ct::enable_if<is_image<imageT>::value, fexporter&>::type
		operator<<(const imageT& image) {
			GWorldPtr gworld = NULL;
			Rect rect;
			MacSetRect(&rect, 0, 0, image.width(), image.height());
			// Convert to GWorld
			_VVIS_CHECK_OSERR(
				QTNewGWorld(&gworld, k32ARGBPixelFormat, &rect, 0, NULL, 0),
				setstate(badbit); goto cleanup);
			LockPixels(GetGWorldPixMap(gworld));
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_gworld(gworld, image, image.width(), image.height());
			UnlockPixels(GetGWorldPixMap(gworld));
			// Perform Export
			_VVIS_CHECK_OSERR(
				GraphicsExportSetInputGWorld(_ge, gworld),
				setstate(badbit); goto cleanup);
			_VVIS_CHECK_OSERR(
				GraphicsExportSetOutputFile(_ge, &_fss),
				setstate(badbit); goto cleanup);
			_VVIS_CHECK_OSERR(
				GraphicsExportDoExport(_ge, NULL),
				setstate(badbit); goto cleanup);
		cleanup:
			DisposeGWorld(gworld);
			return *this;
		}
	protected:
		GraphicsExportComponent _ge;
		FSSpec _fss;
		bool _is_open;
	};

#if !defined VVIS_USER_DISABLE_SEQUENCE_GRABBER
	//====================================================================================
	// Sequence Grabber
	//------------------------------------------------------------------------------------
	namespace priv {
	
		/** sgimporter_record
		 * For running record loop
		 */
		template<typename sgimporterT>
		class sgimporter_record {
		public:
			sgimporter_record(sgimporterT& sgi) : _sgi(sgi) {
			}
			void operator()() {
				OSErr err;
				_VVIS_CHECK_OSERR(SGPrepare(_sgi._seq_grabber, false, true),
					_sgi.setstate(impex_base::badbit); return;);
				_VVIS_CHECK_OSERR(SGStartRecord(_sgi._seq_grabber),
					_sgi.setstate(impex_base::badbit); return;);
				// Create decompression sequence if required
				if(_sgi._decomp_seq == 0) {
					ImageDescriptionHandle image_desc = reinterpret_cast<ImageDescriptionHandle>(NewHandle(0));
					if((err = SGGetChannelSampleDescription(_sgi._video_channel, reinterpret_cast<Handle>(image_desc))) != noErr) {
						_VVIS_WARN("SGGetChannelSampleDescription failed with QuickTime Error: " << err);
						_sgi.setstate(impex_base::badbit);
						return;
					}
					// Scale image to correct size;
					MatrixRecord scale_matrix;
					Rect source_rect = { 0, 0 };
					source_rect.right = (**image_desc).width;
					source_rect.bottom = (**image_desc).height;
					RectMatrix(&scale_matrix, &source_rect, &_sgi._bounds_rect);
					// begin the process of decompressing a sequence of frames
					// this is a set-up call and is only called once for the sequence - the ICM will interrogate different codecs
					// and construct a suitable decompression chain, as this is a time consuming process we don't want to do this
					// once per frame (eg. by using DecompressImage)
					// for more information see Ice Floe #8 http://developer.apple.com/quicktime/icefloe/dispatch008.html
					// the destination is specified as the GWorld
					err = DecompressSequenceBegin(&_sgi._decomp_seq,	// pointer to field to receive unique ID for sequence
													image_desc,			// handle to image description structure
													_sgi._gworld,   // port for the DESTINATION image
													NULL,					// graphics device handle, if port is set, set to NULL
													NULL,					// source rectangle defining the portion of the image to decompress 
													&scale_matrix,			// transformation matrix
													srcCopy,				// transfer mode specifier
													(RgnHandle)NULL,		// clipping region in dest. coordinate system to use as a mask
													NULL,					// flags
													codecNormalQuality, 	// accuracy in decompression
													bestSpeedCodec);		// compressor identifier or special identifiers ie. bestSpeedCodec
					if(err != noErr) {
						_VVIS_WARN("DecompressSequenceBegin failed with QuickTime Error: " << err);
						_sgi.setstate(impex_base::badbit);
						return;
					}
					DisposeHandle((Handle)image_desc);
				}
				while((err = SGIdle(_sgi._seq_grabber)) == noErr) {
				}
				if(err == grabTimeComplete) {
					_sgi.setstate(impex_base::eofbit);
				} else if(err != cantDoThatInCurrentMode) {
					// err becomes cantDoThatInCurrentMode if SGStop is called
					_sgi.setstate(impex_base::failbit);
				} else {
					_VVIS_WARN("sgimporter_record ended unexpectedly with QuickTime Error: " << err);
					_sgi.setstate(impex_base::badbit);
					return;
				}
				_VVIS_CHECK_OSERR(SGStop(_sgi._seq_grabber),
					_sgi.setstate(impex_base::badbit); return;);
				return;
			}
		private:
			sgimporterT& _sgi;
		};

		/** sgimporter_data_proc
		 * Called to get images for recording
		 */
		template<typename sgimporterT>
		OSErr sgimporter_data_proc(SGChannel c, Ptr p, long len, long* offset, long chRefCon,
		TimeValue time, short writeType, long refCon) {
			sgimporterT* sgi = reinterpret_cast<sgimporterT*>(refCon);
			ComponentResult err = noErr;
			CodecFlags ignore;
			/*
			if(decomp_seq == 0) {
				ImageDescriptionHandle image_desc = reinterpret_cast<ImageDescriptionHandle>(NewHandle(0));
				if((err = SGGetChannelSampleDescription(c, reinterpret_cast<Handle>(image_desc))) != noErr) {
					cerr << "SGGetChannelSampleDescription Error: " << err << endl;
					return err;
				}
				// Scale image to correct size;
				MatrixRecord scale_matrix;
				Rect source_rect = { 0, 0 };
				source_rect.right = (**image_desc).width;
				source_rect.bottom = (**image_desc).height;
				RectMatrix(&scale_matrix, &source_rect, &sgi->_bounds_rect);
				// begin the process of decompressing a sequence of frames
				// this is a set-up call and is only called once for the sequence - the ICM will interrogate different codecs
				// and construct a suitable decompression chain, as this is a time consuming process we don't want to do this
				// once per frame (eg. by using DecompressImage)
				// for more information see Ice Floe #8 http://developer.apple.com/quicktime/icefloe/dispatch008.html
				// the destination is specified as the GWorld
				err = DecompressSequenceBegin(&decomp_seq,	// pointer to field to receive unique ID for sequence
												image_desc,			// handle to image description structure
												sgi->_gworld,   // port for the DESTINATION image
												NULL,					// graphics device handle, if port is set, set to NULL
												NULL,					// source rectangle defining the portion of the image to decompress 
												&scale_matrix,			// transformation matrix
												srcCopy,				// transfer mode specifier
												(RgnHandle)NULL,		// clipping region in dest. coordinate system to use as a mask
												NULL,					// flags
												codecNormalQuality, 	// accuracy in decompression
												bestSpeedCodec);		// compressor identifier or special identifiers ie. bestSpeedCodec
				if(err != noErr) {
					cerr << "DecompressSequenceBegin Error: " << err << endl;
					return err;
				}
				DisposeHandle((Handle)image_desc);
			}
			*/
			// Signal that the frame is going to be decompressed
			sgi->frame_decompressing(time);
			// Synchronous decompression
			if((err = DecompressSequenceFrameS(sgi->_decomp_seq, p, len, 0, &ignore, NULL)) != noErr) {
				_VVIS_WARN("DecompressSequenceFrameS failed with QuickTime Error: " << err);
				return err;
			}
			// Signal that a frame is ready
			sgi->frame_ready(time);
			return noErr;
		}
	} // End of priv namespace
	
	//- sgsettings -----------------------------------------------------------------------
	/** \class sgsettings "impex.h" <vvis/impex.h>
	 * Capture Settings.
	 * Used to configure the sgimporter.
	 */
	struct sgsettings {
		/** Default Constructor.
		 * Does nothing.
		 */
		sgsettings();
		/** sgsettings Constructor.
		 * \param w Width
		 * \param h Height
		 * \param pf Pixel Format. Use 0 for default
		 */
		sgsettings(const int w, const int h, const OSType pf = 0);
		int width;
		int height;
		OSType pixel_format;	// Set this to the pixelformat
	};
	
	//- sync_capture -----------------------------------------------------------
	/** \class sync_capture "impex.h" <vvis/impex.h>
	 * sgimporter policy class which performs synchronous capturing.
	 */
	template<typename sgimporterT> class sync_capture : public virtual base_impex {
	public:
		sync_capture(sgimporterT& sgi);
	protected:
		~sync_capture();
	public:
		void stop();
		template<typename imageT>
		typename ct::enable_if<is_image<imageT>::value, sync_capture&>::type
		operator>>(imageT& image) {
			boost::mutex::scoped_lock frame_ready_lock(_frame_ready_mutex);
			_frame_ready_cond.wait(frame_ready_lock);
			boost::mutex::scoped_lock scoped_lock(_gworld_in_use_mutex);
			// Resize image if required
			image.resize(_sgimporter._settings.width, _sgimporter._settings.height);
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_image(image, _sgimporter._gworld,
				_sgimporter._settings.width, _sgimporter._settings.height);
			return *this;
		}
		GWorldPtr wait_for_locked_gworld() {
			_temp_frame_lock.lock();
			_frame_ready_cond.wait(_temp_frame_lock);
			_temp_gworld_lock.lock();
			return _sgimporter._gworld;
		}
		void release_locked_gworld() {
			_temp_gworld_lock.unlock();
			_temp_frame_lock.unlock();
		}
	protected:
		bool component_opened();
		void component_closed();
		void component_stopped();
		inline void frame_decompressing(const TimeValue time);
		inline void frame_ready(const TimeValue time);
	protected:
		sgimporterT& _sgimporter;
		ImageSequence _decomp_seq;
		boost::thread_group _record_threads;
		boost::condition _frame_ready_cond;
		boost::mutex _frame_ready_mutex;
		boost::mutex _gworld_in_use_mutex;
		boost::mutex::scoped_lock _gworld_in_use_lock;
		boost::mutex::scoped_lock _temp_gworld_lock, _temp_frame_lock;
	};
	template<typename sgimporterT>
	sync_capture<sgimporterT>::sync_capture(sgimporterT& sgi)
	: _sgimporter(sgi), _decomp_seq(0), _gworld_in_use_lock(_gworld_in_use_mutex, false),
		_temp_gworld_lock(_gworld_in_use_mutex, false) , _temp_frame_lock(_frame_ready_mutex, false) {
	}
	template<typename sgimporterT>
	sync_capture<sgimporterT>::~sync_capture() {
	}
	template<typename sgimporterT>
	bool sync_capture<sgimporterT>::component_opened() {
		// Start a thread in the background to capture
		_VVIS_CHECK_OSERR(
			SGSetDataRef(_sgimporter.seq_grabber(), 0, 0, seqGrabDontMakeMovie),
			setstate(badbit); return false);
		_VVIS_CHECK_OSERR(
			SGSetDataProc(_sgimporter.seq_grabber(),
				NewSGDataUPP((SGDataProcPtr)priv::sgimporter_data_proc<sgimporterT>),
				(long)reinterpret_cast<sgimporterT*>(this)),	// TO DO: Decide how to pass this via refCon
			setstate(badbit); return false);
		_VVIS_CHECK_OSERR(
			SGSetMaximumRecordTime(_sgimporter.seq_grabber(), 0),
			setstate(badbit); return false);
		// Set up mutexes and condition variables and start the record thread
		priv::sgimporter_record<sgimporterT> record_process(_sgimporter);
		_record_threads.create_thread(record_process);
		return true;
	}
	template<typename sgimporterT>
	void sync_capture<sgimporterT>::component_closed() {
		_VVIS_CHECK_OSERR(
			SGStop(_sgimporter.seq_grabber()),
			setstate(badbit));
		// Record threads should also stop. So wait for them
		_record_threads.join_all();
	}
	template<typename sgimporterT>
	void sync_capture<sgimporterT>::component_stopped() {
		_sgimporter->setstate(eofbit);
	}
	template<typename sgimporterT>
	void sync_capture<sgimporterT>::frame_decompressing(const TimeValue) {
		_gworld_in_use_lock.lock();
	}
	template<typename sgimporterT>
	void sync_capture<sgimporterT>::frame_ready(const TimeValue) {
		_gworld_in_use_lock.unlock();
		_frame_ready_cond.notify_all();
	}
	template<typename sgimporterT>
	void sync_capture<sgimporterT>::stop() {
		// Stop the sequence grabber and the capture thread
		component_closed();
	}
	
	//- async_capture ----------------------------------------------------------
	/** \class async_capture "impex.h" <vvis/impex.h>
	 * sgimporter policy class which performs asynchronous capturing.
	 */
	template<typename sgimporterT> class async_capture : public virtual base_impex {
	public:
		typedef boost::function3<void, sgimporterT&, const TimeValue, void*> callback;
	public:
		async_capture(sgimporterT& sgi);
	protected:
		~async_capture();
	public:
		void start(callback func, void* data);
		/** Start asynchronous frame grabbing
		 * 60 Ticks = 1 second
		 * Set ticks to 0 if you want to record indefinitely
		 */
		void start(const int ticks, callback func, void* data);
		/// Stops the asynchronous frame grabbing
		void stop();
		/// Wait for the asynchronous frame grabbing operation to complete
		void wait_for();
		const TimeScale time_scale();
	public:
		template<typename imageT>
		typename ct::enable_if<is_image<imageT>::value, async_capture&>::type
		operator>>(imageT& image){
			// Resize image if required
			image.resize(_sgimporter._settings.width, _sgimporter._settings.height);
			priv::gworld_converter<k32ARGBPixelFormat, imageT>::to_image(image, _sgimporter._gworld,
				_sgimporter._settings.width, _sgimporter._settings.height);
			return *this;
		}
	protected:
		bool component_opened();
		void component_closed();
		void component_stopped();
		inline void frame_decompressing(const TimeValue time);
		inline void frame_ready(const TimeValue time);
	protected:
		sgimporterT& _sgimporter;
		TimeScale _time_scale;
		int _record_ticks;
		callback _user_function;
		void* _user_data;
		ImageSequence _decomp_seq;
		boost::thread_group _record_threads;
	};
	template<typename sgimporterT>
	async_capture<sgimporterT>::async_capture(sgimporterT& sgi) : _sgimporter(sgi), _time_scale(0), _decomp_seq(0) {
	}
	template<typename sgimporterT>
	async_capture<sgimporterT>::~async_capture() {
	}
	template<typename sgimporterT>
	bool async_capture<sgimporterT>::component_opened() {
		return true;
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::component_closed() {
		stop();
		// Record threads should also stop. So wait for them
		_record_threads.join_all();
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::component_stopped() {
		setstate(this->_record_time == 0 ? eofbit : failbit);
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::frame_decompressing(const TimeValue) {
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::frame_ready(const TimeValue time) {
		_user_function(_sgimporter, time, _user_data);
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::start(
	typename async_capture<sgimporterT>::callback func, void* data) {
		start(0, func, data);
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::start(const int ticks,
	typename async_capture<sgimporterT>::callback func,
	void* data) {
		_record_ticks = ticks;
		// Reset time scale
		_time_scale = 0;
		// Set data for sequence grabber
		_user_function = func;
		_user_data = data;
		_VVIS_CHECK_OSERR(
			SGSetDataRef(_sgimporter.seq_grabber(), 0, 0, seqGrabDontMakeMovie),
			setstate(badbit); return);
		_VVIS_CHECK_OSERR(
			SGSetDataProc(_sgimporter.seq_grabber(),
				NewSGDataUPP(priv::sgimporter_data_proc<sgimporterT>),
				reinterpret_cast<long int>(this)),	// TO DO: Decide how to pass this via refCon
			setstate(badbit); return);
		// Only call SGSetMaximumRecordTime if required
		if(_record_ticks > 0) {
			_VVIS_CHECK_OSERR(
				SGSetMaximumRecordTime(_sgimporter.seq_grabber(), _record_ticks),
				setstate(badbit); return);
		}
		priv::sgimporter_record<sgimporterT> record_process(_sgimporter);
		_record_threads.create_thread(record_process);
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::stop() {
		_VVIS_CHECK_OSERR(
			SGStop(_sgimporter.seq_grabber()),
			_sgimporter.setstate(badbit); return);
		// Reset time scale
		_time_scale = 0;
	}
	template<typename sgimporterT>
	void async_capture<sgimporterT>::wait_for() {
		_record_threads.join_all();
	}
	template<typename sgimporterT>
	const TimeScale async_capture<sgimporterT>::time_scale() {
		if(_time_scale == 0) {
			// Get Time Scale
			_VVIS_CHECK_OSERR(
				SGGetChannelTimeScale(_sgimporter._video_channel, &_time_scale),
				setstate(badbit); return 0);
		}
		return _time_scale;
	}
	
	//- sgimporter -------------------------------------------------------------
	/** \class sgimporter "impex.h" <vvis/impex.h>
	 * Grabs images from the a sequence grabbber.
	 * \param captureP Capture Policy. Use vvis::sync_capture or vvis::async_capture.
	 *                 Or leave blank for the default.
	 */
	template<
		template<typename> class captureP = VVIS_DEFAULT_CAPTURE_POLICY
	> class sgimporter
	: public captureP<sgimporter<captureP> >, public virtual base_impex {
	public:
		sgimporter();
		/** Initialize sgimporter from sgsettings
		 * Will configure sgimporter to capture from the first available
		 * sequence grabber
		 * \param s sgsettings describing capture settings
		 */
		sgimporter(const sgsettings& s);
		/** Initialize sgimporter from Component and sgsettings
		 * Will configure sgimporter to capture from the Component
		 * specified. Use this constructor if there is more than one
		 * sequence grabber connected.
		 * \param c Sequence grabber component.
		 * \param s sgsettings describing capture settings
		 */
		sgimporter(Component c, const sgsettings& s);
		virtual ~sgimporter();
	public:
		bool is_open() const;
		void open(const sgsettings& s);
		void open(Component c, const sgsettings& s);
		void close();
	public:
		const sgsettings settings() const;
	public:
		SeqGrabComponent seq_grabber() const;
		SGChannel video_channel() const;
		const GWorldPtr gworld() const;
	protected:
		typedef sgimporter<captureP> self_type; 
		typedef captureP<self_type> capture_policy;
		sgsettings _settings;
		SeqGrabComponent _seq_grabber;
		SGChannel _video_channel;
		GWorldPtr _gworld;
		Rect _bounds_rect;
		friend class captureP<sgimporter<captureP> >;
		friend class priv::sgimporter_record<sgimporter<captureP> >;
		template<typename sgimporterT> friend OSErr priv::sgimporter_data_proc(SGChannel c, Ptr p,
			long len, long* offset, long chRefCon, TimeValue time, short writeType, long refCon);
	};
	template<template<typename> class captureP>
	sgimporter<captureP>::sgimporter()
	: captureP<sgimporter<captureP> >(*this), _settings(), _seq_grabber(0),
	_video_channel(0) {
	}
	template<template<typename> class captureP>
	sgimporter<captureP>::sgimporter(const sgsettings& s)
	: captureP<sgimporter<captureP> >(*this), _settings(), _seq_grabber(0),
	_video_channel(0) {
		open(s);
	}
	template<template<typename> class captureP>
	sgimporter<captureP>::sgimporter(Component c, const sgsettings& s)
	: captureP<sgimporter<captureP> >(*this), _settings(), _seq_grabber(0),
	_video_channel(0) {
		open(c, s);
	}
	template<template<typename> class captureP>
	sgimporter<captureP>::~sgimporter() {
		close();
	}
	template<template<typename> class captureP>
	bool sgimporter<captureP>::is_open() const {
		return _seq_grabber != NULL;
	}
	template<template<typename> class captureP>
	void sgimporter<captureP>::open(const sgsettings& s) {
		// Open first sequence grabber component
		ComponentDescription cd;
		cd.componentType = SeqGrabComponentType;
		cd.componentSubType = 0;
		cd.componentManufacturer = 0;
		cd.componentFlags = 0;
		cd.componentFlagsMask = 0;
		Component component = 0;
		while((component = FindNextComponent(component, &cd)) != 0) {
			open(component, s);
			if(good())
				return;
		}
		setstate(badbit);
	}
	template<template<typename> class captureP>
	void sgimporter<captureP>::open(Component c, const sgsettings& s) {
		_settings = s;
		close();	// Close existing Sequence Grabber if any
		// Create an instance of the sequence grabber
		_seq_grabber = OpenComponent(c);
		if(_seq_grabber == NULL) {
			setstate(badbit);
			return;
		}
		// Initialise Sequence Grabber
		_VVIS_CHECK_OSERR(
			SGInitialize(_seq_grabber),
			setstate(badbit); return);
		// Create Video Channel
		_VVIS_CHECK_OSERR(
			SGNewChannel(_seq_grabber, VideoMediaType, &_video_channel),
			setstate(badbit); return);
		// Say that we are going to record video
		_VVIS_CHECK_OSERR(
			SGSetChannelUsage(_video_channel, seqGrabRecord | seqGrabLowLatencyCapture),
			setstate(badbit); return);
		if(_settings.width == 0 || _settings.height == 0) {
			// Get Video Source Size
			_VVIS_CHECK_OSERR(
				SGGetSrcVideoBounds(_video_channel, &_bounds_rect),
				setstate(badbit); return);
			if(_settings.width == 0) {
				_settings.width = _bounds_rect.right - _bounds_rect.left;
			}
			if(_settings.height == 0) {
				_settings.height = _bounds_rect.bottom - _bounds_rect.top;
			}
		}
		// Setup _bounds_rect
		MacSetRect(&_bounds_rect, 0, 0, _settings.width, _settings.height);
		// Set Video Bounds
		_VVIS_CHECK_OSERR(
			SGSetChannelBounds(_video_channel, &_bounds_rect),
			setstate(badbit); return);
		// Create offscreen GWorld
		_VVIS_CHECK_OSERR(
			QTNewGWorld(&_gworld, k32ARGBPixelFormat, &_bounds_rect, 0, NULL, 0),
			setstate(badbit); return);
		// Lock the pixmap because we can't decompress to an unlocked pixmap
		if(!LockPixels(GetGWorldPixMap(_gworld))) {
			setstate(badbit);
			return;
		}
		// Set GWorld to the offscreen GWorld
		_VVIS_CHECK_OSERR(SGSetGWorld(_seq_grabber, _gworld, NULL), setstate(badbit); return);
		// Tell capture_policy
		if(capture_policy::component_opened()) {
			// Update state
			ready(true);
			clear();
		}
	}
	template<template<typename> class captureP>
	void sgimporter<captureP>::close() {
		// If there is no sequence grabber, then there is nothing to do
		if(_seq_grabber == NULL)
			return;
		// Inform capture_policy
		capture_policy::component_closed();
		// Close Video Channel only if it exists
		if(_video_channel) {
			// Assume _seq_grabber still exists, otherwise we can't dispose the video channel
			SGDisposeChannel(_seq_grabber, _video_channel);
			_video_channel = NULL;
		}
		// Close the sequence grabber
		CloseComponent(_seq_grabber);
		// Update state
		_seq_grabber = NULL;
		ready(false);
		clear();
	}
	template<template<typename> class captureP>
	const sgsettings sgimporter<captureP>::settings() const {
		return _settings;
	}
	template<template<typename> class captureP>
	SeqGrabComponent sgimporter<captureP>::seq_grabber() const {
		return _seq_grabber;
	}
	template<template<typename> class captureP>
	SGChannel sgimporter<captureP>::video_channel() const {
		return _video_channel;
	}
	template<template<typename> class captureP>
	const GWorldPtr sgimporter<captureP>::gworld() const {
		return _gworld;
	}
#endif
} // End of vvis namespace

#endif