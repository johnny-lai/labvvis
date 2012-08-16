//
//  UnitTests.m
//  UnitTests
//
//  Created by Johnny Lai on 8/8/12.
//
//

#import "UnitTests.h"
#import "QTKit/QTKit.h"

@implementation UnitTests

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testExample
{
    framesCaptured = 0;
    
    QTCaptureSession *session = [[QTCaptureSession alloc] init];

    QTCaptureDevice *device = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
    STAssertNotNil(device, @"Device was opened");

    NSError *error;
    STAssertTrue([device open:&error], @"Device opened");
    
    QTCaptureDeviceInput *input = [[QTCaptureDeviceInput alloc] initWithDevice:device];
    STAssertTrue([session addInput:input error:&error], @"Add Capture Device Input to session");
    
    QTCaptureDecompressedVideoOutput *output = [[QTCaptureDecompressedVideoOutput alloc] init];
    [output setPixelBufferAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
        [NSNumber numberWithInt:320], kCVPixelBufferWidthKey,
        [NSNumber numberWithInt:240], kCVPixelBufferHeightKey,
        [NSNumber numberWithInt:kCVPixelFormatType_32RGBA], kCVPixelBufferPixelFormatTypeKey,
        nil]];
    [output setDelegate:self];
    STAssertTrue([session addOutput:output error:&error], @"Add Capture Device Input to session");

    [session startRunning];
    
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:2]];
    
    [session stopRunning];
    
    STAssertTrue(framesCaptured > 0, @"Got frames");

    [[input device] close];
    
    [input release];

    [device release];
    [session release];

    NSLog(@"Device attributes: %@", [device modelUniqueID]);
}

- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
    CVPixelBufferLockBaseAddress(videoFrame, 0);
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(videoFrame);
    STAssertTrue(bytesPerRow >= 320 * 4, @"Video Buffer bytes per row is %d", bytesPerRow);
    STAssertTrue(CVPixelBufferGetWidth(videoFrame) == 320, @"Video Buffer width is 320");
    STAssertTrue(CVPixelBufferGetHeight(videoFrame) == 240, @"Video Buffer height is 240");
    STAssertTrue(CVPixelBufferGetBaseAddress(videoFrame) != NULL, @"Source Buffer exists");
    CVPixelBufferUnlockBaseAddress(videoFrame, 0);
    
    ++framesCaptured;
}

@end
