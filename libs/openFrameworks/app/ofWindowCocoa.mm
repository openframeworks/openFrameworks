/*
 *  ofWindowCocoa.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowCocoa.h"

@interface CocoaWindowDelegate : NSObject
{
}

- (void)windowWillClose:(NSNotification *)notification;
- (void)windowDidBecomeKey:(NSNotification *)notification;
- (void)windowDidResignKey:(NSNotification *)notification;
- (void)windowDidExpose:(NSNotification *)notification;
- (void)windowDidResize:(NSNotification *)notification;
- (void)windowDidMove:(NSNotification *)notification;
- (void)windowWillMove:(NSNotification *)notification;
@end

@implementation CocoaWindowDelegate : NSObject

- (void)windowWillClose:(NSNotification *)notification
{
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
}

- (void)windowDidResignKey:(NSNotification *)notification
{
}

- (void)windowDidExpose:(NSNotification *)notification
{
}

- (void)windowDidMove:(NSNotification *)notification
{
}

- (void)windowWillMove:(NSNotification *)notification
{
}

- (void)windowDidResize:(NSNotification *)notification
{

}
@end

//////////////////////

@interface CocoaWindow: NSWindow
{
	
}
@end

@implementation CocoaWindow

-(BOOL)canBecomeKeyWindow
{
	return YES;
}

- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
	return NSDragOperationNone;
}

- (BOOL)wantsPeriodicDraggingUpdates
{
	return NO; 
}

- (NSDragOperation)draggingUpdated:(id < NSDraggingInfo >)sender
{
	return NSDragOperationNone;
}

- (void)draggingExited:(id < NSDraggingInfo >)sender
{
	
}

- (BOOL)prepareForDragOperation:(id < NSDraggingInfo >)sender
{
	return NO;
}

- (BOOL)performDragOperation:(id < NSDraggingInfo >)sender
{
	return NO;
}

@end


ofWindowCocoa::ofWindowCocoa(){

};

ofWindowCocoa::~ofWindowCocoa(){

};

bool ofWindowCocoa::open(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	
	NSRect windowRect = NSMakeRect(10.0f, 10.0f, 800.0f, 600.0f);
	
	NSWindow *window = [[NSWindow alloc] initWithContentRect:windowRect 
												styleMask:( NSResizableWindowMask | NSClosableWindowMask | NSTitledWindowMask) 
												backing:NSBackingStoreBuffered defer:NO];
	
	[window makeKeyAndOrderFront:nil];
	
	cout << "OPENING WINDOW " << endl;
	
	/*
	
	NSRect rect;
	rect.size.width = 500;
	rect.size.height = 500;
	
	nsWindow = [[CocoaWindow alloc] initWithContentRect:rect
											  styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask
												backing:NSBackingStoreBuffered defer:NO];
	
	CocoaWindowDelegate *windowDelegate = [[CocoaWindowDelegate alloc] init];
	[nsWindow setDelegate:windowDelegate];
	
	[nsWindow setLevel : NSNormalWindowLevel ];
	
	NSOpenGLPixelFormat *nsglFormat;
	
    NSOpenGLPixelFormatAttribute attr[] = 
	{
        NSOpenGLPFADoubleBuffer,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFAColorSize, 32,
		NSOpenGLPFADepthSize, 32,
        0
	};
	
	nsglFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
	
	//Creates the OpenGL View inside the window
	openGLView = [[NSOpenGLView alloc] initWithFrame:rect pixelFormat:nsglFormat];
	
	[nsWindow setContentView:openGLView];
*/
	
	[pool drain];
}