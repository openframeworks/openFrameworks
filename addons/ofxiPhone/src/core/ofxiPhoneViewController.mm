//
//  ofxiPhoneViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#import "ofxiPhoneViewController.h"

#import "ofBaseApp.h"
#import "ofxiOSEAGLView.h"

@interface ofxiPhoneViewController() <EAGLViewDelegate> {
    UIInterfaceOrientation currentInterfaceOrientation;
}
@end

@implementation ofxiPhoneViewController

@synthesize glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app {

    if((self = [super init])) {
        currentInterfaceOrientation = UIInterfaceOrientationPortrait;
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

- (void)rotateToInterfaceOrientationPortrait {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationPortrait animated:NO duration:0];
}

- (void)rotateToInterfaceOrientationPortraitUpsideDown {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationPortraitUpsideDown animated:NO duration:0];
}

- (void)rotateToInterfaceOrientationLandscapeLeft {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationLandscapeLeft animated:NO duration:0];
}

- (void)rotateToInterfaceOrientationLandscapeRight {
    [self rotateToInterfaceOrientation:UIInterfaceOrientationLandscapeRight animated:NO duration:0];
}

- (void)rotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
                            animated:(BOOL)animated
                            duration:(NSTimeInterval)duration {
    
    if(currentInterfaceOrientation == interfaceOrientation) {
        return;
    }
    
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    
    CGSize glViewSize = screenSize;
    if(UIInterfaceOrientationIsLandscape(interfaceOrientation)) {
        glViewSize.width = screenSize.height;
        glViewSize.height = screenSize.width;
    }
    
    CGAffineTransform rotate;
    if (interfaceOrientation == UIInterfaceOrientationPortrait) {
        rotate = CGAffineTransformMakeRotation(0);          // 0 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {
        rotate = CGAffineTransformMakeRotation(M_PI);       // 180 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeLeft) {
        rotate = CGAffineTransformMakeRotation(M_PI * 1.5); // 270 degrees.
    } else if (interfaceOrientation == UIInterfaceOrientationLandscapeRight) {
        rotate = CGAffineTransformMakeRotation(M_PI * 0.5); // 90 degrees.
    } else {
        return;
    }
    
    if(animated) {
        [UIView animateWithDuration:duration animations:^{
            self.glView.transform = rotate;
            self.glView.bounds = CGRectMake(0, 0, glViewSize.width, glViewSize.height);
        }];
    } else {
        self.glView.transform = rotate;
        self.glView.bounds = CGRectMake(0, 0, glViewSize.width, glViewSize.height);
    }
    
    currentInterfaceOrientation = interfaceOrientation;
}

@end
