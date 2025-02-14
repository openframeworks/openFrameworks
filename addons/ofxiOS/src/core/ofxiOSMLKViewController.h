//
//  ofxiOSMLKViewController.h
//  iOS+OFLib
//
//  Created by Dan Rosser on 11/5/22.
//

#ifndef ofxiOSMLKViewController_h
#define ofxiOSMLKViewController_h

#pragma once

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#if defined(OF_METAL)

#import <MetalANGLE/MGLKViewController.h>
#import <MetalANGLE/MGLKit.h>
#import "EAMLKView.h"
#import "ofxiOSMLKView.h"

class ofxiOSApp;
//@class ofxiOSMLKView;


@interface ofxiOSMLKViewController : MGLKViewController


@property (nonatomic, strong) MGLKView * glView; // no subclass
@property (nonatomic, strong) ofxiOSMLKView * ofView;

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app;
- (void)setupApp:(ofxiOSApp *)appPtr;

- (void)mglkView:(MGLKView *)view drawInRect:(CGRect)rect;

- (UIInterfaceOrientation)currentInterfaceOrientation;
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient;
- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated;
- (BOOL)isReadyToRotate;
- (void)setPreferredFPS:(int)fps;
- (void)setMSAA:(bool)value;

@end

#endif

#endif /* OF_METAL_KIT */

#endif /* ofxiOSMLKViewController_h */
