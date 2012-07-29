//
//  GWorldView.h
//  cocoa-vvis
//
//  Created by Bing-Chang Lai on Tue Mar 16 2004.
//  Copyright (c) 2004 University of Wollongong. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <QuickTime/QuickTime.h>

@interface GWorldView : NSQuickDrawView {
	bool firstDraw;
	GWorldPtr gworld;
	NSLock* gworldLock;
	
	ImageSequence decompressSeqID;	// unique identifier for our draw sequence
    long imageSize;			// size of the image in our buffer to draw
}
- (void) displayGWorld:(GWorldPtr)gworldToDisplay;
- (ComponentResult)initDecompressSeq:(CGrafPtr)dstPort;
- (OSErr)decompress;
- (OSErr)endDecompress;
@end
