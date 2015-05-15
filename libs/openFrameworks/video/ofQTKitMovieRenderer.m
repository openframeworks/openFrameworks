#ifdef OF_VIDEO_PLAYER_QTKIT

#import "ofQTKitMovieRenderer.h"
#import <Accelerate/Accelerate.h>

//secret selectors!
@interface QTMovie (QTFrom763)
- (QTTime)frameStartTime: (QTTime)atTime;
- (QTTime)frameEndTime: (QTTime)atTime;
- (QTTime)keyframeStartTime:(QTTime)atTime;
@end

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#if defined(MAC_OS_X_VERSION_10_9)
	#warning Using QTKit, which is deprecated in OSX 10.9
#endif

//--------------------------------------------------------------
//This method is called whenever a new frame comes in from the visual context
//it's called on the back thread so locking is performed in Renderer class
static void frameAvailable(QTVisualContextRef _visualContext, const CVTimeStamp *frameTime, void *refCon)
{
	@autoreleasepool {
		CVImageBufferRef	currentFrame;
		OSStatus			err;
		QTKitMovieRenderer		*renderer	= (QTKitMovieRenderer *)refCon;

		if ((err = QTVisualContextCopyImageForTime(_visualContext, NULL, frameTime, &currentFrame)) == kCVReturnSuccess) {
			[renderer frameAvailable:currentFrame];
		}
		else{
			[renderer frameFailed];
		}
	}
}


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
@synthesize synchronousSeek;

- (NSDictionary*) pixelBufferAttributes
{
    return [NSDictionary dictionaryWithObjectsAndKeys:
            //if we have a texture, make the pixel buffer OpenGL compatible
            [NSNumber numberWithBool:self.useTexture], (NSString*)kCVPixelBufferOpenGLCompatibilityKey, 
            [NSNumber numberWithInt:kCVPixelFormatType_32ARGB], (NSString*)kCVPixelBufferPixelFormatTypeKey,
            nil];
}

- (BOOL) loadMovie:(NSString*)moviePath pathIsURL:(BOOL)isURL allowTexture:(BOOL)doUseTexture allowPixels:(BOOL)doUsePixels allowAlpha:(BOOL)doUseAlpha
{
    // if the path is local, make sure the file exists before proceeding
    if (!isURL && ![[NSFileManager defaultManager] fileExistsAtPath:moviePath])
    {
		NSLog(@"No movie file found at %@", moviePath);
		return NO;
	}
	
	//create visual context
	useTexture = doUseTexture;
	usePixels = doUsePixels;
	useAlpha = doUseAlpha;
    

    // build the movie URL
    NSURL *movieURL;
    if (isURL) {
        movieURL = [NSURL URLWithString:moviePath];
    }
    else {
        movieURL = [NSURL fileURLWithPath:[moviePath stringByStandardizingPath]];
    }

	NSError* error;
	NSDictionary* movieAttributes = [NSDictionary dictionaryWithObjectsAndKeys:
                                        movieURL, QTMovieURLAttribute,
                                        [NSNumber numberWithBool:NO], QTMovieOpenAsyncOKAttribute,
                                        nil];

	_movie = [[QTMovie alloc] initWithAttributes:movieAttributes 
										   error: &error];
	
	if(error || _movie == NULL){
		NSLog(@"Error Loading Movie: %@", error);
		return NO;
	}
    lastMovieTime = QTMakeTime(0,1);
	movieSize = [[_movie attributeForKey:QTMovieNaturalSizeAttribute] sizeValue];
    //	NSLog(@"movie size %f %f", movieSize.width, movieSize.height);
	
	movieDuration = [_movie duration];
    
	[_movie gotoBeginning];
    
    [_movie gotoEnd];
    QTTime endTime = [_movie currentTime];
    
    [_movie gotoBeginning];
    QTTime curTime = [_movie currentTime];
    
    long numFrames = 0;
	NSMutableArray* timeValues = [NSMutableArray array];
    while(true) {
        //        % get the end time of the current frame
		[timeValues addObject:[NSNumber numberWithLongLong:curTime.timeValue]];

        curTime = [_movie frameEndTime:curTime];
        numFrames++;
//        int time = curTime.timeValue;
//        NSLog(@" num frames %ld, %lld/%ld , dif %lld, current time %f", numFrames,curTime.timeValue,curTime.timeScale, curTime.timeValue - time, 1.0*curTime.timeValue/curTime.timeScale);
        if (QTTimeCompare(curTime, endTime) == NSOrderedSame ||
            QTTimeCompare(curTime, [_movie frameEndTime:curTime])  == NSOrderedSame ){ //this will happen for audio files since they have no frames.
            break;
        }
    }
    
	if(frameTimeValues != NULL){
		[frameTimeValues release];
	}
	frameTimeValues = [[NSArray arrayWithArray:timeValues] retain];
	
	frameCount = numFrames;
//	frameStep = round((double)(movieDuration.timeValue/(double)(numFrames)));
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
	//if we are using a texture, just create an OpenGL visual context 
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
	
	QTVisualContextSetImageAvailableCallback(_visualContext, frameAvailable, self);
	synchronousSeekLock = [[NSCondition alloc] init];
	
	//borrowed from WebCore:
	// http://opensource.apple.com/source/WebCore/WebCore-1298/platform/graphics/win/QTMovie.cpp
	hasVideo = (NULL != GetMovieIndTrackType([_movie quickTimeMovie], 1, VisualMediaCharacteristic, movieTrackCharacteristic | movieTrackEnabledOnly));
	hasAudio = (NULL != GetMovieIndTrackType([_movie quickTimeMovie], 1, AudioMediaCharacteristic,  movieTrackCharacteristic | movieTrackEnabledOnly));
//	NSLog(@"has video? %@ has audio? %@", (hasVideo ? @"YES" : @"NO"), (hasAudio ? @"YES" : @"NO") );
	loadedFirstFrame = NO;
	self.volume = 1.0;
	self.loops = YES;
    self.palindrome = NO;

	return YES;
}

- (void) dealloc
{
	@synchronized(self){
		
		if(_visualContext != NULL){
			QTVisualContextSetImageAvailableCallback(_visualContext, NULL, NULL);
		}

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
		
		if(frameTimeValues != NULL){
			[frameTimeValues release];
			frameTimeValues = NULL;
		}
		
		if(synchronousSeekLock != nil){
			[synchronousSeekLock release];
			synchronousSeekLock = nil;
		}
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

- (void)frameAvailable:(CVImageBufferRef)image
{

	@synchronized(self){
		
		if(_visualContext == NULL){
			return;
		}

		if(self.usePixels){
			if(_latestPixelFrame != NULL){
				CVPixelBufferRelease(_latestPixelFrame);
				_latestPixelFrame = NULL;
			}
			_latestPixelFrame = image;
			
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
				}
			}
		}
		//just get the texture
		else if(self.useTexture){
			if(_latestTextureFrame != NULL){
				CVOpenGLTextureRelease(_latestTextureFrame);
				_latestTextureFrame = NULL;
			}
			_latestTextureFrame = image;
		}
		frameIsNew = YES;	
	}

//	NSLog(@"incoming frame time: %lld/%ld and movie time is %lld", correctedFrameTime.timeValue, correctedFrameTime.timeScale, self.timeValue);

//	lastMovieTime = (1.0*frameTime.timeValue)/frameTime.timeScale;
//	lastMovieTime = frameTime;
	if(self.justSetFrame){
		CVAttachmentMode mode = kCVAttachmentMode_ShouldPropagate;
		NSDictionary* timeDictionary = (NSDictionary*)CVBufferGetAttachment (image, kCVBufferMovieTimeKey, &mode);
		QTTime frameTime = QTMakeTime([[timeDictionary valueForKey:@"TimeValue"] longLongValue],
									  [[timeDictionary valueForKey:@"TimeScale"] longValue]);
		//
		QTTime correctedFrameTime = [_movie frameEndTime:frameTime];
		//Incoming frames will often be earlier times than requested. So we have to signal
		//the waiting thread to try the MovieTask() again to get another frame.
		//sometimes timestamps don't contain data, we have no choice but to assume it's the right frame
		if(correctedFrameTime.timeValue >= self.timeValue || frameTime.timeValue == 0){
//			NSLog(@"Time is good ");
			justSetFrame = NO;
		}
		//signal to the waiting thread that the pixels are updated
		[synchronousSeekLock lock];
		[synchronousSeekLock signal];
		[synchronousSeekLock unlock];
	}

	QTVisualContextTask(_visualContext);
}

- (void)frameFailed
{
	NSLog(@"QTRenderer -- Error failed to get frame on callback");
}

- (BOOL) update
{
   	BOOL newFrame = frameIsNew;
	frameIsNew = false;
	return newFrame;

}

- (void) stepForward
{
    if(_movie){
		self.justSetFrame = YES;
        [_movie stepForward];
		[self synchronizeSeek];
    }
}

- (void) stepBackward
{
    if(_movie){
		self.justSetFrame = YES;
        [_movie stepBackward];
		[self synchronizeSeek];
    }
}

- (void) gotoBeginning
{
	if(_movie){
		self.justSetFrame = YES;
    	[_movie gotoBeginning];
		[self synchronizeSeek];
    }
}

//writes out the pixels in RGB or RGBA format to outbuf
- (void) pixels:(unsigned char*) outbuf
{
	@synchronized(self){
		if(!self.usePixels || _latestPixelFrame == NULL){
			return;
		}
		
	//    NSLog(@"pixel buffer width is %ld height %ld and bpr %ld, movie size is %d x %d ",
	//          CVPixelBufferGetWidth(_latestPixelFrame),
	//          CVPixelBufferGetHeight(_latestPixelFrame), 
	//          CVPixelBufferGetBytesPerRow(_latestPixelFrame),
	//          (NSInteger)movieSize.width, (NSInteger)movieSize.height);
		if((NSInteger)movieSize.width != CVPixelBufferGetWidth(_latestPixelFrame) ||
		   (NSInteger)movieSize.height != CVPixelBufferGetHeight(_latestPixelFrame)){
			NSLog(@"CoreVideo pixel buffer is %ld x %ld while QTKit Movie reports size of %d x %d. This is most likely caused by a non-square pixel video format such as HDV. Open this video in texture only mode to view it at the appropriate size",
				  CVPixelBufferGetWidth(_latestPixelFrame), CVPixelBufferGetHeight(_latestPixelFrame), (NSInteger)movieSize.width, (NSInteger)movieSize.height);
			return;
		}
		
		if(CVPixelBufferGetPixelFormatType(_latestPixelFrame) != kCVPixelFormatType_32ARGB){
			NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_32ARGB: %d, instead %ld",kCVPixelFormatType_32ARGB,CVPixelBufferGetPixelFormatType(_latestPixelFrame));
			return;
		}
		
		CVPixelBufferLockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
		//If we are using alpha, the ofQTKitPlayer class will have allocated a buffer of size
		//movieSize.width * movieSize.height * 4
		//CoreVideo creates alpha video in the format ARGB, and openFrameworks expects RGBA,
		//so we need to swap the alpha around using a vImage permutation
		vImage_Buffer src = {
			CVPixelBufferGetBaseAddress(_latestPixelFrame),
			CVPixelBufferGetHeight(_latestPixelFrame),
			CVPixelBufferGetWidth(_latestPixelFrame),
			CVPixelBufferGetBytesPerRow(_latestPixelFrame)
		};
		vImage_Error err;
		if(self.useAlpha){
			vImage_Buffer dest = { outbuf, movieSize.height, movieSize.width, movieSize.width*4 };
			uint8_t permuteMap[4] = { 1, 2, 3, 0 }; //swizzle the alpha around to the end to make ARGB -> RGBA
			err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
		}
		//If we are are doing RGB then ofQTKitPlayer will have created a buffer of size movieSize.width * movieSize.height * 3
		//so we use vImage to copy them int the out buffer
		else {
			vImage_Buffer dest = { outbuf, movieSize.height, movieSize.width, movieSize.width*3 };
			err = vImageConvert_ARGB8888toRGB888(&src, &dest, 0);
// NO LONGER USED: keep for reference
// was needed when requesting RGB buffers straight from QTKit, but this resulted in strange behavior in many cases
//			else{
//				//This branch is not intended to be used anymore as we will use vImage all the time, getting only ARGB frames
//				if (CVPixelBufferGetPixelFormatType(_latestPixelFrame) != kCVPixelFormatType_24RGB){
//					NSLog(@"QTKitMovieRenderer - Frame pixelformat not kCVPixelFormatType_24RGB: %d, instead %ld",kCVPixelFormatType_24RGB,CVPixelBufferGetPixelFormatType(_latestPixelFrame));
//				}
//				size_t dstBytesPerRow = movieSize.width * 3;
//				if (CVPixelBufferGetBytesPerRow(_latestPixelFrame) == dstBytesPerRow) {
//					memcpy(outbuf, CVPixelBufferGetBaseAddress(_latestPixelFrame), dstBytesPerRow*CVPixelBufferGetHeight(_latestPixelFrame));
//				}
//				else {
//					unsigned char *dst = outbuf;
//					unsigned char *src = (unsigned char*)CVPixelBufferGetBaseAddress(_latestPixelFrame);
//					size_t srcBytesPerRow = CVPixelBufferGetBytesPerRow(_latestPixelFrame);
//					size_t copyBytesPerRow = MIN(dstBytesPerRow, srcBytesPerRow); // should always be dstBytesPerRow but be safe
//					int y;
//					for(y = 0; y < movieSize.height; y++){
//						memcpy(dst, src, copyBytesPerRow);
//						dst += dstBytesPerRow;
//						src += srcBytesPerRow;
//					}
//				}
//			}
		}
		
		CVPixelBufferUnlockBaseAddress(_latestPixelFrame, kCVPixelBufferLock_ReadOnly);
		
		if(err != kvImageNoError){
			NSLog(@"Error in Pixel Copy vImage_error %ld", err);
		}
		
	}
}

- (BOOL) textureAllocated
{
	return self.useTexture && _latestTextureFrame != NULL;
}

- (GLuint) textureID
{
	@synchronized(self){
		return CVOpenGLTextureGetName(_latestTextureFrame);
	}
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
	if(self.synchronousSeek && self.justSetFrame){
		//in case we are in the middle of waiting for an update signal that thread to end
		[synchronousSeekLock lock];
		self.justSetFrame = NO;
		[synchronousSeekLock signal];
		[synchronousSeekLock unlock];
	}
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
	return [_movie volume];
}

- (void) setBalance:(float) balance
{
    SetMovieAudioBalance([_movie quickTimeMovie], balance, 0);
}

- (void) setPosition:(CGFloat) position
{
	float oldRate = self.rate;
	if(self.rate != 0){
		_movie.rate = 0;
	}

    QTTime t = QTMakeTime(position*movieDuration.timeValue, movieDuration.timeScale);
	QTTime startTime =[_movie frameStartTime:t];
//	QTTime endTime =[_movie frameEndTime:t];
	if(QTTimeCompare(startTime, _movie.currentTime) != NSOrderedSame){
		_movie.currentTime = startTime;
		[self synchronizeSeek];
	}
	
	if(oldRate != 0){
		self.rate = oldRate;
	}
	
}

- (void) setFrame:(NSInteger) frame
{
	float oldRate = self.rate;
	if(self.rate != 0){
		_movie.rate = 0;
	}
	//QTTime t = QTMakeTime(frame*frameStep, movieDuration.timeScale);
	QTTime t = QTMakeTime([[frameTimeValues objectAtIndex:frame%frameTimeValues.count] longLongValue], movieDuration.timeScale);
	QTTime startTime =[_movie frameStartTime:t];
	QTTime endTime =[_movie frameEndTime:t];
//	NSLog(@"calculated frame time %lld, frame start end [%lld, %lld]", t.timeValue, startTime.timeValue, endTime.timeValue);
	if(QTTimeCompare(startTime, _movie.currentTime) != NSOrderedSame){
		self.justSetFrame = YES;
		_movie.currentTime = startTime;
//		//		NSLog(@"set time to %f", 1.0*_movie.currentTime.timeValue / _movie.currentTime.timeScale);
//  NSLog(@"nsorderedsame calculated frame time %lld, frame start end [%lld, %lld]", t.timeValue, startTime.timeValue, endTime.timeValue);
		[self synchronizeSeek];
	}

	if(oldRate != 0){
		self.rate = oldRate;
	}
}

- (CGFloat) position
{
//	return 1.0*lastMovieTime.timeValue / movieDuration.timeValue;
	return 1.0*_movie.currentTime.timeValue / movieDuration.timeValue;
}

- (CGFloat) time
{
	//return lastMovieTime;
	//return 1.0*lastMovieTime.timeValue / lastMovieTime.timeScale;
	return _movie.currentTime.timeValue / movieDuration.timeScale;
}

//internal
- (long long) timeValue
{
	return _movie.currentTime.timeValue;
}

//This thread will guarantee that the current frame is in memory
//before proceeding. If something goes weird, it has 1.0 second timeout
//that it will print a warning and proceed.
//It works by blocking with a condition, which is signaled
//in the frameAvailable callback when the time matches the requested time
- (void) synchronizeSeek
{
	if(!self.synchronousSeek || !hasVideo){
		self.justSetFrame = NO;
		return;
	}
	
//	NSLog(@" current time %lld vs duration %lld", _movie.currentTime.timeValue, movieDuration.timeValue);
	//if requesting the last frame, don't synchronize update
	if(_movie.currentTime.timeValue == movieDuration.timeValue){
		self.justSetFrame = NO;
		return;
	}

	//no synchronous seeking for images or audio files!
	//except on the first frame.
	if(self.frameCount < 2 && loadedFirstFrame){
		self.justSetFrame = NO;
		return;		
	}

	int numTries = 0;
	while(self.justSetFrame && numTries++ < 10){
		[synchronousSeekLock lock];
		
		QTVisualContextTask(_visualContext);
		MoviesTask([_movie quickTimeMovie], 0);
		
		if(![synchronousSeekLock waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:2.0]]){
			NSLog(@"synchronizeUpdate timed out in QTMovieRenderer");
			self.justSetFrame = NO;
		}
		
		[synchronousSeekLock unlock];
	}
	loadedFirstFrame = true;
}

//complicated!!! =( Do a search through the frame time values
//to find the index of the current time, then return that index
// http://stackoverflow.com/questions/3995949/how-to-write-objective-c-blocks-inline
- (NSInteger) frame
{
	return [frameTimeValues indexOfObject:[NSNumber numberWithLongLong:_movie.currentTime.timeValue]
							inSortedRange:NSMakeRange(0, frameTimeValues.count)
								  options:NSBinarySearchingInsertionIndex
						  usingComparator:^(id lhs, id rhs) {
							  if ([lhs longLongValue] < [rhs longLongValue])
								  return (NSComparisonResult)NSOrderedAscending;
							  else if([lhs longLongValue] > [rhs longLongValue])
								  return (NSComparisonResult)NSOrderedDescending;
							  return (NSComparisonResult)NSOrderedSame;
						  }];
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

- (void) setPalindrome:(BOOL)palindrome
{
	[_movie setAttribute:[NSNumber numberWithBool:palindrome]
				  forKey:QTMovieLoopsBackAndForthAttribute];
}

- (BOOL) palindrome
{
	return [[_movie attributeForKey:QTMovieLoopsBackAndForthAttribute] boolValue];
}

- (BOOL) isFinished
{
	return !self.loops && !self.palindrome && _movie.currentTime.timeValue == movieDuration.timeValue;
}

#pragma clang diagnostic pop

@end

#endif
