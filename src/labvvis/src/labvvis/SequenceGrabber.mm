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

- (id)init
{
    self = [super init];
    if (self) {
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)startCaptureAtSize:(NSSize)size error:(NSError **)error
{
    if (session != nil) {
        *error = [NSError errorWithDomain:NSPOSIXErrorDomain code:EBUSY userInfo:nil];
        return;
    }
    
    // Create new Thread
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSNumber numberWithInt:size.width], kCVPixelBufferWidthKey,
                             [NSNumber numberWithInt:size.height], kCVPixelBufferHeightKey,
                             [NSNumber numberWithInt:kCVPixelFormatType_32RGBA], kCVPixelBufferPixelFormatTypeKey,
                             nil];

    NSRunLoop *loop = [NSRunLoop currentRunLoop];
    
    session = [[QTCaptureSession alloc] init];
    device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];

    [device open:error];
    if (*error != nil) {
        goto fail;
    }
    
    input = [[QTCaptureDeviceInput alloc] initWithDevice:device];
    [session addInput:input error:error];
    if (*error != nil) {
        goto fail;
    }
        
    output = [[QTCaptureDecompressedVideoOutput alloc] init];
    [output setPixelBufferAttributes:options];
    [output setDelegate:self];
    [session addOutput:output error:error];
    if (*error != nil) {
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

- (void)waitUntilFrameCapturedTo:(lv::image *)buffer error:(NSError **)error
{
    if (session) {
        targetBuffer = buffer;
        while (targetBuffer != NULL) {
            @autoreleasepool {
                CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, true);
            }
        }
    }
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
    if(targetBuffer == NULL) {
        return;
    }
    
    CVPixelBufferLockBaseAddress(videoFrame, 0);
    
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(videoFrame);
    int width = CVPixelBufferGetWidth(videoFrame);
    int height = CVPixelBufferGetHeight(videoFrame);
    UInt8 *source = (UInt8*)CVPixelBufferGetBaseAddress(videoFrame);
    
    for(int y = 0; y < height; ++y) {
        UInt8 *sourceRow = source + (bytesPerRow * y);
        for(int x = 0; x < width; ++x) {
            UInt8 *sourcePixel = sourceRow + (x * 4);
            switch(targetBuffer->channel_count()) {
            case 1:
                targetBuffer->pixel(0, x, y) = (sourcePixel[1] + sourcePixel[2] + sourcePixel[3]) / 3;
                break;
            case 4:
                targetBuffer->pixel(0, x, y) = sourcePixel[1];
                targetBuffer->pixel(1, x, y) = sourcePixel[2];
                targetBuffer->pixel(2, x, y) = sourcePixel[3];
                targetBuffer->pixel(3, x, y) = sourcePixel[0];
                break;
            }
        }
    }
    
    targetBuffer = NULL;
    CVPixelBufferUnlockBaseAddress(videoFrame, 0);
}

- (void)stopCapture
{
    if (session != nil) {
        [session stopRunning];
        targetBuffer = NULL;
        
        [output release];   output = nil;
        [input release];    input = nil;
        [device release];   device = nil;
        [session release];  session = nil;
    }
}

@end
