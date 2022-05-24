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

/// ???: inherit GLKViewDelegate?
@protocol EAGLKViewDelegate <NSObject>
@optional
- (void)glViewAnimationStarted;
- (void)glViewAnimationStopped;
- (void)glViewDraw;
- (void)glViewResized;
@end

@interface EAGLKView : GLKView
{

@protected
  
    CGFloat scaleFactor;
    CGFloat scaleFactorPref;
    
    BOOL bUseDepth;
    BOOL bUseMSAA;
    BOOL bUseRetina;
    NSInteger msaaSamples;
    ESRendererVersion rendererVersion;
}

/// TODO: need to give protocol explicity.
/// ???: can we assume EAGLKViewDelegate is inherit GLKViewDelegate?
@property (nonatomic, weak) id delegate;

- (instancetype)initWithFrame:(CGRect)frame
          andPreferedRenderer:(ESRendererVersion)rendererVersion
                        andAA:(bool)msaaEnabled
                    andRetina:(bool)retinaEnabled
               andRetinaScale:(CGFloat)retinaScale
                   sharegroup:(EAGLSharegroup*)sharegroup
                  colorFormat:(GLKViewDrawableColorFormat)colorFormat
                  depthFormat:(GLKViewDrawableDepthFormat)depthFormat
                stencilFormat:(GLKViewDrawableStencilFormat)stencilFormat;



- (void)setup;
- (void)update;
- (void)draw;
- (void)destroy;

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
#ifdef __IPHONE_9_1
- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches NS_AVAILABLE_IOS(9_1);
#endif
- (void)setMSAA:(bool)on;
- (void)notifyAnimationStarted;
- (void)notifyAnimationStopped;
- (void)notifyDraw;
- (void)notifyResized;

@end
