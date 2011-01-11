/*
 *  ofxQTKitMoviePlayer
 *
 **********************************************************
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 * 
 * ofxQTKitVideoPlayer provides QTKit/CoreVideo accelerated movie playback
 * for openFrameworks on OS X
 * 
 * This class replaces almost all of the functionality of ofVideoPlayer on OS X
 * and uses the modern QTKit and CoreVideo libraries for playback
 *
 * adapted by James George
 * http://www.jamesgeorge.org
 *
 * Special Thanks to Marek Bereza for his initial QTKitVideoTexture
 * http://www.mrkbrz.com/
 *
 * Thanks to Anton Marini for help wrangling buffer contexts
 * http://vade.info/ 
 */

#include "ofxQTKitVideoPlayer.h"

ofxQTKitVideoPlayer::ofxQTKitVideoPlayer(){
	moviePlayer = NULL;
	moviePixels = NULL;
	
	bPlaying = false;
	bPaused = false;
		
	type = OF_IMAGE_COLOR;
	
	bHavePixelsChanged = false;
	bNewFrame = false;
	duration = 0;
	nFrames = 0;
	
}


ofxQTKitVideoPlayer::~ofxQTKitVideoPlayer(){
	clearMemory();	
}

bool ofxQTKitVideoPlayer::loadMovie(string path){
	loadMovie(path, OFXQTVIDEOPLAYER_MODE_PIXELS_ONLY);
}

bool ofxQTKitVideoPlayer::loadMovie(string movieFilePath, int mode){
	if(mode < 0 || mode > 2){
		ofLog(OF_LOG_ERROR, "ofxQTKitVideoPlayer -- Error, invalid mode specified for");
		return false;
	}
	
	if(moviePlayer != NULL){
		clearMemory();
	}
	
	setType(OF_IMAGE_COLOR);

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	bool useTexture = (mode == OFXQTVIDEOPLAYER_MODE_TEXTURE_ONLY || OFXQTVIDEOPLAYER_MODE_PIXELS_AND_TEXTURE);
	bool usePixels  = (mode == OFXQTVIDEOPLAYER_MODE_PIXELS_ONLY  || OFXQTVIDEOPLAYER_MODE_PIXELS_AND_TEXTURE);
	
	moviePlayer = [[QTKitMovieRenderer alloc] init];
	
	movieFilePath = ofToDataPath(movieFilePath, false);
	BOOL success = [moviePlayer loadMovie:[NSString stringWithCString:movieFilePath.c_str() encoding:NSUTF8StringEncoding] 
							 allowTexture:useTexture 
							  allowPixels:usePixels];

	if(success){
		duration = moviePlayer.duration;
		nFrames = moviePlayer.frameCount;
		width = moviePlayer.movieSize.width;
		height = moviePlayer.movieSize.height;
	}
	else {
		ofLog(OF_LOG_ERROR, "ofxQTKitVideoPlayer -- Loading file " + movieFilePath + " failed");
		[moviePlayer release];
		moviePlayer = NULL;
	}
	
	//allows opt into rgba format
	moviePlayer.useRGBAFormat = (type == OF_IMAGE_COLOR_ALPHA);
	
	[pool release];
	
	return success;
}

void ofxQTKitVideoPlayer::closeMovie(){
	clearMemory();
}

bool ofxQTKitVideoPlayer::isLoaded(){
	return moviePlayer != NULL;
}

void ofxQTKitVideoPlayer::clearMemory(){
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	if(moviePlayer != NULL){
		[moviePlayer release];
		moviePlayer = NULL;
	}
	
	if(moviePixels != NULL){
		delete moviePixels;
		moviePixels = NULL;
	}
	
	duration = 0;
	nFrames = 0;
	
	bPlaying = false;
	bPaused = false;
	bNewFrame = false;
	bHavePixelsChanged = false;
	
	[pool release];	
}

void ofxQTKitVideoPlayer::stop(){
	if(moviePlayer == NULL) return;
	
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	[moviePlayer stop];
	
	bPlaying = false;
	bPaused  = false;
	
	[pool release];	
	
	
}

void ofxQTKitVideoPlayer::setPaused(bool bPause){
	if(moviePlayer == NULL) return;
	if(!bPlaying) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	if(bPause){
		[moviePlayer setRate:0];
	}
	else {
		[moviePlayer setRate: currentSpeed];
	}

	[pool release];
	
	bPaused = bPause;		
}

bool ofxQTKitVideoPlayer::isPlaying(){
	return bPlaying;
}

bool ofxQTKitVideoPlayer::isPaused(){
	return bPaused;
}

void ofxQTKitVideoPlayer::setSpeed(float speed){
	if(moviePlayer == NULL) return;
	
	if(!bPlaying) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	currentSpeed = speed;
	[moviePlayer setRate:speed];

	[pool release];	
}

void ofxQTKitVideoPlayer::play(){	
	if(moviePlayer == NULL) return;

	bPlaying = true;
	bPaused = false;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	currentSpeed = 1.0;
	[moviePlayer setRate: 1.0];
	
	[pool release];
}

void ofxQTKitVideoPlayer::idleMovie(){
	if(moviePlayer == NULL) return;
	
	bNewFrame = [moviePlayer update];
	if (bNewFrame) {
		bHavePixelsChanged = true;
	}
}

bool ofxQTKitVideoPlayer::isFrameNew(){
	return bNewFrame;
}
		
void ofxQTKitVideoPlayer::bind(){
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;
	
	[moviePlayer bindTexture];	
}

void ofxQTKitVideoPlayer::unbind(){
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;
	
	[moviePlayer unbindTexture];
}

void ofxQTKitVideoPlayer::draw(float x, float y){
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;
	
	draw(x,y, moviePlayer.movieSize.width, moviePlayer.movieSize.height);
}

void ofxQTKitVideoPlayer::draw(float x, float y, float w, float h){
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;
	
	[moviePlayer draw:NSMakeRect(x, y, w, h)];
}


void ofxQTKitVideoPlayer::setType(int imageType){
	if(type != OF_IMAGE_COLOR || type != OF_IMAGE_COLOR_ALPHA){
		ofLog(OF_LOG_ERROR, "ofxQTKitVideoPlayer - does not support given type, use OF_IMAGE_COLOR or OF_IMAGE_COLOR_ALPHA"); 
		return;
	}
	
	if(type != imageType && moviePixels != NULL){
		delete moviePixels;
	}
	type = imageType;
	
	if(moviePlayer != NULL){
		moviePlayer.useRGBAFormat = (type == OF_IMAGE_COLOR_ALPHA);
	}
	
}

unsigned char* ofxQTKitVideoPlayer::getPixels(){
	if(moviePlayer == NULL || !moviePlayer.usePixels) {
		return NULL;
	}
		
	if(moviePixels == NULL){
		if(type == OF_IMAGE_COLOR){
			moviePixels = new unsigned char[int(moviePlayer.movieSize.width) * int(moviePlayer.movieSize.height) * 3];		
		}
		else if(type == OF_IMAGE_COLOR_ALPHA){
			moviePixels = new unsigned char[int(moviePlayer.movieSize.width) * int(moviePlayer.movieSize.height) * 4];
		}
	}
		
	//don't get the pixels every frame if it hasn't updated
	if(bHavePixelsChanged){
		[moviePlayer pixels:moviePixels];
		bHavePixelsChanged = false;
	}
	
	return moviePixels;
}

void ofxQTKitVideoPlayer::setPosition(float pct){
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.position = pct;
	
	[pool release];
}

void ofxQTKitVideoPlayer::setVolume(int volume){
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.volume = volume;
	
	[pool release];
}

void ofxQTKitVideoPlayer::setFrame(int frame){
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.frame = frame % moviePlayer.frameCount;
	
	[pool release];
	
}

int ofxQTKitVideoPlayer::getCurrentFrame(){
	if(moviePlayer == NULL) return 0;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	int currentFrame = moviePlayer.frame;
	
	[pool release];	
	
	return currentFrame;	
}

int ofxQTKitVideoPlayer::getTotalNumFrames(){
	return nFrames;
}

void ofxQTKitVideoPlayer::setLoops(bool loops){
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.loops = loops;
	
	[pool release];
}

void ofxQTKitVideoPlayer::setLoopState(int state){
	if(state == OF_LOOP_NONE){
		setLoopState(false);
	}
	else if(state == OF_LOOP_NORMAL){
		setLoopState(true);
	}
	else if(state == OF_LOOP_PALINDROME){
		if(moviePlayer == NULL) return;
		
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		
		[moviePlayer loopsBackAndForth];
		
		[pool release];
	}
	
}

bool ofxQTKitVideoPlayer::getMovieLoops(){
	
	if(moviePlayer == NULL) return false;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	bool loops = moviePlayer.loops;
	
	[pool release];
	
	return loops;
}

float ofxQTKitVideoPlayer::getSpeed(){
	if(moviePlayer == NULL) return 0;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	float rate = moviePlayer.rate;
	
	[pool release];
	
	return rate;
}

float ofxQTKitVideoPlayer::getDuration(){
	return duration;
}

float ofxQTKitVideoPlayer::getPositionInSeconds(){
	return getPosition() * duration;
}

float ofxQTKitVideoPlayer::getPosition(){
	if(moviePlayer == NULL) return 0;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	float pos = moviePlayer.position;
	
	[pool release];
	
	return pos;
}

bool ofxQTKitVideoPlayer::getIsMovieDone(){
	if(moviePlayer == NULL) return false;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];	

	bool isDone = moviePlayer.isFinished;
	
	[pool release];
	
	return isDone;
}

float ofxQTKitVideoPlayer::getWidth(){
	return moviePlayer.movieSize.width;
}

float ofxQTKitVideoPlayer::getHeight(){
	return moviePlayer.movieSize.height;
}

