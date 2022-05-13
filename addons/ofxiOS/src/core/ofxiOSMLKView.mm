//
//  ofxiOSGLKView.mm
//  iPhone+OF Static Library
//
//  Created by Dan Rosser on 7/3/18.
//

#include "ofxiOSMLKView.h"
#include "ofxiOSApp.h"
#include "ofAppiOSWindow.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include <TargetConditionals.h>
#import <GameController/GameController.h>

static ofxiOSMLKView * _instanceRef = nil;

@interface ofxiOSMLKView() {
    BOOL bInit;
    shared_ptr<ofAppiOSWindow> window;
    shared_ptr<ofxiOSApp> app;
    BOOL bSetup;
}
- (void)updateDimensions;
@end

@implementation ofxiOSMLKView

@synthesize screenSize;
@synthesize windowSize;
@synthesize windowPos;


+ (ofxiOSMLKView *) getInstance {
    return _instanceRef;
}

+(id)alloc{
    NSLog(@"Allocating...");
//    return self;
   return [super alloc];
}

- (instancetype)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)appPtr {
   
    window = dynamic_pointer_cast<ofAppiOSWindow>(ofGetMainLoop()->getCurrentWindow());
    
    if(window.get() == NULL) {
        ofLog(OF_LOG_FATAL_ERROR, "ofxiOSEAMLView::initWithFrame - window is NULL");
        return nil;
    }
    
//    ESRendererVersion preferedRendererVersion = (ESRendererVersion)window->getSettings().glesVersion;
//
     
    
//    self = [self initWithFrame:frame
//           andPreferedRenderer:preferedRendererVersion
//                         andAA:window->isAntiAliasingEnabled()
//                     andRetina:window->isRetinaEnabled()
//                andRetinaScale:window->getRetinaScale()
//                   colorFormat:(MGLDrawableColorFormat)window->getRendererColorType()
//                   depthFormat:(MGLDrawableDepthFormat)window->getRendererDepthType()
//                 stencilFormat:(MGLDrawableStencilFormat)window->getRendererStencilType()];
    
    bSetup = NO;
    if(self) {
        
        _instanceRef = self;
        
        app = shared_ptr<ofxiOSApp>(appPtr);
        activeTouches = [[NSMutableDictionary alloc] init];
                
        screenSize = new glm::vec2();
        windowSize = new glm::vec2();
        windowPos = new glm::vec2();
        ofSetOrientation(window->getOrientation());
        [self updateDimensions];
        
        bInit = YES;
    }
    
    return self;
}

- (void)setup {
    if(window.get() == NULL) {
        ofLog(OF_LOG_FATAL_ERROR, "ofxiOSEAGLView setup. Failed setup. window is NULL");
        return;
    }
    
    if(app.get() != ofGetAppPtr()) { // check if already running.
        
        ofSetMainLoop(shared_ptr<ofMainLoop>(NULL)); // destroy old main loop.
        auto mainLoop = std::make_shared<ofMainLoop>(); // make new main loop.
        ofSetMainLoop(mainLoop);
        
        ofiOSWindowSettings windowSettings = window->getSettings();
        window = NULL;

        window = dynamic_pointer_cast<ofAppiOSWindow>(ofCreateWindow(windowSettings));

        ofRunApp(app);
    }
    
    if(window->isProgrammableRenderer() == true) {
        static_cast<ofGLProgrammableRenderer*>(window->renderer().get())->setup(window->getSettings().glesVersion, 0);
    } else{
        static_cast<ofGLRenderer*>(window->renderer().get())->setup();
    }
    
    ofxiOSAlerts.addListener(app.get());
    
    ofDisableTextureEdgeHack();
    
    window->events().notifySetup();
    bSetup = YES;
    window->renderer()->clear();
}

- (void)destroy {
    if(!bInit) {
        return;
    }

    window->events().notifyExit();
    
    ofxiOSAlerts.removeListener(app.get());

    ofGetMainLoop()->exit();
    
    app = NULL;
    window = NULL;
    
    activeTouches = nil;
    delete screenSize;
    screenSize = NULL;
    delete windowSize;
    windowSize = NULL;
    delete windowPos;
    windowPos = NULL;
    
    _instanceRef = nil;
    
    bInit = NO;
    
//    [super destroy];
}
- (void)mglkView:(MGLKView *)view drawInRect:(CGRect)rect {
//    [super mglkView:view drawInRect:rect];
}

- (void)dealloc {
    [self destroy];
}

- (void)layoutSubviews {
//    [super layoutSubviews];
//    [self updateDimensions];
//
//    [super notifyResized];
    window->events().notifyWindowResized(ofGetWidth(), ofGetHeight());
}

- (void)updateDimensions {
    *windowPos = glm::vec2(theMetal.frame.origin.x * scaleFactor, theMetal.frame.origin.y * scaleFactor);
    *windowSize = glm::vec2(theMetal.bounds.size.width * scaleFactor, theMetal.bounds.size.height * scaleFactor);

    UIScreen * currentScreen = theMetal.window.screen;  // current screen is the screen that GLView is attached to.
    if(!currentScreen) {                            // if GLView is not attached, assume to be main device screen.
        currentScreen = [UIScreen mainScreen];
    }
    *screenSize = glm::vec2(currentScreen.bounds.size.width * scaleFactor, currentScreen.bounds.size.height * scaleFactor);
}

- (void) setMSAA:(bool)on
{
    if(on)
        theMetal.drawableMultisample = MGLDrawableMultisample4X;
    else
        theMetal.drawableMultisample = MGLDrawableMultisampleNone;
}

- (void)notifyResized {
    // blank this.
    // we want to notifyResized at the end of layoutSubviews.
}

- (void)update {
    if(bSetup == NO) return;
    
    window->events().notifyUpdate();
}


- (void)draw {
    if(bSetup == NO) return;
    
    window->renderer()->startRender();
    
    if(window->isSetupScreenEnabled()) {
        window->renderer()->setupScreen();
    }
    
    //------------------------------------------ draw.
    
    window->events().notifyDraw();
    
    //------------------------------------------
    
    window->renderer()->finishRender();
    
//    [super notifyDraw];   // alerts delegate that a new frame has been drawn.
}


- (void)notifyDraw {
    // blank this.
    // we want to notifyDraw at the end of drawView.
}

//------------------------------------------------------
- (CGPoint)orientateTouchPoint:(CGPoint)touchPoint {
    
    if(ofAppiOSWindow::getInstance()->doesHWOrientation()) {
        return touchPoint;
    }
    
    ofOrientation orientation = ofGetOrientation();
    CGPoint touchPointOriented = CGPointZero;
    
    switch(orientation) {
        case OF_ORIENTATION_180:
            touchPointOriented.x = ofGetWidth() - touchPoint.x;
            touchPointOriented.y = ofGetHeight() - touchPoint.y;
            break;
            
        case OF_ORIENTATION_90_RIGHT:
            touchPointOriented.x = touchPoint.y;
            touchPointOriented.y = ofGetHeight() - touchPoint.x;
            break;
            
        case OF_ORIENTATION_90_LEFT:
            touchPointOriented.x = ofGetWidth() - touchPoint.y;
            touchPointOriented.y = touchPoint.x;
            break;
            
        case OF_ORIENTATION_DEFAULT:
        default:
            touchPointOriented = touchPoint;
            break;
    }
    return touchPointOriented;
}

//------------------------------------------------------

-(void) resetTouches {

    [activeTouches removeAllObjects];
}

- (void)touchesBegan:(NSSet *)touches
           withEvent:(UIEvent *)event{
    
    if(!bInit || !bSetup) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }
    
    for(UITouch *touch in touches) {
        int touchIndex = 0;
        while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]){
            touchIndex++;
        }
        
        [activeTouches setObject:@(touchIndex) forKey:[NSValue valueWithPointer:(__bridge void *)touch]];
        
        CGPoint touchPoint = [touch locationInView:theMetal];
        
        touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
        touchPoint.y *= scaleFactor;
        touchPoint = [self orientateTouchPoint:touchPoint];
        
        if( touchIndex==0 ){
            window->events().notifyMousePressed(touchPoint.x, touchPoint.y, 0);
        }
        
        ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:theMetal] count];
        touchArgs.x = touchPoint.x;
        touchArgs.y = touchPoint.y;
        touchArgs.id = touchIndex;
        if([touch tapCount] == 2){
            touchArgs.type = ofTouchEventArgs::doubleTap;
            ofNotifyEvent(window->events().touchDoubleTap,touchArgs);   // send doubletap
        }
        touchArgs.type = ofTouchEventArgs::down;
        ofNotifyEvent(window->events().touchDown,touchArgs);    // but also send tap (upto app programmer to ignore this if doubletap came that frame)
    }
}

- (void)setupMetal:(MGLKView *)metal {

    theMetal = metal;
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches
           withEvent:(UIEvent *)event{
    
    if(!bInit || !bSetup) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }
    
    for(UITouch *touch in touches){
        int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(__bridge void *)touch]] intValue];
        
        CGPoint touchPoint = [touch locationInView:theMetal];
        
        touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
        touchPoint.y *= scaleFactor;
        touchPoint = [self orientateTouchPoint:touchPoint];
        
        if( touchIndex==0 ){
            window->events().notifyMouseDragged(touchPoint.x, touchPoint.y, 0);
        }
        ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:theMetal] count];
        touchArgs.x = touchPoint.x;
        touchArgs.y = touchPoint.y;
        touchArgs.id = touchIndex;
        touchArgs.type = ofTouchEventArgs::move;
        ofNotifyEvent(window->events().touchMoved, touchArgs);
    }
}

//------------------------------------------------------
- (void)touchesEnded:(NSSet *)touches
           withEvent:(UIEvent *)event{
    
    if(!bInit || !bSetup) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }
    
    for(UITouch *touch in touches){
        int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(__bridge void *)touch]] intValue];
        
        [activeTouches removeObjectForKey:[NSValue valueWithPointer:(__bridge void *)touch]];
        
        CGPoint touchPoint = [touch locationInView:theMetal];
        
        touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
        touchPoint.y *= scaleFactor;
        touchPoint = [self orientateTouchPoint:touchPoint];
        
        if( touchIndex==0 ){
            window->events().notifyMouseReleased(touchPoint.x, touchPoint.y, 0);
        }
        
        ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:theMetal] count] - [touches count];
        touchArgs.x = touchPoint.x;
        touchArgs.y = touchPoint.y;
        touchArgs.id = touchIndex;
        touchArgs.type = ofTouchEventArgs::up;
        ofNotifyEvent(window->events().touchUp, touchArgs);
    }
}

//------------------------------------------------------
- (void)touchesCancelled:(NSSet *)touches
               withEvent:(UIEvent *)event{
    
    if(!bInit || !bSetup) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return;
    }
    
    for(UITouch *touch in touches){
        int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:(__bridge void *)touch]] intValue];
        
        CGPoint touchPoint = [touch locationInView:theMetal];
        
        touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
        touchPoint.y *= scaleFactor;
        touchPoint = [self orientateTouchPoint:touchPoint];
        
        ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:theMetal] count];
        touchArgs.x = touchPoint.x;
        touchArgs.y = touchPoint.y;
        touchArgs.id = touchIndex;
        touchArgs.type = ofTouchEventArgs::cancel;
        ofNotifyEvent(window->events().touchCancelled, touchArgs);
    }
    
    [self touchesEnded:touches withEvent:event];
}


- (void)updateScaleFactor {
    //GLKView *view = (GLKView *)self;
    
    scaleFactor = MIN(scaleFactorPref, [theMetal contentScaleFactor]);
    if(scaleFactor != theMetal.contentScaleFactor) {
        theMetal.contentScaleFactor = scaleFactor;
    }
}







@end
