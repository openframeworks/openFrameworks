//  ofxtvOSGLKViewController.mm
//  tvOS+OFLib
//
//  Created by Dan Rosser on 10/3/18.

#pragma once

#include "ofxiOSConstants.h"
#if defined(TARGET_OF_TVOS) && defined(OF_UI_KIT) &&  defined(OF_GL_KIT) && defined(TARGET_OF_IOS) && !defined(TARGET_OF_WATCHOS) && !defined(TARGET_OF_XROS)
#ifndef ofxtvOSGLKViewController_h
#define ofxtvOSGLKViewController_h

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class ofxiOSApp;
@class ofxiOSGLKView;

@interface ofxtvOSGLKViewController : GLKViewController

@property (nonatomic, retain) ofxiOSGLKView * glView;

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup;

- (void)setPreferredFPS:(int)fps;
- (EAGLSharegroup *)getSharegroup;

@end

#endif /* ofxtvOSViewController_h */
#endif
