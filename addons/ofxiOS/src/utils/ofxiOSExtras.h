/* -----------------------------------------------------------------------
 ofxiOSExtras.h
 
 Contains core functionality for iPhone / iPod Touch
 Any .cpp files that include this needs to be renamed to .mm (to support Objective C++)
 -----------------------------------------------------------------------
 
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
 * ----------------------------------------------------------------------- */ 

#pragma once

#import <UIKit/UIKit.h>
#import "ofMain.h"
#import "ofxiOS.h"
#import "ofAppiOSWindow.h"
#import "ofxiOSAppDelegate.h"
#import "ofxiOSEAGLView.h"
#import "ofxiOSKeyboard.h"
#import "ofxiOSCoreLocation.h"
#import "ofxiOSImagePicker.h"
#import "ofxiOSMapKit.h"
#include <sys/sysctl.h>


enum ofxiOSDeviceType {
    OFXIOS_DEVICE_IPHONE = 0,
    OFXIOS_DEVICE_IPODTOUCH,
    OFXIOS_DEVICE_IPAD,
    OFXIOS_DEVICE_UNKNOWN
};

// this is the new way for getting device info.
// we can add other parameters later.
// maybe also methods for checking if device is newer or older than a certain model.
class ofxiOSDeviceInfo{
    public:
    
        ofxiOSDeviceType deviceType;
        string deviceString;
        int versionMajor;
        int versionMinor;
};


// Possible return values for ofxiOSGetDeviceRevision
// NOTE that these #defines are deprecated in favor of ofxiOSGetDeviceInfo()
//-------------------------------------------------------------------------------

// iPhones
#define OFXIOS_DEVICE_IPHONE_2G         "iPhone1,1"     // iPhone (Original/1st Gen/EDGE) - January 2007
#define OFXIOS_DEVICE_IPHONE_3G         "iPhone1,2"     // iPhone 3G - June 2008
#define OFXIOS_DEVICE_IPHONE_3GS        "iPhone2,1"     // iPhone 3GS - June 2009
#define OFXIOS_DEVICE_IPHONE_4          "iPhone3,1"     // iPhone 4 (GSM) - June 2010

// iPods (touch)
#define OFXIOS_DEVICE_IPOD_1STGEN       "iPod1,1"       // iPod touch (Original/1st Gen) - September 2007
#define OFXIOS_DEVICE_IPOD_2NDGEN       "iPod2,1"       // iPod touch (2nd Gen) - September 2008
#define OFXIOS_DEVICE_IPOD_3RDGEN       "iPod3,1"       // iPod touch (3rd Gen/32 & 64 GB) - September 2009
#define OFXIOS_DEVICE_IPOD_4THGEN       "iPod4,1"       // iPod touch (4th Gen/FaceTime) - September 2010

// iPads
#define OFXIOS_DEVICE_IPAD_1STGEN               "iPad1,1"       // iPad Wi-Fi/3G/GPS (Original/1st Gen) - January 2010


//-------------------------------------------------------------------------------

// possible values for iPhoneSetOrientation or iPhoneGetOrientation
#define OFXIOS_ORIENTATION_PORTRAIT          OF_ORIENTATION_DEFAULT  // UIDeviceOrientationPortrait
#define OFXIOS_ORIENTATION_UPSIDEDOWN        OF_ORIENTATION_180      // UIDeviceOrientationPortraitUpsideDown
#define OFXIOS_ORIENTATION_LANDSCAPE_RIGHT   OF_ORIENTATION_90_RIGHT // UIDeviceOrientationLandscapeRight
#define OFXIOS_ORIENTATION_LANDSCAPE_LEFT    OF_ORIENTATION_90_LEFT  // UIDeviceOrientationLandscapeLeft

// whether device has audio in
bool ofxiOSHasAudioIn();

// returns normalized (0...1) Average and Peak audio levels on mic
float ofxiOSGetMicAverageLevel();

// return device type
ofxiOSDeviceType ofxiOSGetDeviceType();

// return device revision
string ofxiOSGetDeviceRevision();

// return device revision and type parsd from string
ofxiOSDeviceInfo ofxiOSGetDeviceInfo();

// return application key UIWindow
UIWindow *ofxiOSGetUIWindow();

// return openglview
ofxiOSEAGLView *ofxiOSGetGLView();

// return opengl parent view
UIView * ofxiOSGetGLParentView();

// return OpenFrameworks iPhone Window
ofAppiOSWindow* ofxiOSGetOFWindow();

// return application delegate
ofxiOSAppDelegate *ofxiOSGetAppDelegate();

// return iphone view controller.
ofxiOSViewController *ofxiOSGetViewController();

// brings the OpenGL view to the front of any other UIViews
// the OpenGL view will receive touchXXXXX events, but other UIViews will not
// disable OpenGL user interaction (ofxiOSSetGLViewUserInteraction) to allow UIViews to receive touch events
void ofxiOSSendGLViewToFront();


// send OpenGL view to back of any other UIViews
// the OpenGL view will not receive touchXXX events if the other UIViews have userInteraction enabled
void ofxiOSSendGLViewToBack();


// make the background of the opengl view transparent or not
// useful to make OpenGL View transparent if it's in front of other UIViews (see ofxiOSSendGLViewToBack)
void ofxiOSSetGLViewTransparent(bool b);


// enable or disable User interaction in OpenGL View
// if disabled, touchXXXX events will not be called, but UIViews behind the OpenGL view will receive touch events
void ofxiOSSetGLViewUserInteraction(bool b);


// enable or disable the iPhone idle timer
// (screen dims and goes off after x amount of inactivity)
void ofxiOSDisableIdleTimer();
void ofxiOSEnableIdleTimer();


// lock/unlock the opengl context to allow sharing between threads
// (you'll also need to use a sharegroup)
// http://developer.apple.com/iphone/library/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/WorkingwithEAGLContexts/WorkingwithEAGLContexts.html#//apple_ref/doc/uid/TP40008793-CH103-SW12
void ofxiOSLockGLContext();
void ofxiOSUnlockGLContext();



// experimental feature.
// call this once in testApp::setup() to run the update+draw in a separate NSThread instead of NSTimer
// MUST be called before ofSetFrameRate(xx);
// disabled by default
void ofxiOSEnableLoopInThread();


OF_DEPRECATED_MSG("ofxiOSSetOrientation is deprecated, use ofSetOrientation instead.", void ofxiOSSetOrientation(ofOrientation orientation));
OF_DEPRECATED_MSG("ofxiOSGetOrientation is deprecated, use ofGetOrientation instead.", UIDeviceOrientation ofxiOSGetOrientation());


// load an image from the app bundle into a texture
// NOTE: renamed this function to something more clearer
// WAS: void iPhoneLoadImageFromBundle(NSString *filename, GLuint *spriteTexture);
bool ofxiOSBundleImageToGLTexture(NSString *filename, GLuint *spriteTexture);

// load an image from UIImage into an opengl texture
// NOTE: renamed this function to something more clearer
// WAS: void iPhoneLoadImageFromUIImage(UIImage *uiImage, GLuint *spriteTexture);
bool ofxiOSUIImageToGLTexture(UIImage *uiImage, GLuint *spriteTexture);


// create an ofImage out of a UIImage
// you do not need to allocate anything before calling this, it is done in the function
// targetWidth, targetHeight are target dimensions (UIImage is resized to this size and ofImage is created)
// .... omit targetWidth & targetHeight to use original image dimensions and not resize
// TODO: take into consideration UI image orentation
bool ofxiOSUIImageToOFImage(UIImage *uiImage, ofImage &outImage, int targetWidth = 0, int targetHeight = 0);

bool ofxiOSUIImageToOFTexture(UIImage *uiImage, ofTexture &outTexture, int targetWidth, int targetHeight);

bool ofxiOSCGImageToPixels(CGImageRef & ref, unsigned char * pixels);

// save current opengl screen to photos app
// based on code from http://www.bit-101.com/blog/?p=1861
void ofxiOSScreenGrab(id delegate);


// utility fuctions for converting strings and NSStrings back and forth
string ofxiOSNSStringToString(NSString * s);
NSString * ofxiOSStringToNSString(string s);

// It returns the path to the folder which your app has read/write access to.
string ofxiOSGetDocumentsDirectory();

// opens url in safari.
void ofxiOSLaunchBrowser(string url);

void ofxiOSSetClipboardString(string clipboardString);
string ofxiOSGetClipboardString();

//-------------------------------------------------------------------------------
// backwards compatibility < 0.8.0
#define ofxiPhone_DEVICE_IPHONE                 OFXIOS_DEVICE_IPHONE
#define ofxiPhone_DEVICE_IPODTOUCH              OFXIOS_DEVICE_IPODTOUCH
#define ofxiPhone_DEVICE_IPAD                   OFXIOS_DEVICE_IPAD
#define ofxiPhone_UNKNOWN_DEVICE                OFXIOS_DEVICE_UNKNOWN
#define ofxiPhone_DEVICE_IPHONE_2G              OFXIOS_DEVICE_IPHONE_2G
#define ofxiPhone_DEVICE_IPHONE_3G              OFXIOS_DEVICE_IPHONE_3G
#define ofxiPhone_DEVICE_IPHONE_3GS             OFXIOS_DEVICE_IPHONE_3GS
#define ofxiPhone_DEVICE_IPHONE_4               OFXIOS_DEVICE_IPHONE_4
#define ofxiPhone_DEVICE_IPOD_1STGEN            OFXIOS_DEVICE_IPOD_1STGEN
#define ofxiPhone_DEVICE_IPOD_2NDGEN            OFXIOS_DEVICE_IPOD_2NDGEN
#define ofxiPhone_DEVICE_IPOD_3RDGEN            OFXIOS_DEVICE_IPOD_3RDGEN
#define ofxiPhone_DEVICE_IPOD_4THGEN            OFXIOS_DEVICE_IPOD_4THGEN
#define ofxiPhone_DEVICE_IPAD_1STGEN            OFXIOS_DEVICE_IPAD_1STGEN

#define ofxiPhone_ORIENTATION_PORTRAIT          OFXIOS_ORIENTATION_PORTRAIT
#define ofxiPhone_ORIENTATION_UPSIDEDOWN        OFXIOS_ORIENTATION_UPSIDEDOWN
#define ofxiPhone_ORIENTATION_LANDSCAPE_RIGHT   OFXIOS_ORIENTATION_LANDSCAPE_RIGHT
#define ofxiPhone_ORIENTATION_LANDSCAPE_LEFT    OFXIOS_ORIENTATION_LANDSCAPE_LEFT

// backwards compatibility == 0.8.1
#define ofxiOS_DEVICE_IPHONE                    OFXIOS_DEVICE_IPHONE
#define ofxiOS_DEVICE_IPODTOUCH                 OFXIOS_DEVICE_IPODTOUCH
#define ofxiOS_DEVICE_IPAD                      OFXIOS_DEVICE_IPAD
#define ofxiOS_UNKNOWN_DEVICE                   OFXIOS_DEVICE_UNKNOWN
#define ofxiOS_DEVICE_IPHONE_2G                 OFXIOS_DEVICE_IPHONE_2G
#define ofxiOS_DEVICE_IPHONE_3G                 OFXIOS_DEVICE_IPHONE_3G
#define ofxiOS_DEVICE_IPHONE_3GS                OFXIOS_DEVICE_IPHONE_3GS
#define ofxiOS_DEVICE_IPHONE_4                  OFXIOS_DEVICE_IPHONE_4
#define ofxiOS_DEVICE_IPOD_1STGEN               OFXIOS_DEVICE_IPOD_1STGEN
#define ofxiOS_DEVICE_IPOD_2NDGEN               OFXIOS_DEVICE_IPOD_2NDGEN
#define ofxiOS_DEVICE_IPOD_3RDGEN               OFXIOS_DEVICE_IPOD_3RDGEN
#define ofxiOS_DEVICE_IPOD_4THGEN               OFXIOS_DEVICE_IPOD_4THGEN
#define ofxiOS_DEVICE_IPAD_1STGEN               OFXIOS_DEVICE_IPAD_1STGEN

#define ofxiOS_ORIENTATION_PORTRAIT             OFXIOS_ORIENTATION_PORTRAIT
#define ofxiOS_ORIENTATION_UPSIDEDOWN           OFXIOS_ORIENTATION_UPSIDEDOWN
#define ofxiOS_ORIENTATION_LANDSCAPE_RIGHT      OFXIOS_ORIENTATION_LANDSCAPE_RIGHT
#define ofxiOS_ORIENTATION_LANDSCAPE_LEFT       OFXIOS_ORIENTATION_LANDSCAPE_LEFT

// backwards compatibility < 0.8.0
#define ofxiPhoneHasAudioIn ofxiOSHasAudioIn
#define ofxiPhoneGetMicAverageLevel ofxiOSGetMicAverageLevel
#define ofxiPhoneGetDeviceType ofxiOSGetDeviceType
#define ofxiPhoneGetDeviceRevision ofxiOSGetDeviceRevision
#define ofxiPhoneGetUIWindow ofxiOSGetUIWindow
#define ofxiPhoneGetGLView ofxiOSGetGLView
#define ofxiPhoneGetGLParentView ofxiOSGetGLParentView
#define ofxiPhoneGetOFWindow ofxiOSGetOFWindow
#define ofxiPhoneGetAppDelegate ofxiOSGetAppDelegate
#define ofxiPhoneGetViewController ofxiOSGetViewController
#define ofxiPhoneSendGLViewToFront ofxiOSSendGLViewToFront
#define ofxiPhoneSendGLViewToBack ofxiOSSendGLViewToBack
#define ofxiPhoneSetGLViewTransparent ofxiOSSetGLViewTransparent
#define ofxiPhoneSetGLViewUserInteraction ofxiOSSetGLViewUserInteraction
#define ofxiPhoneDisableIdleTimer ofxiOSDisableIdleTimer
#define ofxiPhoneEnableIdleTimer ofxiOSEnableIdleTimer
#define ofxiPhoneLockGLContext ofxiOSLockGLContext
#define ofxiPhoneUnlockGLContext ofxiOSUnlockGLContext
#define ofxiPhoneEnableLoopInThread ofxiOSEnableLoopInThread
#define ofxiPhoneSetOrientation ofxiOSSetOrientation
#define ofxiPhoneGetOrientation ofxiOSGetOrientation
#define ofxiPhoneBundleImageToGLTexture ofxiOSBundleImageToGLTexture
#define ofxiPhoneUIImageToGLTexture ofxiOSUIImageToGLTexture
#define ofxiPhoneUIImageToOFImage ofxiOSUIImageToOFImage
#define ofxiPhoneUIImageToPixels ofxiOSUIImageToPixels
#define ofxiPhoneCGImageToPixels ofxiOSCGImageToPixels
#define ofxiPhoneScreenGrab ofxiOSScreenGrab
#define ofxiPhoneGetDocumentsDirectory ofxiOSGetDocumentsDirectory
#define ofxiPhoneLaunchBrowser ofxiOSLaunchBrowser
#define ofxNSStringToString ofxiOSNSStringToString
#define ofxStringToNSString ofxiOSStringToNSString
