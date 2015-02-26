//
//  ofAVFoundationPlayer.mm
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

//--------------------------------------------------------------
#import "ofAVFoundationPlayer.h"
#import "ofAVFoundationVideoPlayer.h"

//--------------------------------------------------------------
#ifdef TARGET_OF_IOS
CVOpenGLESTextureCacheRef _videoTextureCache = NULL;
CVOpenGLESTextureRef _videoTextureRef = NULL;
#endif

#ifdef TARGET_OSX
CVOpenGLTextureCacheRef _videoTextureCache = NULL;
CVOpenGLTextureRef _videoTextureRef = NULL;
#endif

ofAVFoundationPlayer::ofAVFoundationPlayer() {
	videoPlayer = NULL;
    pixelFormat = OF_PIXELS_RGBA;
	
    bFrameNew = false;
    bResetPixels = false;
    bUpdatePixels = false;
    bUpdateTexture = false;
    bTextureCacheSupported = false;
#ifdef TARGET_OF_IOS
    bTextureCacheSupported = (CVOpenGLESTextureCacheCreate != NULL);
#endif
#ifdef TARGET_OSX
    bTextureCacheSupported = (CVOpenGLTextureCacheCreate != NULL);
#endif
}

//--------------------------------------------------------------
ofAVFoundationPlayer::~ofAVFoundationPlayer() {
	close();
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::loadAsync(string name){
	loadPlayer(name, true);
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::load(string name) {
	return loadPlayer(name, false);
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::loadPlayer(string name, bool bAsync) {
	
    if(videoPlayer == NULL) {
        videoPlayer = [[ofAVFoundationVideoPlayer alloc] init];
        [videoPlayer setWillBeUpdatedExternally:YES];
    }
	
    NSString * videoPath = [NSString stringWithUTF8String:name.c_str()];
	NSString * videoLocalPath = [NSString stringWithUTF8String:ofToDataPath(name).c_str()];

	BOOL bStream = NO;
	
	bStream = bStream || (ofIsStringInString(name, "http://"));
	bStream = bStream || (ofIsStringInString(name, "https://"));
	bStream = bStream || (ofIsStringInString(name, "rtsp://"));
	
	NSURL * url = nil;
	if(bStream == YES) {
		url = [NSURL URLWithString:videoPath];
	} else {
		url = [NSURL fileURLWithPath:videoLocalPath];
	}
	
	bool bLoaded = [videoPlayer loadWithURL:url async:bAsync];
	
    bResetPixels = true;
    bUpdatePixels = true;
    bUpdateTexture = true;
    
    bool bCreateTextureCache = true;
    bCreateTextureCache = bCreateTextureCache && (bTextureCacheSupported == true);
    bCreateTextureCache = bCreateTextureCache && (_videoTextureCache == NULL);
    
    if(bCreateTextureCache == true) {

        CVReturn err;
        
#if defined(TARGET_OF_IOS) && defined(__IPHONE_6_0)
        err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                           NULL,
                                           [EAGLContext currentContext],
                                           NULL,
                                           &_videoTextureCache);
#endif
        
#if defined(TARGET_OF_IOS) && !defined(__IPHONE_6_0)
        err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                           NULL,
                                           (__bridge void *)[EAGLContext currentContext],
                                           NULL,
                                           &_videoTextureCache);
#endif
        
#ifdef TARGET_OSX
        err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault,
                                         NULL,
                                         CGLGetCurrentContext(),
                                         CGLGetPixelFormat(CGLGetCurrentContext()),
                                         NULL,
                                         &_videoTextureCache);
#endif
        
        if(err) {
            ofLogWarning("ofxiOSVideoPlayer") << "load(): error when creating texture cache, " << err;
        }
    }
	
    return bLoaded;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::close() {
	if(videoPlayer != NULL) {
		
		pixels.clear();
        
        videoTexture.clear();
		
        videoPlayer.delegate = nil;
		[videoPlayer release];
        
        if(bTextureCacheSupported == true) {
            killTextureCache();
        }
	}
	videoPlayer = NULL;
    
    bFrameNew = false;
    bResetPixels = false;
    bUpdatePixels = false;
    bUpdateTexture = false;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::setPixelFormat(ofPixelFormat value) {
    bool bValid = false;
    bValid = bValid || (value == OF_PIXELS_RGB);
    bValid = bValid || (value == OF_PIXELS_RGBA);
    
    if(bValid == false) {
        ofLogWarning("ofxiOSVideoPlayer") << "setPixelFormat(): unsupported ofPixelFormat, " << value;
        return false;
    }
    
    if(pixelFormat == value) {
        return true;
    }
    
    pixelFormat = value;
    bResetPixels = true;
    
	return true;
}

//--------------------------------------------------------------
ofPixelFormat ofAVFoundationPlayer::getPixelFormat() const{
	return pixelFormat;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::update() {
    
    bFrameNew = false; // default.
    
    if(!isLoaded()) {
        return;
    }
    
    [videoPlayer update];
    bFrameNew = [videoPlayer isNewFrame]; // check for new frame staright after the call to update.
    
    if(bFrameNew) {
        /**
         *  mark pixels to be updated.
         *  pixels are then only updated if the getPixels() method is called,
         *  internally or externally to this class.
         *  this ensures the pixels are updated only once per frame.
         */
        bUpdatePixels = true;
        bUpdateTexture = true;
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::draw() {
    draw(0, 0);
}

void ofAVFoundationPlayer::draw(float x, float y) {
    draw(x, y, getWidth(), getHeight());
}

void ofAVFoundationPlayer::draw(const ofRectangle & rect) {
    draw(rect.x, rect.y, rect.width, rect.height);
}

void ofAVFoundationPlayer::draw(float x, float y, float w, float h) {
    getTexturePtr()->draw(x, y, w, h);
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::play() {
    if(videoPlayer == NULL) {
        ofLogWarning("ofxiOSVideoPlayer") << "play(): video not loaded";
    }
    
	[videoPlayer play];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::stop() {
    if(videoPlayer == NULL) {
        return;
    }
    
    [videoPlayer pause];
    [videoPlayer setPosition:0];
}		

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isFrameNew() const {
	if(videoPlayer != NULL) {
		return bFrameNew;
	}	
	return false;
}

//--------------------------------------------------------------
const ofPixels & ofAVFoundationPlayer::getPixels() const {
    return const_cast<ofAVFoundationPlayer *>(this)->getPixels();
}

ofPixels & ofAVFoundationPlayer::getPixels() {
    if(isLoaded() == false) {
        ofLogError("ofxiOSVideoPlayer") << "getPixels(): Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixels.";
        return pixels;
    }
    
    if(bUpdatePixels == false) {
        // if pixels have not changed,
        // return the already calculated pixels.
        return pixels;
    }
    
    if(bResetPixels == true) {
        pixels.allocate(getWidth(), getHeight(), pixelFormat);
        bResetPixels = false;
    }
    
    CVImageBufferRef imageBuffer = [videoPlayer getCurrentFrame];
    
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    unsigned long imageBufferPixelFormat = CVPixelBufferGetPixelFormatType(imageBuffer);
    
    vImage_Buffer src = {
        CVPixelBufferGetBaseAddress(imageBuffer),
        CVPixelBufferGetHeight(imageBuffer),
        CVPixelBufferGetWidth(imageBuffer),
        CVPixelBufferGetBytesPerRow(imageBuffer)
    };
    
    vImage_Buffer dest = {
        pixels.getData(),
        static_cast<vImagePixelCount>(pixels.getHeight()),
        static_cast<vImagePixelCount>(pixels.getWidth()),
        static_cast<size_t>(pixels.getWidth() * pixels.getNumChannels())
    };
    
    vImage_Error err = kvImageNoError;
    
    if(pixelFormat == OF_PIXELS_RGBA) {
        
        if(imageBufferPixelFormat == kCVPixelFormatType_32ARGB) {
            
            uint8_t permuteMap[4] = { 1, 2, 3, 0 };
            err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
            
        } else if(imageBufferPixelFormat == kCVPixelFormatType_32BGRA) {
            
            uint8_t permuteMap[4] = { 2, 1, 0, 3 };
            err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
        }
        
    } else if(pixelFormat == OF_PIXELS_RGB) {
        
        if(imageBufferPixelFormat == kCVPixelFormatType_32ARGB) {
            
            err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
            
        } else if(imageBufferPixelFormat == kCVPixelFormatType_32BGRA) {
            
#ifdef __IPHONE_6_0
            err = vImageConvert_BGRA8888toRGB888(&src, &dest, 0);
#else
            ofLogError("ofxiOSVideoPlayer") << "getPixels(): OF_PIXELS_RGB is not supported, use setPixelFormat() to set the pixel format to OF_PIXELS_RGBA";
#endif
        }
    }
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    if(err != kvImageNoError) {
        ofLogError("ofxiOSVideoPlayer") << "getPixels(): error in pixel copy, vImage_error = " << err;
    }
    
    bUpdatePixels = false;
    
    return pixels;
}

//--------------------------------------------------------------
ofTexture * ofAVFoundationPlayer::getTexturePtr() {
    
    if(isLoaded() == false) {
        return &videoTexture;
    }
    
    if(bUpdateTexture == false) {
        return &videoTexture;
    }
    
    if(bTextureCacheSupported == true) {
        
        initTextureCache();
        
    } else {
        
        /**
         *  no video texture cache.
         *  load texture from pixels.
         *  this method is the slower alternative.
         */
        
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        
        if(getWidth() > maxTextureSize || getHeight() > maxTextureSize) {
            ofLogWarning("ofxiOSVideoPlayer") << "getTexturePtr(): " << getWidth() << "x" << getHeight() << " video image is bigger then max supported texture size " << maxTextureSize;
            return NULL;
        }
        
        videoTexture.loadData(getPixels());
    }
    
    bUpdateTexture = false;
    
    return &videoTexture;
}

//-------------------------------------------------------------- texture cache
void ofAVFoundationPlayer::initTextureCache() {

    CVImageBufferRef imageBuffer = [videoPlayer getCurrentFrame];
    if(imageBuffer == nil) {
        return;
    }
    
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    /**
     *  video texture cache is available.
     *  this means we don't have to copy any pixels,
     *  and we can reuse the already existing video texture.
     *  this is very fast! :)
     */
    
    /**
     *  CVOpenGLESTextureCache does this operation for us.
     *  it automatically returns a texture reference which means we don't have to create the texture ourselves.
     *  this creates a slight problem because when we create an ofTexture objects, it also creates a opengl texture for us,
     *  which is unecessary in this case because the texture already exists.
     *  so... we can use ofTexture::setUseExternalTextureID() to get around this.
     */
    
    int videoTextureW = getWidth();
    int videoTextureH = getHeight();
    videoTexture.allocate(videoTextureW, videoTextureH, GL_RGBA);
    
    ofTextureData & texData = videoTexture.getTextureData();
    texData.tex_t = 1.0f; // these values need to be reset to 1.0 to work properly.
    texData.tex_u = 1.0f; // assuming this is something to do with the way ios creates the texture cache.

    CVReturn err;
    unsigned int textureCacheID;
    
#ifdef TARGET_OF_IOS
    
    /**
     *  create video texture from video image.
     *  inside this function, ios is creating the texture for us.
     *  a video texture reference is returned.
     */
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,     // CFAllocatorRef allocator
                                                       _videoTextureCache,      // CVOpenGLESTextureCacheRef textureCache
                                                       imageBuffer,             // CVImageBufferRef sourceImage
                                                       NULL,                    // CFDictionaryRef textureAttributes
                                                       texData.textureTarget,   // GLenum target
                                                       texData.glTypeInternal,  // GLint internalFormat
                                                       texData.width,           // GLsizei width
                                                       texData.height,          // GLsizei height
                                                       GL_BGRA,                 // GLenum format
                                                       GL_UNSIGNED_BYTE,        // GLenum type
                                                       0,                       // size_t planeIndex
                                                       &_videoTextureRef);      // CVOpenGLESTextureRef *textureOut
    
    textureCacheID = CVOpenGLESTextureGetName(_videoTextureRef);
    
#endif
    
#ifdef TARGET_OSX
    
    err = CVOpenGLTextureCacheCreateTextureFromImage(NULL,
                                                     _videoTextureCache,
                                                     imageBuffer,
                                                     NULL,
                                                     &_videoTextureRef);

    textureCacheID = CVOpenGLTextureGetName(_videoTextureRef);
    
#endif
    
    videoTexture.setUseExternalTextureID(textureCacheID);
    videoTexture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
    videoTexture.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    if(ofIsGLProgrammableRenderer() == false) {
        videoTexture.bind();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        videoTexture.unbind();
    }
    
    if(err) {
        ofLogError("ofxiOSVideoPlayer") << "initTextureCache(): error creating texture cache from image " << err;
    }
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
#ifdef TARGET_OF_IOS
    
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }
    
#endif
    
#ifdef TARGET_OSX

    CVOpenGLTextureCacheFlush(_videoTextureCache, 0);
    if(_videoTextureRef) {
        CVOpenGLTextureRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }
    
#endif
}

void ofAVFoundationPlayer::killTextureCache() {
#ifdef TARGET_OF_IOS
    
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }

    if(_videoTextureCache) {
        CFRelease(_videoTextureCache);
        _videoTextureCache = NULL;
    }
    
#endif
    
#ifdef TARGET_OSX
    
    if (_videoTextureRef != NULL) {
        CVOpenGLTextureRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }
    
    if(_videoTextureCache != NULL) {
        CVOpenGLTextureCacheRelease(_videoTextureCache);
        _videoTextureCache = NULL;
    }
    
#endif
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getWidth() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [videoPlayer getWidth];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getHeight() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [videoPlayer getHeight];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPaused() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return ![videoPlayer isPlaying];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoaded() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [videoPlayer isReady];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPlaying() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [videoPlayer isPlaying];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPosition() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [videoPlayer getPosition];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getSpeed() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [videoPlayer getSpeed];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getDuration() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [videoPlayer getDurationInSec];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::getIsMovieDone() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [videoPlayer isFinished];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPaused(bool bPause) {
    if(videoPlayer == NULL) {
        return;
    }
    
    if(bPause) {
        [videoPlayer pause];
    } else {
        [videoPlayer play];
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPosition(float pct) {
    if(videoPlayer == NULL) {
        return;
    }
    
    [videoPlayer setPosition:pct];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setVolume(float volume) {
    if(videoPlayer == NULL) {
        return;
    }
	if(volume > 1.0) {
		ofLogWarning("ofxiOSVideoPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0";
		volume = 1.0;
	}
    [videoPlayer setVolume:volume];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setLoopState(ofLoopType state) {
    if(videoPlayer == NULL) {
        return;
    }
    
    bool bLoop = false;
    if((state == OF_LOOP_NORMAL) || 
       (state == OF_LOOP_PALINDROME)) {
        bLoop = true;
    }
    [videoPlayer setLoop:bLoop];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setSpeed(float speed) {
    if(videoPlayer == NULL) {
        return;
    }
    
    [videoPlayer setSpeed:speed];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setFrame(int frame) {
    if(videoPlayer == NULL) {
        return;
    }

    [videoPlayer setFrame:frame];
}

//--------------------------------------------------------------
int	ofAVFoundationPlayer::getCurrentFrame() const {
    if(videoPlayer == NULL){
        return 0;
    }
    return [videoPlayer getCurrentFrameNum];
}

//--------------------------------------------------------------
int	ofAVFoundationPlayer::getTotalNumFrames() const {
    if(videoPlayer == NULL){
        return 0;
    }
    return [videoPlayer getDurationInFrames];
}

//--------------------------------------------------------------
ofLoopType	ofAVFoundationPlayer::getLoopState() const {
    if(videoPlayer == NULL) {
        return OF_LOOP_NONE;
    }
    
    bool bLoop =  [videoPlayer getLoop];
    if(bLoop) {
        return OF_LOOP_NORMAL;
    }
    return OF_LOOP_NONE;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::firstFrame() {
    if(videoPlayer == NULL) {
        return;
    }
    
    [videoPlayer setPosition:0];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::nextFrame() {
    int nextFrameNum = ofClamp(getCurrentFrame() + 1, 0, getTotalNumFrames());
    setFrame(nextFrameNum);
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::previousFrame() {
    int prevFrameNum = ofClamp(getCurrentFrame() - 1, 0, getTotalNumFrames());
    setFrame(prevFrameNum);
}

//--------------------------------------------------------------
#ifdef __OBJC__

ofAVFoundationVideoPlayer * ofAVFoundationPlayer::getAVFoundationVideoPlayer() {
	return videoPlayer;
}

#else

void * ofAVFoundationPlayer::getAVFoundationVideoPlayer() {
	return videoPlayer;
}

#endif

//-------------------------------------------------------------- DEPRECATED.
bool ofAVFoundationPlayer::loadMovie(string name) {
    return load(name);
}

ofPixels & ofAVFoundationPlayer::getPixelsRef() {
    return getPixels();
}

const ofPixels & ofAVFoundationPlayer::getPixelsRef() const {
    return getPixels();
}

ofTexture * ofAVFoundationPlayer::getTexture() {
    return getTexturePtr();
}

