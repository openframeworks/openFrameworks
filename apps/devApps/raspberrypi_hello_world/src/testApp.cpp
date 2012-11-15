#include "testApp.h"

	
//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,0);

i = 0;
}

//--------------------------------------------------------------
void testApp::exit(){}

//--------------------------------------------------------------
void testApp::update(){
i += 3;
i = i % 360;
ofLogError("testApp") << "It's all happening. " << i << endl;

}

//--------------------------------------------------------------
void testApp::draw() {
ofPushMatrix();
ofTranslate(ofGetWidth()/2.0f-100,ofGetHeight()/2.0f-100);
ofRotateZ(i);
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofRect(10, 10, 100, 100);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofRect(10, 10, 100, 100);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofTriangle(180, 10, 130, 110, 230, 110);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofTriangle(180, 10, 130, 110, 230, 110);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofCircle(60, 170, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofCircle(60, 170, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofFill();
    ofSetColor(ofColor::red);
    ofEllipse(180, 170, 100, 50);
    ofPopStyle();

    ofPushStyle();
    ofNoFill();
    ofSetColor(ofColor::green);
    ofEllipse(180, 170, 100, 50);
    ofPopStyle();
    
    ofPushStyle();
    ofEnableSmoothing();
    ofFill();
    ofSetColor(ofColor::red);
    ofLine(260, 120, 300, 220);
    ofPopStyle();
ofPopMatrix();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){}

//--------------------------------------------------------------
void testApp::keyReleased(int key){}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
