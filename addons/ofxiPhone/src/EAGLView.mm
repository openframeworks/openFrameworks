/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

#import "EAGLView.h"


#import "ofMain.h"
//#import "iPhoneGlobals.h"
#import  "ofxiPhoneExtras.h"


//CLASS IMPLEMENTATIONS:

@implementation EAGLView

@synthesize surfaceSize=_size, framebuffer = _framebuffer, pixelFormat = _format, depthFormat = _depthFormat, context = _context;

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (BOOL) _createSurface
{
	CAEAGLLayer*			eaglLayer = (CAEAGLLayer*)[self layer];
	CGSize					newSize;
	GLuint					oldRenderbuffer;
	GLuint					oldFramebuffer;
	
	if(![EAGLContext setCurrentContext:_context]) {
		return NO;
	}
	
	newSize = [eaglLayer bounds].size;
	newSize.width = roundf(newSize.width);
	newSize.height = roundf(newSize.height);
	
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	glGenRenderbuffersOES(1, &_renderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderbuffer);
	
	if(![_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)eaglLayer]) {
		glDeleteRenderbuffersOES(1, &_renderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_BINDING_OES, oldRenderbuffer);
		return NO;
	}
	
	glGenFramebuffersOES(1, &_framebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _framebuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _renderbuffer);
	if (_depthFormat) {
		glGenRenderbuffersOES(1, &_depthBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depthBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, _depthFormat, newSize.width, newSize.height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, _depthBuffer);
	}
	
	_size = newSize;
	if(!_hasBeenCurrent) {
		glViewport(0, 0, newSize.width, newSize.height);
		glScissor(0, 0, newSize.width, newSize.height);
		_hasBeenCurrent = YES;
	}
	else {
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	}
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);
	
	//	isLandscape = false;
	
	return YES;
}

- (void) _destroySurface
{
	EAGLContext *oldContext = [EAGLContext currentContext];
	
	if (oldContext != _context)
		[EAGLContext setCurrentContext:_context];
	
	if(_depthFormat) {
		glDeleteRenderbuffersOES(1, &_depthBuffer);
		_depthBuffer = 0;
	}
	
	glDeleteRenderbuffersOES(1, &_renderbuffer);
	_renderbuffer = 0;
	
	glDeleteFramebuffersOES(1, &_framebuffer);
	_framebuffer = 0;
	
	if (oldContext != _context)
		[EAGLContext setCurrentContext:oldContext];
}

- (id) initWithFrame:(CGRect)frame
{
	return [self initWithFrame:frame pixelFormat:GL_RGB565_OES depthFormat:0 preserveBackbuffer:NO];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(GLuint)format 
{
	return [self initWithFrame:frame pixelFormat:format depthFormat:0 preserveBackbuffer:NO];
}

- (id) initWithFrame:(CGRect)frame pixelFormat:(GLuint)format depthFormat:(GLuint)depth preserveBackbuffer:(bool)retained
{
	if((self = [super initWithFrame:frame])) {
		CAEAGLLayer*			eaglLayer = (CAEAGLLayer*)[self layer];
		
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		_format = format;
		_depthFormat = depth;
		
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		if(_context == nil) {
			[self release];
			return nil;
		}
		
		if(![self _createSurface]) {
			[self release];
			return nil;
		}
		
		self.multipleTouchEnabled = true;
		self.opaque = YES;
		bzero(activeTouches, sizeof(activeTouches));
	}
	
	return self;
}
/*
 - (void) setLandscape:(bool)landscape
 {
 isLandscape = landscape;
 }
 */
- (void) swapBuffers
{
	EAGLContext *oldContext = [EAGLContext currentContext];
	GLuint oldRenderbuffer;
	
	if(oldContext != _context)
		[EAGLContext setCurrentContext:_context];
	
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderbuffer);
	
	if(![_context presentRenderbuffer:GL_RENDERBUFFER_OES])
		ofLog(OF_LOG_ERROR, string("Failed to swap renderbuffer in ") + __FUNCTION__);
	
	if(oldContext != _context)
		[EAGLContext setCurrentContext:oldContext];
}

- (void) dealloc
{
	[self _destroySurface];
	
	[_context release];
	_context = nil;
	
	[super dealloc];
}


/******************* TOUCH EVENTS ********************/
//------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while(touchIndex < OF_MAX_TOUCHES && activeTouches[touchIndex] != 0) touchIndex++;
		if(touchIndex==OF_MAX_TOUCHES) {
			ofLog(OF_LOG_WARNING, "touchesBegan - weird!");
			touchIndex=0;	
		}
		
		activeTouches[touchIndex] = touch;
		
		CGPoint touchPoint = [touch locationInView:self];
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofGetAppPtr()->mouseX = touchPoint.x;
			ofGetAppPtr()->mouseY = touchPoint.y;
			ofGetAppPtr()->mousePressed(touchPoint.x, touchPoint.y, 1);
		}
		
		//		if([touch tapCount] == 1) ofxMultiTouch.touchDown(touchPoint.x, touchPoint.y, touchIndex, &multitouchData);
		//		else ofxMultiTouch.touchDoubleTap(touchPoint.x, touchPoint.y, touchIndex, &multitouchData);
		
		ofTouchEventArgs touchArgs;
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		if([touch tapCount] == 2) ofNotifyEvent(ofEvents.touchDoubleTap,touchArgs);	// send doubletap
		ofNotifyEvent(ofEvents.touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
	}
	
//	[self.nextResponder touchesBegan:touches withEvent:event];
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	//	NSLog(@"touchesMoved: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while(touchIndex < OF_MAX_TOUCHES && (activeTouches[touchIndex] != touch)) touchIndex++;
		if(touchIndex==OF_MAX_TOUCHES) {
			ofLog(OF_LOG_WARNING, "touchesMoved - weird!");
			continue;	
		}
		
		CGPoint touchPoint = [touch locationInView:self];
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofGetAppPtr()->mouseX = touchPoint.x;
			ofGetAppPtr()->mouseY = touchPoint.y;
			ofGetAppPtr()->mouseDragged(touchPoint.x, touchPoint.y, 1);
		}		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents.touchMoved, touchArgs);
	}
	
//	[self.nextResponder touchesMoved:touches withEvent:event];
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	//	NSLog(@"touchesEnded: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while(touchIndex < OF_MAX_TOUCHES && (activeTouches[touchIndex] != touch)) touchIndex++;
		if(touchIndex==OF_MAX_TOUCHES) {
			ofLog(OF_LOG_WARNING, "touchesEnded - weird!");
			continue;	
		}
		
		activeTouches[touchIndex] = 0;
		
		CGPoint touchPoint = [touch locationInView:self];
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofGetAppPtr()->mouseX = touchPoint.x;
			ofGetAppPtr()->mouseY = touchPoint.y;
			ofGetAppPtr()->mouseReleased(touchPoint.x, touchPoint.y, 1);
			ofGetAppPtr()->mouseReleased();
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		ofNotifyEvent(ofEvents.touchUp, touchArgs);
	}
//	[self.nextResponder touchesEnded:touches withEvent:event];
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for(int i=0; i<OF_MAX_TOUCHES; i++){
		if(activeTouches[i]){
			
			CGPoint touchPoint = [activeTouches[i] locationInView:self];
			iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
			
			activeTouches[i] = 0;
			
			if( i==0 ){
				ofGetAppPtr()->mouseX = touchPoint.x;
				ofGetAppPtr()->mouseY = touchPoint.y;
				ofGetAppPtr()->mouseReleased(touchPoint.x, touchPoint.y, 1);
				ofGetAppPtr()->mouseReleased();
			}
			ofTouchEventArgs touchArgs;
			touchArgs.numTouches = 0;
			touchArgs.x = touchPoint.x;
			touchArgs.y = touchPoint.y;
			touchArgs.id = i;
			ofNotifyEvent(ofEvents.touchUp, touchArgs);
		}
	}
//	[self.nextResponder touchesCancelled:touches withEvent:event];
}

@end
