/*
 *  ofWindowCocoa.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowCocoa.h"

//WINDOW DELEGATE

@interface CocoaWindowDelegate : NSWindowController <NSWindowDelegate>
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

@implementation CocoaWindowDelegate
- (void)windowWillClose:(NSNotification *)notification
{
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	cout << "I AM KEY!" << endl;
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

void ofWindowCocoa::initializeWindow(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSRect rect = NSMakeRect(x, y, width, height);
	
	nsWindow = [[CocoaWindow alloc] initWithContentRect:rect
									styleMask: (NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask)
									backing:NSBackingStoreBuffered defer:NO];
	
	CocoaWindowDelegate *windowDelegate = [[CocoaWindowDelegate alloc] init];
	[nsWindow setDelegate:windowDelegate];
		
	NSOpenGLPixelFormat *nsglFormat;
	
	NSOpenGLPixelFormatAttribute attr[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 32,
		0
	};
	
	nsglFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attr];
	
	//Creates the OpenGL View inside the window
	openGLView = [[NSOpenGLView alloc] initWithFrame:rect pixelFormat:nsglFormat];
	
	[nsWindow setContentView:openGLView];
	
	[nsWindow makeKeyAndOrderFront:nil];
	[pool drain];
}

void ofWindowCocoa::enableContext(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[[openGLView openGLContext] makeCurrentContext];
	[pool drain];
}

void ofWindowCocoa::processEvents(){

}

void ofWindowCocoa::postDraw(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[[openGLView openGLContext] flushBuffer]; 
	[pool drain];
}