
#include "ofQTKitPlayer.h"

#include "Poco/String.h"

ofQTKitPlayer::ofQTKitPlayer() {
	moviePlayer = NULL;
	bNewFrame = false;
    bPaused = true;
	duration = 0;
    speed = 0;
	//default this to true so the player update behavior matches ofQuicktimePlayer
	bSynchronousSeek = true;
    //ofQTKitPlayer supports RGB and RGBA
    pixelFormat = OF_PIXELS_RGB;
    currentLoopState = OF_LOOP_NORMAL;
}


ofQTKitPlayer::~ofQTKitPlayer() {
	close();	
}

bool ofQTKitPlayer::loadMovie(string path){
	return loadMovie(path, OF_QTKIT_DECODE_PIXELS_ONLY);
}

bool ofQTKitPlayer::loadMovie(string movieFilePath, ofQTKitDecodeMode mode) {
	if(mode != OF_QTKIT_DECODE_PIXELS_ONLY && mode != OF_QTKIT_DECODE_TEXTURE_ONLY && mode != OF_QTKIT_DECODE_PIXELS_AND_TEXTURE){
		ofLogError("ofQTKitPlayer") << "Invalid ofQTKitDecodeMode mode specified while loading movie.";
		return false;
	}
	
	if(moviePlayer != NULL){
		close(); //auto released 
	}
    
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    decodeMode = mode;
	bool useTexture = (mode == OF_QTKIT_DECODE_TEXTURE_ONLY || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
	bool usePixels  = (mode == OF_QTKIT_DECODE_PIXELS_ONLY  || mode == OF_QTKIT_DECODE_PIXELS_AND_TEXTURE);
	bool useAlpha = (pixelFormat == OF_PIXELS_RGBA);

    bool isURL = false;
	
    if (Poco::icompare(movieFilePath.substr(0,7), "http://")  == 0 ||
        Poco::icompare(movieFilePath.substr(0,8), "https://") == 0 ||
        Poco::icompare(movieFilePath.substr(0,7), "rtsp://")  == 0) {
        isURL = true;
    }
    else {
        movieFilePath = ofToDataPath(movieFilePath, false);
    }

	moviePlayer = [[QTKitMovieRenderer alloc] init];
	BOOL success = [moviePlayer loadMovie:[NSString stringWithCString:movieFilePath.c_str() encoding:NSUTF8StringEncoding]
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
	}
	else {
		ofLogError("ofQTKitPlayer") << "Loading file " << movieFilePath << " failed.";
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
	
	if(moviePlayer != NULL){
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		[moviePlayer release];
		moviePlayer = NULL;
        [pool release];
	}
	
	pixels.clear();
	duration = 0;
}

void ofQTKitPlayer::setPaused(bool _bPaused){
	
    bPaused = _bPaused;
    
    if(isPlaying() == true) {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

        if (bPaused == true) {
            [moviePlayer setRate:0.0];
        } else {
            [moviePlayer setRate:speed];
        }

        [pool release];
    }
    
}

bool ofQTKitPlayer::isPaused() {
	return bPaused;
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
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
    [pool release];
}

void ofQTKitPlayer::nextFrame(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [moviePlayer stepForward];
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
    [pool release];
}

void ofQTKitPlayer::previousFrame(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [moviePlayer stepBackward];
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	
    [pool release];
    
}

void ofQTKitPlayer::setSpeed(float rate){
	if(moviePlayer == NULL) return;

    speed = rate;

    if(isPlaying()) {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        [moviePlayer setRate:rate];
        [pool release];
    }
}

void ofQTKitPlayer::play(){
	if(moviePlayer == NULL) return;
    
    bPaused = false;
    
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
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
        ofLogError("ofQTKitPlayer") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef.";
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
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	
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
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	
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

void ofQTKitPlayer::setLoopState(ofLoopType state) {
	if(moviePlayer == NULL) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
	if(state == OF_LOOP_NONE){
        moviePlayer.loops = false;
        moviePlayer.palindrome = false;
	}
	else if(state == OF_LOOP_NORMAL){
        moviePlayer.loops = true;
        moviePlayer.palindrome = false;
	}
	else if(state == OF_LOOP_PALINDROME) {
        moviePlayer.loops = false;
        moviePlayer.palindrome = true;
    }
	
    [pool release];

    currentLoopState = state;
}

ofLoopType ofQTKitPlayer::getLoopState(){
	if(moviePlayer == NULL) return OF_LOOP_NONE;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	ofLoopType state = OF_LOOP_NONE;
	if(!moviePlayer.loops && !moviePlayer.palindrome){
		state = OF_LOOP_NONE;
	}
	else if(moviePlayer.loops && !moviePlayer.palindrome){
		state =  OF_LOOP_NORMAL;
	}
	else if(!moviePlayer.loops && moviePlayer.palindrome) {
    	state = OF_LOOP_PALINDROME;
	}
	else{
		ofLogError("ofQTKitPlayer") << "Invalid loop state" << endl;
	}
	
	[pool release];
	
	return state;
}

float ofQTKitPlayer::getSpeed(){
	return speed;
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

bool ofQTKitPlayer::setPixelFormat(ofPixelFormat newPixelFormat){
    if(newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofQTKitPlayer") << "Pixel format " << newPixelFormat << " is not supported.";
        return false;
    }

    if(newPixelFormat != pixelFormat){
        pixelFormat = newPixelFormat;
        //if we already have a movie loaded we need to reallocate the pixels
        if(isLoaded()){
            loadMovie(moviePath, decodeMode);
        }
    }	
	return true;
}

ofPixelFormat ofQTKitPlayer::getPixelFormat(){
	return pixelFormat;
}

ofQTKitDecodeMode ofQTKitPlayer::getDecodeMode(){
    return decodeMode;
}

void ofQTKitPlayer::setSynchronousSeeking(bool synchronous){
	bSynchronousSeek = synchronous;
	if(moviePlayer != nil){
        moviePlayer.synchronousSeek = synchronous;
    }
}

bool ofQTKitPlayer::getSynchronousSeeking(){
	return 	bSynchronousSeek;
}

void ofQTKitPlayer::reallocatePixels(){
    if(pixelFormat == OF_PIXELS_RGBA){
        pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR_ALPHA);
    }
    else {
        pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR);
    }
}


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

