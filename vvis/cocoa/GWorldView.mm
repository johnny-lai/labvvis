//
//  GWorldView.mm
//  cocoa-vvis
//
//  Created by Bing-Chang Lai on Tue Mar 16 2004.
//  Copyright (c) 2004 University of Wollongong. All rights reserved.
//

#import "GWorldView.h"
#import <vvis/impex.h>

@implementation GWorldView
- (id) initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
		// No GWorld to display
		gworld = NULL;
		// Initialize gworld Lock
		gworldLock = [[NSLock alloc] init];
		// Set First Draw to true
		firstDraw = true;
	}
    return self;
}

- (void) dealloc {
	[gworldLock release];
	[self endDecompress];
}

- (void) displayGWorld:(GWorldPtr)gworldToDisplay {
	[gworldLock lock];
	gworld = gworldToDisplay;
	[gworldLock unlock];
	[self setNeedsDisplay:YES];
}

- (void) drawRect:(NSRect)rect {
	if(gworld == NULL)
		return;
	if(firstDraw) {
		// Initialize decompression
		[self initDecompressSeq:(OpaqueGrafPtr*)[self qdPort]];
		//[self initDecompressSeq:NULL];
		firstDraw = false;
	}
	[gworldLock lock];
	[self decompress];
	[gworldLock unlock];
}

// From CocoaVideoFrameToGWorld sample
-(ComponentResult)initDecompressSeq:(CGrafPtr)dstPort
{
	ComponentResult			err = noErr;
    Rect					bounds;
    ImageDescriptionHandle	imageDesc = (ImageDescriptionHandle)NewHandle(0);
    PixMapHandle 			hPixMap = NULL;
        
    NSAssert(imageDesc != NULL, @"null ImageDescriptionHandle");
	NSAssert(gworld != NULL, @"gWorld NULL");
    
    /* Set up getting grabbed data into the Window */
    hPixMap = GetGWorldPixMap(gworld);
    NSAssert(hPixMap != NULL, @"null PixMapHandle");
    NSAssert(LockPixels(hPixMap), @"LockPixels failed");
	
    GetPixBounds(hPixMap,&bounds);
    decompressSeqID = 0;
    
    // returns an image description for the GWorlds PixMap
    // on entry the imageDesc is NULL, on return it is correctly filled out
    // you are responsible for disposing it
    err = MakeImageDescriptionForPixMap(hPixMap, &imageDesc);
    if (err == noErr)
    {
        imageSize = (GetPixRowBytes(hPixMap) * (*imageDesc)->height);
        
        // begin the process of decompressing a sequence of frames - see above notes on this call.
        // destination is specified as the QuickDraw port for our NSView
        err = DecompressSequenceBeginS( &decompressSeqID,
                                        imageDesc,
                                        GetPixBaseAddr(hPixMap),
                                        imageSize,
                                        dstPort,
										NULL,
                                        &bounds,
                                        NULL,
                                        ditherCopy,
                                        NULL,
                                        0,
                                        codecHighQuality,
                                        anyCodec);
    }

    NSAssert(err == noErr, @"DecompressSequenceBeginS error");
    UnlockPixels(hPixMap);
    
    if (imageDesc)
    {
        DisposeHandle((Handle)imageDesc);
    }
    
    return (err);
}

// From CocoaVideoFrameToGWorld sample
-(OSErr)decompress
{
	ComponentResult err = noErr;
    CodecFlags 		ignore;

    err = DecompressSequenceFrameS( decompressSeqID,
                                    GetPixBaseAddr(GetGWorldPixMap(gworld)),
                                    imageSize,
                                    0,
                                    &ignore,
                                    NULL);

    NSAssert(err == noErr, @"DecompressSequenceFrameS error");

    return err;

}

// From CocoaVideoFrameToGWorld sample
- (OSErr) endDecompress
{
    NSAssert(decompressSeqID != NULL, @"Null draw seq");
    
    return (CDSequenceEnd(decompressSeqID));
}

@end
