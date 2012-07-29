//
//  MyDocument.m
//  cocoa-vvis
//
//  Created by Bing-Chang Lai on Tue Mar 16 2004.
//  Copyright (c) 2004 University of Wollongong. All rights reserved.
//

#import "MyDocument.h"
#import "CameraViewController.h"

@implementation MyDocument

- (id) init {
	self = [super init];
	if (self) {
		// Add your subclass-specific initialization here.
		// If an error occurs here, send a [self release] message and return nil.
	}
	return self;
}

- (void) makeWindowControllers {
	CameraViewController *ctl = [[CameraViewController alloc] initWithWindowNibName:@"CameraView"];
	[ctl autorelease];
	[self addWindowController:ctl];
}

- (void) windowControllerDidLoadNib:(NSWindowController *) aController {
	[super windowControllerDidLoadNib:aController];
// Add any code here that needs to be executed once the windowController has loaded the document's window.
}

- (NSData *) dataRepresentationOfType:(NSString *)aType {
	// Insert code here to write your document from the given data.  You can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.
return nil;
}

- (BOOL) loadDataRepresentation:(NSData *)data ofType:(NSString *)aType {
	// Insert code here to read your document from the given data.  You can also choose to override -loadFileWrapperRepresentation:ofType: or -readFromFile:ofType: instead.
	return YES;
}

@end
