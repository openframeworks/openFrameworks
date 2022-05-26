//
//  ofxiOSViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#include <TargetConditionals.h>
#if TARGET_OS_IOS || (TARGET_OS_IPHONE && !TARGET_OS_TV)

#import <QuartzCore/QuartzCore.h>

#include "ofxiOSViewController.h"
#include "ofxiOSEAGLView.h"
#include "ofAppiOSWindow.h"
#import "ofxiOSExtras.h"

@interface ofxiOSViewController() <EAGLViewDelegate> {
    UIInterfaceOrientation currentInterfaceOrientation;
    UIInterfaceOrientation pendingInterfaceOrientation;
    BOOL bReadyToRotate;
    BOOL bFirstUpdate;
    BOOL bAnimated;
    EAGLSharegroup * sharegroup;
}
@end

@implementation ofxiOSViewController

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    return [self initWithFrame:frame app:app sharegroup:nil];
}

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup{
    currentInterfaceOrientation = pendingInterfaceOrientation = UIInterfaceOrientationPortrait;
    if((self = [super init])) {
        currentInterfaceOrientation = pendingInterfaceOrientation = self.interfaceOrientation;
        if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
            bReadyToRotate  = NO;
        }else{
            bReadyToRotate  = YES;
        }
        bFirstUpdate    = NO;
        bAnimated       = NO;
        self.glView = [[ofxiOSEAGLView alloc] initWithFrame:frame andApp:app sharegroup:sharegroup];
        
        [self.glView setMultipleTouchEnabled:ofxiOSGetOFWindow()->isMultiTouch()];
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
    // so now when we call setup in our OF app, a reference to ofxiOSViewController will exists.
    
    [self.view addSubview:self.glView];
    [self.glView performSelector:@selector(setup) withObject:nil afterDelay:0];
    [self.glView startAnimation];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.glView startAnimation];
    [self.glView resetTouches];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
    // rotation of the glView only works properly after viewDidAppear.
    // this is something to do with either the bounds, center or transform properties not being initialised earlier.
    // so now that glView is ready, we rotate it to the pendingInterfaceOrientation.
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        bReadyToRotate  = YES;
        bFirstUpdate    = YES;
        [self rotateToInterfaceOrientation:pendingInterfaceOrientation animated:NO];
    }
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

//-------------------------------------------------------------- orientation.
- (UIInterfaceOrientation)currentInterfaceOrientation {
    return currentInterfaceOrientation;
}

//-------------------------------------------------------------- orientation.
- (void)setCurrentInterfaceOrientation:(UIInterfaceOrientation) orient {
    currentInterfaceOrientation = pendingInterfaceOrientation = orient;
}

- (float)rotationForOrientation:(UIInterfaceOrientation)interfaceOrientation {
    if (interfaceOrientation == UIInterfaceOrientationPortrait) {
        return 0;           // 0 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
        return M_PI * 0.5;  // 90 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
        return M_PI;        // 180 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft) {
        return M_PI * 1.5;  // 270 degrees.
    } else {
        return 0;
    }
}

- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated {
    bAnimated = animated;

    
    if(bReadyToRotate == NO) {
        pendingInterfaceOrientation = interfaceOrientation;
        
        // we need to update the dimensions here, so if ofSetOrientation is called in setup,
        // then it will return the correct width and height
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width   = screenSize.height;
            bounds.size.height  = screenSize.width;
        }
        self.glView.bounds = bounds;
        [self.glView updateDimensions];
        
        return;
    }
 
    
    if(currentInterfaceOrientation == interfaceOrientation && !bFirstUpdate) {
        return;
    }
    
    if(pendingInterfaceOrientation != interfaceOrientation) {
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width   = screenSize.height;
            bounds.size.height  = screenSize.width;
        }
        self.glView.bounds = bounds;
        [self.glView updateDimensions];
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    CGPoint center;
    CGRect bounds = CGRectMake(0, 0, screenSize.width, screenSize.height);
    
    if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
        center.x = screenSize.height * 0.5;
        center.y = screenSize.width * 0.5;
    } else {
        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
    }
    
    // Is the iOS version less than 8?
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
            bounds.size.width = screenSize.height;
            bounds.size.height = screenSize.width;
        }
    } else {
        // Fixes for iOS 8 Portrait to Landscape issues
        if((UIInterfaceOrientationIsPortrait(interfaceOrientation) && screenSize.width >= screenSize.height) ||
           (UIInterfaceOrientationIsLandscape(interfaceOrientation) && screenSize.height >= screenSize.width)) {
            bounds.size.width = screenSize.height;
            bounds.size.height = screenSize.width;
        } else {
            bounds.size.width = screenSize.width;
            bounds.size.height = screenSize.height;
        }
        //borg
        //NSLog(@"w %f h %f",[UIScreen mainScreen].bounds.size.width,[UIScreen mainScreen].bounds.size.height);

        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
        
        //NSLog(@"rotating to portrait %i, is portrait %i, currentInterfaceOrientation %i, bound: w %f h %f",UIInterfaceOrientationIsPortrait(interfaceOrientation),UIInterfaceOrientationIsPortrait(self.interfaceOrientation),UIInterfaceOrientationIsPortrait(currentInterfaceOrientation),bounds.size.width,bounds.size.height);
    }
    
    float rot1 = [self rotationForOrientation:currentInterfaceOrientation];
    float rot2 = [self rotationForOrientation:interfaceOrientation];
    float rot3 = rot2 - rot1;
    CGAffineTransform rotate = CGAffineTransformMakeRotation(rot3);
    rotate = CGAffineTransformConcat(rotate, self.glView.transform);
    
    if(animated) {
        NSTimeInterval duration = 0.3;
        if((UIInterfaceOrientationIsLandscape(currentInterfaceOrientation) && UIInterfaceOrientationIsLandscape(interfaceOrientation)) ||
           (UIInterfaceOrientationIsPortrait(currentInterfaceOrientation) && UIInterfaceOrientationIsPortrait(interfaceOrientation))) {
               duration = 0.6;
           }
        [self.glView.layer removeAllAnimations];
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = rotate;
            self.glView.bounds = bounds;
        }];
    } else {
        self.glView.center = center;
        self.glView.transform = rotate;
        self.glView.bounds = bounds;
    }
    
    currentInterfaceOrientation = interfaceOrientation;
    bFirstUpdate = NO;
    
    [self.glView updateDimensions];
}


//-------------------------------------------------------------- orientation callbacks.
// http://developer.apple.com/library/ios/#featuredarticles/ViewControllerPGforiPhoneOS/RespondingtoDeviceOrientationChanges/RespondingtoDeviceOrientationChanges.html

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
                                duration:(NSTimeInterval)duration {

    // CALLBACK 1.
    // The window calls the root view controller’s willRotateToInterfaceOrientation:duration: method.
    // Container view controllers forward this message on to the currently displayed content view controllers.
    // You can override this method in your custom content view controllers to hide views or make other changes to your view layout before the interface is rotated.
    // Deprecated in iOS 8. See viewWillTransitionToSize below.
}

- (void)viewWillLayoutSubviews {
    [super viewWillLayoutSubviews];
    
    // CALLBACK 2.
    // The window adjusts the bounds of the view controller’s view.
    // This causes the view to layout its subviews, triggering the view controller’s viewWillLayoutSubviews method.
    // When this method runs, you can query the app object’s statusBarOrientation property to determine the current user interface layout.
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
                                         duration:(NSTimeInterval)duration {
    
    // CALLBACK 3.
    // This method is called from within an animation block so that any property changes you make
    // are animated at the same time as other animations that comprise the rotation.
    // Deprecated in iOS 8. See viewWillTransitionToSize below.
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;

    CGPoint center;
    // Is the iOS version less than 8?
    if( [[[UIDevice currentDevice] systemVersion] compare:@"8.0" options:NSNumericSearch] == NSOrderedAscending ) {
        if(UIInterfaceOrientationIsLandscape(self.interfaceOrientation)) {
            center.x = screenSize.height * 0.5;
            center.y = screenSize.width * 0.5;
        } else {
            center.x = screenSize.width * 0.5;
            center.y = screenSize.height * 0.5;
        }
    } else {
        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
    }
    
    if(bAnimated) {
        NSTimeInterval duration = 0.3;
        [self.glView.layer removeAllAnimations];
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = CGAffineTransformMakeRotation(0);
        }];
    } else {
        self.glView.center = center;
        self.glView.transform = CGAffineTransformMakeRotation(0);
    }
}

// iOS8+ version of willAnimateRotationToInterfaceOrientation
//NOTE: Only called if actually resizing and not masked
//http://stackoverflow.com/questions/25935006/ios8-interface-rotation-methods-not-called

//borg
#ifdef __IPHONE_8_0
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {

    CGPoint center;
    
    center.x = size.width * 0.5;
    center.y = size.height * 0.5;

    
    if(bAnimated) {
        NSTimeInterval duration = 0.3;
        [self.glView.layer removeAllAnimations];
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = CGAffineTransformMakeRotation(0);
            self.glView.frame = CGRectMake(0, 0, size.width,size.height);
        }];
    } else {
        self.glView.center = center;
        self.glView.transform = CGAffineTransformMakeRotation(0);
        self.glView.frame = CGRectMake(0, 0, size.width,size.height);
    }
}
#endif


- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {

    // CALLBACK 4.
    // This action marks the end of the rotation process.
    // You can use this method to show views, change the layout of views, or make other changes to your app.
}

//-------------------------------------------------------------- iOS5 and earlier.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return (toInterfaceOrientation == currentInterfaceOrientation);
}

//-------------------------------------------------------------- iOS6.
#ifdef __IPHONE_6_0
- (NSUInteger)supportedInterfaceOrientations {
    switch (currentInterfaceOrientation) {
        case UIInterfaceOrientationPortrait:
            return UIInterfaceOrientationMaskPortrait;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            return UIInterfaceOrientationMaskPortraitUpsideDown;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            return UIInterfaceOrientationMaskLandscapeLeft;
            break;
        case UIInterfaceOrientationLandscapeRight:
            return UIInterfaceOrientationMaskLandscapeRight;
            break;
        default:
            break;
    }
    // defaults to orientations selected in the .plist file ('Supported Interface Orientations' in the XCode Project)
    return -1; 
}
- (BOOL)shouldAutorotate {
    return YES;
}
#endif

- (BOOL)isReadyToRotate {
    return bReadyToRotate;
}

#ifdef __IPHONE_7_0
-(BOOL)prefersStatusBarHidden{
    return YES;
}
#endif

@end

#endif
