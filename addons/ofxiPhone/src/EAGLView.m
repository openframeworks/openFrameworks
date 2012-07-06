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

#import "EAGLView.h"

#import "ES1Renderer.h"
#import "ES2Renderer.h"

@interface EAGLView()
- (void) drawView:(id)sender;
@end

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;
@dynamic animationFrameRate;

// You must implement this method
+ (Class) layerClass {
	return [CAEAGLLayer class];
}

- (id) initWithFrame:(CGRect)frame 
            andDepth:(bool)depth 
               andAA:(bool)fsaaEnabled 
       andNumSamples:(int)samples 
           andRetina:(bool)retinaEnabled{

	if((self = [super initWithFrame:frame])) {
        
        bUseDepth = depth;
        bUseFSAA = fsaaEnabled;
        bUseRetina = retinaEnabled;
        fsaaSamples = samples;

        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)super.layer;
		
        eaglLayer.opaque = true;
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:YES], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
		
		touchScaleFactor=1;
		if(bUseRetina){
			if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]){
				if ([[UIScreen mainScreen] scale] > 1){
					[self setContentScaleFactor:[[UIScreen mainScreen] scale]];
					touchScaleFactor=[[UIScreen mainScreen] scale];
				} else {
                    bUseRetina = false;
                }
			} else {
                bUseRetina = false;
            }
		}
		
        renderer = [[ES2Renderer alloc] initWithDepth:bUseDepth 
                                                andAA:bUseFSAA 
                                       andFSAASamples:fsaaSamples 
                                            andRetina:bUseRetina];
		
        if(!renderer){
            renderer = [[ES1Renderer alloc] initWithDepth:bUseDepth 
                                                    andAA:bUseFSAA 
                                           andFSAASamples:fsaaSamples 
                                                andRetina:bUseRetina];
			
            if(!renderer){
				[self release];
				return nil;
			}
        }
		
		self.multipleTouchEnabled = true;
		self.opaque = true;
        
		animating = NO;
		displayLinkSupported = NO;
		animationFrameInterval = 1;
		displayLink = nil;
		animationTimer = nil;
        
        glLock = [[NSLock alloc] init];
		
		// A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
		// class is used as fallback when it isn't available.
		NSString *reqSysVer = @"3.1";
		NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
		if([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending) {
            /**
             *  enabling display link support doesn't play nice with UIKit interaction.
             *  gl framerate runs at full 60 fps but touch events slow down considerably.
             *  here is a potential solution (but for another day),
             *  http://stackoverflow.com/questions/5944050/cadisplaylink-opengl-rendering-breaks-uiscrollview-behaviour/5956119#5956119
             *  for now, displayLinkSupported is disabled.
             */
//			displayLinkSupported = YES;
        }
	}
	
	return self;
}

- (void) dealloc{
    [renderer release];
    [glLock release];
	[super dealloc];
}

- (void) drawView:(id)sender {
    [self drawView];
}

- (void) drawView {
    NSLog(@"[EAGLView drawView] - this method need to be extended.");
}

- (void)startRender {
    [renderer startRender];
}
	
- (void)finishRender {
    [renderer finishRender];
}
	
- (void)layoutSubviews{
    NSLog(@"layoutSubviews");
    
    UIScreen *currentScreen;
    currentScreen = self.window.screen;
    if(!currentScreen){
        currentScreen = [UIScreen mainScreen];
    }
    
    if(bUseRetina){
        if([currentScreen respondsToSelector:@selector(scale)]){
            if(touchScaleFactor != [currentScreen scale]){
                touchScaleFactor = [currentScreen scale];
                [self setContentScaleFactor:touchScaleFactor];
            }
        } else {
            if(touchScaleFactor != 1){
                touchScaleFactor = 1;
                [self setContentScaleFactor:touchScaleFactor];
            }
        }
    } else {
        if(touchScaleFactor != 1){
            touchScaleFactor = 1;
            [self setContentScaleFactor:touchScaleFactor];
        }
    }

    [renderer startRender];
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [renderer finishRender];
}


//------------------------------------------------------------------- lock/unlock GL context.
- (void)lockGL {
	[glLock lock];
}

- (void)unlockGL {
	[glLock unlock];
}

//------------------------------------------------------------------- animation timer.
- (float) animationFrameInterval {
	return animationFrameInterval;
}

- (void) setAnimationFrameInterval:(float)frameInterval {
	// Frame interval defines how many display frames must pass between each time the
	// display link fires. The display link will only fire 30 times a second when the
	// frame internal is two on a display that refreshes 60 times a second. The default
	// frame interval setting of one will fire 60 times a second when the display refreshes
	// at 60 times a second. A frame interval setting of less than one results in undefined
	// behavior.
	if(frameInterval >= 1) {
		animationFrameInterval = frameInterval;
		
		if(animating) {
			[self stopAnimation];
			[self startAnimation];
		}
	}
}

- (void)setAnimationFrameRate:(float)rate {
	if(rate > 0) {
        [self setAnimationFrameInterval:60.0/rate];
        [self startAnimation];
    } else {
        [self stopAnimation];
    }
}

- (void) startAnimation {
	if(!animating) {
		if(displayLinkSupported) {
			// CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
			// if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
			// not be called in system versions earlier than 3.1.
			
			displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
			[displayLink setFrameInterval:animationFrameInterval];
			[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		} else {
			animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) 
                                                              target:self 
                                                            selector:@selector(drawView:) 
                                                            userInfo:nil 
                                                             repeats:TRUE];
        }
		
		animating = YES;
	}
}

- (void)stopAnimation {
	if(animating) {
		if (displayLinkSupported) {
			[displayLink invalidate];
			displayLink = nil;
		} else {
			[animationTimer invalidate];
			animationTimer = nil;
		}
		
		animating = NO;
	}
}

-(EAGLContext*) context{
	return [renderer context];
}

@end
