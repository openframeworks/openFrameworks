//
//  ofAVFoundationPlayer.mm
//  Created by Lukasz Karluk on 06/07/14.
//	Merged with code by Sam Kronick, James George and Elie Zananiri.
//

//--------------------------------------------------------------
#import "ofAVFoundationPlayer.h"
#import "ofAVFoundationVideoPlayer.h"
#include "ofRectangle.h"
#include "ofGLUtils.h"

#ifdef TARGET_OSX
	#include "ofTexture.h"
#endif

//--------------------------------------------------------------
ofAVFoundationPlayer::ofAVFoundationPlayer() {
    videoPlayer = nullptr;
    pixelFormat = OF_PIXELS_RGBA;

    bFrameNew = false;
    bResetPixels = false;
    bUpdatePixels = false;
    bUpdateTexture = false;
	bUseTextureCache = true;
}

//--------------------------------------------------------------
ofAVFoundationPlayer::~ofAVFoundationPlayer() {
    disposePlayer();
}

//--------------------------------------------------------------
ofAVFoundationPlayer& ofAVFoundationPlayer::operator=(ofAVFoundationPlayer other)
{
	// clear pixels
	pixels.clear();
	videoTexture.clear();

	// get rid of the textures
	killTextureCache();

	bFrameNew = false;
	bResetPixels = false;
	bUpdatePixels = false;
	bUpdateTexture = false;
	bUseTextureCache = true;

	std::swap(videoPlayer, other.videoPlayer);
	return *this;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::loadAsync(std::string name){
    loadPlayer(name, true);
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::load(std::string name) {
    return loadPlayer(name, false);
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::loadPlayer(std::string name, bool bAsync) {
	if( ofGetUsingArbTex() == false ){
        killTextureCache();
		bUseTextureCache = false;
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

	bFrameNew = false;
	bResetPixels = true;
	bUpdatePixels = true;
	bUpdateTexture = true;

	bool bLoaded = false;

	if(videoPlayer == nullptr) {
		// create a new player if its not allocated
		videoPlayer = [[ofAVFoundationVideoPlayer alloc] init];
		[videoPlayer setWillBeUpdatedExternally:YES];
	}

	bLoaded = [videoPlayer loadWithURL:url async:bAsync stream:bStream];

	pixels.clear();
	videoTexture.clear();

	bool bCreateTextureCache = bLoaded && bUseTextureCache && (_videoTextureCache == nullptr);

	if(bCreateTextureCache == true) {

		CVReturn err;

#if defined(TARGET_OF_IOS) && defined(__IPHONE_6_0)
		err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
										   nullptr,
										   [EAGLContext currentContext],
										   nullptr,
										   &_videoTextureCache);
#endif

#if defined(TARGET_OF_IOS) && !defined(__IPHONE_6_0)
		err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
										   nullptr,
										   (__bridge void *)[EAGLContext currentContext],
										   nullptr,
										   &_videoTextureCache);
#endif

#ifdef TARGET_OSX
		err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault,
										 nullptr,
										 CGLGetCurrentContext(),
										 CGLGetPixelFormat(CGLGetCurrentContext()),
										 nullptr,
										 &_videoTextureCache);
#endif

		if(err) {
			ofLogWarning("ofAVFoundationPlayer") << "load(): error when creating texture cache, " << err << ".";
		}
	}


	if( bAsync == false && bLoaded ){
		pixels.allocate(getWidth(), getHeight(), getPixelFormat());
	}

    return bLoaded;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::disposePlayer() {

	if (videoPlayer != nullptr) {

		// clear pixels
		pixels.clear();
		videoTexture.clear();

		// dispose videoplayer
		__block ofAVFoundationVideoPlayer *currentPlayer = videoPlayer;
		dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
			@autoreleasepool {
				[currentPlayer unloadVideo]; // synchronious call to unload video
				[currentPlayer autorelease]; // release
			}
		});

		videoPlayer = nullptr;
	}

	// get rid of the textures
	killTextureCache();


	bFrameNew = false;
	bResetPixels = false;
	bUpdatePixels = false;
	bUpdateTexture = false;
	bUseTextureCache = true;
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::close() {
    if(videoPlayer != nullptr) {

        pixels.clear();
        videoTexture.clear();

		[videoPlayer close];
    }

    bFrameNew = false;
    bResetPixels = false;
    bUpdatePixels = false;
    bUpdateTexture = false;
	bUseTextureCache = true;
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::setPixelFormat(ofPixelFormat value) {
    bool bValid = false;
    bValid = bValid || (value == OF_PIXELS_RGB);
    bValid = bValid || (value == OF_PIXELS_RGBA);

    if(bValid == false) {
        ofLogWarning("ofAVFoundationPlayer") << "setPixelFormat(): unsupported ofPixelFormat, "
			<< ofToString(value) << ".";
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

    if(!isLoaded() || !isReady()) {
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
    if(isLoaded() && isReady()) {

		ofTexture * texturePtr = getTexturePtr();
		if( texturePtr != NULL ){
			if( texturePtr->isAllocated() ){
				texturePtr->draw(x, y, w, h);
			}
		}
    }
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::play() {
    if(videoPlayer == nullptr) {
        ofLogWarning("ofAVFoundationPlayer") << "play(): video not loaded.";
    }

    [videoPlayer play];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::stop() {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer pause];
    [videoPlayer setPosition:0];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isFrameNew() const {
    if(videoPlayer != nullptr) {
        return bFrameNew;
    }
    return false;
}

//--------------------------------------------------------------
const ofPixels & ofAVFoundationPlayer::getPixels() const {
    return const_cast<ofAVFoundationPlayer *>(this)->getPixels();
}

ofPixels & ofAVFoundationPlayer::getPixels() {
    if(isLoaded() == false || pixels.size() == 0) {
        ofLogError("ofAVFoundationPlayer") << "getPixels(): Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixels.";
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
            ofLogError("ofAVFoundationPlayer") << "getPixels(): OF_PIXELS_RGB is not supported, use setPixelFormat() to set the pixel format to OF_PIXELS_RGBA.";
#endif
        }
    }

    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

    if(err != kvImageNoError) {
        ofLogError("ofAVFoundationPlayer") << "getPixels(): error in pixel copy, vImage_error = " << err << ".";
    }

    bUpdatePixels = false;

    return pixels;
}

//--------------------------------------------------------------
ofTexture * ofAVFoundationPlayer::getTexturePtr() {

	if( bUseTextureCache == false ){
		return NULL;
	}

    if(isLoaded() == false || isReady() == false) {
        return &videoTexture;
    }

    if(bUpdateTexture == false) {
        return &videoTexture;
    }

    initTextureCache();

    bUpdateTexture = false;

    return &videoTexture;
}

//-------------------------------------------------------------- texture cache
void ofAVFoundationPlayer::initTextureCache() {
	//just in case - we return here if we shouldn't be using a texture cache
	if( bUseTextureCache == false ){
		return;
	}

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
                                                       nullptr,                    // CFDictionaryRef textureAttributes
                                                       texData.textureTarget,   // GLenum target
                                                       texData.glInternalFormat,  // GLint internalFormat
                                                       texData.width,           // GLsizei width
                                                       texData.height,          // GLsizei height
                                                       GL_BGRA,                 // GLenum format
                                                       GL_UNSIGNED_BYTE,        // GLenum type
                                                       0,                       // size_t planeIndex
                                                       &_videoTextureRef);      // CVOpenGLESTextureRef *textureOut

    textureCacheID = CVOpenGLESTextureGetName(_videoTextureRef);

#endif

#ifdef TARGET_OSX

    err = CVOpenGLTextureCacheCreateTextureFromImage(nullptr,
                                                     _videoTextureCache,
                                                     imageBuffer,
                                                     nullptr,
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
        ofLogError("ofAVFoundationPlayer") << "initTextureCache(): error creating texture cache from image " << err << ".";
    }

    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

#ifdef TARGET_OF_IOS

    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = nullptr;
    }

#endif

#ifdef TARGET_OSX

    CVOpenGLTextureCacheFlush(_videoTextureCache, 0);
    if(_videoTextureRef) {
        CVOpenGLTextureRelease(_videoTextureRef);
        _videoTextureRef = nullptr;
    }

#endif
}

void ofAVFoundationPlayer::killTexture() {
#ifdef TARGET_OF_IOS
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = nullptr;
    }
#elif defined TARGET_OSX
    if (_videoTextureRef != nullptr) {
        CVOpenGLTextureRelease(_videoTextureRef);
        _videoTextureRef = nullptr;
    }
#endif
}

void ofAVFoundationPlayer::killTextureCache() {

    killTexture();

#ifdef TARGET_OF_IOS
    if(_videoTextureCache) {
        CFRelease(_videoTextureCache);
        _videoTextureCache = nullptr;
    }
#endif

#ifdef TARGET_OSX
    if(_videoTextureCache != nullptr) {
        CVOpenGLTextureCacheRelease(_videoTextureCache);
        _videoTextureCache = nullptr;
    }

#endif
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getWidth() const {
    if(videoPlayer == nullptr) {
        return 0;
    }

    return [videoPlayer getWidth];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getHeight() const {
    if(videoPlayer == nullptr) {
        return 0;
    }

    return [videoPlayer getHeight];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPaused() const {
    if(videoPlayer == nullptr) {
        return false;
    }

    return ![videoPlayer isPlaying];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isLoaded() const {
    if(videoPlayer == nullptr) {
        return false;
    }

    return [videoPlayer isLoaded];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isReady() const {
    if(videoPlayer == nullptr) {
        return false;
    }

    return [videoPlayer isReady];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::isPlaying() const {
    if(videoPlayer == nullptr) {
        return false;
    }

    return [videoPlayer isPlaying];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getPosition() const {
    if(videoPlayer == nullptr) {
        return 0;
    }

    return [videoPlayer getPosition];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getSpeed() const {
    if(videoPlayer == nullptr) {
        return 0;
    }

    return [videoPlayer getSpeed];
}

//--------------------------------------------------------------
float ofAVFoundationPlayer::getDuration() const {
    if(videoPlayer == nullptr) {
        return 0;
    }

    return [videoPlayer getDurationInSec];
}

//--------------------------------------------------------------
bool ofAVFoundationPlayer::getIsMovieDone() const {
    if(videoPlayer == nullptr) {
        return false;
    }

    return [videoPlayer isFinished];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setPaused(bool bPause) {
    if(videoPlayer == nullptr) {
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
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer setPosition:pct];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setVolume(float volume) {
    if(videoPlayer == nullptr) {
        return;
    }
    if(volume > 1.0) {
        ofLogWarning("ofAVFoundationPlayer") << "setVolume(): expected range is 0-1, limiting requested volume " << volume << " to 1.0.";
        volume = 1.0;
    }
    [videoPlayer setVolume:volume];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setLoopState(ofLoopType state) {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer setLoop:(playerLoopType)state];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setSpeed(float speed) {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer setSpeed:speed];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::setFrame(int frame) {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer setFrame:frame];
}

//--------------------------------------------------------------
int	ofAVFoundationPlayer::getCurrentFrame() const {
    if(videoPlayer == nullptr){
        return 0;
    }
    return [videoPlayer getCurrentFrameNum];
}

//--------------------------------------------------------------
int	ofAVFoundationPlayer::getTotalNumFrames() const {
    if(videoPlayer == nullptr){
        return 0;
    }
    return [videoPlayer getDurationInFrames];
}

//--------------------------------------------------------------
ofLoopType	ofAVFoundationPlayer::getLoopState() const {
    if(videoPlayer == nullptr) {
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
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer setPosition:0];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::nextFrame() {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer stepByCount:1];
}

//--------------------------------------------------------------
void ofAVFoundationPlayer::previousFrame() {
    if(videoPlayer == nullptr) {
        return;
    }

    [videoPlayer stepByCount:-1];
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
bool ofAVFoundationPlayer::loadMovie(std::string name) {
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
