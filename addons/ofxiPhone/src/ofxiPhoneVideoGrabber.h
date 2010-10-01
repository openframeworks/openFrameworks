/*
 *  ofxiPhoneVideoGrabber.h
*
 */

#pragma once

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <CoreVideo/CoreVideo.h>
#import <CoreMedia/CoreMedia.h>

#include "ofxiPhone.h"

class ofxiPhoneVideoGrabber;

@interface iPhoneVideoGrabber : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate> {

	@public
	
	AVCaptureDeviceInput		*captureInput;	
	AVCaptureSession			*_captureSession;
	CGImageRef currentFrame;	
	
	int width;
	int height;
	
	ofxiPhoneVideoGrabber * grabberPtr;
	
	bool bInitCalled;
}

-(void)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h;
-(void)startCapture;
-(void)stopCapture;
-(void)lockExposureAndFocus;

-(CGImageRef)getCurrentFrame;

@property (nonatomic, retain) AVCaptureSession *captureSession;

@end

class ofxiPhoneVideoGrabber : public ofBaseHasPixels, ofBaseDraws{

	public:		
		ofxiPhoneVideoGrabber();
		~ofxiPhoneVideoGrabber();
		
		void clear();
		void setCaptureRate(int capRate);
	
		void initGrabber(int w, int h);
		void updatePixelsCB( CGImageRef & ref );		
		void draw(float x, float y);
		void draw(float x, float y, float w, float h);
	
		unsigned char * getPixels(){
			return pixels;
		}
		float getWidth(){
			return width;
		}
		float getHeight(){
			return height;
		}
	
	protected:
		
		bool convertCGImageToPixels(CGImageRef & ref, unsigned char * pixels);
		int width, height;
	
		bool bUpdateTex;
		int fps;
		ofTexture tex;
		unsigned char * pixels;
		iPhoneVideoGrabber * grabber;
};