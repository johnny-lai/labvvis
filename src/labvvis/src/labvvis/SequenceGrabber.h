//
//  SequenceGrabber.h
//  labvvis
//
//  Created by Johnny Lai on 8/9/12.
//
//

#import <Foundation/Foundation.h>
#import <QTKit/QTKit.h>

@interface SequenceGrabber : NSObject {
    QTCaptureSession *session;
    QTCaptureDevice *device;
    QTCaptureDeviceInput *input;
    QTCaptureDecompressedVideoOutput *output;
}

+ (SequenceGrabber *)sharedInstance;

- (void)startCaptureAtSize:(NSSize)size error:(NSError **)error;
- (void)stopCapture;

@end
