/*
 *  ofxQTKitMoviePlayer
 *
 **********************************************************
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 * 
 * ofxQTKitVideoPlayer provides QTKit/CoreVideo accelerated movie playback
 * for openFrameworks on OS X
 * 
 * This class replaces almost all of the functionality of ofVideoPlayer on OS X
 * and uses the modern QTKit and CoreVideo libraries for playback
 *
 * adapted by James George
 * http://www.jamesgeorge.org
 *
 * Special Thanks to Marek Bereza for his initial QTKitVideoTexture
 * http://www.mrkbrz.com/
 *
 * Thanks to Anton Marini for help wrangling buffer contexts
 * http://vade.info/ 
 */

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import <QTKit/QTKit.h>
#import <OpenGL/OpenGL.h>


@interface QTKitMovieRenderer : NSObject 
{
    QTMovie*    _movie;
    QTVisualContextRef  _visualContext;
	
	CVOpenGLTextureCacheRef _textureCache;	
    CVOpenGLTextureRef _latestTextureFrame;
	CVPixelBufferRef _latestPixelFrame;
	
	NSSize movieSize;
	QTTime movieDuration;
	NSInteger frameCount;
	NSInteger frameStep;
		
	BOOL frameUpdated;
	BOOL useTexture;
	BOOL usePixels;
	BOOL useRGBAFormat; //default is off
}

@property(nonatomic, readonly) NSSize movieSize;
@property(nonatomic, readonly) BOOL useTexture;
@property(nonatomic, readonly) BOOL usePixels;
@property(nonatomic, readonly) NSTimeInterval duration; //duration in seconds
@property(nonatomic, readonly) NSInteger frameCount;  //total frames
@property(nonatomic, readonly) BOOL isFinished;  //returns true if the movie is not looping and over

@property(nonatomic, readwrite) float rate;
@property(nonatomic, readwrite) float volume;
@property(nonatomic, readwrite) CGFloat position;  //set and get frame position by percent
@property(nonatomic, readwrite) NSInteger frame;  //set and get frame position by percent
@property(nonatomic, readwrite) BOOL loops;  //set and get loopstate
@property(nonatomic, readwrite) BOOL useRGBAFormat;

- (void) draw:(NSRect)drawRect;
- (BOOL) loadMovie:(NSString*)moviePath allowTexture:(BOOL)useTexture allowPixels:(BOOL)usePixels;
- (BOOL) update;
- (void) stop;
- (void) loopsBackAndForth;

- (void) bindTexture;
- (void) unbindTexture;

//copies ARGB pixels to RGBA into the outbuf
- (void) pixels:(unsigned char*) outbuf;

@end
