
#include "ofQTKitPlayer.h"

#include "Poco/String.h"

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
bool ofQTKitPlayer::loadMovie(string path){
	return loadMovie(path, OF_QTKIT_DECODE_PIXELS_ONLY);
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::loadMovie(string movieFilePath, ofQTKitDecodeMode mode) {
	if(mode != OF_QTKIT_DECODE_PIXELS_ONLY && mode != OF_QTKIT_DECODE_TEXTURE_ONLY && mode != OF_QTKIT_DECODE_PIXELS_AND_TEXTURE){
		ofLogError("ofQTKitPlayer") << "Invalid ofQTKitDecodeMode mode specified while loading movie.";
		return false;
	}
	
	if(isLoaded()){
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
		firstFrame(); //will load the first frame
	}
	else {
		ofLogError("ofQTKitPlayer") << "Loading file " << movieFilePath << " failed.";
		[moviePlayer release];
		moviePlayer = NULL;
	}
	
	[pool release];
	
	return success;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::closeMovie() {
	close();
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isLoaded() {
	return moviePlayer != NULL;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::close() {
	
	if(isLoaded()){
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		[moviePlayer release];
		moviePlayer = NULL;
        [pool release];
	}
	
	pixels.clear();
	duration = 0;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setPaused(bool _bPaused){
    if(!isLoaded()) return;

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

//--------------------------------------------------------------------
bool ofQTKitPlayer::isPaused() {
	return bPaused;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::stop() {
	setPaused(true);
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isPlaying(){
    if(!isLoaded()) return false;

	return !moviePlayer.isFinished && !isPaused(); 
}

//--------------------------------------------------------------------
void ofQTKitPlayer::firstFrame(){
	if(!isLoaded()) return;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

    [moviePlayer gotoBeginning];
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
    [pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::nextFrame(){
    if(!isLoaded()) return;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    [moviePlayer stepForward];
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
    [pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::previousFrame(){
    if(!isLoaded()) return;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    [moviePlayer stepBackward];
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
    [pool release];
    
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setSpeed(float rate){
	if(!isLoaded()) return;

    speed = rate;

    if(isPlaying()) {
        NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
        [moviePlayer setRate:rate];
        [pool release];
    }
}

//--------------------------------------------------------------------
void ofQTKitPlayer::play(){
	if(!isLoaded()) return;
    
    bPaused = false;
    
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    [moviePlayer setRate:speed];
	[pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::idleMovie() {
	update();
}

//--------------------------------------------------------------------
void ofQTKitPlayer::update() {
	if(!isLoaded()) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	bNewFrame = [moviePlayer update];
	if (bNewFrame) {
		bHavePixelsChanged = true;
	}
    [pool release];
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::isFrameNew() {
	return bNewFrame;
}
		
//--------------------------------------------------------------------
void ofQTKitPlayer::bind() {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	if(!isLoaded() || !moviePlayer.useTexture) return;
	
	updateTexture();
	tex.bind();
    [pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::unbind(){
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	if(!isLoaded() || !moviePlayer.useTexture) return;

	tex.unbind();
    [pool release];
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
ofPixelsRef	ofQTKitPlayer::getPixelsRef(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	if(isLoaded() && moviePlayer.usePixels) {
	   //don't get the pixels every frame if it hasn't updated
	   if(bHavePixelsChanged){
		   [moviePlayer pixels:pixels.getPixels()];
		   bHavePixelsChanged = false;
	   }
	}
    else{
        ofLogError("ofQTKitPlayer") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixelsRef.";
    }

    [pool release];
	return pixels;	   
}

//--------------------------------------------------------------------
unsigned char* ofQTKitPlayer::getPixels() {
	return getPixelsRef().getPixels();
}

//--------------------------------------------------------------------
ofTexture* ofQTKitPlayer::getTexture() {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    ofTexture* texPtr = NULL;
    
	if(moviePlayer.textureAllocated){
		updateTexture();
        texPtr = &tex;
	} else {
        ofLogWarning("ofQTKitPlayer") << "Texture not allocated. Returning NULL ofTexture pointer.  Check ofQTKitDecodeMode.";
        texPtr = NULL;
    }
    
    [pool release];

    return texPtr;

}

//--------------------------------------------------------------------
void ofQTKitPlayer::setPosition(float pct) {
	if(!isLoaded()) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
	moviePlayer.position = pct;
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	
	[pool release];	
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setVolume(float volume) {
	if(!isLoaded()) return;
	
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	moviePlayer.volume = volume;
	[pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setBalance(float balance) {
	if(!isLoaded()) return;
	
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	[moviePlayer setBalance:balance];
	[pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setFrame(int frame) {
	if(!isLoaded()) return;

	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	moviePlayer.frame = frame % moviePlayer.frameCount;
	bHavePixelsChanged = bNewFrame = bSynchronousSeek;
	
	[pool release];
}

//--------------------------------------------------------------------
int ofQTKitPlayer::getCurrentFrame() {
	if(!isLoaded()) return 0;

    int currentFrame = 0;
    
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	currentFrame = moviePlayer.frame;
	[pool release];	
	
	return currentFrame;	
}

//--------------------------------------------------------------------
int ofQTKitPlayer::getTotalNumFrames() {
	if(!isLoaded()) return 0;
	
    int frameCount = 0;

    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    frameCount = moviePlayer.frameCount;
    [pool release];

	return frameCount;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setLoopState(ofLoopType state) {
	if(!isLoaded()) return;
	
    currentLoopState = state;

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

}

//--------------------------------------------------------------------
ofLoopType ofQTKitPlayer::getLoopState(){
	if(!isLoaded()) return OF_LOOP_NONE;
	
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
		ofLogError("ofQTKitPlayer") << "Invalid loop state " << state << "." << endl;
	}
	
	[pool release];
	
	return state;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getSpeed(){
	return speed;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getDuration(){
	return duration;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getPositionInSeconds(){
	return getPosition() * duration;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getPosition(){
	if(!isLoaded()) return 0;
	
    float pos = 0.0f;
    
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	pos = moviePlayer.position;
	[pool release];
	
	return pos;
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::getIsMovieDone(){
	if(!isLoaded()) return false;
	bool isDone = false;
	
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    isDone = moviePlayer.isFinished;
	[pool release];
    
	return isDone;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getWidth() {
    float w = 0.0f;
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    w = moviePlayer.movieSize.width;
	[pool release];

    return w;
}

//--------------------------------------------------------------------
float ofQTKitPlayer::getHeight() {
    float h = 0.0f;
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    h = moviePlayer.movieSize.width;
	[pool release];

    return h;
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::setPixelFormat(ofPixelFormat newPixelFormat){
    if(newPixelFormat != OF_PIXELS_RGB && newPixelFormat != OF_PIXELS_RGBA) {
        ofLogWarning("ofQTKitPlayer") << "Pixel format " << newPixelFormat << " is not supported.";
        return false;
    }

    if(newPixelFormat != pixelFormat){
        pixelFormat = newPixelFormat;
        // If we already have a movie loaded we need to reload
        // the movie with the new settings correctly allocated.
        if(isLoaded()){
            loadMovie(moviePath, decodeMode);
        }
    }	
	return true;
}

//--------------------------------------------------------------------
ofPixelFormat ofQTKitPlayer::getPixelFormat(){
	return pixelFormat;
}

//--------------------------------------------------------------------
ofQTKitDecodeMode ofQTKitPlayer::getDecodeMode(){
    return decodeMode;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::setSynchronousSeeking(bool synchronous){
	bSynchronousSeek = synchronous;
    
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	
    if(isLoaded()){
        moviePlayer.synchronousSeek = synchronous;
    }
	
    [pool release];
}

//--------------------------------------------------------------------
bool ofQTKitPlayer::getSynchronousSeeking(){
	return 	bSynchronousSeek;
}

//--------------------------------------------------------------------
void ofQTKitPlayer::reallocatePixels(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
    
    if(pixelFormat == OF_PIXELS_RGBA){
        pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR_ALPHA);
    }
    else {
        pixels.allocate(moviePlayer.movieSize.width, moviePlayer.movieSize.height, OF_IMAGE_COLOR);
    }
    
    [pool release];
}

//--------------------------------------------------------------------
void ofQTKitPlayer::updateTexture(){
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

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
    
    [pool release];

}

