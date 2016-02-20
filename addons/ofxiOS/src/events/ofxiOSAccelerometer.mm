/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#import <UIKit/UIKit.h>

#include "ofxAccelerometer.h"
#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#include "ofUtils.h"

/************ Interface for iPhone Accelerometer Delegate ************/
@interface ofxiOSAccelerometerDelegate : NSObject <UIAccelerometerDelegate> {
}

@end

// define instance of delegate 
static ofxiOSAccelerometerDelegate* iOSAccelerometerDelegate;



/************ Impementation of standard C++ accel interface tailored for iPhone ************/
// call this to setup the accelerometer
void ofxAccelerometerHandler::setup() {
	ofLogVerbose("ofxAccelerometer") << "setup(): initing iPhoneAccelerometerDelegate";
	[iOSAccelerometerDelegate release];		// in case we've already called it for some reason
	iOSAccelerometerDelegate = [[ofxiOSAccelerometerDelegate alloc] init];
//	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications]; 

#if TARGET_IPHONE_SIMULATOR
	update(1, 0, 0);
#endif
}
	
// call this when accelerometer is no longer needed
void ofxAccelerometerHandler::exit() {
	ofLogVerbose("ofxAccelerometer") << "exit(): releasing iPhoneAccelerometerDelegate";
	[iOSAccelerometerDelegate release];
}





/************ Implementation of iPhone Accelerometer Delegate ************/
@implementation ofxiOSAccelerometerDelegate

-(ofxiOSAccelerometerDelegate*) init {
	if(self = 	[super init]) {
		UIAccelerometer *accelerometer = [UIAccelerometer sharedAccelerometer];
		accelerometer.delegate = self;
		accelerometer.updateInterval = 1.0f/60.0f;
	}
	return self;
}


-(void) accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration*)accel {
	ofxAccelerometer.update(accel.x, accel.y, accel.z);
}

@end

#else 

@interface ofxiOSAccelerometerDelegate : NSObject {
}

@end

// define instance of delegate
static ofxiOSAccelerometerDelegate* iOSAccelerometerDelegate;

/************ Impementation of standard C++ accel interface tailored for iPhone ************/
// call this to setup the accelerometer
void ofxAccelerometerHandler::setup() {

}
void ofxAccelerometerHandler::exit() {
    
}

#endif
