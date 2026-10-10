#pragma once
#include "ofxiOSConstants.h"
#if defined(TARGET_OF_IOS) && OF_USE_ANGLE

#import <UIKit/UIKit.h>
#include "ofConstants.h"
#include <glm/vec2.hpp>

class ofxiOSApp;

/// UIViewController that hosts an MGLKView (MetalANGLE). MGLKView is not subclassed
/// for drawing; this controller is the MGLKViewDelegate.
@interface ofxiOSMGLViewController : UIViewController
+ (ofxiOSMGLViewController *)getInstance;
- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (glm::vec2 *)getWindowPosition;
- (glm::vec2 *)getWindowSize;
- (glm::vec2 *)getScreenSize;
@end

#endif
