#import "ofiPhoneVideoPlayer.h"
#import "ofxiPhoneExtras.h"
#import "AVFoundationVideoPlayer.h"

#ifdef __IPHONE_5_0
CVOpenGLESTextureCacheRef _videoTextureCache;
CVOpenGLESTextureRef _videoTextureRef;
#endif

ofiPhoneVideoPlayer::ofiPhoneVideoPlayer() {
	videoPlayer=NULL;
	pixels = NULL;
	pixelsTmp = NULL;
	
	videoWasStopped=false;

    bUpdatePixels = false;
    bFrameNew = false;
    frameCount = -1;
	
	width = 0;
	height = 0;
	playbackSpeed=1;
	
	vol = 100;
}

//----------------------------------------

ofiPhoneVideoPlayer::~ofiPhoneVideoPlayer() {
	close();
}

//----------------------------------------

bool ofiPhoneVideoPlayer::loadMovie(string name) {
	
	if(videoPlayer != NULL)
		close();
	
	videoPath = name;
	initWithPath(videoPath);
    
#ifdef __IPHONE_5_0
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, 
                                                NULL, 
                                                (__bridge void *)ofxiPhoneGetGLView().context,
                                                NULL, 
                                                &_videoTextureCache);
    if(err) {
        NSLog(@"Error at CVOpenGLESTextureCacheCreate %d", err);
    }    
#endif
	
	if(videoPlayer != NULL)
		if(! [(AVFoundationVideoPlayer *)videoPlayer isInErrorState])
			return true;
	return false;
}

//----------------------------------------

void ofiPhoneVideoPlayer::setPixelFormat(ofPixelFormat _internalPixelFormat) {
	if(_internalPixelFormat == OF_PIXELS_RGB)
		internalGLFormat = GL_RGB;
	else if(_internalPixelFormat == OF_PIXELS_RGBA)
		internalGLFormat = GL_RGBA;
	else if(_internalPixelFormat == OF_PIXELS_BGRA)
		internalGLFormat = GL_BGRA;
}

//----------------------------------------

void ofiPhoneVideoPlayer::close() {
	if(videoPlayer != NULL) {
		
		if(pixelsTmp != NULL) {
			free(pixelsTmp);
			pixelsTmp = NULL;
		}
		if(pixels != NULL) {
			free(pixels);
			pixels = NULL;
		}
		
		width = height = 0;
		
		[(AVFoundationVideoPlayer *)videoPlayer release];
        
#ifdef __IPHONE_5_0
        if(_videoTextureRef) {
            CFRelease(_videoTextureRef);
            _videoTextureRef = NULL;
        }
        if(_videoTextureCache) {
            CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
            CFRelease(_videoTextureCache);
        }
#endif
	}
	videoPlayer = NULL;
}

//----------------------------------------

void ofiPhoneVideoPlayer::play() {
	
	lastUpdateTime=ofGetElapsedTimef();
	
	if(videoPlayer != NULL)
		[(AVFoundationVideoPlayer *)videoPlayer play];
	else if(videoWasStopped || getIsMovieDone()) {
		[(AVFoundationVideoPlayer *)videoPlayer release];
		initWithPath(videoPath);
		play();
	}
	else
		ofLog(OF_LOG_WARNING, "ofiPhoneVideoPlayer: video is not loaded, cannot be played");
}

//----------------------------------------

void ofiPhoneVideoPlayer::stop() {
	if(videoPlayer != NULL) {
		[(AVFoundationVideoPlayer *)videoPlayer pause];
		close();
		videoWasStopped=true;
	}
	
	ofLog(OF_LOG_WARNING, "ofiPhoneVideoPlayer: video is not loaded, cannot be stopped");
}		

//----------------------------------------

bool ofiPhoneVideoPlayer::isFrameNew() {
	if(videoPlayer != NULL) {
		return bFrameNew;
	}	
	return false;
}

//----------------------------------------

unsigned char * ofiPhoneVideoPlayer::getPixels() {

	if(videoPlayer != NULL && isPlaying())
	{
        if(!bUpdatePixels) { // if pixels have not changed, return the already calculated pixels.
            return pixels;
        }
        
		CGImageRef currentFrameRef;
		
		NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
		
		CVImageBufferRef imageBuffer = [(AVFoundationVideoPlayer *)videoPlayer getCurrentFrame]; 
        [(AVFoundationVideoPlayer *)videoPlayer updateFrameTimeDifference];
        
		/*Lock the image buffer*/
		CVPixelBufferLockBaseAddress(imageBuffer,0); 
		
		/*Get information about the image*/
		uint8_t *baseAddress	= (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer); 
		size_t bytesPerRow		= CVPixelBufferGetBytesPerRow(imageBuffer); 
		size_t widthIn			= CVPixelBufferGetWidth(imageBuffer); 
		size_t heightIn			= CVPixelBufferGetHeight(imageBuffer);  
		
		/*Create a CGImageRef from the CVImageBufferRef*/
		CGColorSpaceRef colorSpace	= CGColorSpaceCreateDeviceRGB(); 
		CGContextRef newContext		= CGBitmapContextCreate(baseAddress, widthIn, heightIn, 8, bytesPerRow, colorSpace, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
		CGImageRef newImage			= CGBitmapContextCreateImage(newContext); 
		
		currentFrameRef = CGImageCreateCopy(newImage);		
		
		/*We release some components*/
		CGContextRelease(newContext); 
		CGColorSpaceRelease(colorSpace);
		
		/*We relase the CGImageRef*/
		CGImageRelease(newImage);
		
		/*We unlock the  image buffer*/
		CVPixelBufferUnlockBaseAddress(imageBuffer,0);
		
		if(width==0 && widthIn != 0  && pixels == NULL) {
			if(internalGLFormat == GL_RGB)
				pixels = (GLubyte *) malloc(widthIn * heightIn * 3);
			else
				pixels = (GLubyte *) malloc(widthIn * heightIn * 4);
			
			pixelsTmp	= (GLubyte *) malloc(widthIn * heightIn * 4);
		}
		
		width = widthIn;
		height = heightIn;
		[pool drain];
		
		if(width != 0) {
			//ofxiPhoneCGImageToPixels(currentFrameRef, pixels);
			
			CGContextRef spriteContext;
			
			spriteContext = CGBitmapContextCreate(pixelsTmp, width, height, CGImageGetBitsPerComponent(currentFrameRef), width * 4, CGImageGetColorSpace(currentFrameRef), kCGImageAlphaPremultipliedLast);
						
			CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), currentFrameRef);
			
			CGContextRelease(spriteContext);
			
			if(internalGLFormat == GL_RGB)
			{
				unsigned int *isrc4 = (unsigned int *)pixelsTmp;
				unsigned int *idst3 = (unsigned int *)pixels;
				unsigned int *ilast4 = &isrc4[width*height-1];
				while (isrc4 < ilast4){
					*(idst3++) = *(isrc4++);
					idst3 = (unsigned int *) (((unsigned char *) idst3) - 1);
				}
			}
			else if(internalGLFormat == GL_RGBA || internalGLFormat == GL_BGRA)
				memcpy(pixels, pixelsTmp, width*height*4);
			
			CGImageRelease(currentFrameRef);
		}
		
        bUpdatePixels = false;
        
		return pixels;
	}
	
	return NULL;
}

ofTexture * ofiPhoneVideoPlayer::getTexture()
{
	if(videoPlayer != NULL) {

		CVImageBufferRef imageBuffer = [(AVFoundationVideoPlayer *)videoPlayer getCurrentFrame]; 
		
        CVPixelBufferLockBaseAddress(imageBuffer,0); 
        
        size_t widthIn = CVPixelBufferGetWidth(imageBuffer);
        size_t heightIn = CVPixelBufferGetHeight(imageBuffer);
        
#ifdef __IPHONE_5_0        
        if(_videoTextureCache) {
            /**
             *  video texture cache is available.
             *  this means we don't have to copy any pixels,
             *  and we can reuse the already existing video texture.
             *  this is very fast! :)
             */
            
            if(_videoTextureRef) {
                CFRelease(_videoTextureRef);
                _videoTextureRef = NULL;
            }
            CVOpenGLESTextureCacheFlush(_videoTextureCache, 0); // Periodic texture cache flush every frame

            /**
             *  CVOpenGLESTextureCache does this operation for us.
             *  it automatically returns a texture reference which means we don't have to create the texture ourselves.
             *  but we do want to return an ofTexture object...
             *  this creates a slight problem because when we create an ofTexture objects, it also creates a opengl texture for us,
             *  which is unecessary in this case because the texture already exists.
             *  so... the below is somewhat of a hack to reuse an existing texture and give it to a ofTexture object.
             */
            
            ofTextureData texData;
            texData.tex_w = texData.width = widthIn;
            texData.tex_h = texData.height = heightIn;
            texData.tex_w = ofNextPow2(texData.tex_w);
            texData.tex_h = ofNextPow2(texData.tex_h);
            texData.tex_t = 1.0f;
            texData.tex_u = 1.0f;
            texData.textureTarget = GL_TEXTURE_2D;
            texData.glTypeInternal = GL_RGBA; // opengl format
            texData.glType = GL_BGRA; // native iOS format
            texData.pixelType = GL_UNSIGNED_BYTE;
            texData.bAllocated = true;
            
            glActiveTexture(GL_TEXTURE0);
            
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
                                                               texData.glTypeInternal,  // GLint internalFormat
                                                               texData.width,           // GLsizei width
                                                               texData.height,          // GLsizei height
                                                               texData.glType,          // GLenum format
                                                               texData.pixelType,       // GLenum type
                                                               0,                       // size_t planeIndex
                                                               &_videoTextureRef);      // CVOpenGLESTextureRef *textureOut
            
            /**
             *  get the generated textureID and textureTarget.
             *  configure the texture (this is from inside ofTexture::allocate).
             *  and add give it to an ofTexture object.
             */
            texData.textureID = CVOpenGLESTextureGetName(_videoTextureRef);
            texData.textureTarget = CVOpenGLESTextureGetTarget(_videoTextureRef);
            
            glEnable(texData.textureTarget);
            glBindTexture(texData.textureTarget, (GLuint)texData.textureID);
            glTexParameterf(texData.textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(texData.textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(texData.textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glDisable(texData.textureTarget);
            
            videoTexture.texData = texData;

            if(err) {
                NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
            }            
        } else 
#endif            
        {
            /**
             *  no video texture cache.
             *  load texture from pixels.
             *  this method is the slower alternative.
             */
            
            int maxTextureSize = 0;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
            
            if((int)widthIn > maxTextureSize || (int)heightIn > maxTextureSize) {
                ofLog(OF_LOG_WARNING, "ofiPhoneVideoPlayer::getTexture() - video image is bigger then supported texture size");
            }
            
            widthIn = min(size_t(maxTextureSize), widthIn);
            heightIn = min(size_t(maxTextureSize), heightIn);
            
            if((width != widthIn) || (height != heightIn)) {
                
                if(videoTexture.bAllocated()) {
                    videoTexture.clear();
                }
				
                if(width == 0 && widthIn != 0 && pixels == NULL) {
                    
                    if(internalGLFormat == GL_RGB) {
                        pixels = (GLubyte *)malloc(widthIn * heightIn * 3);
                    } else {
                        pixels = (GLubyte *)malloc(widthIn * heightIn * 4);
                    }
                    pixelsTmp = (GLubyte *)malloc(widthIn * heightIn * 4);
                }				
				
                width = widthIn;
                height = heightIn;
                videoTexture.allocate(width, height, internalGLFormat);
            }
            
            videoTexture.loadData(getPixels(), width, height, internalGLFormat);
        }
        
        CVPixelBufferUnlockBaseAddress(imageBuffer,0); // unlock the image buffer
		
		return &videoTexture;
	}
	
	return NULL;
}

//----------------------------------------

float ofiPhoneVideoPlayer::getWidth() {
	if(videoPlayer != NULL)
		return width;
	
	ofLog(OF_LOG_WARNING, "ofiPhoneVideoPlayer: video is not loaded, cannot getWidth");
	return 0;
}

//----------------------------------------

float ofiPhoneVideoPlayer::getHeight() {
	if(videoPlayer != NULL)
		return height;
	
	ofLog(OF_LOG_WARNING, "ofiPhoneVideoPlayer: video is not loaded, cannot getHeigt");
	return 0;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isPaused() {
	if(videoPlayer != NULL)
		return [(AVFoundationVideoPlayer *)videoPlayer isPaused];
	
	return false;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isLoaded() {
	if(videoPlayer != NULL && ! [(AVFoundationVideoPlayer *)videoPlayer isInErrorState])
		return true;
	else
		return false;
}

//----------------------------------------

bool ofiPhoneVideoPlayer::isPlaying() {
	if(videoPlayer != NULL) {
		if([(AVFoundationVideoPlayer *)videoPlayer isFinished] || [(AVFoundationVideoPlayer *)videoPlayer isPaused] || [(AVFoundationVideoPlayer *)videoPlayer isInErrorState])
			return false;
		else
			return true;
	}
	
	return false;
}

void ofiPhoneVideoPlayer::update() {
    
    bFrameNew = false; // default.
    
    if(videoPlayer == NULL) {
        return;
    }
    
    if(frameCount == ofGetFrameNum()) {
        /**
         *  the player has already been updated on this frame.
         *  this is to prevent update being called multiple times on the same frame.
         */
        return;
    } else {
        frameCount = ofGetFrameNum();
    }
    
    float t = ofGetElapsedTimef();
    [(AVFoundationVideoPlayer *)videoPlayer updateWithElapsedTime:(t-lastUpdateTime)*playbackSpeed];
    lastUpdateTime=t;
    
    bFrameNew = [(AVFoundationVideoPlayer *)videoPlayer hasNewFrame]; // check for new frame staright after the call to update.
    
    if(bFrameNew) {
        /**
         *  mark pixels to be updated.
         *  pixels are then only updated if the getPixels() method is called,
         *  internally or externally to this class.
         *  this ensures the pixels are updated only once per frame.
         */
        bUpdatePixels = true;
    }
}

float ofiPhoneVideoPlayer::getPosition() {
	if(videoPlayer != NULL)
		return [(AVFoundationVideoPlayer *)videoPlayer getVideoPosition];
	else
	return 0;
}

float ofiPhoneVideoPlayer::getDuration() {
	if(videoPlayer != NULL)
		return [(AVFoundationVideoPlayer *)videoPlayer getDuration];
	else
		return 0;

}

bool ofiPhoneVideoPlayer::getIsMovieDone() {
	if(videoPlayer != NULL)
		return [(AVFoundationVideoPlayer *)videoPlayer isFinished];
	else
		return true;
}

void ofiPhoneVideoPlayer::setPaused(bool bPause) {
	if(bPause)
		[(AVFoundationVideoPlayer *)videoPlayer pause];
	else {
		if([(AVFoundationVideoPlayer *)videoPlayer isPaused])
			[(AVFoundationVideoPlayer *)videoPlayer play];
	}
}

void ofiPhoneVideoPlayer::setVolume(int volume) {
	vol = volume;
	if(videoPlayer != NULL)
		[(AVFoundationVideoPlayer *)videoPlayer setVolume:(float)volume/100];
}

//----------------------------------------

void ofiPhoneVideoPlayer::initWithPath(string path) {
	videoPlayer = [[AVFoundationVideoPlayer alloc] initWithPath:ofxStringToNSString(ofToDataPath(path))];
	videoWasStopped=false;
	setVolume(vol);
}


ofPixelsRef ofiPhoneVideoPlayer::getPixelsRef()
{
    static ofPixels dummy;
    return dummy;
}
