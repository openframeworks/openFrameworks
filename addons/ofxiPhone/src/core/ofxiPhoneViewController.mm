//
//  ofxiPhoneViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#import "ofxiPhoneViewController.h"

#import "ofBaseApp.h"
#import "ofxiOSEAGLView.h"

@interface ofxiPhoneViewController() <EAGLViewDelegate> {
    UIInterfaceOrientation currentInterfaceOrientation;
    UIInterfaceOrientation actualInterfaceOrientation;
}
@end

@implementation ofxiPhoneViewController

@synthesize glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app {

    if((self = [super init])) {
        currentInterfaceOrientation = actualInterfaceOrientation = self.interfaceOrientation;
        self.glView = [[[ofxiOSEAGLView alloc] initWithFrame:frame andApp:app] autorelease];
        self.glView.delegate = self;
    }
    
    return self;
}

- (void) dealloc {
    [self.glView stopAnimation];
    [self.glView removeFromSuperview];
    self.glView.delegate = nil;
    self.glView = nil;
    
    [super dealloc];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    /**
     *  glView is added here because if it is added inside initWithFrame,
     *  it automatically triggers viewDidLoad, before initWithFrame has had a chance to return.
     *  so now when we call setup in our OF app, a reference to ofxiPhoneViewController will exists.
     */
    [self.view addSubview:self.glView];
    
    [self.glView setup];
    [self.glView startAnimation];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.glView startAnimation];
}

- (void) viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
    [self.glView stopAnimation];
}

//-------------------------------------------------------------- orientation.
- (UIInterfaceOrientation) getInterfaceOrientation {
    return currentInterfaceOrientation;
}

- (void)rotateToInterfaceOrientationPortrait:(BOOL)animated; {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationPortrait animated:animated];
}

- (void)rotateToInterfaceOrientationPortraitUpsideDown:(BOOL)animated; {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationPortraitUpsideDown animated:animated];
}

- (void)rotateToInterfaceOrientationLandscapeLeft:(BOOL)animated; {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationLandscapeLeft animated:animated];
}

- (void)rotateToInterfaceOrientationLandscapeRight:(BOOL)animated; {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationLandscapeRight animated:animated];
}

- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
                            animated:(BOOL)animated {
    
    if(currentInterfaceOrientation == interfaceOrientation) {
        return;
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGPoint center;
    if(UIInterfaceOrientationIsLandscape(actualInterfaceOrientation)) {
        center.x = screenSize.height * 0.5;
        center.y = screenSize.width * 0.5;
    } else {
        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
    }
    
    CGSize glViewSize = screenSize;
    if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
        glViewSize.width = screenSize.height;
        glViewSize.height = screenSize.width;
    }
    
    float rot1 = [self rotationForOrinetation:currentInterfaceOrientation];
    float rot2 = [self rotationForOrinetation:interfaceOrientation];
    float rot3 = rot2 - rot1;
    CGAffineTransform rotate = CGAffineTransformMakeRotation(rot3);
    rotate = CGAffineTransformConcat(rotate, self.glView.transform);
    
    if(animated) {
        NSTimeInterval duration = 0.3;
        if((UIInterfaceOrientationIsLandscape(currentInterfaceOrientation) && 
           UIInterfaceOrientationIsLandscape(interfaceOrientation)) ||
           (UIInterfaceOrientationIsPortrait(currentInterfaceOrientation) &&
            UIInterfaceOrientationIsPortrait(interfaceOrientation))) {
               duration = 0.6;
           }
        [UIView animateWithDuration:duration animations:^{
            self.glView.center = center;
            self.glView.transform = rotate;
            self.glView.bounds = CGRectMake(0, 0, glViewSize.width, glViewSize.height);
        }];
    } else {
        self.glView.center = center;
        self.glView.transform = rotate;
        self.glView.bounds = CGRectMake(0, 0, glViewSize.width, glViewSize.height);
    }
    
    currentInterfaceOrientation = interfaceOrientation;
}

- (float)rotationForOrinetation:(UIInterfaceOrientation)interfaceOrientation {
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

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation 
                                         duration:(NSTimeInterval)duration {

    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGPoint center;
    if(UIInterfaceOrientationIsLandscape(toInterfaceOrientation)) {
        center.x = screenSize.height * 0.5;
        center.y = screenSize.width * 0.5;
    } else {
        center.x = screenSize.width * 0.5;
        center.y = screenSize.height * 0.5;
    }
    
    self.glView.center = center;
    self.glView.transform = CGAffineTransformMakeRotation(0);
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation 
                                duration:(NSTimeInterval)duration {
    
    actualInterfaceOrientation = toInterfaceOrientation;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return (toInterfaceOrientation == currentInterfaceOrientation);
}

@end
