//
//  ofxiOSEAGLView.h
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#pragma once
#include <TargetConditionals.h>
#import <UIKit/UIKit.h>
#import "EAGLView.h"
#include <glm/glm.hpp>

class ofxiOSApp;
class ofAppiOSWindow;

@interface ofxiOSEAGLView : EAGLView {

@protected
    NSMutableDictionary<NSValue *, NSNumber *> *activeTouches;
    glm::vec2 * screenSize;   // because glm::vec2 is forward declared,
    glm::vec2 * windowSize;   // these values have to be pointers.
    glm::vec2 * windowPos;
}

@property (readonly, nonatomic, getter=getScreenSize) glm::vec2 * screenSize;
@property (readonly, nonatomic, getter=getWindowSize) glm::vec2 * windowSize;
@property (readonly, nonatomic, getter=getWindowPosition) glm::vec2 * windowPos;

+ (ofxiOSEAGLView *) getInstance;

- (instancetype)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)app;
- (instancetype)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup;
- (void)setup;
- (void)updateDimensions;
- (void)destroy;
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint;
- (void)resetTouches;

@end

#define ofxiPhoneEAGLView ofxiOSEAGLView
