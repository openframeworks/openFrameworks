//
//  ofxiOSGLKViewController.h
//  iOS+OFLib
//
//  Created by Dan R on 7/3/18.
//

#ifndef ofxiOSGLKViewController_h
#define ofxiOSGLKViewController_h

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class ofxiOSApp;
@class ofxiOSGLKView;

@interface ofxiOSGLKViewController : GLKViewController

@property (nonatomic, strong) ofxiOSGLKView * glView;

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup;

- (UIInterfaceOrientation)currentInterfaceOrientation;
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;
- (BOOL)isReadyToRotate;
- (void)setPreferredFPS:(int)fps;
- (void)setMSAA:(bool)value;
- (EAGLSharegroup *)getSharegroup;

@end

#endif


#endif /* ofxiOSGLKViewController_h */
