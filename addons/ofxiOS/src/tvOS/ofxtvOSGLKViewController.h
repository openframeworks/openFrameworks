//  ofxtvOSGLKViewController.mm
//  tvOS+OFLib
//
//  Created by Dan Rosser on 10/3/18.

#pragma once

#ifndef ofxtvOSViewController_h
#define ofxtvOSViewController_h

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

class ofxiOSApp;
@class ofxiOSGLKView;

@interface ofxtvOSGLKViewController : GLKViewController

@property (nonatomic, retain) ofxiOSGLKView * glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (id)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup;

- (void)setPreferredFPS:(int)fps;
- (EAGLSharegroup *)getSharegroup;

@end

#endif /* ofxtvOSViewController_h */
