#if !defined(MAC_OS_X_VERSION_10_12) || MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_12
#ifdef OF_VIDEO_PLAYER_QTKIT

#include "ofQTKitPlayer.h"

using namespace std;

//--------------------------------------------------------------------
ofQTKitPlayer::ofQTKitPlayer() {
	moviePlayer = NULL;
	bNewFrame = false;
    bPaused = true;
	duration = 0.0f;
    speed = 1.0f;
	// default this to true so the player update behavior matches ofQuicktimePlayer
	bSynchronousSeek = true;

    pixelFormat = OF_PIXELS_RGB;
    currentLoopState = OF_LOOP_NORMAL;
}

//--------------------------------------------------------------------
ofQTKitPlayer::~ofQTKitPlayer() {
	close();
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::load(string path){
	return load(path, OF_QTKIT_DECODE_PIXELS_ONLY);
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::load(string movieFilePath, ofQTKitDecodeMode mode) {
	if(mode != OF_QTKIT_DECODE_PIXELS_ONLY && mode != OF_QTKIT_DECODE_TEXTURE_ONLY && mode != OF_QTKIT_DECODE_PIXELS_AND_TEXTURE){
		ofLogError("ofQTKitPlayer") << "loadMovie(): unknown ofQTKitDecodeMode mode";
		return false;
	}

	if(isLoaded()){
		close(); //auto released
	}

	BOOL success = NO;
	@autoreleasepool {
		decodeMode = mode;
		bool useTexture = (mode == OF_QTKIT_DECODE_TEXTURE_ONLY || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
		bool usePixels  = (mode == OF_QTKIT_DECODE_PIXELS_ONLY  || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
		bool useAlpha = (pixelFormat == OF_PIXELS_RGBA);

		bool isURL = false;

		if (movieFilePath.substr(0,7) == "http://" ||
			movieFilePath.substr(0,8) == "https://" ||
			movieFilePath.substr(0,7) == "rtsp://") {
			isURL = true;
		}
		else {
			movieFilePath = ofToDataPath(movieFilePath, false);
		}

		moviePlayer = [[QTKitMovieRenderer alloc] init];
		success = [moviePlayer loadMovie:[NSString stringWithCString:movieFilePath.c_str() encoding:NSUTF8StringEncoding]
                               pathIsURL:isURL
                            allowTexture:useTexture
                             allowPixels:usePixels
                              allowAlpha:useAlpha];

		if(success){
			moviePlayer.synchronousSeek = bSynchronousSeek;
			reallocatePixels();
			moviePath = movieFilePath;
			duration = moviePlayer.duration;

			setLoopState(currentLoopState);
			setSpeed(1.0f);
			firstFrame(); //will load the first frame
		}
		else {
			ofLogError("ofQTKitPlayer") << "loadMovie(): couldn't load \"" << movieFilePath << "\"";
			[moviePlayer release];
			moviePlayer = NULL;
		}
	}

	return success;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::closeMovie() {
	close();
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isLoaded() const {
	return moviePlayer != NULL;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::close() {

	if(isLoaded()){
		@autoreleasepool {
			[moviePlayer release];
			moviePlayer = NULL;
		}
	}

	pixels.clear();
	duration = 0;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setPaused(bool _bPaused){
	if(!isLoaded()) return;

	bPaused = _bPaused;

	@autoreleasepool {
		if (bPaused) {
			[moviePlayer setRate:0.0f];
		} else {
			[moviePlayer setRate:speed];
		}
	}
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isPaused() const {
	return bPaused;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::stop() {
	setPaused(true);
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isPlaying() const {
    if(!isLoaded()) return false;

	return !moviePlayer.isFinished && !isPaused();
}

//--------------------------------------------------------------------
void ofQTKitPlayer::firstFrame(){
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer gotoBeginning];
		bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::nextFrame(){
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer stepForward];
		bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::previousFrame(){
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer stepBackward];
		bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setSpeed(float rate){
	if(!isLoaded()) return;

	speed = rate;

	if(isPlaying()) {
		@autoreleasepool {
			[moviePlayer setRate:rate];
		}
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::play(){
	if(!isLoaded()) return;

	bPaused = false;

	@autoreleasepool {
		[moviePlayer setRate:speed];
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::idleMovie() {
	update();
}

//--------------------------------------------------------------------
void ofQTKitPlayer::update() {
	if(!isLoaded()) return;

	@autoreleasepool {
		bNewFrame = [moviePlayer update];
		if (bNewFrame) {
			bHavePixelsChanged = true;
		}
	}
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isFrameNew() const {
	return bNewFrame;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::draw(float x, float y) {
	draw(x,y, moviePlayer.movieSize.width, moviePlayer.movieSize.height);
}

//--------------------------------------------------------------------
void ofQTKitPlayer::draw(float x, float y, float w, float h) {
	updateTexture();
	tex.draw(x,y,w,h);
}

//--------------------------------------------------------------------
ofPixels& ofQTKitPlayer::getPixels(){
	@autoreleasepool {
		if(isLoaded() && moviePlayer.usePixels) {
			//don't get the pixels every frame if it hasn't updated
			if(bHavePixelsChanged){
				[moviePlayer pixels:pixels.getPixels()];
				bHavePixelsChanged = false;
			}
		}
		else{
			ofLogError("ofQTKitPlayer") << "getPixels(): returning pixels that may be unallocated, make sure to initialize the video player before calling this function";
		}
	}
	return pixels;
}

const ofPixels& ofQTKitPlayer::getPixels() const {
    return pixels;
}

//--------------------------------------------------------------------
ofTexture* ofQTKitPlayer::getTexturePtr() {
    ofTexture* texPtr = NULL;
	if(moviePlayer.textureAllocated){
		updateTexture();
        return &tex;
	} else {
        return NULL;
    }
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setPosition(float pct) {
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer setPosition:pct];
	}

	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setVolume(float volume) {
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer setVolume:volume];
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setBalance(float balance) {
	if(!isLoaded()) return;

	@autoreleasepool {
		[moviePlayer setBalance:balance];
	}
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setFrame(int frame) {
	if(!isLoaded()) return;

	frame %= [moviePlayer frameCount];

	@autoreleasepool {
		[moviePlayer setFrame:frame];
	}

	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
}

//--------------------------------------------------------------------
int ofQTKitPlayer::getCurrentFrame() const {
	if(!isLoaded()) return 0;
    return [moviePlayer frame];
}

//--------------------------------------------------------------------
int ofQTKitPlayer::getTotalNumFrames() const {
	if(!isLoaded()) return 0;
	return [moviePlayer frameCount];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setLoopState(ofLoopType state) {
	if(!isLoaded()) return;

	@autoreleasepool {
		currentLoopState = state;

		if(state == OF_LOOP_NONE){
			[moviePlayer setLoops:false];
			[moviePlayer setPalindrome:false];
		}
		else if(state == OF_LOOP_NORMAL){
			[moviePlayer setLoops:true];
			[moviePlayer setPalindrome:false];
		}
		else if(state == OF_LOOP_PALINDROME) {
			[moviePlayer setLoops:false];
			[moviePlayer setPalindrome:true];
		}
	}
}

//--------------------------------------------------------------------
ofLoopType ofQTKitPlayer::getLoopState() const {
	if(!isLoaded()) return OF_LOOP_NONE;

	ofLoopType state = OF_LOOP_NONE;

    if(![moviePlayer loops] && ![moviePlayer palindrome]){
		state = OF_LOOP_NONE;
	}
	else if([moviePlayer loops] && ![moviePlayer palindrome]){
		state =  OF_LOOP_NORMAL;
	}
	else if([moviePlayer loops] && [moviePlayer palindrome]) {
    	state = OF_LOOP_PALINDROME;
	}
	else{
		ofLogError("ofQTKitPlayer") << "unknown loop state";
	}

	return state;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getSpeed() const {
	return speed;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getDuration() const {
	return duration;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getPositionInSeconds() const {
	return getPosition() * duration;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getPosition() const {
	if(!isLoaded()) return 0;
	return [moviePlayer position];
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::getIsMovieDone() const {
	if(!isLoaded()) return false;
	return [moviePlayer isFinished];
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getWidth() const {
    return [moviePlayer movieSize].width;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getHeight() const {
    return [moviePlayer movieSize].height;
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::setPixelFormat(ofPixelFormat newPixelFormat){
    if(newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofQTKitPlayer") << "setPixelFormat(): pixel format "
			<< ofToString(newPixelFormat) << " is not supported";
        return false;
    }

    if(newPixelFormat != pixelFormat){
        pixelFormat = newPixelFormat;
        // If we already have a movie loaded we need to reload
        // the movie with the new settings correctly allocated.
        if(isLoaded()){
            load(moviePath, decodeMode);
        }
    }
	return true;
}

//--------------------------------------------------------------------
ofPixelFormat ofQTKitPlayer::getPixelFormat() const {
	return pixelFormat;
}

//--------------------------------------------------------------------
ofQTKitDecodeMode ofQTKitPlayer::getDecodeMode() const {
    return decodeMode;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setSynchronousSeeking(bool synchronous){
	bSynchronousSeek = synchronous;
    if(isLoaded()){
        moviePlayer.synchronousSeek = synchronous;
    }
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::getSynchronousSeeking() const {
	return bSynchronousSeek;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::reallocatePixels(){
    if(pixelFormat == OF_PIXELS_RGBA){
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR_ALPHA);
    } else {
        pixels.allocate(getWidth(), getHeight(), OF_IMAGE_COLOR);
    }
}

//--------------------------------------------------------------------
void ofQTKitPlayer::updateTexture(){
	if(moviePlayer.textureAllocated){

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

#endif
#endif
