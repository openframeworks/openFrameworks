
#include "ofQTKitPlayer.h"

ofQTKitPlayer::ofQTKitPlayer() {
	moviePlayer = NULL;
	bNewFrame = false;
	duration = 0;
    speed = 0;
    //ofQTKitPlayer supports RGB and RGBA
    pixelFormat = OF_PIXELS_RGB;
}


ofQTKitPlayer::~ofQTKitPlayer() {
	close();	
}

bool ofQTKitPlayer::loadMovie(string path){
	return loadMovie(path, OF_QTKIT_DECODE_PIXELS_ONLY);
}

bool ofQTKitPlayer::loadMovie(string movieFilePath, ofQTKitDecodeMode mode) {
	if(mode < 0 || mode > 2){
		ofLog(OF_LOG_ERROR, "ofQTKitPlayer -- Error, invalid mode specified for");
		return false;
	}
	
	if(moviePlayer != NULL){
		close(); //auto released 
	}
	decodeMode = mode;
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
	bool useTexture = (mode == OF_QTKIT_DECODE_TEXTURE_ONLY || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
	bool usePixels  = (mode == OF_QTKIT_DECODE_PIXELS_ONLY  || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);

    bool isURL = false;
    if (movieFilePath.substr(0, 7) == "http://" || movieFilePath.substr(0,7) == "rtsp://") {
        isURL = true;
    }
    else {
        movieFilePath = ofToDataPath(movieFilePath, false);
    }

	moviePlayer = [[QTKitMovieRenderer alloc] init];
	
	BOOL success = [moviePlayer loadMovie:[NSString stringWithCString:movieFilePath.c_str() encoding:NSUTF8StringEncoding]
                                pathIsURL:isURL
							 allowTexture:useTexture
							  allowPixels:usePixels];

	if(success){
        reallocatePixels();
		duration = moviePlayer.duration;
        setLoopState(OF_LOOP_NONE);
	}
	else {
		ofLog(OF_LOG_ERROR, "ofQTKitPlayer -- Loading file " + movieFilePath + " failed");
		[moviePlayer release];
		moviePlayer = NULL;
	}
	
	[pool release];
	
	return success;
}

void ofQTKitPlayer::closeMovie() {
	close();
}

bool ofQTKitPlayer::isLoaded() {
	return moviePlayer != NULL;
}

void ofQTKitPlayer::close() {
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	if(moviePlayer != NULL){
		[moviePlayer release];
		moviePlayer = NULL;
	}
	
	pixels.clear();
	
	duration = 0;

	[pool release];	
}

void ofQTKitPlayer::setPaused(bool bPaused){
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	if(bPaused){
		[moviePlayer setRate:0.0];
    }
    else{
        [moviePlayer setRate:speed];
    }
	[pool release];	
}

bool ofQTKitPlayer::isPaused() {
	return getSpeed() == 0.0;
}

void ofQTKitPlayer::stop() {
	setPaused(true);
}

bool ofQTKitPlayer::isPlaying(){
	return !moviePlayer.isFinished && !isPaused(); 
}

void ofQTKitPlayer::firstFrame(){
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    [moviePlayer gotoBeginning];
    
    [pool release];
}

void ofQTKitPlayer::nextFrame(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [moviePlayer stepForward];
    
    [pool release];
    
}

void ofQTKitPlayer::previousFrame(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [moviePlayer stepBackward];
    
    [pool release];
    
}

void ofQTKitPlayer::setSpeed(float rate){
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
	speed = rate;
	[moviePlayer setRate:rate];

	[pool release];	
}

void ofQTKitPlayer::play(){	
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    if(speed == 0.0){
    	speed = 1.0; //default to full speed if something specific isn't set
    }
    [moviePlayer setRate:speed];
	
	[pool release];
}

void ofQTKitPlayer::idleMovie() {
	update();
}

void ofQTKitPlayer::update() {
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	bNewFrame = [moviePlayer update];
	if (bNewFrame) {
		bHavePixelsChanged = true;
	}
    [pool release];
}

bool ofQTKitPlayer::isFrameNew() {
	return bNewFrame;
}
		
void ofQTKitPlayer::bind() {
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;
	
	updateTexture();
	tex.bind();
}

void ofQTKitPlayer::unbind(){
	if(moviePlayer == NULL || !moviePlayer.useTexture) return;

	tex.unbind();

}

void ofQTKitPlayer::draw(ofRectangle drawRect){
	draw(drawRect.x, drawRect.y, drawRect.width, drawRect.height);
}

void ofQTKitPlayer::draw(float x, float y) {
	draw(x,y, moviePlayer.movieSize.width, moviePlayer.movieSize.height);
}

void ofQTKitPlayer::draw(float x, float y, float w, float h) {
	updateTexture();
	tex.draw(x,y,w,h);	
}

ofPixelsRef	ofQTKitPlayer::getPixelsRef(){
	if(moviePlayer != NULL && moviePlayer.usePixels) {
	   //don't get the pixels every frame if it hasn't updated
	   if(bHavePixelsChanged){
		   [moviePlayer pixels:pixels.getPixels()];
		   bHavePixelsChanged = false;
	   }
	}
    else{
        ofLogError("ofQTKitPlayer") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef";
    }

	return pixels;	   
}

unsigned char* ofQTKitPlayer::getPixels() {
	return getPixelsRef().getPixels();
}

ofTexture* ofQTKitPlayer::getTexture() {
	if(moviePlayer.textureAllocated){
		updateTexture();
        return &tex;
	}
	return NULL;
}

void ofQTKitPlayer::setPosition(float pct) {
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.position = pct;
	
	[pool release];
}

void ofQTKitPlayer::setVolume(float volume) {
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.volume = volume;
	
	[pool release];
}

void ofQTKitPlayer::setBalance(float balance) {
	if(moviePlayer == NULL) return;
	
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	[moviePlayer setBalance:balance];
	
	[pool release];
}

void ofQTKitPlayer::setFrame(int frame) {
	if(moviePlayer == NULL) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.frame = frame % moviePlayer.frameCount;
	
	[pool release];
	
}

int ofQTKitPlayer::getCurrentFrame() {
	if(moviePlayer == NULL) return 0;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	int currentFrame = moviePlayer.frame;
	
	[pool release];	
	
	return currentFrame;	
}

int ofQTKitPlayer::getTotalNumFrames() {
	if(moviePlayer == NULL) return 0;
	
	return moviePlayer.frameCount;
}

void ofQTKitPlayer::setLoopState(bool loops) {
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.loops = loops;
	
	[pool release];
}

void ofQTKitPlayer::setLoopState(ofLoopType state) {
	if(state == OF_LOOP_NONE){
		setLoopState(false);
	}
	else if(state == OF_LOOP_NORMAL){
		setLoopState(true);
	}
	
	//TODO: support OF_LOOP_PALINDROME
}

bool ofQTKitPlayer::getMovieLoopState(){
	if(moviePlayer == NULL) return NO;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	bool loops = moviePlayer.loops;
	
	[pool release];
	
	return loops;
}

float ofQTKitPlayer::getSpeed()
{
	if(moviePlayer == NULL) return 0;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	float rate = moviePlayer.rate;
	
	[pool release];
	
	return rate;
}

float ofQTKitPlayer::getDuration(){
	return duration;
}

float ofQTKitPlayer::getPositionInSeconds(){
	return getPosition() * duration;
}

float ofQTKitPlayer::getPosition(){
	if(moviePlayer == NULL) return 0;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	float pos = moviePlayer.position;
	
	[pool release];
	
	return pos;
}

bool ofQTKitPlayer::getIsMovieDone(){
	if(moviePlayer == NULL) return false;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];	

	bool isDone = moviePlayer.isFinished;
	
	[pool release];
	
	return isDone;
}

float ofQTKitPlayer::getWidth() {
	return moviePlayer.movieSize.width;
}

float ofQTKitPlayer::getHeight() {
	return moviePlayer.movieSize.height;
}

void ofQTKitPlayer::setPixelFormat(ofPixelFormat newPixelFormat){
    if(newPixelFormat != pixelFormat){
        if(newPixelFormat == OF_PIXELS_RGB){
            pixelFormat = OF_PIXELS_RGB;
            reallocatePixels();
        }
        else if(newPixelFormat == OF_PIXELS_RGBA){
            pixelFormat = OF_PIXELS_RGBA;
            reallocatePixels();
        }
        else {
            ofLogError("ofQTKitPlayer::setPixelFormat -- Pixel format " + ofToString(newPixelFormat) + " is not supported");
			setPixelFormat(OF_PIXELS_RGBA);
        }
    }
}

ofQTKitDecodeMode ofQTKitPlayer::getDecodeMode(){
    return decodeMode;
}

void ofQTKitPlayer::setSynchronousScrubbing(bool synchronous){
	if(moviePlayer != nil){
        moviePlayer.synchronousScrub = synchronous;
    }
}

bool ofQTKitPlayer::getSynchronousScrubbing(){
	if(moviePlayer != nil){
        return moviePlayer.synchronousScrub;
    }
}


void ofQTKitPlayer::reallocatePixels(){
    if(moviePlayer != nil){
        if(pixelFormat == OF_PIXELS_RGBA){
            moviePlayer.allowAlpha = true;
            pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR_ALPHA);   
        }
        else {
            moviePlayer.allowAlpha = false;
            pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR);
        }
    }
}

//Hack an OF texture to point to our texture that is managed by CoreVideo
//this will throw a few errors in the console but is harmless and fast
void ofQTKitPlayer::updateTexture(){
	if(moviePlayer.textureAllocated){
	   ofTextureData& data = tex.getTextureData();
		data.bAllocated = true;
		data.textureID = moviePlayer.textureID;
		data.textureTarget = moviePlayer.textureTarget;
		data.width = getWidth();
		data.height = getHeight();
		data.tex_w = getWidth();
		data.tex_h = getHeight();
		data.tex_t = getWidth();
		data.tex_u = getHeight();
	}
}

