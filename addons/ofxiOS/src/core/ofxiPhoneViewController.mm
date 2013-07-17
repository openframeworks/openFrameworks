//
//  ofxiPhoneViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#import "ofxiPhoneViewController.h"
#import "ofxiOSEAGLView.h"

@interface ofxiPhoneViewController() <EAGLViewDelegate> {
    //
}
@end

@implementation ofxiPhoneViewController

@synthesize glView;
@synthesize currentInterfaceOrientation = _currentInterfaceOrientation;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app {
    if((self = [super init])) {
        _currentInterfaceOrientation = self.interfaceOrientation;
        
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
- (UIInterfaceOrientation)currentInterfaceOrientation {
    return _currentInterfaceOrientation;
}

- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
                            animated:(BOOL)animated {
    
    if(_currentInterfaceOrientation == interfaceOrientation) {
        return;
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGPoint center;
    center.x = screenSize.width * 0.5;
    center.y = screenSize.height * 0.5;
    
    CGSize glViewSize = screenSize;
    if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
        glViewSize.width = screenSize.height;
        glViewSize.height = screenSize.width;
    }
    
    float rot1 = [self rotationForOrinetation:_currentInterfaceOrientation];
    float rot2 = [self rotationForOrinetation:interfaceOrientation];
    float rot3 = rot2 - rot1;
    CGAffineTransform rotate = CGAffineTransformMakeRotation(rot3);
    rotate = CGAffineTransformConcat(rotate, self.glView.transform);
    
    if(animated) {
        NSTimeInterval duration = 0.3;
        if((UIInterfaceOrientationIsLandscape(_currentInterfaceOrientation) &&
            UIInterfaceOrientationIsLandscape(interfaceOrientation)) ||
           (UIInterfaceOrientationIsPortrait(_currentInterfaceOrientation) &&
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
    
    _currentInterfaceOrientation = interfaceOrientation;
    
    [self.glView updateDimensions];
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

@end
