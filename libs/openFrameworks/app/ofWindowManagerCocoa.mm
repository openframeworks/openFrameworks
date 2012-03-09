/*
 *  ofWindowManagerCocoa.mm
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */

#include "ofWindowManagerCocoa.h"

#include "ofWindowCocoa.h"


@interface CocoaAppDelegate : NSObject {
}
- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename;
- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender;
- (void)applicationWillTerminate:(NSNotification *)aNotification;
- (void)applicationWillBecomeActive:(NSNotification *)aNotification;
@end

@implementation CocoaAppDelegate : NSObject
- (BOOL)application:(NSApplication *)theApplication openFile:(NSString *)filename
{
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender
{
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	
}

- (void)applicationWillBecomeActive:(NSNotification *)aNotification
{
}
@end



void ofWindowManagerCocoa::init(){
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	[NSApplication sharedApplication];	
	
	CocoaAppDelegate *appDelegate = [[CocoaAppDelegate alloc] init];
	[NSApp setDelegate:appDelegate];
	
	[[NSNotificationCenter defaultCenter]
	 postNotificationName:NSApplicationWillFinishLaunchingNotification
	 object:NSApp];
	
	[[NSNotificationCenter defaultCenter]
	 postNotificationName:NSApplicationDidFinishLaunchingNotification
	 object:NSApp];
	
	[pool drain];
}

ofWindow* ofWindowManagerCocoa::createWindow(){
	ofWindow* win = new ofWindowCocoa();
	win->open();
	addWindow(win);
	cout << "MAKING A WINDOW" << endl;
	return win;
}

void ofWindowManagerCocoa::processEvents(){
	NSEvent *event =
	[NSApp
	 nextEventMatchingMask:NSAnyEventMask
	 untilDate:[NSDate distantFuture]
	 inMode:NSDefaultRunLoopMode
	 dequeue:YES];
	
	[NSApp sendEvent:event];
	[NSApp updateWindows];
}