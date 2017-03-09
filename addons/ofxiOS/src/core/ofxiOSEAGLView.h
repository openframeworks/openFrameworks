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
    NSMutableDictionary	* activeTouches;
	glm::vec2 * screenSize;   // because glm::vec2 is forward declared,
	glm::vec2 * windowSize;   // these values have to be pointers.
	glm::vec2 * windowPos;
}


+ (ofxiOSEAGLView *) getInstance;

- (id)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)app;
- (void)setup;
- (void)updateDimensions;
- (void)destroy;
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint;
- (void) resetTouches;
-(glm::vec2*)getScreenSize;
-(glm::vec2*)getWindowSize;
-(glm::vec2*)getWindowPosition;

@end

#define ofxiPhoneEAGLView ofxiOSEAGLView
