#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofBackgroundHex(0x57554c);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // slow       = 1
    // faster     = 2
    // super fast = 3
    mode = 1;
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
    
    // add points all the time
    if(points.size() < 500000) {
        for (int i=0; i<30; i++) {
            addPoint(ofGetWidth()/2, ofGetHeight()/2);
        }    
    }
    
    // move all the points around
    for (unsigned int i=0; i<points.size(); i++) {
        
        speeds[i].y += 0.04; // some grav
        points[i]   += speeds[i];
        speeds[i]   *= 0.98;
        
        // move from the mouse
        ofVec2f mouseVec = ofVec2f(ofGetMouseX(), ofGetMouseY()) - points[i];
        if(mouseVec.length() < 100) {
            mouseVec.normalize();
            speeds[i] -= mouseVec;
        }
        
        // wrap the screen
        if(points[i].x > ofGetWidth())    points[i].x = 1;
        if(points[i].x < 0)               points[i].x = ofGetWidth()-1;
        if(points[i].y > ofGetHeight())   points[i].y = 1;
        if(points[i].y < 0)               points[i].y = ofGetHeight()-1;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    // draw the points the slow way
    if(mode == 1) {
        #ifdef TARGET_OPENGLES
        ofSetColor(255);
        ofDrawBitmapString("OpenGL immediate mode not available in OpenGL ES. Press 2 or 3.",ofGetWidth() / 2.0f - 300,ofGetHeight() / 2.0f);
        #else 
        ofSetColor(255);
        glBegin(GL_POINTS);
        for (unsigned int i=0; i<points.size(); i++) {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();
        #endif
    }
    
    // a bit faster
    else if(mode == 2) {
        ofSetColor(255);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, &points[0].x);
        glDrawArrays(GL_POINTS, 0, (int)points.size());
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    // super fast (vbo)
    else if(mode == 3) {
        ofSetColor(255);
        vbo.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
        vbo.draw(GL_POINTS, 0, (int)points.size());
    }
    
    

    ofSetColor(0);
    ofRect(0, 0, 250, 90);
    ofSetColor(255);
    ofDrawBitmapString("Mode "+ofToString(mode), 20, 20);
    ofDrawBitmapString("FPS "+ofToString(ofGetFrameRate(), 0), 20, 40);
    ofDrawBitmapString("Total Points "+ofToString((int)points.size()), 20, 60);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    
    if(key == '1') mode = 1;
    if(key == '2') mode = 2;
    if(key == '3') mode = 3;
    
    
    // clear all the points
    if(key == 'c') {
        points.clear();
        speeds.clear();
    }
    
    // add crazy amount
    if(key == 'z') {
        for (int i=0; i<400000; i++) {
            addPoint(ofRandomWidth(), ofRandomHeight());
        }    
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    // add a bunch as you drag
    for (int i=0; i<400; i++) {
        addPoint(x, y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
