//
//  ofxiOSEAGLView.m
//  iOS+OFLib
//
//  Created by lukasz karluk on 5/07/12.
//

#include "ofxiOSEAGLView.h"
#include "ofxiOSApp.h"
#include "ofAppiOSWindow.h"
#include "ofGLRenderer.h"
#include "ofGLProgrammableRenderer.h"
#include <TargetConditionals.h>
#import <GameController/GameController.h>

static ofxiOSEAGLView * _instanceRef = nil;

@interface ofxiOSEAGLView() {
    BOOL bInit;
	shared_ptr<ofAppiOSWindow> window;
	shared_ptr<ofxiOSApp> app;
}
- (void)updateDimensions;
@end

@implementation ofxiOSEAGLView

@synthesize screenSize;
@synthesize windowSize;
@synthesize windowPos;

+ (ofxiOSEAGLView *) getInstance {
    return _instanceRef;
}

- (id)initWithFrame:(CGRect)frame andApp:(ofxiOSApp *)appPtr {
	
	window = dynamic_pointer_cast<ofAppiOSWindow>(ofGetMainLoop()->getCurrentWindow());
	
    if(window.get() == NULL) {
        ofLog(OF_LOG_FATAL_ERROR, "ofxiOSEAGLView::initWithFrame - window is NULL");
        return nil;
    }
	
    ESRendererVersion preferedRendererVersion = (ESRendererVersion)window->getSettings().glesVersion;
    
    self = [self initWithFrame:frame
           andPreferedRenderer:preferedRendererVersion
                      andDepth:window->isDepthBufferEnabled()
                         andAA:window->isAntiAliasingEnabled()
                 andNumSamples:window->getAntiAliasingSampleCount()
                     andRetina:window->isRetinaEnabled()
                andRetinaScale:window->getRetinaScale()];
    
    if(self) {
        
        _instanceRef = self;
        
        app = shared_ptr<ofxiOSApp>(appPtr);
        activeTouches = [[NSMutableDictionary alloc] init];
                
        screenSize = new ofVec3f();
        windowSize = new ofVec3f();
        windowPos = new ofVec3f();
		ofSetOrientation(window->getOrientation());
        [self updateDimensions];
        
        NSNotificationCenter * center = [NSNotificationCenter defaultCenter];
        [center addObserver:self
                   selector:@selector(controllerDidConnect)
                       name:GCControllerDidConnectNotification
                     object:nil];
        [center addObserver:self
                   selector:@selector(controllerDidDisconnect)
                       name:GCControllerDidDisconnectNotification
                     object:nil];
        
        [self controllerDidConnect];
        
		
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
	
	[activeTouches release];
	
	delete screenSize;
	screenSize = NULL;
	delete windowSize;
	windowSize = NULL;
	delete windowPos;
	windowPos = NULL;
	
    _instanceRef = nil;
    
    bInit = NO;
    
    [super destroy];
}

- (void)dealloc {
    [self destroy];
    [[NSNotificationCenter defaultCenter] removeObserver: self];
    [super dealloc];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self updateDimensions];
    
    [super notifyResized];
    window->events().notifyWindowResized(ofGetWidth(), ofGetHeight());
}

- (void)updateDimensions {
    windowPos->set(self.frame.origin.x * scaleFactor, self.frame.origin.y * scaleFactor, 0);
    windowSize->set(self.bounds.size.width * scaleFactor, self.bounds.size.height * scaleFactor, 0);
    
    UIScreen * currentScreen = self.window.screen;  // current screen is the screen that GLView is attached to.
    if(!currentScreen) {                            // if GLView is not attached, assume to be main device screen.
        currentScreen = [UIScreen mainScreen];
    }
    screenSize->set(currentScreen.bounds.size.width * scaleFactor, currentScreen.bounds.size.height * scaleFactor, 0);
}

- (void)notifyResized {
    // blank this.
    // we want to notifyResized at the end of layoutSubviews.
}

- (void)drawView {
    window->events().notifyUpdate();

    //------------------------------------------
    
    [self lockGL];
    [self startRender];
    
    window->renderer()->startRender();
    
    if(window->isSetupScreenEnabled()) {
        window->renderer()->setupScreen();
    }
    
    //------------------------------------------ draw.
    
    window->events().notifyDraw();
    
    //------------------------------------------
    
    window->renderer()->finishRender();
    
    [self finishRender];
    [self unlockGL];
    
    [super notifyDraw];   // alerts delegate that a new frame has been drawn.
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
			
		case OF_ORIENTATION_90_LEFT:
			touchPointOriented.x = touchPoint.y;
			touchPointOriented.y = ofGetHeight() - touchPoint.x;
			break;
			
		case OF_ORIENTATION_90_RIGHT:
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
- (void)touchesBegan:(NSSet *)touches 
           withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches) {
		int touchIndex = 0;
		while([[activeTouches allValues] containsObject:[NSNumber numberWithInt:touchIndex]]){
			touchIndex++;
		}
		
		[activeTouches setObject:[NSNumber numberWithInt:touchIndex] forKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
            window->events().notifyMousePressed(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
        touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
        if([touch tapCount] == 2){
			touchArgs.type = ofTouchEventArgs::doubleTap;
			ofNotifyEvent(window->events().touchDoubleTap,touchArgs);	// send doubletap
        }
		touchArgs.type = ofTouchEventArgs::down;
		ofNotifyEvent(window->events().touchDown,touchArgs);	// but also send tap (upto app programmer to ignore this if doubletap came that frame)
	}	
}

//------------------------------------------------------
- (void)touchesMoved:(NSSet *)touches 
           withEvent:(UIEvent *)event{
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
            window->events().notifyMouseDragged(touchPoint.x, touchPoint.y, 0);
		}		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
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
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
    
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		[activeTouches removeObjectForKey:[NSValue valueWithPointer:touch]];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		if( touchIndex==0 ){
			window->events().notifyMouseReleased(touchPoint.x, touchPoint.y, 0);
		}
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count] - [touches count];
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
    
    if(!bInit) {
        // if the glView is destroyed which also includes the OF app,
        // we no longer need to pass on these touch events.
        return; 
    }
	
	for(UITouch *touch in touches){
		int touchIndex = [[activeTouches objectForKey:[NSValue valueWithPointer:touch]] intValue];
		
		CGPoint touchPoint = [touch locationInView:self];
		
		touchPoint.x *= scaleFactor; // this has to be done because retina still returns points in 320x240 but with high percision
		touchPoint.y *= scaleFactor;
		touchPoint = [self orientateTouchPoint:touchPoint];
		
		ofTouchEventArgs touchArgs;
		touchArgs.numTouches = [[event touchesForView:self] count];
		touchArgs.x = touchPoint.x;
		touchArgs.y = touchPoint.y;
		touchArgs.id = touchIndex;
		touchArgs.type = ofTouchEventArgs::cancel;
		ofNotifyEvent(window->events().touchCancelled, touchArgs);
	}
	
	[self touchesEnded:touches withEvent:event];
}

- (void)controllerButtonEvent:(int)controllerID withType:(ofControllerEventArgs::Type)type withControllerType:(ofControllerEventArgs::ControllerType)controllerType withValue:(float)value withPressed:(BOOL)pressed {
    ofControllerEventArgs controllerArgs;
    controllerArgs.eventType = type;
    controllerArgs.value = value;
    controllerArgs.isPressed = pressed;
    controllerArgs.controllerID = controllerID;
    controllerArgs.controllerType = controllerType;
    if(pressed == true) {
        ofNotifyEvent(window->events().controllerPressed, controllerArgs);
    } else {
        ofNotifyEvent(window->events().controllerReleased, controllerArgs);
    }
}

- (void)controllerThumbstickEvent:(int)controllerID withType:(ofControllerEventArgs::Type)type withControllerType:(ofControllerEventArgs::ControllerType)controllerType withX:(float)xValue withY:(float)yValue {
    ofControllerEventArgs controllerArgs;
    controllerArgs.eventType = type;
    controllerArgs.x = xValue;
    controllerArgs.y = yValue;
    controllerArgs.controllerID = controllerID;
    controllerArgs.controllerType = controllerType;
    ofNotifyEvent(window->events().controllerPressed, controllerArgs);

}

#pragma mark - Notification Center
-(void) controllerDidConnect
{
    [[GCController controllers] enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop){
        GCController *controller=(GCController *)obj;
        int controllerID = (int)idx;
        string vendorName = [controller.vendorName UTF8String];
        long playerIndex = controller.playerIndex;
        ofLog(OF_LOG_VERBOSE, "Controller connected: id:" + ofToString(idx) + " vendor:" + vendorName + " playerIndex:" + ofToString(playerIndex));
        ofControllerEventArgs::ControllerType theControllerType;
        if(vendorName == "Remote") {
            theControllerType = ofControllerEventArgs::REMOTE;
        } else {
            theControllerType = ofControllerEventArgs::GENERIC;
        }
        
        
        ofControllerEventArgs controllerArgs;
        controllerArgs.eventType = ofControllerEventArgs::connected;
        controllerArgs.controllerID = controllerID;
        controllerArgs.value = playerIndex;
        controllerArgs.controllerType = theControllerType;
        ofNotifyEvent(window->events().controllerPressed, controllerArgs);
        
        GCGamepad * pad = controller.gamepad;
        if(pad != nil) {
            pad.buttonA.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::buttonA
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.buttonB.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::buttonB
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.buttonX.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::buttonX
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.buttonY.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::buttonY
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.dpad.up.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::upButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.dpad.down.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::downButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.dpad.left.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::leftButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            pad.dpad.right.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::rightButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
        }
        
        // -------------------------- Extended Game Pad
        GCExtendedGamepad *extendPad = controller.extendedGamepad;
        if(extendPad != nil) {
            extendPad.leftThumbstick.valueChangedHandler = ^(GCControllerDirectionPad *dpad, float xValue, float yValue){
                [self controllerThumbstickEvent:controllerID
                                       withType:ofControllerEventArgs::Type::leftThumbstick
                             withControllerType:theControllerType
                                          withX:xValue
                                          withY:yValue];
            };
            extendPad.rightThumbstick.valueChangedHandler = ^(GCControllerDirectionPad *dpad, float xValue, float yValue){
                [self controllerThumbstickEvent:controllerID
                                       withType:ofControllerEventArgs::Type::rightThumbstick
                             withControllerType:theControllerType
                                          withX:xValue
                                          withY:yValue];
            };
            extendPad.leftShoulder.valueChangedHandler= ^ (GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::l1Button
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            extendPad.rightShoulder.valueChangedHandler= ^ (GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::r1Button
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            extendPad.rightTrigger.valueChangedHandler = ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::r2Button
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            extendPad.leftTrigger.valueChangedHandler = ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::l2Button
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
        }
#if TARGET_OS_TV
        // -------------------------- Micro Game Pad (AppleTV)
        GCMicroGamepad *microPad = controller.microGamepad;
        microPad.reportsAbsoluteDpadValues = YES;
        microPad.allowsRotation = YES;
        if(microPad != nil) {
            microPad.buttonA.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microButtonA
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            microPad.buttonX.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microButtonX
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            microPad.dpad.up.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microUpButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            microPad.dpad.down.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microDownButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            microPad.dpad.left.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microLeftButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
            microPad.dpad.right.valueChangedHandler =  ^(GCControllerButtonInput *button, float value, BOOL pressed){
                [self controllerButtonEvent:controllerID
                                   withType:ofControllerEventArgs::Type::microRightButton
                         withControllerType:theControllerType
                                  withValue:value
                                withPressed:pressed];
            };
        }

#endif
        
    
    }];
    
    
}
-(void) controllerDidDisconnect
{
    ofControllerEventArgs controllerArgs;
    controllerArgs.eventType = ofControllerEventArgs::disconnected;
    controllerArgs.controllerID = -1;
    ofNotifyEvent(window->events().controllerPressed, controllerArgs);
    
    [GCController startWirelessControllerDiscoveryWithCompletionHandler:^{
        ofLog(OF_LOG_VERBOSE, "startWirelessControllerDiscoveryWithCompletionHandler");
    }];
}

#if TARGET_OS_TV
- (void)handleTap:(UITapGestureRecognizer *)sender {
    if (sender.state == UIGestureRecognizerStateEnded) {
        [self controllerButtonEvent:-1
                           withType:ofControllerEventArgs::Type::menuButton
                 withControllerType:ofControllerEventArgs::REMOTE
                          withValue:0
                        withPressed:NO];
    }
}
#endif

@end
