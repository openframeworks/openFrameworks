#import "ofiPhoneVideoPlayer.h"
#import "ofxiPhoneExtras.h"
#import "AVFoundationVideoPlayer.h"

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
	
	if(videoPlayer != NULL)
	{
		CVImageBufferRef imageBuffer = [(AVFoundationVideoPlayer *)videoPlayer getCurrentFrame]; 
		CVPixelBufferLockBaseAddress(imageBuffer,0); 
        size_t widthIn = CVPixelBufferGetWidth(imageBuffer);
        size_t heightIn = CVPixelBufferGetHeight(imageBuffer);
		CVPixelBufferUnlockBaseAddress(imageBuffer,0); // unlock the image buffer

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
