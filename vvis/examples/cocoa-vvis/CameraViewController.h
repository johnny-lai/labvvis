//
//  CameraViewController.h
//  cocoa-vvis
//
//  Created by Bing-Chang Lai on Tue Mar 16 2004.
//  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <vvis/vvis.h>
#import "GWorldView.h"

typedef vvis::sgimporter<vvis::async_capture> sgimporter_t;
typedef vvis::image<vvis::uint8> image_t;
//typedef vvis::rgb_image<vvis::uint8> image_t;
enum operation {
	noop = 0, invert, threshold, convolve,
	plus_op = 21, minus_op,	multiply_op, divide_op, plus_saturated_op, minus_saturated_op,
		modulus_op, negate_op,
	equal_to_op = 41, not_equal_to_op, greater_op, less_op, greater_equal_op, less_equal_op,
	logical_and_op = 61, logical_or_op, logical_not_op,
	bitwise_and_op = 81, bitwise_or_op, bitwise_not_op
};

@interface CameraViewController : NSWindowController {
	sgimporter_t* seqGrabber;
	image_t* firstImage;
	image_t* secondImage;
	image_t* resultImage;
	GWorldPtr secondGWorld;
	GWorldPtr resultGWorld;

	int currentOp;
	NSLock* imageLock;
	
	IBOutlet GWorldView* cameraView;
	IBOutlet GWorldView* secondView;
	IBOutlet GWorldView* resultView;
	IBOutlet NSTextField* frameRate;
	
	long frameCount;
	float lastTime;
	float averageFramesPerSecond, currentFramesPerSecond;
}
- (void) fatalError:(NSNotification*)notification;
- (void) doNothing:(id)nothing;
- (void) updateFrameRate:(id)theTimer;
- (void) frameAvailable:(const TimeValue)time;
- (IBAction) changeOperation:(id)sender;
- (IBAction) startCapture:(id)sender;
- (IBAction) stopCapture:(id)sender;
- (IBAction) grabCapture:(id)sender;
@end
