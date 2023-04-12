//
//  ofxiOSViewController.h
//  Created by lukasz karluk on 12/12/11.
//

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <UIKit/UIKit.h>

class ofxiOSApp;
@class ofxiOSEAGLView;

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
