//  ofxtvOSGLKViewController.mm
//  tvOS+OFLib
//
//  Created by Dan Rosser on 10/3/18.

#include "ofxtvOSGLKViewController.h"
#include "ofxiOSGLKView.h"

@interface ofxtvOSGLKViewController() <EAGLKViewDelegate, GLKViewControllerDelegate> {
    UITapGestureRecognizer *tapRecognizer;
}
@end

@implementation ofxtvOSGLKViewController

@synthesize glView;

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app {
    return [self initWithFrame:frame app:app sharegroup:nil];
}

- (instancetype)initWithFrame:(CGRect)frame app:(ofxiOSApp *)app sharegroup:(EAGLSharegroup *)sharegroup {
    if((self = [super init])) {
        self.glView = [[ofxiOSGLKView alloc] initWithFrame:frame andApp:app sharegroup:sharegroup];
        self.glView.delegate = self;
    }
    
    return self;
}

- (void) dealloc {
    [self.glView removeFromSuperview];
    self.glView.delegate = nil;
    self.glView = nil;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    GLKView *view = (GLKView *)self.view;
    view.context = [self.glView context];
    self.delegate = self;
    self.preferredFramesPerSecond = 60; //default
    [self.glView setup];
    
    tapRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(handleTap:)];
    tapRecognizer.allowedPressTypes = @[[NSNumber numberWithInteger:UIPressTypeMenu]];
    [self.view addGestureRecognizer:tapRecognizer];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}

- (void)glkViewControllerUpdate:(GLKViewController *)controller {
    [self.glView update];
}

- (void)glkViewController:(GLKViewController *)controller willPause:(BOOL)pause {
    
}

- (void) glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    [view bindDrawable];
    [self.glView draw];
}

-(void) checkError
{
    GLenum error = glGetError();
    
    if (error == GL_NO_ERROR)
        return;
    
    switch (error)
    {
        case GL_INVALID_ENUM:
            NSLog(@"Invalid Enum");
            break;
    }
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

- (EAGLSharegroup *)getSharegroup {
    if(self.glView != nil) {
        EAGLContext * context = [self.glView context];
        if(context)
            return [context sharegroup];
    }
    return nil;
}


- (void)setPreferredFPS:(int)fps {
    if(self.glView != nil) {
        self.preferredFramesPerSecond = fps;
    }
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
        [self.glView performSelector:@selector(handleTap:) withObject:sender];
    }
}

@end
