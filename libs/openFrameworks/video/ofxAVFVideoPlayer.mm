//
//  ofxAVFoundationvVideoPlayer.mm
//  AVFoundationTest
//
//  Created by Sam Kronick on 5/31/13.
//
//

#include "ofxAVFVideoPlayer.h"
#include "Poco/String.h"

//--------------------------------------------------------------
ofxAVFVideoPlayer::ofxAVFVideoPlayer()
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
    
    bTheFutureIsNow = false;
}

//--------------------------------------------------------------
ofxAVFVideoPlayer::~ofxAVFVideoPlayer()
{
	close();
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::loadMovie(string path)
{

    if (bInitialized) {
        close();
    }
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
    moviePlayer = [[AVFVideoRenderer alloc] init];
    [moviePlayer setUseAlpha:(pixelFormat == OF_PIXELS_RGBA)];
    [moviePlayer setUseTexture:YES];
    
    bTheFutureIsNow = moviePlayer.theFutureIsNow;

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
void ofxAVFVideoPlayer::closeMovie()
{
    close();
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::close()
{
    pixels.clear();
	
    if (moviePlayer != nil) {
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        [moviePlayer autorelease];
        moviePlayer = nil;
		[pool release];

    }
    
    bInitialized = false;
    bNewFrame = false;
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::idleMovie()
{
    update();
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::update()
{
    if (!moviePlayer) return;
    
    if ([moviePlayer isLoaded]) {
        if (!bInitialized) {
            if (bTheFutureIsNow) {
                reallocatePixels();
            }
            else {
                fbo.allocate(moviePlayer.width, moviePlayer.height);
            }
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
        
        if (bTheFutureIsNow) {
            bNewFrame = [moviePlayer update];
        }
        else {
            // Render movie into FBO so we can get a texture
            fbo.begin();
            [moviePlayer render];
            fbo.end();
            bNewFrame = true;
        }
        bHavePixelsChanged = bNewFrame;
    }
    else {
        ofLogNotice("ofxAVFVideoPlayer::update()") << "Movie player not ready";
    }
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::play()
{
	if (bInitialized) {
        ofLogVerbose("ofxAVFVideoPlayer::play()") << "Initialized and playing at time " << getCurrentTime();
		[moviePlayer play];
	}
	else {
		bShouldPlay = true;
	}
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::stop()
{
    [moviePlayer stop];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isFrameNew()
{
    return bNewFrame;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getAmplitude(int channel)
{
    return getAmplitudeAt(getPosition(), channel);
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getAmplitudeAt(float pos, int channel)
{
    if (bTheFutureIsNow == false) return 0;
    
    pos = ofClamp(pos, 0, 1);
    channel = ofClamp(channel, 0, 1);
    
    if (!moviePlayer || ![moviePlayer isAudioLoaded] || [moviePlayer numAmplitudes] == 0 || !bInitialized) {
        return 0;
    }
    
    int idx = (int)(pos * ([moviePlayer numAmplitudes] - 2));
    
    // Make sure the index is pointing at the right channel
    // EZ: I know this is ghetto, but it works...
    if (idx % 2 == 0 && channel == 1) {
        ++idx;
    }
    else if (idx % 2 == 1 && channel == 0) {
        --idx;
    }

    float amp;
    [moviePlayer.amplitudes getBytes:&amp range:NSMakeRange(idx * sizeof(float), sizeof(float))];
    return amp;
}

//--------------------------------------------------------------
int ofxAVFVideoPlayer::getNumAmplitudes()
{
    return [moviePlayer numAmplitudes];
}

//--------------------------------------------------------------
float * ofxAVFVideoPlayer::getAllAmplitudes()
{
    if (bTheFutureIsNow == false) return NULL;
    
    return (float *)[moviePlayer.amplitudes bytes];
}

//--------------------------------------------------------------
unsigned char * ofxAVFVideoPlayer::getPixels()
{
    if (bTheFutureIsNow) {
        return getPixelsRef().getPixels();
    }
    
    if (!moviePlayer || ![moviePlayer isLoaded] || !bInitialized) return NULL;
    
    if (bHavePixelsChanged) {
        fbo.readToPixels(pixels);
        bHavePixelsChanged = false; // Don't read pixels until next update() is called
    }
    return pixels.getPixels();
}

//--------------------------------------------------------------
ofPixelsRef ofxAVFVideoPlayer::getPixelsRef()
{
    if (bTheFutureIsNow) {
        if (isLoaded()) {
            // Don't get the pixels every frame if it hasn't updated
            if (bHavePixelsChanged) {
                [moviePlayer pixels:pixels.getPixels()];
                bHavePixelsChanged = false;
            }
        }
        else {
            ofLogError("ofxAVFVideoPlayer::getPixelsRef()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef.";
        }
    }
    else {
        getPixels();
    }
    
	return pixels;
}

//--------------------------------------------------------------
ofTexture* ofxAVFVideoPlayer::getTexture()
{
    if (bTheFutureIsNow) {
        if (moviePlayer.textureAllocated) {
            updateTexture();
            return &tex;
        }

        return NULL;
    }
    
    if (!moviePlayer || ![moviePlayer isLoaded] || !bInitialized) return NULL;
    
    return &fbo.getTextureReference();
}

//--------------------------------------------------------------
ofTexture& ofxAVFVideoPlayer::getTextureReference()
{
    if (bTheFutureIsNow) {
        getTexture();
        return tex;
    }

//    if (!moviePlayer || ![moviePlayer isLoaded] || !bInitialized) return;
    
    return fbo.getTextureReference();
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isLoading()
{
    return moviePlayer && [moviePlayer isLoading];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isLoaded()
{
    return bInitialized;
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isAudioLoaded()
{
    return moviePlayer && [moviePlayer isAudioLoaded];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::errorLoading()
{
    if (!moviePlayer) return false;
    
    // Error if movie player is not loading and is not ready.
    return ![moviePlayer isLoading] && ![moviePlayer isLoaded];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isPlaying()
{
    return moviePlayer && [moviePlayer isPlaying];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::getIsMovieDone()
{
    return moviePlayer.isMovieDone;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getPosition()
{
    return moviePlayer.position;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getCurrentTime()
{
    return moviePlayer.currentTime;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getPositionInSeconds()
{
    return getCurrentTime();
}

//--------------------------------------------------------------
int ofxAVFVideoPlayer::getCurrentFrame()
{
    return moviePlayer.currentFrame;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getDuration()
{
    return moviePlayer.duration;
}

//--------------------------------------------------------------
int ofxAVFVideoPlayer::getTotalNumFrames()
{
    return moviePlayer.totalFrames;
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::isPaused()
{
    return moviePlayer && [moviePlayer isPaused];
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getSpeed()
{
    if (moviePlayer) {
        return moviePlayer.playbackRate;
    }
    
    return 0;
}

//--------------------------------------------------------------
ofLoopType ofxAVFVideoPlayer::getLoopState()
{
    if (moviePlayer && [moviePlayer loops])
        return OF_LOOP_NORMAL;
    
	return OF_LOOP_NONE;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getVolume()
{
    if (moviePlayer) {
        return moviePlayer.volume;
    }
    
    return 0;
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setPosition(float pct)
{
    [moviePlayer setPosition:pct];
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setTime(float position)
{
	if (![moviePlayer isLoaded]) {
		ofLogNotice("ofxAVFVideoPlayer::setCurrentTime()") << "Video player not ready, declaring to scrub to time " << scrubToTime;
		scrubToTime = position;
	}
	else {
        [moviePlayer setCurrentTime:position];
	}
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setPositionInSeconds(float seconds)
{
    setTime(seconds);
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setFrame(int frame)
{
    [moviePlayer setCurrentFrame:frame];
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setVolume(float volume)
{
    [moviePlayer setVolume:volume];
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setBalance(float balance)
{

}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setPaused(bool bPaused)
{
    [moviePlayer setPaused:bPaused];
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setLoopState(ofLoopType state)
{
    if (moviePlayer) {
        [moviePlayer setLoops:(state == OF_LOOP_NORMAL)];
    }
    
    if (state == OF_LOOP_PALINDROME) {
        ofLogWarning("ofxAVFVideoPlayer::setLoopState") << "No palindrome yet, sorry!";
    }
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::setSpeed(float speed)
{
    [moviePlayer setPlaybackRate:speed];
}

//--------------------------------------------------------------
bool ofxAVFVideoPlayer::setPixelFormat(ofPixelFormat newPixelFormat)
{
    if (newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofxAVFVideoPlayer::setPixelFormat") << "Pixel format " << newPixelFormat << " is not supported.";
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
ofPixelFormat ofxAVFVideoPlayer::getPixelFormat()
{
    return pixelFormat;
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::draw(float x, float y)
{
    if (!bInitialized) return;
    
    if (bTheFutureIsNow) {
        draw(x, y, getWidth(), getHeight());
    }
    else {
        fbo.draw(x, y);
    }
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::draw(float x, float y, float w, float h)
{
    if (!bInitialized) return;
    
    if (bTheFutureIsNow) {
        updateTexture();
        tex.draw(x, y, w, h);
    }
    else {
        fbo.draw(x, y, w, h);
    }
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getWidth()
{
    return moviePlayer.width;
}

//--------------------------------------------------------------
float ofxAVFVideoPlayer::getHeight()
{
    return moviePlayer.height;
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::firstFrame()
{
    
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::nextFrame()
{
    
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::previousFrame()
{
    
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::updateTexture()
{
    if (bTheFutureIsNow == false) return;
    
    if (moviePlayer.textureAllocated) {
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
}

//--------------------------------------------------------------
void ofxAVFVideoPlayer::reallocatePixels()
{
    if (bTheFutureIsNow == false) return;
    
    if (pixelFormat == OF_PIXELS_RGBA) {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR_ALPHA);
    }
    else {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR);
    }
}
