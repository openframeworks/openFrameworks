//
//  ofxiPhoneViewController.h
//  emptyExample
//
//  Created by lukasz karluk on 12/12/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"

@interface ofxiPhoneViewController : UIViewController
{
    //
}

@property (nonatomic, retain) EAGLView* glView;
@property (nonatomic, retain) NSLock*   glLock;
@property (nonatomic, retain) NSTimer*  animTimer;
@property (nonatomic, assign) float		animFrameInterval;
@property (nonatomic, assign) BOOL		animating;
@property (nonatomic, assign) BOOL		displayLinkSupported;
@property (nonatomic, assign) id		displayLink;


- (id) initWithFrame : (CGRect) frame 
                 app : (ofBaseApp*) app;

- (void) initApp : (ofBaseApp*) app;
- (void) initGLLock;
- (void) initGLViewWithFrame : (CGRect)frame;
- (void) initAnimationVars;
- (void) setupApp;
- (void) clearBuffers;
- (void) reloadTextures;

- (void) lockGL;
- (void) unlockGL;

- (void) timerLoop;

- (void) stopAnimation;
- (void) startAnimation;

- (void) setAnimationFrameInterval:(float)frameInterval;
- (void) setFrameRate:(float)frameRate;

- (void) destroy;

@end
