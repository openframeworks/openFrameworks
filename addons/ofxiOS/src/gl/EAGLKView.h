//
//  EAGLKView.h
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 7/3/18.
//


#pragma once

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import "ESRenderer.h"

@protocol EAGLViewDelegate <NSObject>
@optional
- (void)glViewAnimationStarted;
- (void)glViewAnimationStopped;
- (void)glViewDraw;
- (void)glViewResized;
@end

@interface EAGLKView : GLKView
{
@public
    id<EAGLViewDelegate> delegate;
    
@protected
    id <ESRenderer> renderer;
    CGFloat scaleFactor;
    CGFloat scaleFactorPref;
    
    BOOL bUseDepth;
    BOOL bUseMSAA;
    BOOL bUseRetina;
    NSInteger msaaSamples;
    ESRendererVersion rendererVersion;
    
	BOOL animating;
	BOOL displayLinkSupported;
    NSTimer * animationTimer;
    
    NSLock * glLock;
}

@property (nonatomic, assign) id delegate;

- (id)initWithFrame:(CGRect)frame
andPreferedRenderer:(ESRendererVersion)rendererVersion
           andDepth:(bool)depth
              andAA:(bool)msaaEnabled
      andNumSamples:(int)samples
          andRetina:(bool)retinaEnabled
     andRetinaScale:(CGFloat)retinaScale;

- (void) setup;
- (void) update;
- (void) draw;

- (void) lockGL;
- (void) unlockGL;

- (void) startRender;
- (void) finishRender;

- (void) destroy;

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
#ifdef __IPHONE_9_1
- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches NS_AVAILABLE_IOS(9_1);
#endif

- (EAGLContext *) context;

- (GLint) getWidth;
- (GLint) getHeight;

- (void) notifyAnimationStarted;
- (void) notifyAnimationStopped;
- (void) notifyDraw;
- (void) notifyResized;

@end
