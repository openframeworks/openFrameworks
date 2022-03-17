//
//  ofxiOSGLKView.mm
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 7/3/18.
//


#pragma once
#include <TargetConditionals.h>
#import <UIKit/UIKit.h>
#import "EAGLKView.h"
#include <glm/glm.hpp>

class ofxiOSApp;
class ofAppiOSWindow;

@interface ofxiOSGLKView : EAGLKView {

@protected
    NSMutableDictionary<NSValue *, NSNumber *> * activeTouches;
    glm::vec2 * screenSize;   // because glm::vec2 is forward declared,
    glm::vec2 * windowSize;   // these values have to be pointers.
    glm::vec2 * windowPos;
}

@property (readonly, nonatomic, getter=getScreenSize) glm::vec2 * screenSize;
@property (readonly, nonatomic, getter=getWindowSize) glm::vec2 * windowSize;
@property (readonly, nonatomic, getter=getWindowPosition) glm::vec2 * windowPos;

+ (ofxiOSGLKView *) getInstance;

- (instancetype)initWithFrame:(CGRect)frame
                       andApp:(ofxiOSApp *)app;
- (instancetype)initWithFrame:(CGRect)frame
                       andApp:(ofxiOSApp *)app
                   sharegroup:(EAGLSharegroup *)sharegroup;
- (void)setup;
- (void)update;
- (void)draw;
- (void)setMSAA:(bool)on;
- (void)updateDimensions;
- (void)destroy;
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint;
- (void)resetTouches;
- (UIImage*)getSnapshot;

@end

#define ofxiPhoneEAGLView ofxiOSEAGLView
