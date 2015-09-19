/***********************************************************************
 
 Copyright (c) 2008, 2009, 2010 Memo Akten, www.memo.tv
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

#import <UIKit/UIKit.h>
#import "ESRenderer.h"

@protocol EAGLViewDelegate <NSObject>
@optional
- (void)glViewAnimationStarted;
- (void)glViewAnimationStopped;
- (void)glViewDraw;
- (void)glViewResized;
@end

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView
{
@public
    id<EAGLViewDelegate> delegate;
    
@protected
    id <ESRenderer> renderer;
    CGFloat scaleFactor;
    CGFloat scaleFactorPref;
    
    BOOL bUseDepth;
    BOOL bUseFSAA;
    BOOL bUseRetina;
    NSInteger fsaaSamples;
    ESRendererVersion rendererVersion;
    
	BOOL animating;
	BOOL displayLinkSupported;
	float animationFrameInterval;
	// Use of the CADisplayLink class is the preferred method for controlling your animation timing.
	// CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
	// The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
	// isn't available.
	id displayLink;
    NSTimer * animationTimer;
    
    NSLock * glLock;
}

@property (nonatomic, assign) id delegate;
@property (readonly, nonatomic, getter=isAnimating) BOOL animating;
@property (nonatomic) float animationFrameInterval;
@property (nonatomic) float animationFrameRate;

- (id)initWithFrame:(CGRect)frame
andPreferedRenderer:(ESRendererVersion)rendererVersion
           andDepth:(bool)depth
              andAA:(bool)fsaaEnabled
      andNumSamples:(int)samples
          andRetina:(bool)retinaEnabled
     andRetinaScale:(CGFloat)retinaScale;

- (void) startAnimation;
- (void) stopAnimation;
- (void) drawView;

- (void) lockGL;
- (void) unlockGL;

- (void) startRender;
- (void) finishRender;

- (void) destroy;

- (EAGLContext *) context;

- (GLint) getWidth;
- (GLint) getHeight;

- (void) notifyAnimationStarted;
- (void) notifyAnimationStopped;
- (void) notifyDraw;
- (void) notifyResized;

@end
