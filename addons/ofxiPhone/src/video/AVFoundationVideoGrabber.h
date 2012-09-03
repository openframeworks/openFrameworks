/*
 *  AVFoundationVideoGrabber.h
*
 */

#pragma once

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>

//#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_3_2

#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"

#if defined  __arm__

class AVFoundationVideoGrabber;


@interface iPhoneVideoGrabber : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate> {

	@public
	
	AVCaptureDeviceInput		*captureInput;	
	AVCaptureSession			*_captureSession;
	CGImageRef currentFrame;	
	
	int width;
	int height;
	
	AVFoundationVideoGrabber * grabberPtr;
	
	bool bInitCalled;
	int device;
}

-(bool)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h;
-(void)startCapture;
-(void)stopCapture;
-(void)lockExposureAndFocus;
-(void)listDevices;
-(void)setDevice:(int)_device;
-(void)eraseGrabberPtr;

-(CGImageRef)getCurrentFrame;

@property (nonatomic, retain) AVCaptureSession *captureSession;

@end

class AVFoundationVideoGrabber{

	public:		
		AVFoundationVideoGrabber();
		~AVFoundationVideoGrabber();
		
		void clear();
		void setCaptureRate(int capRate);
	
		bool initGrabber(int w, int h);
		void updatePixelsCB( CGImageRef & ref );
	
		void update();
	
		bool isFrameNew();
		
		void listDevices();
		void setDevice(int deviceID);
		bool setPixelFormat(ofPixelFormat PixelFormat);
		ofPixelFormat getPixelFormat();
	
		unsigned char * getPixels(){
			return pixels;
		}
		float getWidth(){
			return width;
		}
		float getHeight(){
			return height;
		}
	
		GLint internalGlDataType;
		unsigned char * pixels;
		bool newFrame;
	
		int width, height;
	
	protected:
		
		
		int device;
	
		bool bHavePixelsChanged;
		
		int fps;
		ofTexture tex;
		iPhoneVideoGrabber * grabber;
		GLubyte *pixelsTmp;
};


#endif	// (__arm__) compile only for ARM

//#else   // (> 3.2 SDK) compile for 4.0+
//
//#warning "skipping AVFoundationVideoGrabber compilation because you need > 3.2 iOS SDK"
//
//#endif
