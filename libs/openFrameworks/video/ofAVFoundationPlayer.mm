//
//  ofxAVFoundationvVideoPlayer.mm
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//  Modified by James George and Elie Zananiri.
//

#include "ofAVFoundationPlayer.h"
#include "Poco/String.h"

//--------------------------------------------------------------
ofAVFoundationPlayer::ofAVFoundationPlayer()
{
    moviePlayer = NULL;
	bNewFrame = false;
    bPaused = true;
	duration = 0.0f;
    speed = 1.0f;
	
    scrubToTime = 0.0;
    bInitialized = false;
    
    pixelFormat = OF_PIXELS_RGB;
    currentLoopState = OF_LOOP_NORMAL;

}

//--------------------------------------------------------------
ofAVFoundationPlayer::~ofAVFoundationPlayer()
{
	close();
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::loadMovie(string path)
{

    if (bInitialized) {
        close();
    }
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
    moviePlayer = [[AVFMovieRenderer alloc] init];
    [moviePlayer setUseAlpha:(pixelFormat == OF_PIXELS_RGBA)];
    [moviePlayer setUseTexture:YES];


	if (Poco::icompare(path.substr(0, 7), "http://")  == 0 ||
        Poco::icompare(path.substr(0, 8), "https://") == 0 ||
        Poco::icompare(path.substr(0, 7), "rtsp://")  == 0) {
        [moviePlayer loadURLPath:[NSString stringWithUTF8String:path.c_str()]];
    }
    else {
        path = ofToDataPath(path, false);
        [moviePlayer loadFilePath:[NSString stringWithUTF8String:path.c_str()]];
    }
	[pool release];
    
    bShouldPlay = false;
    return true;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::closeMovie()
{
    close();
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::close()
{
    pixels.clear();
	
    if (moviePlayer != nil) {
//		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        [moviePlayer release];
        moviePlayer = nil;
//		[pool release];

    }
    
    bInitialized = false;
    bNewFrame = false;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::idleMovie()
{
    update();
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::update()
{
    if (!moviePlayer) return;
    
    if ([moviePlayer isLoaded]) {
        if (!bInitialized) {
			reallocatePixels();
            bInitialized = true;

            if (scrubToTime != 0.0f) {
				setTime(scrubToTime);
				scrubToTime = 0.0f;
			}
            
			if (bShouldPlay){
				play();
				bShouldPlay = false;
			}
        }
        
		bNewFrame = [moviePlayer update];
        bHavePixelsChanged = bNewFrame;
    }
    else {
        ofLogNotice("ofAVFoundationPlayer::update()") << "Movie player not ready";
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::play()
{
	if (bInitialized) {
        ofLogVerbose("ofAVFoundationPlayer::play()") << "Initialized and playing at time " << getCurrentTime();
		[moviePlayer play];
	}
	else {
		bShouldPlay = true;
	}
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::stop()
{
    [moviePlayer stop];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isFrameNew()
{
    return bNewFrame;
}

//--------------------------------------------------------------
unsigned char * ofAVFoundationPlayer::getPixels()
{
	return getPixelsRef().getPixels();
}

//--------------------------------------------------------------
ofPixelsRef ofAVFoundationPlayer::getPixelsRef()
{
	if (isLoaded()) {
		// Don't get the pixels every frame if it hasn't updated
		if (bHavePixelsChanged) {
			[moviePlayer pixels:pixels.getPixels()];
			bHavePixelsChanged = false;
		}
	}
	else {
		ofLogError("ofAVFoundationPlayer::getPixelsRef()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef.";
	}
	return pixels;
}

//--------------------------------------------------------------
ofTexture* ofAVFoundationPlayer::getTexture()
{
	//TODO: Allow AVF's direct to texture
	if (moviePlayer.textureAllocated) {
		updateTexture();
	}
	return &tex;

//    return NULL;
}

//--------------------------------------------------------------
ofTexture& ofAVFoundationPlayer::getTextureReference()
{
//	getTexture();
	updateTexture();
	return tex;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoading()
{
    return moviePlayer && [moviePlayer isLoading];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoaded()
{
    return bInitialized;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPlaying()
{
    return moviePlayer && [moviePlayer isPlaying];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::getIsMovieDone()
{
    return moviePlayer.isMovieDone;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPosition()
{
    return moviePlayer.position;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getCurrentTime()
{
    return moviePlayer.currentTime;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPositionInSeconds()
{
    return getCurrentTime();
}

//--------------------------------------------------------------
int ofAVFoundationPlayer::getCurrentFrame()
{
    return moviePlayer.currentFrame;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getDuration()
{
    return moviePlayer.duration;
}

//--------------------------------------------------------------
int ofAVFoundationPlayer::getTotalNumFrames()
{
    return moviePlayer.totalFrames;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPaused()
{
    return moviePlayer && [moviePlayer isPaused];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getSpeed()
{
    if (moviePlayer) {
        return moviePlayer.playbackRate;
    }
    
    return 0;
}

//--------------------------------------------------------------
ofLoopType ofAVFoundationPlayer::getLoopState()
{
    if (moviePlayer && [moviePlayer loops])
        return OF_LOOP_NORMAL;
    
	return OF_LOOP_NONE;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getVolume()
{
    if (moviePlayer) {
        return moviePlayer.volume;
    }
    
    return 0;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPosition(float pct)
{
    [moviePlayer setPosition:pct];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setTime(float position)
{
	if (![moviePlayer isLoaded]) {
		ofLogNotice("ofAVFoundationPlayer::setCurrentTime()") << "Video player not ready, declaring to scrub to time " << scrubToTime;
		scrubToTime = position;
	}
	else {
        [moviePlayer setCurrentTime:position];
	}
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPositionInSeconds(float seconds)
{
    setTime(seconds);
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setFrame(int frame)
{
    [moviePlayer setCurrentFrame:frame];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setVolume(float volume)
{
    [moviePlayer setVolume:volume];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setBalance(float balance)
{

}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPaused(bool bPaused)
{
    [moviePlayer setPaused:bPaused];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setLoopState(ofLoopType state)
{
    if (moviePlayer) {
        [moviePlayer setLoops:(state == OF_LOOP_NORMAL)];
    }
    
    if (state == OF_LOOP_PALINDROME) {
        ofLogWarning("ofAVFoundationPlayer::setLoopState") << "No palindrome yet, sorry!";
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setSpeed(float speed)
{
    [moviePlayer setPlaybackRate:speed];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::setPixelFormat(ofPixelFormat newPixelFormat)
{
    if (newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofAVFoundationPlayer::setPixelFormat") << "Pixel format " << newPixelFormat << " is not supported.";
        return false;
    }
    
    if (newPixelFormat != pixelFormat) {
        pixelFormat = newPixelFormat;
        // If we already have a movie loaded we need to reload
        // the movie with the new settings correctly allocated.
        if (isLoaded()) {
            loadMovie(moviePath);
        }
    }
	return true;
}

//--------------------------------------------------------------
ofPixelFormat ofAVFoundationPlayer::getPixelFormat()
{
    return pixelFormat;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::draw(float x, float y)
{
    if (!bInitialized) return;
    
	draw(x, y, getWidth(), getHeight());
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::draw(float x, float y, float w, float h)
{
    if (!bInitialized) return;
    
	updateTexture();
	tex.draw(x, y, w, h);
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getWidth()
{
    return moviePlayer.width;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getHeight()
{
    return moviePlayer.height;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::firstFrame()
{
    //TODO:
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::nextFrame()
{
    //TODO:
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::previousFrame()
{
	//TODO:
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::updateTexture()
{
    if(!moviePlayer.textureAllocated) {
		return;
	}
	
	tex.setUseExternalTextureID(moviePlayer.textureID);
	ofTextureData& data = tex.getTextureData();
	data.textureTarget = moviePlayer.textureTarget;
	data.width = getWidth();
	data.height = getHeight();
	data.tex_w = getWidth();
	data.tex_h = getHeight();
	data.tex_t = getWidth();
	data.tex_u = getHeight();

}

//--------------------------------------------------------------
void ofAVFoundationPlayer::reallocatePixels()
{
    if (pixelFormat == OF_PIXELS_RGBA) {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR_ALPHA);
    }
    else {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR);
    }
}
