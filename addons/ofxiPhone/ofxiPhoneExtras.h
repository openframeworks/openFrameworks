/***********************************************************************
 
 ofxiPhone.h
 
 Contains core functionality for iPhone / iPod Touch
 Any .cpp files that include this needs to be renamed to .mm (to support Objective C++)

 
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

#pragma once

#import <UIKit/UIKit.h>
#import "ofMain.h"
#import "ofxiPhone.h"
#import "ofAppiPhoneWindow.h"
#import "ofxiPhoneAppDelegate.h"
#import "EAGLView.h"
#import "ofxiPhoneKeyboard.h"
#import "ofxiPhoneCoreLocation.h"
#import "ofxiPhoneImagePicker.h"
#import "ofxiPhoneMapKit.h"


enum ofxiPhoneDeviceType {
	OFXIPHONE_DEVICE_IPHONE,
	OFXIPHONE_DEVICE_IPODTOUCH
};


// possible return values for iPhoneGetDeviceRevision
#define OFXIPHONE_DEVICE_IPHONE_2G		"iPhone1,1"
#define OFXIPHONE_DEVICE_IPHONE_3G		"iPhone1,2"
#define OFXIPHONE_DEVICE_IPHONE_3GS		"iPhone2,1"
#define OFXIPHONE_DEVICE_IPOD_1STGEN	"iPod1,1"
#define OFXIPHONE_DEVICE_IPOD_2NDGEN	"iPod2,1"
#define OFXIPHONE_DEVICE_IPOD_3RDGEN	"iPod3,1"


// possible values for iPhoneSetOrientation or iPhoneGetOrientation
#define	OFXIPHONE_ORIENTATION_PORTRAIT			UIDeviceOrientationPortrait
#define OFXIPHONE_ORIENTATION_UPSIDEDOWN		UIDeviceOrientationPortraitUpsideDown
#define OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT	UIDeviceOrientationLandscapeRight
#define OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT	UIDeviceOrientationLandscapeLeft

// whether device has audio in
bool iPhoneHasAudioIn();


// returns normalized (0...1) Average and Peak audio levels on mic
float iPhoneGetMicAverageLevel();
//float iPhoneGetMicPeakLevel();

// return device type
ofxiPhoneDeviceType	iPhoneGetDeviceType();

// return device revision
string iPhoneGetDeviceRevision();

// return application key UIWindow
UIWindow *iPhoneGetUIWindow();

// return openglview
EAGLView *iPhoneGetGLView();

// return OpenFrameworks iPhone Window
ofAppiPhoneWindow* iPhoneGetOFWindow();

// return application delegate
ofxiPhoneAppDelegate *iPhoneGetAppDelegate();


// brings the OpenGL view to the front of any other UIViews
// the OpenGL view will receive touchXXXXX events, but other UIViews will not
// disable OpenGL user interaction (iPhoneSetGLViewUserInteraction) to allow UIViews to receive touch events
void iPhoneSendGLViewToFront();


// send OpenGL view to back of any other UIViews
// the OpenGL view will not receive touchXXX events if the other UIViews have userInteraction enabled
void iPhoneSendGLViewToBack();


// make the background of the opengl view transparent or not
// useful to make OpenGL View transparent if it's in front of other UIViews (see iPhoneSendGLViewToBack)
void iPhoneSetGLViewTransparent(bool b);


// enable or disable User interaction in OpenGL View
// if disabled, touchXXXX events will not be called, but UIViews behind the OpenGL view will receive touch events
void iPhoneSetGLViewUserInteraction(bool b);


// enable or disable the iPhone idle timer
// (screen dims and goes off after x amount of inactivity)
void iPhoneDisableIdleTimer();
void iPhoneEnableIdleTimer();


// lock/unlock the opengl context to allow sharing between threads
// (you'll also need to use a sharegroup)
// http://developer.apple.com/iphone/library/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html#//apple_ref/doc/uid/TP40008793-CH103-SW12
void iPhoneLockGLContext();
void iPhoneUnlockGLContext();



// experimental feature.
// call this once in testApp::setup() to run the update+draw in a separate NSThread instead of NSTimer
// MUST be called before ofSetFrameRate(xx);
// disabled by default
void iPhoneEnableLoopInThread();


// set orientation of device (affects statusbar, opengl viewport, touch positions)
void iPhoneSetOrientation(UIDeviceOrientation orientation);
UIDeviceOrientation iPhoneGetOrientation();


//void iPhoneEnableMultitouch();


// load an image from the app bundle into a texture
// NOTE: renamed this function to something more clearer
// WAS: void iPhoneLoadImageFromBundle(NSString *filename, GLuint *spriteTexture);
bool iPhoneBundleImageToGLTexture(NSString *filename, GLuint *spriteTexture);

// load an image from UIImage into an opengl texture
// NOTE: renamed this function to something more clearer
// WAS: void iPhoneLoadImageFromUIImage(UIImage *uiImage, GLuint *spriteTexture);
bool iPhoneUIImageToGLTexture(UIImage *uiImage, GLuint *spriteTexture);


// create an ofImage out of a UIImage
// you do not need to allocate anything before calling this, it is done in the function
// targetWidth, targetHeight are target dimensions (UIImage is resized to this size and ofImage is created)
// .... omit targetWidth & targetHeight to use original image dimensions and not resize
// TODO: take into consideration UI image orentation
bool iPhoneUIImageToOFImage(UIImage *uiImage, ofImage &outImage, int targetWidth = 0, int targetHeight = 0);


// save current opengl screen to photos app
// based on code from http://www.bit-101.com/blog/?p=1861
void iPhoneScreenGrab(id delegate);


// utility fuctions for converting strings and NSStrings back and forth
string ofxNSStringToString(NSString * s);
NSString * ofxStringToNSString(string s);

// It returns the path to the folder which your app has read/write access to.
string ofxiPhoneGetDocumentsDirectory();
