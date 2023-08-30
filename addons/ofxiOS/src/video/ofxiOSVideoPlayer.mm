#include "ofxiOSVideoPlayer.h"
#include "ofxiOSExtras.h"
#include "ofxiOSEAGLView.h"
#import "AVFoundationVideoPlayer.h"
#include "ofGLUtils.h"
#include "ofMath.h"

using std::string;

CVOpenGLESTextureCacheRef _videoTextureCache = NULL;
CVOpenGLESTextureRef _videoTextureRef = NULL;

ofxiOSVideoPlayer::ofxiOSVideoPlayer() {
	videoPlayer = NULL;
	pixelFormat = OF_PIXELS_RGBA;
	
    bFrameNew = false;
    bResetPixels = false;
    bUpdatePixels = false;
    bUpdateTexture = false;
    bTextureCacheSupported = (&CVOpenGLESTextureCacheCreate != NULL);
    bTextureCacheEnabled = true;
}

//----------------------------------------
ofxiOSVideoPlayer::~ofxiOSVideoPlayer() {
	close();
}

//----------------------------------------
void ofxiOSVideoPlayer::enableTextureCache() {
    bTextureCacheEnabled = true;
}

void ofxiOSVideoPlayer::disableTextureCache() {
    bTextureCacheEnabled = false;
    killTextureCache();
}

//----------------------------------------
bool ofxiOSVideoPlayer::load(string name) {
	
    if(!videoPlayer) {
        videoPlayer = (__bridge_retained void *)[[AVFoundationVideoPlayer alloc] init];
        [(__bridge AVFoundationVideoPlayer *)videoPlayer setWillBeUpdatedExternally:YES];
    }
    
    NSString * videoPath = [NSString stringWithUTF8String:ofToDataPath(name).c_str()];
    [(__bridge AVFoundationVideoPlayer*)videoPlayer loadWithPath:videoPath];
    
    bResetPixels = true;
    bUpdatePixels = true;
    bUpdateTexture = true;
    
    if(bTextureCacheSupported == true && bTextureCacheEnabled == true) {
        if(_videoTextureCache == NULL) {
#ifdef __IPHONE_6_0
            CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                        NULL,
                                                        ofxiOSGetGLView().context,
                                                        NULL,
                                                        &_videoTextureCache);
#else
            CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault,
                                                        NULL,
                                                        (__bridge void *)ofxiOSGetGLView().context,
                                                        NULL,
                                                        &_videoTextureCache);
#endif
            if(err) {
                ofLogWarning("ofxiOSVideoPlayer::load()") << "Error at CVOpenGLESTextureCacheCreate " << err;
            }
        }
    }
    
    return true;
}

//----------------------------------------
void ofxiOSVideoPlayer::close() {
	if(videoPlayer != NULL) {
		
        pixels.clear();
        
        videoTexture.clear();
		
        ((__bridge AVFoundationVideoPlayer *)videoPlayer).delegate = nil;
        
        __autoreleasing AVFoundationVideoPlayer *player = (__bridge_transfer AVFoundationVideoPlayer *)videoPlayer;
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

//----------------------------------------
bool ofxiOSVideoPlayer::setPixelFormat(ofPixelFormat value) {
    bool bValid = false;
    bValid = bValid || (value == OF_PIXELS_RGB);
    bValid = bValid || (value == OF_PIXELS_RGBA);
    
    if(bValid == false) {
        ofLogWarning("ofxiOSVideoPlayer::setPixelFormat()") << "unsupported ofPixelFormat " << ofToString(value);
        return false;
    }
    
    if(pixelFormat == value) {
        return true;
    }
    
    pixelFormat = value;
    bResetPixels = true;
    
	return true;
}


//---------------------------------------------------------------------------
ofPixelFormat ofxiOSVideoPlayer::getPixelFormat() const {
	return pixelFormat;
}

//----------------------------------------
void ofxiOSVideoPlayer::update() {
    
    bFrameNew = false; // default.
    
    if(!isLoaded()) {
        return;
    }
    
    [(__bridge AVFoundationVideoPlayer *)videoPlayer update];
    bFrameNew = [(__bridge AVFoundationVideoPlayer *)videoPlayer isNewFrame]; // check for new frame staright after the call to update.
    
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

//----------------------------------------
void ofxiOSVideoPlayer::play() {
    if(videoPlayer == NULL) {
        ofLogWarning("ofxiOSVideoPlayer::play()") << "video not loaded";
    }
    
	[(__bridge AVFoundationVideoPlayer *)videoPlayer play];
}

//----------------------------------------
void ofxiOSVideoPlayer::stop() {
    if(videoPlayer == NULL) {
        return;
    }
    
    [(__bridge AVFoundationVideoPlayer *)videoPlayer pause];
    [(__bridge AVFoundationVideoPlayer *)videoPlayer setPosition:0];
}		

//----------------------------------------
bool ofxiOSVideoPlayer::isFrameNew() const {
	if(videoPlayer != NULL) {
		return bFrameNew;
	}	
	return false;
}

//----------------------------------------
ofPixels & ofxiOSVideoPlayer::getPixels() {
    if(isLoaded() == false) {
        ofLogError("ofxiOSVideoPlayer::getPixels()") << "Returning pixels that may be unallocated. Make sure to initialize the video player before calling getPixels.";
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
    
    CVImageBufferRef imageBuffer = [(__bridge AVFoundationVideoPlayer *)videoPlayer getCurrentFrame];
    
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
            ofLogError("ofxiOSVideoPlayer::getPixels()") << "OF_PIXELS_RGB is not supported, use setPixelFormat() to set the pixel format to OF_PIXELS_RGBA";
#endif
        }
    }
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    if(err != kvImageNoError) {
        ofLogError("ofxiOSVideoPlayer::getPixels()") << "Error in Pixel Copy vImage_error " << err;
    }
    
    bUpdatePixels = false;
    
    return pixels;
}

//----------------------------------------
const ofPixels & ofxiOSVideoPlayer::getPixels() const {
    return const_cast<ofxiOSVideoPlayer*>(this)->getPixels();
}

//----------------------------------------
ofTexture * ofxiOSVideoPlayer::getTexturePtr() {
    
    if(isLoaded() == false) {
        return &videoTexture;
    }
    
    if(bUpdateTexture == false) {
        return &videoTexture;
    }

    if(bTextureCacheSupported == true && bTextureCacheEnabled == true) {
        
        initTextureCache();
        
    } else {

        /**
         *  no video texture cache.
         *  load texture from pixels.
         *  this method is the slower alternative.
         */
        
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        
        if([(__bridge AVFoundationVideoPlayer *)videoPlayer getWidth] > maxTextureSize ||
           [(__bridge AVFoundationVideoPlayer *)videoPlayer getHeight] > maxTextureSize) {
            ofLogWarning("ofxiOSVideoPlayer::getTexturePtr()")
            << [(__bridge AVFoundationVideoPlayer *)videoPlayer getWidth] << "x" << [(__bridge AVFoundationVideoPlayer *)videoPlayer getHeight]
            << " video image is bigger then max supported texture size " << maxTextureSize;
            return NULL;
        }
        
        videoTexture.loadData(getPixels());
    }
    
    bUpdateTexture = false;
    
    return &videoTexture;
}

//---------------------------------------- texture cache
void ofxiOSVideoPlayer::initTextureCache() {

    CVImageBufferRef imageBuffer = [(__bridge AVFoundationVideoPlayer *)videoPlayer getCurrentFrame];
    if(imageBuffer == nil) {
        return;
    }
    
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    
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
    
    int videoTextureW = [(__bridge AVFoundationVideoPlayer *)videoPlayer getWidth];
    int videoTextureH = [(__bridge AVFoundationVideoPlayer *)videoPlayer getHeight];
    videoTexture.allocate(videoTextureW, videoTextureH, GL_RGBA);
    
    ofTextureData & texData = videoTexture.getTextureData();
    texData.tex_t = 1.0f; // these values need to be reset to 1.0 to work properly.
    texData.tex_u = 1.0f; // assuming this is something to do with the way ios creates the texture cache.
    
    /**
     *  create video texture from video image.
     *  inside this function, ios is creating the texture for us.
     *  a video texture reference is returned.
     */
    CVReturn err;
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,     // CFAllocatorRef allocator
                                                       _videoTextureCache,      // CVOpenGLESTextureCacheRef textureCache
                                                       imageBuffer,             // CVImageBufferRef sourceImage
                                                       NULL,                    // CFDictionaryRef textureAttributes
                                                       texData.textureTarget,   // GLenum target
                                                       texData.glInternalFormat,  // GLint internalFormat
                                                       texData.width,           // GLsizei width
                                                       texData.height,          // GLsizei height
                                                       GL_BGRA,                 // GLenum format
                                                       GL_UNSIGNED_BYTE,        // GLenum type
                                                       0,                       // size_t planeIndex
                                                       &_videoTextureRef);      // CVOpenGLESTextureRef *textureOut
    
    unsigned int textureCacheID = CVOpenGLESTextureGetName(_videoTextureRef);
    videoTexture.setUseExternalTextureID(textureCacheID);
    videoTexture.setTextureMinMagFilter(GL_LINEAR, GL_LINEAR);
    videoTexture.setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    if(!ofIsGLProgrammableRenderer()) {
        videoTexture.bind();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        videoTexture.unbind();
    }
    
    if(err) {
        ofLogError("ofxiOSVideoPlayer::initTextureCache()") << "Error at CVOpenGLESTextureCacheCreateTextureFromImage " << err;
    }
    
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
    
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }
}

void ofxiOSVideoPlayer::killTextureCache() {
    if(_videoTextureRef) {
        CFRelease(_videoTextureRef);
        _videoTextureRef = NULL;
    }

    if(_videoTextureCache) {
        CFRelease(_videoTextureCache);
        _videoTextureCache = NULL;
    }
}

//----------------------------------------
float ofxiOSVideoPlayer::getWidth() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getWidth];
}

//----------------------------------------
float ofxiOSVideoPlayer::getHeight() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getHeight];
}

//----------------------------------------
bool ofxiOSVideoPlayer::isPaused() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return ![((__bridge AVFoundationVideoPlayer *)videoPlayer) isPlaying];
}

//----------------------------------------
bool ofxiOSVideoPlayer::isLoaded() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) isReady];
}

//----------------------------------------
bool ofxiOSVideoPlayer::isPlaying() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) isPlaying];
}

//----------------------------------------
float ofxiOSVideoPlayer::getPosition() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getPosition];
}

//----------------------------------------
float ofxiOSVideoPlayer::getSpeed() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getSpeed];
}

//----------------------------------------
float ofxiOSVideoPlayer::getDuration() const {
    if(videoPlayer == NULL) {
        return 0;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getDurationInSec];
}

//----------------------------------------
bool ofxiOSVideoPlayer::getIsMovieDone() const {
    if(videoPlayer == NULL) {
        return false;
    }
    
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) isFinished];
}

//----------------------------------------
void ofxiOSVideoPlayer::setPaused(bool bPause) {
    if(videoPlayer == NULL) {
        return;
    }
    
    if(bPause) {
        [((__bridge AVFoundationVideoPlayer *)videoPlayer) pause];
    } else {
        [((__bridge AVFoundationVideoPlayer *)videoPlayer) play];
    }
}

//----------------------------------------
void ofxiOSVideoPlayer::setPosition(float pct) {
    if(videoPlayer == NULL) {
        return;
    }
    
    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setPosition:pct];
}

//----------------------------------------
void ofxiOSVideoPlayer::setVolume(float volume) {
    if(videoPlayer == NULL) {
        return;
    }
	if(volume > 1.0f) {
		ofLogWarning("ofxiOSVideoPlayer::setVolume()") << "expected range is 0-1, limiting requested volume " << volume << " to 1.0";
		volume = 1.0f;
	}
    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setVolume:volume];
}

//----------------------------------------
void ofxiOSVideoPlayer::setLoopState(ofLoopType state) {
    if(videoPlayer == NULL) {
        return;
    }
    
    bool bLoop = false;
    if((state == OF_LOOP_NORMAL) || 
       (state == OF_LOOP_PALINDROME)) {
        bLoop = true;
    }
    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setLoop:bLoop];
}

//----------------------------------------
void ofxiOSVideoPlayer::setSpeed(float speed) {
    if(videoPlayer == NULL) {
        return;
    }
    
    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setSpeed:speed];
}

//----------------------------------------
void ofxiOSVideoPlayer::setFrame(int frame) {
    if(videoPlayer == NULL) {
        return;
    }

    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setFrame:frame];
}

//----------------------------------------
int	ofxiOSVideoPlayer::getCurrentFrame() const {
    if(videoPlayer == NULL){
        return 0;
    }
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getCurrentFrameNum];
}

//----------------------------------------
int	ofxiOSVideoPlayer::getTotalNumFrames() const {
    if(videoPlayer == NULL){
        return 0;
    }
    return [((__bridge AVFoundationVideoPlayer *)videoPlayer) getDurationInFrames];
}

//----------------------------------------
ofLoopType	ofxiOSVideoPlayer::getLoopState() const {
    if(videoPlayer == NULL) {
        return OF_LOOP_NONE;
    }
    
    bool bLoop =  [((__bridge AVFoundationVideoPlayer *)videoPlayer) getLoop];
    if(bLoop) {
        return OF_LOOP_NORMAL;
    }
    return OF_LOOP_NONE;
}

//----------------------------------------
void ofxiOSVideoPlayer::firstFrame() {
    if(videoPlayer == NULL) {
        return;
    }
    
    [((__bridge AVFoundationVideoPlayer *)videoPlayer) setPosition:0];
}

//----------------------------------------
void ofxiOSVideoPlayer::nextFrame() {
    int nextFrameNum = ofClamp(getCurrentFrame() + 1, 0, getTotalNumFrames());
    setFrame(nextFrameNum);
}

//----------------------------------------
void ofxiOSVideoPlayer::previousFrame() {
    int prevFrameNum = ofClamp(getCurrentFrame() - 1, 0, getTotalNumFrames());
    setFrame(prevFrameNum);
}

//----------------------------------------
void * ofxiOSVideoPlayer::getAVFoundationVideoPlayer() {
    return videoPlayer;
}

//---------------------------------------- DEPRECATED.
bool ofxiOSVideoPlayer::loadMovie(string name) {
    return load(name);
}

ofPixels & ofxiOSVideoPlayer::getPixelsRef() {
    return getPixels();
}

const ofPixels & ofxiOSVideoPlayer::getPixelsRef() const {
    return getPixels();
}

ofTexture * ofxiOSVideoPlayer::getTexture() {
    return getTexturePtr();
}

