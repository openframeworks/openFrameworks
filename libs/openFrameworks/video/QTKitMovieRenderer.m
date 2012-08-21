
#import "QTKitMovieRenderer.h"
#import <Accelerate/Accelerate.h>

//secret methods!
@interface QTMovie (QTFrom763)
- (QTTime)frameStartTime: (QTTime)atTime;
- (QTTime)frameEndTime: (QTTime)atTime;
- (QTTime)keyframeStartTime:(QTTime)atTime;
@end

struct OpenGLTextureCoordinates
{
    GLfloat topLeft[2];
    GLfloat topRight[2];
    GLfloat bottomRight[2];
    GLfloat bottomLeft[2];
};

typedef struct OpenGLTextureCoordinates OpenGLTextureCoordinates;

@implementation QTKitMovieRenderer
@synthesize movieSize;
@synthesize useTexture;
@synthesize usePixels;
@synthesize useAlpha;
@synthesize frameCount;
@synthesize justSetFrame;
@synthesize synchronousScrub;

- (NSDictionary*) pixelBufferAttributes
{
    return [NSDictionary dictionaryWithObjectsAndKeys:
            //if we have a texture, make the pixel buffer OpenGL compatible
            [NSNumber numberWithBool:self.useTexture], (NSString*)kCVPixelBufferOpenGLCompatibilityKey, 
            //sorry for the ternary operator but it makes this way simpler
            [NSNumber numberWithInt: self.useAlpha ? kCVPixelFormatType_32ARGB : kCVPixelFormatType_24RGB], (NSString*)kCVPixelBufferPixelFormatTypeKey,
            nil];
}

- (BOOL) loadMovie:(NSString*)moviePath allowTexture:(BOOL)doUseTexture allowPixels:(BOOL)doUsePixels allowAlpha:(BOOL)doUseAlpha
{
    if(![[NSFileManager defaultManager] fileExistsAtPath:moviePath])
    {
		NSLog(@"No movie file found at %@", moviePath);
		return NO;
	}
	
	//create visual context
	useTexture = doUseTexture;
	usePixels = doUsePixels;
	useAlpha = doUseAlpha;
    
	NSError* error;
	NSMutableDictionary* movieAttributes = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                            [NSURL fileURLWithPath:[moviePath stringByStandardizingPath]], QTMovieURLAttribute,
                                            [NSNumber numberWithBool:NO], QTMovieOpenAsyncOKAttribute,
                                            nil];
    
	_movie = [[QTMovie alloc] initWithAttributes:movieAttributes 
										   error: &error];
	
	if(error || _movie == NULL){
		NSLog(@"Error Loading Movie: %@", error);
		return NO;
	}
    
	movieSize = [[_movie attributeForKey:QTMovieNaturalSizeAttribute] sizeValue];
    //	NSLog(@"movie size %f %f", movieSize.width, movieSize.height);
	
	movieDuration = [_movie duration];
    
	[_movie gotoBeginning];
    
    [_movie gotoEnd];
    QTTime endTime = [_movie currentTime];
    
    [_movie gotoBeginning];
    QTTime curTime = [_movie currentTime];
    
    long numFrames = 0;
    while (true)
    {
        int time = curTime.timeValue;
        //        % get the end time of the current frame  
        curTime = [_movie frameEndTime:curTime];
        numFrames++;
//        NSLog(@" num frames %ld current time %f", numFrames, 1.0*curTime.timeValue/curTime.timeScale);
        if (QTTimeCompare(curTime, endTime) == NSOrderedSame ||
            QTTimeCompare(curTime, [_movie frameEndTime:curTime])  == NSOrderedSame ){ //this will happen for audio files since they have no frames.
            break;
        }
    }
    
	frameCount = numFrames;
    frameStep = 1.0*movieDuration.timeValue/numFrames;
    
    
	//NSLog(@" movie has %d frames and frame step %d", frameCount, frameStep);
	
	//if we are using pixels, make the visual context
	//a pixel buffer context with ARGB textures
	if(self.usePixels){
		
		NSMutableDictionary *ctxAttributes = [NSMutableDictionary dictionaryWithObject:[self pixelBufferAttributes]
																				forKey:(NSString*)kQTVisualContextPixelBufferAttributesKey];
		
		OSStatus err = QTPixelBufferContextCreate(kCFAllocatorDefault, (CFDictionaryRef)ctxAttributes, &_visualContext);
		if(err){
			NSLog(@"error %ld creating OpenPixelBufferContext", err);
			return NO;
		}
        
		// if we also have a texture, create a texture cache for it
		if(self.useTexture){
			//create a texture cache			
			err = CVOpenGLTextureCacheCreate(kCFAllocatorDefault, NULL, 
											 CGLGetCurrentContext(), CGLGetPixelFormat(CGLGetCurrentContext()), 
											 (CFDictionaryRef)ctxAttributes, &_textureCache);
			if(err){
				NSLog(@"error %ld creating CVOpenGLTextureCacheCreate", err);
				return NO;
			}
		}
	}
	//if we are using a texture, just create an OpenGL visual context and call it a day
	else if(self.useTexture){
		OSStatus err = QTOpenGLTextureContextCreate(kCFAllocatorDefault,
													CGLGetCurrentContext(), CGLGetPixelFormat(CGLGetCurrentContext()),
													(CFDictionaryRef)NULL, &_visualContext);	
		if(err){
			NSLog(@"error %ld creating QTOpenGLTextureContextCreate", err);
			return NO;
		}
	}
	else {
		NSLog(@"Error - QTKitMovieRenderer - Must specify either Pixels or Texture as rendering strategy");
		return NO;
	}
	
	[_movie setVisualContext:_visualContext];
	
	self.volume = 1.0;
	self.loops = YES;
	
	return YES;
}

- (void) dealloc
{
    
	if(_latestTextureFrame != NULL){
		CVOpenGLTextureRelease(_latestTextureFrame);
		_latestTextureFrame = NULL;
	}
	
	if(_latestPixelFrame != NULL){
		CVPixelBufferRelease(_latestPixelFrame);
		_latestPixelFrame = NULL;
	}
	
	if(_movie != NULL){
		[_movie release];
		_movie = NULL;
	}
	
	if(_visualContext != NULL){
		QTVisualContextRelease(_visualContext);
		_visualContext = NULL;
	}
	
	if(_textureCache != NULL){
		CVOpenGLTextureCacheRelease(_textureCache);
		_textureCache = NULL;
	}
	
    
	[super dealloc];
}

//JG Note, in the OF wrapper this does not get used since we have a modified ofTexture that we use to draw
//this is here in case you want to use this renderer outside of openFrameworks
- (void) draw:(NSRect)drawRect
{   
	
	if(!self.useTexture || _latestTextureFrame == NULL){
		return;
	}
	
	OpenGLTextureCoordinates texCoords;	
	
	CVOpenGLTextureGetCleanTexCoords(_latestTextureFrame, 
									 texCoords.bottomLeft, 
									 texCoords.bottomRight, 
									 texCoords.topRight, 
									 texCoords.topLeft);        
	
	[self bindTexture];
	
	glBegin(GL_QUADS);
	
	glTexCoord2fv(texCoords.topLeft);
	glVertex2f(NSMinX(drawRect), NSMinY(drawRect));
	
	glTexCoord2fv(texCoords.topRight);
	glVertex2f(NSMaxX(drawRect), NSMinY(drawRect));
	
	glTexCoord2fv(texCoords.bottomRight);
	glVertex2f(NSMaxX(drawRect), NSMaxY(drawRect));
	
	glTexCoord2fv(texCoords.bottomLeft);
	glVertex2f(NSMinX(drawRect), NSMaxY(drawRect));
	
	glEnd();
	
	[self unbindTexture];
	
}


- (BOOL) update
{    
    
    if (_visualContext == NULL){
        return NO;
    }
    
	//SYNCHRONOUS SCRUB FIX NORMAL BEHAVIOR below -----
    //This is to help with synchronous updates from setting frames, 
    //so we are guarenteed to get the most recent frame if we have just set it explicitly
    if(self.justSetFrame && self.synchronousScrub){
        
        self.justSetFrame = NO; 
        NSError* nsError = nil;        
        if(self.usePixels){
            //pull the frame
            NSDictionary *dict = [NSDictionary 
                                  dictionaryWithObjectsAndKeys:QTMovieFrameImageTypeCVPixelBufferRef, QTMovieFrameImageType, 
                                  [NSValue valueWithSize:movieSize], QTMovieFrameImageSize,
                                  [NSNumber numberWithBool:YES], QTMovieFrameImageSessionMode,
                                  [NSNumber numberWithBool:YES], QTMovieFrameImageHighQuality,
                                  nil];
            CVPixelBufferRef theImage = (CVPixelBufferRef)[_movie frameImageAtTime:_movie.currentTime withAttributes:dict error:&nsError];
            OSType pixelType = CVPixelBufferGetPixelFormatType(theImage);
            if(theImage == NULL || nsError != nil){
                //                NSLog(@"Error pulling frame image %@", nsError);
                return NO;
            }
            
            if(_latestPixelFrame != NULL){
                CVPixelBufferRelease(_latestPixelFrame);
            }
            
            //frames from frameImageAtTime are auto-released retain it!
            _latestPixelFrame = theImage;
            CVPixelBufferRetain(theImage);
            
            //DEBUG timecode
            CVAttachmentMode mode = kCVAttachmentMode_ShouldPropagate;
            CFDictionaryRef timeDictionary = CVBufferGetAttachment (theImage, kCVBufferMovieTimeKey, &mode);
            //            NSLog(@"Image One current movie time: %f incoming frame time: %f", 1.0*_movie.currentTime.timeValue/_movie.currentTime.timeScale, [[(NSDictionary*)timeDictionary valueForKey:@"TimeValue"] floatValue] / [[(NSDictionary*)timeDictionary valueForKey:@"TimeScale"] floatValue]);
            
   			if(self.useTexture){
                if(_latestTextureFrame != NULL){
                    CVOpenGLTextureRelease(_latestTextureFrame);
                    _latestTextureFrame = NULL;
                    CVOpenGLTextureCacheFlush(_textureCache, 0);	
                }
                
                OSErr err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, _latestPixelFrame, NULL, &_latestTextureFrame);
                if(err != noErr){
                    NSLog(@"Error creating OpenGL texture %d ", err);
                    
                    return NO;
                }
            }
        }
        //just get the texture
        else if(self.useTexture){
            NSDictionary *dict = [NSDictionary 
                                  dictionaryWithObjectsAndKeys:QTMovieFrameImageTypeCVOpenGLTextureRef, QTMovieFrameImageType, 
                                  [NSValue valueWithSize:movieSize], QTMovieFrameImageSize,
                                  [NSNumber numberWithBool:YES], QTMovieFrameImageSessionMode,
                                  [NSNumber numberWithBool:YES], QTMovieFrameImageHighQuality,
                                  [NSValue valueWithPointer:CGLGetCurrentContext()], QTMovieFrameImageOpenGLContext,
                                  [NSValue valueWithPointer:CGLGetPixelFormat(CGLGetCurrentContext())], QTMovieFrameImagePixelFormat,
                                  nil];
            CVOpenGLTextureRef theImage = (CVOpenGLTextureRef)[_movie frameImageAtTime:_movie.currentTime withAttributes:dict error:&nsError];
            if(theImage == NULL || nsError != nil){
                NSLog(@"Error pulling CVOpenGLTextureRef image %@", nsError);
                return NO;
            }
            
            
            if(_latestTextureFrame != NULL){
                CVOpenGLTextureRelease(_latestTextureFrame);
            }
            _latestTextureFrame = theImage;
            
            //frames from frameImageAtTime are auto-released retain it!
            CVOpenGLTextureRetain(_latestTextureFrame);
        }
        QTVisualContextTask(_visualContext);	
        return YES;
    }
    
    //NORMAL BEHAVIOR below -----
    if (!QTVisualContextIsNewImageAvailable(_visualContext, NULL)){
		return NO;
	}
    
	QTVisualContextTask(_visualContext);	
	if(self.usePixels){
		if(_latestPixelFrame != NULL){
			CVPixelBufferRelease(_latestPixelFrame);
			_latestPixelFrame = NULL;
		}
		
		OSStatus error = QTVisualContextCopyImageForTime(_visualContext, NULL, NULL, &_latestPixelFrame);	
        
		if (error != noErr) {
			CVPixelBufferRelease(_latestPixelFrame);
			return NO;
		}
		
        //DEBUG timecode
        CVAttachmentMode mode = kCVAttachmentMode_ShouldPropagate;
        CFDictionaryRef timeDictionary = CVBufferGetAttachment (_latestPixelFrame, kCVBufferMovieTimeKey, &mode);
//        NSLog(@"movie time: %f incoming frame time: %f", 1.0*_movie.currentTime.timeValue/_movie.currentTime.timeScale, [[(NSDictionary*)timeDictionary valueForKey:@"TimeValue"] floatValue] / [[(NSDictionary*)timeDictionary valueForKey:@"TimeScale"] floatValue]);
        
		//if we are using a texture, create one from the texture cache
		if(self.useTexture){
			if(_latestTextureFrame != NULL){
				CVOpenGLTextureRelease(_latestTextureFrame);
				_latestTextureFrame = NULL;
				CVOpenGLTextureCacheFlush(_textureCache, 0);	
			}
			
			OSErr err = CVOpenGLTextureCacheCreateTextureFromImage(NULL, _textureCache, _latestPixelFrame, NULL, &_latestTextureFrame);
			if(err != noErr){
				NSLog(@"Error creating OpenGL texture %d", err);
				return NO;
			}
		}
	}
	//just get the texture
	else if(self.useTexture){
		if(_latestTextureFrame != NULL){
			CVOpenGLTextureRelease(_latestTextureFrame);
			_latestTextureFrame = NULL;
		}
		
		OSStatus error = QTVisualContextCopyImageForTime(_visualContext, NULL, NULL, &_latestTextureFrame);	
		if (error != noErr) {
			CVOpenGLTextureRelease(_latestTextureFrame);
			return NO;
		}
	}
    
	return YES;
}

- (void) stepForward
{
    if(_movie){
        [_movie stepForward];
        self.justSetFrame = YES;
    }
}

- (void) stepBackward
{
    if(_movie){
        [_movie stepBackward];
        self.justSetFrame = YES;
    }
}

- (void) gotoBeginning
{
	if(_movie){
    	[_movie gotoBeginning];
        self.justSetFrame = YES;
    }
}

//writes out the pixels in RGB or RGBA format to outbuf
- (void) pixels:(unsigned char*) outbuf
{
	if(!self.usePixels || _latestPixelFrame == NULL){
		return;
	}
	
	//NOTE:
	//CoreVideo works on ARGB, and openFrameworks is RGBA so we need to swizzle the buffer 
	//before we return it to an openFrameworks app.
	//this is a bit tricky since CV pixel buffer's bytes per row are not always the same as movieWidth*4.  
	//We have to use the BPR given by CV for the input buffer, and the movie size for the output buffer
    //Helpful debugging please leave in
//    NSLog(@"pixel buffer width is %ld height %ld and bpr %ld, movie size is %d x %d ",
//          CVPixelBufferGetWidth(_latestPixelFrame),
//          CVPixelBufferGetHeight(_latestPixelFrame), 
//          CVPixelBufferGetBytesPerRow(_latestPixelFrame),
//          (NSInteger)movieSize.width, (NSInteger)movieSize.height);
    if((NSInteger)movieSize.width != CVPixelBufferGetWidth(_latestPixelFrame) ||
       (NSInteger)movieSize.height != CVPixelBufferGetHeight(_latestPixelFrame)){
        NSLog(@"CoreVideo pixel buffer is %ld x %ld while QTKit Movie reports size of %d x %d. Ths is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
              CVPixelBufferGetWidth(_latestPixelFrame), CVPixelBufferGetHeight(_latestPixelFrame), (NSInteger)movieSize.width, (NSInteger)movieSize.height);
        return;
    }
    
    CVPixelBufferLockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
    //If we are using alpha, the ofQTKitPlayer class will have allocated a buffer of size
    //movieSize.width * movieSize.height * 4
    //CoreVieo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
    //so we need to swap the alpha around using a vImage permutation 
    if(self.useAlpha){
        vImage_Buffer src = { CVPixelBufferGetBaseAddress(_latestPixelFrame),
            				  CVPixelBufferGetHeight(_latestPixelFrame),
            				  CVPixelBufferGetWidth(_latestPixelFrame),
                              CVPixelBufferGetBytesPerRow(_latestPixelFrame)
        					};
        vImage_Buffer dest = { outbuf, movieSize.height, movieSize.width, movieSize.width*4 };
		uint8_t permuteMap[4] = { 1, 2, 3, 0 }; //swizzle the alpha around to the end to make ARGB -> RGBA
        vImage_Error err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
        if(err != kvImageNoError){
            NSLog(@"Error in Pixel Copy vImage_error %ld", err);
        }
    }
    //If we are are doing RGB, then we will have request RGB pixels from the video player
    //and the ofQTKitPlayer will have created a buffer of size movieSize.width * movieSize.height * 3
    //so we can just copy them straight into the outbuffer
    else {
        size_t dstBytesPerRow = movieSize.width * 3;
        if (CVPixelBufferGetBytesPerRow(_latestPixelFrame) == dstBytesPerRow) {
        	memcpy(outbuf, CVPixelBufferGetBaseAddress(_latestPixelFrame), dstBytesPerRow*CVPixelBufferGetHeight(_latestPixelFrame));
        }
        else {
            unsigned char *dst = outbuf;
            unsigned char *src = (unsigned char*)CVPixelBufferGetBaseAddress(_latestPixelFrame);
            size_t srcBytesPerRow = CVPixelBufferGetBytesPerRow(_latestPixelFrame);
            size_t copyBytesPerRow = MIN(dstBytesPerRow, srcBytesPerRow); // should always be dstBytesPerRow but be safe
            int y;
            for(y = 0; y < movieSize.height; y++){
                memcpy(dst, src, copyBytesPerRow);
                dst += dstBytesPerRow;
                src += srcBytesPerRow;
            }
        }
    }
    CVPixelBufferUnlockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
}

- (BOOL) textureAllocated
{
	return self.useTexture && _latestTextureFrame != NULL;
}

- (GLuint) textureID
{
	return CVOpenGLTextureGetName(_latestTextureFrame);
}

- (GLenum) textureTarget
{
	return CVOpenGLTextureGetTarget(_latestTextureFrame);
}

- (void) bindTexture
{
	if(!self.textureAllocated) return;
    
	GLuint texID = 0;
	texID = CVOpenGLTextureGetName(_latestTextureFrame);
	
	GLenum target = GL_TEXTURE_RECTANGLE_ARB;
	target = CVOpenGLTextureGetTarget(_latestTextureFrame);
	
	glEnable(target);
	glBindTexture(target, texID);
	
}

- (void) unbindTexture
{
	if(!self.textureAllocated) return;
	
	GLenum target = GL_TEXTURE_RECTANGLE_ARB;
	target = CVOpenGLTextureGetTarget(_latestTextureFrame);
	glDisable(target);	
}

- (void) setRate:(float) rate
{
    
	[_movie setRate:rate];
}

- (float) rate
{
	return _movie.rate;
}

- (void) setVolume:(float) volume
{
	[_movie setVolume:volume];
}

- (float) volume
{
    [_movie stop];
	return [_movie volume];
}

- (void) setBalance:(float) balance
{
    SetMovieAudioBalance([_movie quickTimeMovie], balance, 0);
}

- (void) setPosition:(CGFloat) position
{
	_movie.currentTime = QTMakeTime(position*movieDuration.timeValue, movieDuration.timeScale);
    self.justSetFrame = YES;
}

- (CGFloat) position
{
	return 1.0*_movie.currentTime.timeValue / movieDuration.timeValue;		
}

- (void) setFrame:(NSInteger) frame
{	
    _movie.currentTime = QTMakeTime(frame*frameStep, movieDuration.timeScale);
    //    NSLog(@"	set time to %f", 1.0*_movie.currentTime.timeValue / _movie.currentTime.timeScale);    
    self.justSetFrame = YES;
}

- (NSInteger) frame
{
	return _movie.currentTime.timeValue / frameStep;
}

- (NSTimeInterval) duration
{
	return 1.0*movieDuration.timeValue / movieDuration.timeScale;
}

- (void) setLoops:(BOOL)loops
{
	[_movie setAttribute:[NSNumber numberWithBool:loops] 
				  forKey:QTMovieLoopsAttribute];
}

- (BOOL) loops
{
	return [[_movie attributeForKey:QTMovieLoopsAttribute] boolValue];
}

- (BOOL) isFinished
{
	return !self.loops && _movie.currentTime.timeValue == movieDuration.timeValue;
}

@end
