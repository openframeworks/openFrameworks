//
//  ofxtvOSAppDelegate.hpp
//  tvOS+OFLib
//
//  Created by Daniel Rosser on 26/10/2015.
//

#pragma once
#include "ofxiOSConstants.h"
#if defined(TARGET_OF_TVOS) && defined(OF_UI_KIT) && defined(TARGET_OF_IOS) && !defined(TARGET_OF_WATCHOS) && !defined(TARGET_OF_XROS)
#ifndef ofxtvOSAppDelegate_h
#define ofxtvOSAppDelegate_h
#import <UIKit/UIKit.h>

@class ofxtvOSViewController;
@class ofxtvOSGLKViewController;

@interface ofxtvOSAppDelegate : NSObject <UIApplicationDelegate> {
}

@property (nonatomic, strong) UIWindow * window;
@property (nonatomic, strong) UIViewController * uiViewController;

- (BOOL)application:(UIApplication*)application
      handleOpenURL:(NSURL*)url;


@end
#endif /* ofxtvOSAppDelegate_h */
#endif
