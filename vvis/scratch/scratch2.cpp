/*
 *  scratch2.cpp
 *  vvis
 *
 *  Created by Lai Bing-Chang on 2006/10/30.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include <QuickTime/QuickTime.h>
#include <vvis/vvis.h>
#include <ostream>

/* Utility to set a SInt32 value in a CFDictionary
*/
OSStatus SetNumberValue(CFMutableDictionaryRef inDict,
                        CFStringRef inKey,
                        SInt32 inValue)
{
    CFNumberRef number;

    number = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type, &inValue);
    if (NULL == number) return coreFoundationUnknownErr;

    CFDictionarySetValue(inDict, inKey, number);

    CFRelease(number);

    return noErr;
}

/* Create a QuickTime Pixel Buffer Context
    This function creates a QuickTime Visual Context which will produce CVPixelBuffers.
  From http://developer.apple.com/qa/qa2005/qa1443.html
*/
OSStatus CreatePixelBufferContext(SInt32 inPixelFormat,
                                  CGRect *inBounds,
                                  QTVisualContextRef *outVisualContext)
{
    QTVisualContextRef      theContext = NULL;
    CFMutableDictionaryRef  pixelBufferOptions = NULL;
    CFMutableDictionaryRef  visualContextOptions = NULL;
    OSStatus                err = noErr;

    *outVisualContext = NULL;

    if (0 == inPixelFormat || CGRectIsNull(*inBounds)) {err = paramErr; goto bail;}

    // Pixel Buffer attributes
    pixelBufferOptions = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                   &kCFTypeDictionaryKeyCallBacks,
                                                   &kCFTypeDictionaryValueCallBacks);
    if (NULL == pixelBufferOptions) {err = coreFoundationUnknownErr; goto bail;}

    // the pixel format we want
    SetNumberValue(pixelBufferOptions, kCVPixelBufferPixelFormatTypeKey, inPixelFormat);

    // size
    SetNumberValue(pixelBufferOptions, kCVPixelBufferWidthKey, inBounds->size.width);
    SetNumberValue(pixelBufferOptions, kCVPixelBufferHeightKey, inBounds->size.height);

    // alignment
    SetNumberValue(pixelBufferOptions, kCVPixelBufferBytesPerRowAlignmentKey, 16);

    // QT Visual Context attributes
    visualContextOptions = CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
                                                     &kCFTypeDictionaryKeyCallBacks,
                                                     &kCFTypeDictionaryValueCallBacks);
    if (NULL == visualContextOptions) {err = coreFoundationUnknownErr; goto bail; }

    // set the pixel buffer attributes for the visual context
    CFDictionarySetValue(visualContextOptions,
                         kQTVisualContextPixelBufferAttributesKey,
                         pixelBufferOptions);

    // create a Pixel Buffer visual context
    err = QTPixelBufferContextCreate(kCFAllocatorDefault,
                                     visualContextOptions,
                                     &theContext);
    if (err != noErr) goto bail;

    *outVisualContext = theContext;
    theContext = NULL;

bail:
    if (NULL != visualContextOptions) CFRelease(visualContextOptions);
    if (NULL != pixelBufferOptions) CFRelease(pixelBufferOptions);
    if (NULL != theContext) QTVisualContextRelease(theContext);

    return err;
}

int main(int argc, char *argv[]) {
	/*if(argc != 3) {
		std::cerr << argv[0] << " <input> <output>" << std::endl;
		return 1;
	}*/
	
	unsigned char data[12];
	data[0] = 'A';
	data[1] = 'B';
	data[2] = 'C';
		
	typedef vvis::chunkys_extstorage<vvis::uint8, vvis::uint8, vvis::uint8> image_t;
	image_t img(data, 4, 3);
	std::cout << sizeof(image_t::pixel_type) << std::endl;
	std::cout << img[0] << std::endl;
	img[0].get<0>() = 'D';
	std::cout << img[0] << std::endl;

/*	vvis::transform(img, vvis::acc<image_t>(),
		img, vvis::acc<image_t>(),
		vvis::bind1st(vvis::minus<image_t::pixel_type>(),
			std::numeric_limits<image_t::pixel_type>::max()));

  */  
	/*
	OSErr status;
	
	QTVisualContextRef theVisualContext = NULL;
	try {
		CGRect theBounds = CGRectMake(0, 0, 320, 240);
		status = CreatePixelBufferContext(k32ARGBPixelFormat, &theBounds, &theVisualContext);
		if (noErr != status && NULL == theVisualContext) {
			throw status;
		}
		
		CFStringRef movieLocation = CFSTR("/Users/bclai/thesis/projects/vvis/samples/PICT0075.MOV");
		
		QTNewMoviePropertyElement inputProperties[3];
		ItemCount inputCount = 0;
		inputProperties[inputCount].propClass =	kQTPropertyClass_DataLocation;
		inputProperties[inputCount].propID = kQTDataLocationPropertyID_CFStringPosixPath;
		inputProperties[inputCount].propValueSize = sizeof(movieLocation);
		inputProperties[inputCount].propValueAddress = &movieLocation;
		++inputCount;
		
		// assign the visual context - could also be NULL
		inputProperties[inputCount].propClass = kQTPropertyClass_Context;
		inputProperties[inputCount].propID = kQTContextPropertyID_VisualContext;
		inputProperties[inputCount].propValueSize = sizeof(theVisualContext);
		inputProperties[inputCount].propValueAddress = &theVisualContext;
		++inputCount;
		
		// The Movie active
		Boolean trueValue;
		inputProperties[inputCount].propClass = kQTPropertyClass_NewMovieProperty;
		inputProperties[inputCount].propID = kQTNewMoviePropertyID_Active;
		inputProperties[inputCount].propValueSize = sizeof(trueValue);
		inputProperties[inputCount].propValueAddress = &trueValue;
		++inputCount;
	
		QTNewMoviePropertyElement outputProperties;
		
		Movie theMovie;
		status = NewMovieFromProperties(2, inputProperties,
			0, NULL, &theMovie);
		if (noErr != status || NULL == theMovie) {
			throw status;
		}
		
		// Read movie
		CVPixelBufferRef currentFrame;
		std::cout << "Playing Movie ";
		StartMovie(theMovie);
		while(!IsMovieDone(theMovie)) {
			// 0: Service each movie exactly once
			MoviesTask(theMovie, 0);
			// For chunky, CVPixelBufferGetBaseAddress returns the start
			// For planar, CVPixelBufferGetBaseAddress returns PlanarComponentInfo
			QTVisualContextTask(theVisualContext);
			if(QTVisualContextIsNewImageAvailable(theVisualContext,nil))   {
				QTVisualContextCopyImageForTime(theVisualContext,kCFAllocatorDefault,nil,&currentFrame);
				CVPixelBufferLockBaseAddress(currentFrame,0);
				unsigned char *data = (unsigned char *)CVPixelBufferGetBaseAddress(currentFrame);
				std::cout << (int)data[0] << " " << (int)data[1] << (int)data[2] << (int)data[3] << std::endl;
				CVPixelBufferUnlockBaseAddress(currentFrame,0);
				//CVOpenGLTextureRelease(currentFrame);
			}
		}
		DisposeMovie(theMovie);
		std::cout << std::endl << "Done";
		
		vvm::vector<vvis::uint8> a(10);
		std::cout <<" A: " <<  (a == a) << " " << VVM_TRUE <<  std::endl;

	} catch(OSErr status) {
		std::cerr << "Failed to create pixel buffer: " << status << std::endl;
	}
	// Release
	QTVisualContextRelease(theVisualContext);
	*/
	return 0;
}
