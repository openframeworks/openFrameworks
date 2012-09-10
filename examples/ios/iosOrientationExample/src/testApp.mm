#include "testApp.h"
#import "ActionSheetDelegateForOF.h"

//--------------------------------------------------------------
void testApp::setup(){	
	ofBackground(70);
    
    ofxiPhoneSetOrientation(OF_ORIENTATION_90_LEFT);
    bAuto = false;
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
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
    ofDrawBitmapString(ofToString(ofGetHeight()), ofGetWidth()-40, ofGetHeight()*0.5-5);
    
    ofSetColor(ofColor::white);
    
    string currentOrientation = "";
    string currentOrientationSyntax = "";
    
    switch (ofxiPhoneGetOrientation()) {
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
void testApp::exit(){
    //
}

//--------------------------------------------------------------
void testApp::rotateToPortrait() {
    ofxiPhoneSetOrientation(OF_ORIENTATION_DEFAULT, true);
}

void testApp::rotateToPortraitUpSideDown() {
    ofxiPhoneSetOrientation(OF_ORIENTATION_180, true);
}

void testApp::rotateToLandscapeLeft() {
    ofxiPhoneSetOrientation(OF_ORIENTATION_90_LEFT, true);
}

void testApp::rotateToLandscapeRight() {
    ofxiPhoneSetOrientation(OF_ORIENTATION_90_RIGHT, true);
}

void testApp::toggleAutoRotation() {
    bAuto = !bAuto;
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs & touch) {
    
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
    [actionSheet showInView:ofxiPhoneGetGLParentView()];
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs & touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){
    if(bAuto) {
        ofxiPhoneSetOrientation((ofOrientation)newOrientation, true);
    }
}

