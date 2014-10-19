#include "ofApp.h"
#include "ofAppiOSWindow.h"
#import "ActionSheetDelegateForOF.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(70);
    
    ofSetOrientation(OF_ORIENTATION_90_LEFT);
    
    bAuto = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetLineWidth(2);
    
    ofPoint p1;
    ofPoint p2;
    
    //-- draw width arrow.
    
    p1.set(15, ofGetHeight()-10);
    p2.set(ofGetWidth()-15, ofGetHeight()-10);
    
	ofSetColor(ofColor::magenta);
    ofPushMatrix();
    ofTranslate(p1.x, p1.y);
    ofRotate(45);
    ofDrawLine(0, 0, 0, -10);
    ofRotate(90);
    ofDrawLine(0, 0, 0, -10);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(p2.x, p2.y);
    ofRotate(-45);
    ofDrawLine(0, 0, 0, -10);
    ofRotate(-90);
    ofDrawLine(0, 0, 0, -10);
    ofPopMatrix();
    ofDrawLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetWidth()), ofGetWidth()*0.5-10, ofGetHeight()-20);
    
    //-- draw height arrow.
    
    p1.set(ofGetWidth()-10, 15);
    p2.set(ofGetWidth()-10, ofGetHeight()-15);
    
    ofSetColor(ofColor::green);
    ofPushMatrix();
    ofTranslate(p1.x, p1.y);
    ofRotate(-135);
    ofDrawLine(0, 0, 0, -10);
    ofRotate(-90);
    ofDrawLine(0, 0, 0, -10);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(p2.x, p2.y);
    ofRotate(-45);
    ofDrawLine(0, 0, 0, -10);
    ofRotate(-270);
    ofDrawLine(0, 0, 0, -10);
    ofPopMatrix();
    ofDrawLine( p1.x, p1.y, p2.x, p2.y);
    ofDrawBitmapString(ofToString(ofGetHeight()), ofGetWidth()-40, ofGetHeight()*0.5-5);
    
    ofSetColor(ofColor::white);
    
    string currentOrientation = "";
    string currentOrientationSyntax = "";
    
    switch (ofGetOrientation()) {
        case OF_ORIENTATION_DEFAULT:
            currentOrientation = "Portrait";
            currentOrientationSyntax = "OF_ORIENTATION_DEFAULT";
            break;
        case OF_ORIENTATION_180:
            currentOrientation = "Portrait Upside Down";
            currentOrientationSyntax = "OF_ORIENTATION_180";
            break;
        case OF_ORIENTATION_90_LEFT:
            currentOrientation = "Landscape Left";
            currentOrientationSyntax = "OF_ORIENTATION_90_LEFT";
            break;
        case OF_ORIENTATION_90_RIGHT:
            currentOrientation = "Landscape Right";
            currentOrientationSyntax = "OF_ORIENTATION_90_RIGHT";
            break;
        default:
            break;
    }
    
    string autoStr = (bAuto ? "ON" : "OFF");
    
    int x = ofGetWidth() * 0.05;
    int y = ofGetHeight() * 0.1;
    
    ofDrawBitmapString("current orientation is,", x, y);
    ofDrawBitmapString(" - " + currentOrientation, x, y+=20);
    ofDrawBitmapString(" - " + currentOrientationSyntax, x, y+=20);
    ofDrawBitmapString("auto rotation is " + autoStr + ".", x, y+=40);
    ofDrawBitmapString("tap screen to change orientation.", x, y+=40);
}

//--------------------------------------------------------------
void ofApp::exit(){
    //
}

//--------------------------------------------------------------
void ofApp::rotateToPortrait() {
    ofSetOrientation(OF_ORIENTATION_DEFAULT);
}

void ofApp::rotateToPortraitUpSideDown() {
    ofSetOrientation(OF_ORIENTATION_180);
}

void ofApp::rotateToLandscapeLeft() {
    ofSetOrientation(OF_ORIENTATION_90_LEFT);
}

void ofApp::rotateToLandscapeRight() {
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);
}

void ofApp::toggleAutoRotation() {
    bAuto = !bAuto;
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch) {
    
    NSString * autoTitle;
    if(bAuto) {
        autoTitle = @"Turn Auto Rotation OFF";
    } else {
        autoTitle = @"Turn Auto Rotation ON";
    }
    
    UIActionSheet * actionSheet;
    actionSheet = [[UIActionSheet alloc] initWithTitle:@"Select Orientation"
                                              delegate:nil
                                     cancelButtonTitle:@"Cancel"
                                destructiveButtonTitle:nil
                                     otherButtonTitles:@"Portrait", @"Portrait Upside Down", @"Landscape Left", @"Landscape Right", autoTitle, nil];
    actionSheet.actionSheetStyle = UIActionSheetStyleBlackTranslucent;
    actionSheet.delegate = [[ActionSheetDelegateForOF alloc] initWithApp:this];
    [actionSheet showInView:ofxiOSGetGLParentView()];
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
    if( bAuto ) {
        ofSetOrientation((ofOrientation)newOrientation);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    //
}

