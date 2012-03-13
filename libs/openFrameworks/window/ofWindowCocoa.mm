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

@interface CocoaWindowDelegate : NSObject 
#ifdef MAC_OS_X_VERSION_10_6
<NSWindowDelegate>
#endif
{
	ofWindowCocoa *window;
}

- (void)setOfWindow:(ofWindowCocoa*)window;
- (void)windowWillClose:(NSNotification *)notification;
- (void)windowDidBecomeKey:(NSNotification *)notification;
- (void)windowDidResignKey:(NSNotification *)notification;
- (void)windowDidExpose:(NSNotification *)notification;
- (void)windowDidResize:(NSNotification *)notification;
- (void)windowDidMove:(NSNotification *)notification;
- (void)windowWillMove:(NSNotification *)notification;
@end

@implementation CocoaWindowDelegate
-(void)setOfWindow:(ofWindowCocoa *)win
{
	window = win;
}

- (void)windowWillClose:(NSNotification *)notification
{
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
	window->focused();
}

- (void)windowDidResignKey:(NSNotification *)notification
{
	window->unfocused();
}

- (void)windowDidExpose:(NSNotification *)notification
{
}

- (void)windowDidMove:(NSNotification *)notification
{
	//window->moved(window->)
}

- (void)windowWillMove:(NSNotification *)notification
{
	window->resized(window->getNSWindow().frame.origin.x, window->getNSWindow().frame.origin.y);
}

- (void)windowDidResize:(NSNotification *)notification
{
	window->resized(window->getNSWindow().frame.size.width, window->getNSWindow().frame.size.height);
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


////////////////////////////
/****
///*/

void ofWindowCocoa::initializeWindow(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		
	NSRect rect = NSMakeRect(x, y, width, height);
	
	nsWindow = [[CocoaWindow alloc] initWithContentRect:rect
									styleMask: (NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask)
									backing:NSBackingStoreBuffered defer:NO];
	
	CocoaWindowDelegate *windowDelegate = [[CocoaWindowDelegate alloc] init];
	[windowDelegate setOfWindow:this];
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

NSWindow* ofWindowCocoa::getNSWindow(){
	return nsWindow;
}