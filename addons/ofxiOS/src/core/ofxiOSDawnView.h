#pragma once
#include "ofxiOSConstants.h"
#if defined(TARGET_OF_IOS) && OF_USE_DAWN

#import <UIKit/UIKit.h>
#include "ofConstants.h"
#include <glm/vec2.hpp>

class ofxiOSApp;
class ofAppiOSWindow;

@interface ofxiOSDawnView : UIView
+ (ofxiOSDawnView *)getInstance;
- (instancetype)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)app;
- (void *)metalLayer;
- (glm::vec2 *)getWindowPosition;
- (glm::vec2 *)getWindowSize;
- (glm::vec2 *)getScreenSize;
- (void)startRender;
- (void)finishRender;
@end

@interface ofxiOSDawnViewController : UIViewController
- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
@property (nonatomic, strong) ofxiOSDawnView * dawnView;
@end

#endif
