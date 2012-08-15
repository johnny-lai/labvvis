//
//  SequenceGrabber.h
//  labvvis
//
//  Created by Johnny Lai on 8/9/12.
//
//

#import <Foundation/Foundation.h>
#import <QTKit/QTKit.h>
#import <Accelerate/Accelerate.h>
#include "image.h"

@interface SequenceGrabber : NSObject {
    QTCaptureSession *session;
    QTCaptureDevice *device;
    QTCaptureDeviceInput *input;
    QTCaptureDecompressedVideoOutput *output;
    
    NSThread *thread;
    BOOL shouldKeepRunning;
    lv::image *targetBuffer;
    NSCondition *targetBufferFilled;
}

+ (SequenceGrabber *)sharedInstance;

- (void)startCaptureAtSize:(NSSize)size error:(NSError **)error;
- (void)waitUntilFrameCapturedTo:(lv::image *)buffer error:(NSError **)error;
- (void)stopCapture;

@end
