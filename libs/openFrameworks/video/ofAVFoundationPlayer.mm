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
bool ofAVFoundationPlayer::load(string path)
{
    if (bInitialized) {
        close();
    }
    
    @autoreleasepool {
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
    }
    
    bShouldPlay = false;
    return true;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::close()
{
    pixels.clear();
    
    if (moviePlayer != nil) {
        [moviePlayer release];
        moviePlayer = nil;
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
        
        // Don't get the pixels every frame if it hasn't updated
        if (bHavePixelsChanged) {
            [moviePlayer pixels:pixels.getData()];
            bHavePixelsChanged = false;
        }
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::play()
{
    if (bInitialized) {
        ofLogVerbose("ofAVFoundationPlayer::play") << "Initialized and playing at time " << getCurrentTime();
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
bool ofAVFoundationPlayer::isFrameNew() const
{
    return bNewFrame;
}

//--------------------------------------------------------------
const ofPixels & ofAVFoundationPlayer::getPixels() const
{
    if (!isLoaded() ){
        ofLogError("ofAVFoundationPlayer::getPixels()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixels.";
    }
    return pixels;
}

//--------------------------------------------------------------
ofPixels & ofAVFoundationPlayer::getPixels()
{
    if (!isLoaded() ){
        ofLogError("ofAVFoundationPlayer::getPixels()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixels.";
    }
    return pixels;
}


//--------------------------------------------------------------
ofTexture * ofAVFoundationPlayer::getTexturePtr()
{
    //TODO: Allow AVF's direct to texture
    if (moviePlayer.textureAllocated) {
        updateTexture();
    }
    return &tex;
}

//--------------------------------------------------------------
ofTexture & ofAVFoundationPlayer::getTextureReference()
{
    updateTexture();
    return tex;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoading() const
{
    return moviePlayer && [moviePlayer isLoading];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoaded() const
{
    return bInitialized;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPlaying() const
{
    return moviePlayer && [moviePlayer isPlaying];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::getIsMovieDone() const
{
    return moviePlayer.isMovieDone;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPosition() const
{
    return moviePlayer.position;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getCurrentTime() const
{
    return moviePlayer.currentTime;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPositionInSeconds() const
{
    return getCurrentTime();
}

//--------------------------------------------------------------
int ofAVFoundationPlayer::getCurrentFrame() const
{
    return moviePlayer.currentFrame;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getDuration() const
{
    return moviePlayer.duration;
}

//--------------------------------------------------------------
int ofAVFoundationPlayer::getTotalNumFrames() const
{
    return moviePlayer.totalFrames;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPaused() const
{
    return moviePlayer && [moviePlayer isPaused];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getSpeed() const
{
    if (moviePlayer) {
        return moviePlayer.playbackRate;
    }
    
    return 0;
}

//--------------------------------------------------------------
ofLoopType ofAVFoundationPlayer::getLoopState() const
{
    if (moviePlayer && [moviePlayer loops])
        return OF_LOOP_NORMAL;
    
    return OF_LOOP_NONE;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getVolume() const
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
        ofLogNotice("ofAVFoundationPlayer::setCurrentTime") << "Video player not ready, declaring to scrub to time " << scrubToTime;
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
    ofLogWarning("ofAVFoundationPlayer::setBalance") << "Not implemented!";
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
            load(moviePath);
        }
    }
    return true;
}

//--------------------------------------------------------------
ofPixelFormat ofAVFoundationPlayer::getPixelFormat() const
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
float ofAVFoundationPlayer::getWidth() const
{
    return moviePlayer.width;
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getHeight() const
{
    return moviePlayer.height;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::firstFrame()
{
    if (moviePlayer == NULL) {
        return;
    }
    
    [moviePlayer setPosition:0];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::nextFrame()
{
    int nextFrameNum = ofClamp(getCurrentFrame() + 1, 0, getTotalNumFrames());
    setFrame(nextFrameNum);
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::previousFrame()
{
    int prevFrameNum = ofClamp(getCurrentFrame() - 1, 0, getTotalNumFrames());
    setFrame(prevFrameNum);
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::updateTexture()
{
    if (!moviePlayer.textureAllocated) {
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
