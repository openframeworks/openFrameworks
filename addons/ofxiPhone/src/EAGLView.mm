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


#import "ofMain.h"
#import  "ofxiPhoneExtras.h"

#import "EAGLView.h"

#import "ES1Renderer.h"
#import "ES2Renderer.h"

@implementation EAGLView

// You must implement this method
+ (Class) layerClass
{
	return [CAEAGLLayer class];
}


- (id) initWithFrame:(CGRect)frame
{
	if((self = [super initWithFrame:frame])) {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
		
        eaglLayer.opaque = true;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		// TODO: add initSettings to override ES2Renderer even if available
        renderer = [[ES2Renderer alloc] init];
		
        if (!renderer) {
            renderer = [[ES1Renderer alloc] init];
		
            if (!renderer) {
			[self release];
			return nil;
		}
        }
		
		[[self context] renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
		
		
		self.multipleTouchEnabled = true;
		self.opaque = true;
		activeTouches = [[NSMutableDictionary alloc] init];
	}
	
	return self;
}

- (void)startRender
 {
    [renderer startRender];
 }
	
- (void)finishRender
{
    [renderer finishRender];
}
	
	
- (void)layoutSubviews
{
	//	NSLog(@"layoutSubviews");
	//    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
	//    [renderer startRender];
	//    [renderer finishRender];
}


- (void) dealloc
{
    [renderer release];
	[activeTouches release];
	[super dealloc];
}

-(EAGLContext*) context
{
	return [renderer context];
}


/******************* TOUCH EVENTS ********************/
//------------------------------------------------------
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]) {
			touchIndex++;
		}
		
		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		iPhoneGetOFWindow()->rotateXY(touchPoint.x, touchPoint.y);
		
		if( touchIndex==0 ){
			ofGetAppPtr()->mouseX = touchPoint.x;
			ofGetAppPtr()->mouseY = touchPoint.y;
			ofGetAppPtr()->mousePressed(touchPoint.x, touchPoint.y, 1);
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		if([touch tapCount] == 2) ofNotifyEvent(ofEvents.touchDoubleTap,touchArgs);	// send doubletap
		ofNotifyEvent(ofEvents.touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
	}
	
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	//	NSLog(@"touchesMoved: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	
	for(UITouch *touch in touches) {
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		//		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
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
	
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
	//	NSLog(@"touchesEnded: %i %i %i", [touches count],  [[event touchesForView:self] count], multitouchData.numTouches);
	
	for(UITouch *touch in touches) {
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:touch]];
		
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
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
	[self touchesEnded:touches withEvent:event];
}

@end
