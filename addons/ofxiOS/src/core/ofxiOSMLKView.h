//
//  ofxiOSGLKView.mm
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 7/3/18.
//


#pragma once
#include <TargetConditionals.h>
#import <UIKit/UIKit.h>
#import "EAMLKView.h"
#import <MetalANGLE/MGLKViewController.h>
#import <MetalANGLE/MGLKit.h>
#include <glm/glm.hpp>

class ofxiOSApp;
class ofAppiOSWindow;

@interface ofxiOSMLKView : NSObject  {

@protected
    NSMutableDictionary<NSValue *, NSNumber *> * activeTouches;
    glm::vec2 * screenSize;   // because glm::vec2 is forward declared,
    glm::vec2 * windowSize;   // these values have to be pointers.
    glm::vec2 * windowPos;
    MGLKView * theMetal;
    CGFloat scaleFactor;
    CGFloat scaleFactorPref;
}

@property (readonly, nonatomic, getter=getScreenSize) glm::vec2 * screenSize;
@property (readonly, nonatomic, getter=getWindowSize) glm::vec2 * windowSize;
@property (readonly, nonatomic, getter=getWindowPosition) glm::vec2 * windowPos;

+ (ofxiOSMLKView *) getInstance;

- (instancetype)initWithFrame:(CGRect)frame
                       andApp:(ofxiOSApp *)app;

- (void)mglkView:(MGLKView *)view drawInRect:(CGRect)rect;
- (void)setupMetal:(MGLKView *)metal;
- (void)setup;
- (void)update;
- (void)draw;
- (void)setMSAA:(bool)on;
- (void)updateDimensions;
- (void)destroy;
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint;
- (void)resetTouches;

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
//- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches;
//- (void)notifyAnimationStarted;
//- (void)notifyAnimationStopped;
//- (void)notifyDraw;
//- (void)notifyResized;


@end
