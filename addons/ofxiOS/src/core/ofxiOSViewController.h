//
//  ofxiOSViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)
#include "ofxiOSConstants.h"
#if defined(OF_UI_KIT)
#import <UIKit/UIKit.h>
#import "EAGLView.h"
#import "ofxiOSEAGLView.h"
class ofxiOSApp;
@class ofxiOSEAGLView;
@class EAGLViewDelegate;

@interface ofxiOSViewController : UIViewController

@property (nonatomic, strong) ofxiOSEAGLView * glView;

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup;

- (UIInterfaceOrientation)currentInterfaceOrientation;
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;
- (BOOL)isReadyToRotate;

@end

#define ofxPhoneViewController ofxiOSViewController

#endif
#endif
