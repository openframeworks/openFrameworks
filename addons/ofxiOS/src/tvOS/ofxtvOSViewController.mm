//
//  ofxtvOSViewController.cpp
//  tvOS+OFLib
//
//  Created by Daniel Rosser on 26/10/2015.

#include "ofxtvOSViewController.h"
#import <QuartzCore/QuartzCore.h>
#include "ofxiOSEAGLView.h"

@interface ofxtvOSViewController() <EAGLViewDelegate> {
    UITapGestureRecognizer *tapRecognizer;
}
@end

@implementation ofxtvOSViewController

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    return [self initWithFrame:frame app:app sharegroup:nil];
}

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup {
    if((self = [super init])) {
        self.glView = [[ofxiOSEAGLView alloc] initWithFrame:frame andApp:app sharegroup:sharegroup];
        self.glView.delegate = self;
    }
    
    return self;
}

- (void) dealloc {
    [self.glView stopAnimation];
    [self.glView removeFromSuperview];
    self.glView.delegate = nil;
    self.glView = nil;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // glView is added here because if it is added inside initWithFrame,
    // it automatically triggers viewDidLoad, before initWithFrame has had a chance to return.
    // so now when we call setup in our OF app, a reference to ofxtvOSViewController will exists.
    
    [self.view addSubview:self.glView];
    [self.glView performSelector:@selector(setup) withObject:nil afterDelay:0];
    [self.glView startAnimation];
    
    tapRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(handleTap:)];
    tapRecognizer.allowedPressTypes = @[[NSNumber numberWithInteger:UIPressTypeMenu]];
    [self.view addGestureRecognizer:tapRecognizer];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.glView startAnimation];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    [self.glView stopAnimation];
}

//-------------------------------------------------------------- glView callbacks.
- (void)glViewAnimationStarted {
    //
}

- (void)glViewAnimationStopped {
    //
}

- (void)glViewDraw {
    //
}

- (void)glViewResized {
    //
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    
    // CALLBACK 2.
    // The window adjusts the bounds of the view controller’s view.
    // This causes the view to layout its subviews, triggering the view controller’s viewWillLayoutSubviews method.
    // When this method runs, you can query the app object’s statusBarOrientation property to determine the current user interface layout.
}

- (void)handleTap:(UITapGestureRecognizer *)sender {
    if([self.view respondsToSelector:@selector(handleTap:)]) {
        [self.glView performSelector:@selector(handleTap:)
                          withObject:sender];
    }
}

@end
