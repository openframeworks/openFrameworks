
#include "ofApp.h"

#import "MyViewController.h"
#import "AlertViewDelegate.h"

MyViewController * viewController = nil;
AlertViewDelegate * alertViewDelegate = nil;

//--------------------------------------------------------------
void ofApp::setup(){	
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	ofBackground(70);
    
    ofSetLogLevel(OF_LOG_VERBOSE);  
    
    //-- create buttons.
    int buttonW = 140;
    int buttonH = 76;
    buttonExternalDisplayRect.width = buttonW;
    buttonExternalDisplayRect.height = buttonH;
    buttonExternalDisplayRect.x = (int)((ofGetWidth() - buttonW * 2) / 3);
    buttonExternalDisplayRect.y = (int)(ofGetHeight() * 0.75);
    
    buttonMirrorDisplayRect.width = buttonW;
    buttonMirrorDisplayRect.height = buttonH;
    buttonMirrorDisplayRect.x = (int)(buttonExternalDisplayRect.x * 2 + buttonW);
    buttonMirrorDisplayRect.y = buttonExternalDisplayRect.y;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	
    ofDisableSmoothing();
    ofSetLineWidth(2);
    
    ofPoint p1;
    ofPoint p2;
    
    //-- draw width arrow.
    
    p1.set(15, ofGetHeight()-10);
    p2.set(ofGetWidth()-15, ofGetHeight()-10);
    
	ofSetColor(ofColor::magenta);
    ofPushMatrix();
    {
        ofTranslate(p1.x, p1.y);
        ofRotate(45);
        ofLine(0, 0, 0, -10);
        ofRotate(90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofPushMatrix();
    {
        ofTranslate(p2.x, p2.y);
        ofRotate(-45);
        ofLine(0, 0, 0, -10);
        ofRotate(-90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetWidth()), ofGetWidth()*0.5-10, ofGetHeight()-20);
    
    //-- draw height arrow.
    
    p1.set(ofGetWidth()-10, 15);
    p2.set(ofGetWidth()-10, ofGetHeight()-15);
    
    ofSetColor(ofColor::green);
    ofPushMatrix();
    {
        ofTranslate(p1.x, p1.y);
        ofRotate(-135);
        ofLine(0, 0, 0, -10);
        ofRotate(-90);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofPushMatrix();
    {
        ofTranslate(p2.x, p2.y);
        ofRotate(-45);
        ofLine(0, 0, 0, -10);
        ofRotate(-270);
        ofLine(0, 0, 0, -10);
    }
    ofPopMatrix();
    ofLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetHeight()), ofGetWidth()-50, ofGetHeight()*0.5-5);
    
    ofSetLineWidth(1);
    
    //-- debug information.
    
    ofSetColor(ofColor::white);
    
    int x = ofGetWidth() * 0.1;
    int y = ofGetHeight() * 0.1;
    int p = 16;

    if(ofxiOSExternalDisplay::isDisplayingOnDeviceScreen()){
        ofDrawBitmapString("DISPLAYING ON DEVICE SCREEN", (int)(ofGetWidth()*0.5-110), (int)(ofGetHeight()* 0.5));
        
        int buttonX;
        int buttonY;

        ofFill();
        ofSetColor(ofColor::white);
        ofRect(buttonExternalDisplayRect.x, buttonExternalDisplayRect.y, buttonExternalDisplayRect.width, buttonExternalDisplayRect.height);
        
        ofNoFill();
        ofSetColor(ofColor::black);
        ofRect(buttonExternalDisplayRect.x, buttonExternalDisplayRect.y, buttonExternalDisplayRect.width, buttonExternalDisplayRect.height);
        
        ofFill();
        ofSetColor(ofColor::black);
        buttonX = buttonExternalDisplayRect.x + 6;
        buttonY = (int)buttonExternalDisplayRect.y + buttonExternalDisplayRect.height * 0.5 + 5;
        ofDrawBitmapString("EXTERNAL DISPLAY", buttonX, buttonY);
        
        ofFill();
        ofSetColor(ofColor::white);
        ofRect(buttonMirrorDisplayRect.x, buttonMirrorDisplayRect.y, buttonMirrorDisplayRect.width, buttonMirrorDisplayRect.height);        
        
        ofNoFill();
        ofSetColor(ofColor::black);
        ofRect(buttonMirrorDisplayRect.x, buttonMirrorDisplayRect.y, buttonMirrorDisplayRect.width, buttonMirrorDisplayRect.height);        
        
        ofFill();
        ofSetColor(ofColor::black);
        buttonX = buttonMirrorDisplayRect.x + 35;
        buttonY = (int)buttonMirrorDisplayRect.y + buttonMirrorDisplayRect.height * 0.5 + 5;
        string mirrorMode = ofxiOSExternalDisplay::isMirroring() ? "OFF" : "ON";
        ofDrawBitmapString("MIRROR " + mirrorMode, buttonX, buttonY);
        
        ofSetColor(ofColor::white);
    } else {
        ofDrawBitmapString("DISPLAYING ON EXTERNAL SCREEN", (int)(ofGetWidth()*0.5-110), (int)(ofGetHeight()* 0.5));
    }
    
    y+=p;
    
    ofDrawBitmapString("frame num   = " + ofToString(ofGetFrameNum()), x, y+=p);
    ofDrawBitmapString("frame rate  = " + ofToString(ofGetFrameRate()), x, y+=p);
    ofDrawBitmapString("touch x     = " + ofToString(mouseX), x, y+=p);
    ofDrawBitmapString("touch y     = " + ofToString(mouseY), x, y+=p);
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::presentExternalDisplayPopup(){
    
    alertViewDelegate = [[[AlertViewDelegate alloc] init] retain];
    
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"External Display" 
                                                      message:@"Select a External Display Mode" 
                                                     delegate:alertViewDelegate 
                                            cancelButtonTitle:@"Cancel" 
                                            otherButtonTitles:nil] retain];
    
    vector<ofxiOSExternalDisplayMode> displayModes;
    displayModes = ofxiOSExternalDisplay::getExternalDisplayModes();
    
    [alert addButtonWithTitle:@"Preferred Mode"];
    
    for(int i = 0; i < displayModes.size(); i++){
        string buttonText = ofToString(displayModes[i].width) + " x " + ofToString(displayModes[i].height);
        [alert addButtonWithTitle:ofxStringToNSString(buttonText)];
    }
    
    [alert show];
    [alert release];
}

//--------------------------------------------------------------
void ofApp::presentExternalDisplayNotFoundPopup(){
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"External Display" 
                                                      message:@"External Display not found.\nConnect to an external display using a VGA adapter or AirPlay."
                                                     delegate:nil 
                                            cancelButtonTitle:@"OK" 
                                            otherButtonTitles:nil] retain];
    [alert show];
    [alert release];
}

void ofApp::presentMirroringFailedPopup(){
    UIAlertView * alert = [[[UIAlertView alloc] initWithTitle:@"Mirroring Failed" 
                                                      message:@"Either you are not connected to an external display or your device does not support mirroring."
                                                     delegate:nil 
                                            cancelButtonTitle:@"OK" 
                                            otherButtonTitles:nil] retain];
    [alert show];
    [alert release];
}

//--------------------------------------------------------------
void ofApp::popupDismissed(){
    if(alertViewDelegate){
        [alertViewDelegate release];
        alertViewDelegate = nil;
    }
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    
    if(buttonExternalDisplayRect.inside(touch.x, touch.y)){
        if(ofxiOSExternalDisplay::isExternalScreenConnected()){
            presentExternalDisplayPopup();
        } else {
            presentExternalDisplayNotFoundPopup();
        }
    }

    if(buttonMirrorDisplayRect.inside(touch.x, touch.y)){
        if(ofxiOSExternalDisplay::isExternalScreenConnected()){
            if(ofxiOSExternalDisplay::isMirroring()){
                ofxiOSExternalDisplay::mirrorOff();
            } else {
                if(!ofxiOSExternalDisplay::mirrorOn()){
                    presentMirroringFailedPopup();
                }
                    
            }
        } else {
            presentMirroringFailedPopup();
        }
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}

//--------------------------------------------------------------
void ofApp::externalDisplayConnected(){
    ofLogVerbose("external display connected.");
    presentExternalDisplayPopup();
}

//--------------------------------------------------------------
void ofApp::externalDisplayDisconnected(){
    ofLogVerbose("external display disconnected.");
}

//--------------------------------------------------------------
void ofApp::externalDisplayChanged(){
    ofLogVerbose("external display changed.");
    if(ofxiOSExternalDisplay::isDisplayingOnDeviceScreen()){
        if(viewController){
            [viewController.view removeFromSuperview];
            [viewController release];
            viewController = nil;
        }
    } else if(ofxiOSExternalDisplay::isDisplayingOnExternalScreen()) {
        if(!viewController){
            viewController = [[[MyViewController alloc] init] retain];
            [ofxiOSGetAppDelegate().window addSubview:viewController.view];  // add to device window.
        }
    }
}

