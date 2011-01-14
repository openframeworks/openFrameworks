/*
 *  ofxofiPhoneVideoPlayer.cpp
 *  ofMoviePlayeriPhoneExample
 *
 *  Created by Zach Gage on 1/13/11.
 *  Copyright 2011 stfj. All rights reserved.
 *
 */
#import "AVFoundationVideoGrabber.h"
#import "ofiPhoneVideoPlayer.h"
#import "ofxiPhoneExtras.h"

AVFoundationVideoGrabber * videoPlayer;
CGImageRef currentFrameRef;

ofiPhoneVideoPlayer::ofiPhoneVideoPlayer() {
	videoPlayer=NULL;
	videoWasStopped=false;
	
	width = 0;
	height = 0;
}

//----------------------------------------

ofiPhoneVideoPlayer::~ofiPhoneVideoPlayer() {
	close();
}

//----------------------------------------

bool ofiPhoneVideoPlayer::loadMovie(string name) {
	
	if(videoPlayer != NULL)
		close();
	
	videoPath = name;
	initWithPath(videoPath);
	
	if(videoPlayer != NULL)
		if(! [videoPlayer isInErrorState])
			return true;
	return false;
}

//----------------------------------------

void ofiPhoneVideoPlayer::close() {
	if(videoPlayer != NULL)
		[videoPlayer release];
	videoPlayer = NULL;
}

//----------------------------------------

void ofiPhoneVideoPlayer::play() {
	
	lastUpdateTime=ofGetElapsedTimef();
	
	if(videoPlayer != NULL)
		[videoPlayer play];
	else if(videoWasStopped || getIsMovieDone()) {
		[videoPlayer release];
		initWithPath(videoPath);
		play();
	}
	else
		cerr<<"video is not loaded - play"<<endl;
}

//----------------------------------------

void ofiPhoneVideoPlayer::stop() {
	if(videoPlayer != NULL) {
		[videoPlayer pause];
		close();
		videoWasStopped=true;
	}
	cerr<<"video is not loaded - stop"<<endl;
}		

//----------------------------------------

bool ofiPhoneVideoPlayer::isFrameNew() {
	if(videoPlayer != NULL) {
		return [videoPlayer hasNewFrame];
	}	
	return false;
}

//----------------------------------------

unsigned char * ofiPhoneVideoPlayer::getPixels() {
	if(videoPlayer != NULL)
	{
		updateCurrentFrameRef();
		ofxiPhoneCGImageToPixels(currentFrameRef, pixels);
		return pixels;
	}
	
	return NULL;
}

//----------------------------------------

float ofiPhoneVideoPlayer::getWidth() {
	if(videoPlayer != NULL)
		return width;
	
	cerr<<"video is not loaded - getWidth"<<endl;
	return 0;
}

//----------------------------------------

float ofiPhoneVideoPlayer::getHeight() {
	if(videoPlayer != NULL)
		return height;
	
	cerr<<"video is not loaded - getHeight"<<endl;
	return 0;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isPaused() {
	if(videoPlayer != NULL)
		return [videoPlayer isPaused];
	
	cerr<<"video is not loaded - isPaused"<<endl;
	return false;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isLoaded() {
	if(videoPlayer != NULL && ! [videoPlayer isInErrorState])
		return true;
	else
		return false;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isPlaying() {
	if(videoPlayer != NULL) {
		if([videoPlayer isFinished] || [videoPlayer isPaused] || [videoPlayer isInErrorState])
			return false;
		else
			return true;
	}
	
	return false;
}

void ofiPhoneVideoPlayer::update() {
	if(videoPlayer != NULL) {
		float t = ofGetElapsedTimef();
		[videoPlayer updateWithElapsedTime:t-lastUpdateTime];
		lastUpdateTime=t;
	}
}

float ofiPhoneVideoPlayer::getPosition() {
	if(videoPlayer != NULL)
		return [videoPlayer getVideoPosition];
	else
	return 0;
}

bool ofiPhoneVideoPlayer::getIsMovieDone() {
	if(videoPlayer != NULL)
		return [videoPlayer isFinished];
	else
	return true;
}

//protected ------------------------------

void ofiPhoneVideoPlayer::updateCurrentFrameRef() {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    CVImageBufferRef imageBuffer = [videoPlayer getCurrentFrame]; 
    /*Lock the image buffer*/
    CVPixelBufferLockBaseAddress(imageBuffer,0); 
	
    /*Get information about the image*/
    uint8_t *baseAddress	= (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer); 
    size_t bytesPerRow		= CVPixelBufferGetBytesPerRow(imageBuffer); 
    size_t widthIn			= CVPixelBufferGetWidth(imageBuffer); 
    size_t heightIn			= CVPixelBufferGetHeight(imageBuffer);  
	
    /*Create a CGImageRef from the CVImageBufferRef*/
    CGColorSpaceRef colorSpace	= CGColorSpaceCreateDeviceRGB(); 
    CGContextRef newContext		= CGBitmapContextCreate(baseAddress, widthIn, heightIn, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    CGImageRef newImage			= CGBitmapContextCreateImage(newContext); 
	
	CGImageRelease(currentFrameRef);	
	currentFrameRef = CGImageCreateCopy(newImage);		
	
    /*We release some components*/
    CGContextRelease(newContext); 
    CGColorSpaceRelease(colorSpace);
	
	/*We relase the CGImageRef*/
	CGImageRelease(newImage);
	
	/*We unlock the  image buffer*/
	CVPixelBufferUnlockBaseAddress(imageBuffer,0);
	
	if(width==0 && widthIn != 0)
		pixels = new unsigned char[widthIn*heightIn*(bytesPerRow/widthIn)];
		
	width = widthIn;
	height = heightIn;
	[pool drain];
}

//----------------------------------------

void ofiPhoneVideoPlayer::initWithPath(string path) {
	videoPlayer = [[AVFoundationVideoGrabber alloc] initWithPath:ofxStringToNSString(ofToDataPath(path))];
	videoWasStopped=false;
}