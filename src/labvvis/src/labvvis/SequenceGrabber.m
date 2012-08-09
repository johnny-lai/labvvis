//
//  SequenceGrabber.m
//  labvvis
//
//  Created by Johnny Lai on 8/9/12.
//
//

#import "SequenceGrabber.h"

static SequenceGrabber *sharedSequenceGrabber = nil;

@implementation SequenceGrabber

+ (SequenceGrabber *)sharedInstance
{
    if (sharedSequenceGrabber == nil) {
        sharedSequenceGrabber = [[SequenceGrabber alloc] init];
    }
    return sharedSequenceGrabber;
}

- (void)startCaptureAtSize:(NSSize)size error:(NSError **)error
{
    if (session != nil) {
        *error = [NSError errorWithDomain:NSPOSIXErrorDomain code:EBUSY userInfo:nil];
        goto fail;
    }
    
    session = [[QTCaptureSession alloc] init];
    device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];

    [device open:error];
    if (error != nil) {
        goto fail;
    }
    
    input = [[QTCaptureDeviceInput alloc] initWithDevice:device];
    [session addInput:input error:error];
    if (error != nil) {
        goto fail;
    }
        
    output = [[QTCaptureDecompressedVideoOutput alloc] init];
    [output setPixelBufferAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithInt:size.width], kCVPixelBufferWidthKey,
        [NSNumber numberWithInt:size.height], kCVPixelBufferHeightKey,
        [NSNumber numberWithInt:kCVPixelFormatType_32RGBA], kCVPixelBufferPixelFormatTypeKey,
        nil]];
    [output setDelegate:self];
    [session addOutput:output error:error];
    if (error != nil) {
        goto fail;
    }
        
    [session startRunning];
    
    return;
    
fail:
    [output release];   output = nil;
    [input release];    input = nil;
    [device release];   device = nil;
    [session release];  session = nil;
}

- (void)stopCapture
{
    if (session != nil) {
        [session stopRunning];
    }
    
    [output release];   output = nil;
    [input release];    input = nil;
    [device release];   device = nil;
    [session release];  session = nil;
}

@end
