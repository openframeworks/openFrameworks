//
//  ofxiPhoneViewController.m
//  Created by lukasz karluk on 12/12/11.
//

#import "ofxiPhoneViewController.h"

#import "ofBaseApp.h"
#import "ofxiOSEAGLView.h"

@interface ofxiPhoneViewController() <EAGLViewDelegate> {
    //
}
@end

@implementation ofxiPhoneViewController

@synthesize glView;

- (id)initWithFrame:(CGRect)frame app:(ofxiPhoneApp *)app {

    if((self = [super init])) {
        self.glView = [[[ofxiOSEAGLView alloc] initWithFrame:frame andApp:app] autorelease];
        self.glView.delegate = self;
        [self.view addSubview:self.glView];
        [self.glView setup];
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

@end
