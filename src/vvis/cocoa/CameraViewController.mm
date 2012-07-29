//
//  CameraViewController.m
//  cocoa-vvis
//
//  Created by Bing-Chang Lai on Tue Mar 16 2004.
//  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
//

#import "CameraViewController.h"

using namespace vvis;

#define TRANSFORM_FIRST_IMAGE(OP) \
		vvis::transform(*firstImage, vvis::pixel_accessor<image_t>(), \
			*resultImage, vvis::pixel_accessor<image_t>(), \
			vvis::OP<image_t::pixel_type>());
			
#define TRANSFORM_BOTH_IMAGES(OP) \
		vvis::transform(*firstImage, vvis::pixel_accessor<image_t>(), \
			*secondImage, vvis::pixel_accessor<image_t>(), \
			*resultImage, vvis::pixel_accessor<image_t>(), \
			vvis::OP<image_t::pixel_type>());

void frame_available(sgimporter<async_capture>& sgi, const TimeValue time, void*data) {
	CameraViewController* controller = (CameraViewController*)data;
	[controller frameAvailable:time];
	OSErr err = SGIdle(sgi.seq_grabber());
	if(err != noErr) {
		NSLog(@"SGIdle returned %d", err);
	}
}

@implementation CameraViewController
- (void) dealloc {
	[imageLock release];
	
	delete seqGrabber;
	delete firstImage;
	delete secondImage;
	delete resultImage;
	DisposeGWorld(secondGWorld);
	DisposeGWorld(resultGWorld);
	[super dealloc];
}

- (void) windowDidLoad {
	// Setup a notification for closing the window
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(fatalError:)
		name:@"fatalError" object:nil];
	
	// Detach a NSThread to switch Cocoa to Multi Threaded mode
	[NSThread detachNewThreadSelector:@selector(doNothing:) toTarget:self withObject:nil];
	
	// Initialize image Lock
	imageLock = [[NSLock alloc] init];
		
	// Set current operation to none
	currentOp = no_op;
	// Initialize Sequence Grabber
	NSRect rect = [cameraView bounds];
	seqGrabber = new sgimporter_t(sgsettings(rect.size.width, rect.size.height));
	if(!(seqGrabber->good())) {
		[[NSNotificationQueue defaultQueue]
			enqueueNotification:[NSNotification
				notificationWithName:@"fatalError"
				object:self
				userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
					@"Sequence Grabber could not be initialized",
					@"Message", nil]]
			postingStyle:NSPostASAP];
	}
	
	// Create images
	firstImage = new image_t(rect.size.width, rect.size.height);
	secondImage = new image_t(rect.size.width, rect.size.height);
	resultImage = new image_t(rect.size.width, rect.size.height);
	
	// Initialise secondImage with a black image
	vvis::transform(*secondImage, vvis::pixel_accessor<image_t>(),
		*secondImage, vvis::pixel_accessor<image_t>(),
		vvis::constant<image_t::pixel_type>(image_t::pixel_type(0)));

	// Initialise resultImage with a black image
	vvis::transform(*resultImage, vvis::pixel_accessor<image_t>(),
		*resultImage, vvis::pixel_accessor<image_t>(),
		vvis::constant<image_t::pixel_type>(image_t::pixel_type(0)));
	
	// Create new GWorld for second and result
	// No need for first because first is showing straight camera output
	Rect r;
	SetRect(&r, 0, 0, rect.size.width, rect.size.height);
	if(QTNewGWorld(&secondGWorld, k32ARGBPixelFormat, &r, 0, NULL, 0) != noErr) {
		[[NSNotificationQueue defaultQueue]
			enqueueNotification:[NSNotification
				notificationWithName:@"fatalError"
				object:self
				userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
					@"Unable to create GWorld",
					@"Message", nil]]
			postingStyle:NSPostASAP];
	}
	if(QTNewGWorld(&resultGWorld, k32ARGBPixelFormat, &r, 0, NULL, 0) != noErr) {
		[[NSNotificationQueue defaultQueue]
			enqueueNotification:[NSNotification
				notificationWithName:@"fatalError"
				object:self
				userInfo:[NSDictionary dictionaryWithObjectsAndKeys:
					@"Unable to create GWorld",
					@"Message", nil]]
			postingStyle:NSPostASAP];
	}
	
	// Start Frames Per Second updater
	averageFramesPerSecond = 0;
	currentFramesPerSecond = 0;
	[NSTimer scheduledTimerWithTimeInterval:0.5 target:self
		selector:@selector(updateFrameRate:) userInfo:NULL repeats:YES];
}

- (BOOL) shouldCloseDocument {
	return YES;
}

- (void) fatalError:(NSNotification*)notification {
	if([notification object] == self) {
		NSRunCriticalAlertPanel(@"Error",[[notification userInfo] objectForKey:@"Message"],@"Close Window", NULL, NULL);
		[self close];
		// Since we are closing, we don't want anymore notifications
		[[NSNotificationCenter defaultCenter] removeObserver:self];
	}
}

- (void) windowWillClose:(NSNotification *)aNotification {
	[self stopCapture:self];
	seqGrabber->close();
}

- (void) doNothing:(id)nothing {
}

- (void) updateFrameRate:(id)theTimer {
	[frameRate setStringValue:[NSString stringWithFormat:@"Average: %.2f fps  Last: %.2f fps",
		averageFramesPerSecond, currentFramesPerSecond]];
}

- (void) frameAvailable:(const TimeValue)time {
	//NSLog(@"isMultiThread: %u", [NSThread isMultiThreaded]);
	//NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[imageLock lock];
	// Get the image
	(*seqGrabber) >> *firstImage;
	
	// Process Image
	switch(currentOp) {
	case no_op:
		*resultImage = *firstImage;
		break;
	case invert:
		vvis::transform(*firstImage, vvis::pixel_accessor<image_t>(),
			*resultImage, vvis::pixel_accessor<image_t>(),
			vvis::bind1st(vvis::minus_saturated<image_t::pixel_type>(),
				std::numeric_limits<vvis::uint8>::max()));
		break;
	case threshold:
		vvis::transform(*firstImage, vvis::pixel_accessor<image_t>(),
			*resultImage, vvis::pixel_accessor<image_t>(),
			vvis::bind2nd(vvis::greater<image_t::pixel_type>(),
				std::numeric_limits<vvis::uint8>::max() / 2));
		break;
	case convolve: {
		sint8 mask[] = { 0,  1,  1,
						-1,  0,  1, 
						-1, -1,  0};
		vvis::kernel2d<sint8> kernel(mask, 3, 3);
		vvis::convolute(*firstImage, vvis::pixel_accessor<image_t, vvis::no_prefetch>(),
			*resultImage, vvis::pixel_accessor<image_t, vvis::no_prefetch>(),
			vvis::linear_filter<sint8, image_t::pixel_type>(kernel));
		}
		break;
	case plus_op: TRANSFORM_BOTH_IMAGES(plus); break;
	case minus_op: TRANSFORM_BOTH_IMAGES(minus); break;
	case multiply_op: TRANSFORM_BOTH_IMAGES(multiplies); break;
	case divide_op: TRANSFORM_BOTH_IMAGES(divides); break;
	case plus_saturated_op: TRANSFORM_BOTH_IMAGES(plus_saturated); break;
	case minus_saturated_op: TRANSFORM_BOTH_IMAGES(minus_saturated); break;
	case modulus_op: TRANSFORM_BOTH_IMAGES(modulus); break;
	case negate_op: TRANSFORM_FIRST_IMAGE(negate); break;
	
	case equal_to_op: TRANSFORM_BOTH_IMAGES(equal_to); break;
	case not_equal_to_op: TRANSFORM_BOTH_IMAGES(not_equal_to); break;
	case greater_op: TRANSFORM_BOTH_IMAGES(greater); break;
	case less_op: TRANSFORM_BOTH_IMAGES(less); break;
	case greater_equal_op: TRANSFORM_BOTH_IMAGES(greater_equal); break;
	case less_equal_op: TRANSFORM_BOTH_IMAGES(less_equal); break;

	case logical_and_op: TRANSFORM_BOTH_IMAGES(logical_and); break;
	case logical_or_op: TRANSFORM_BOTH_IMAGES(logical_or); break;
	case logical_not_op: TRANSFORM_FIRST_IMAGE(logical_not); break;

	case bitwise_and_op: TRANSFORM_BOTH_IMAGES(bitwise_and); break;
	case bitwise_or_op: TRANSFORM_BOTH_IMAGES(bitwise_or); break;
	case bitwise_not_op: TRANSFORM_FIRST_IMAGE(bitwise_not); break;
	}

	[cameraView displayGWorld:seqGrabber->gworld()];
	// Convert to GWorld for display
	draw_image(secondGWorld, *secondImage);
	draw_image(resultGWorld, *resultImage);
	// Show result
	[secondView displayGWorld:secondGWorld];
	[resultView displayGWorld:resultGWorld];
	[imageLock unlock];
	
	// Increment Frame Count
	++frameCount;
	float time_scale = (float)seqGrabber->time_scale();
	averageFramesPerSecond = ((float)frameCount * time_scale) / (float)time;
	currentFramesPerSecond = 0;
	if(lastTime != 0)
		currentFramesPerSecond = time_scale / ((float)time - lastTime);
	lastTime = (float)time;
	//[pool release];
}

- (IBAction) changeOperation:(id)sender {
	currentOp = [[sender selectedItem] tag];
}

- (IBAction) startCapture:(id)sender {
	lastTime = 0;
	frameCount = 0;
	seqGrabber->start(frame_available, self);
}

- (IBAction) stopCapture:(id)sender {
	seqGrabber->stop();
}

- (IBAction) grabCapture:(id)sender {
	[imageLock lock];
	*secondImage = *firstImage;
	[imageLock unlock];
}

@end
