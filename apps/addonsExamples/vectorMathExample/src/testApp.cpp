#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
     nPts = 0;
     rotateAmount.set(0,0,1);
     speedOfRotation = 0.1f;

     ofBackground(255,255,255);



     ofxVec3f center(ofGetWidth()/2, ofGetHeight()/2, 0);

     xAxisMin.set(-100,0,0);
     xAxisMax.set(100,0,0);
     yAxisMin.set(0,-100,0);
     yAxisMax.set(0,100,0);
     zAxisMin.set(0,0,-100);
     zAxisMax.set(0,0,100);

     xAxisMin += center;
     xAxisMax += center;
     yAxisMin += center;
     yAxisMax += center;
     zAxisMin += center;
     zAxisMax += center;

     bDrawnAnything = false;        // added this cause theo saw this example
                                    // and was like "how do I tell zach that a rotating
                                    // cross is not that interesting ??"  before realizing
                                    // that one should draw :)

}

//--------------------------------------------------------------
void testApp::update(){

     // rotation occurs around 0,0,0 position.
     // since 0,0,0 is the top left corner, we need to:
     // (A) move over,
     // (B) rotate
     // (C) move back
     // this could all be one for loop, but broken out here for
     // clarity on the steps:

     ofxVec3f center(ofGetWidth()/2, ofGetHeight()/2,0);

     // move the points so that their center (ofGetW/2, ofGetH/2) is at 0,0,0
     for (int i = 0; i < nPts; i++){
        pts[i]-= center;
     }

     // rotate the points
     for (int i = 0; i < nPts; i++){
        pts[i] = pts[i].rotated(speedOfRotation, rotateAmount);
     }

     // move them back
     for (int i = 0; i < nPts; i++){
        pts[i]+= center;
     }


     xAxisMin -= center;
     xAxisMax -= center;
     yAxisMin -= center;
     yAxisMax -= center;
     zAxisMin -= center;
     zAxisMax -= center;


     xAxisMin.rotate(speedOfRotation, rotateAmount);
     xAxisMax.rotate(speedOfRotation, rotateAmount);
     yAxisMin.rotate(speedOfRotation, rotateAmount);
     yAxisMax.rotate(speedOfRotation, rotateAmount);
     zAxisMin.rotate(speedOfRotation, rotateAmount);
     zAxisMax.rotate(speedOfRotation, rotateAmount);

     xAxisMin += center;
     xAxisMax += center;
     yAxisMin += center;
     yAxisMax += center;
     zAxisMin += center;
     zAxisMax += center;
}

//--------------------------------------------------------------
void testApp::draw(){

     ofSetHexColor(0x000000);
     ofNoFill();
     ofBeginShape();
     for (int i = 0; i < nPts; i++){
         ofVertex(pts[i].x, pts[i].y);
     }
     ofEndShape();

     string info = "speed of rotation (a/z): " + ofToString(speedOfRotation,3) + "\n" +
                  "rotateAmount in x (s/x): " + ofToString(rotateAmount.x,3) + "\n" +
                  "rotateAmount in y (d/c): " + ofToString(rotateAmount.y,3) + "\n" +
                  "rotateAmount in z (f/v): " + ofToString(rotateAmount.z,3);

     ofFill();
     ofSetHexColor(0xE5A93F);
     ofRect(10,10,300,70);
     ofSetHexColor(0x000000);
     ofDrawBitmapString(info,30,30);


     ofSetColor(50,50,255);
     glBegin(GL_LINES);
        glVertex3f(xAxisMin.x, xAxisMin.y, xAxisMin.z);
        glVertex3f(xAxisMax.x, xAxisMax.y, xAxisMax.z);
        glVertex3f(yAxisMin.x, yAxisMin.y, yAxisMin.z);
        glVertex3f(yAxisMax.x, yAxisMax.y, yAxisMax.z);
        glVertex3f(zAxisMin.x, zAxisMin.y, zAxisMin.z);
        glVertex3f(zAxisMax.x, zAxisMax.y, zAxisMax.z);
     glEnd();


    if (bDrawnAnything == false){
        ofSetColor(0,0,0);
        ofDrawBitmapString("draw something!", ofGetWidth()/2 + 100,ofGetHeight()/2 + 20);
    }

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    switch (key){
        case 'a':
            speedOfRotation += 0.05f;
            break;
        case 'z':
            speedOfRotation -= 0.05f;
            break;
        case 's':
            rotateAmount.x += 0.05f;
            break;
        case 'x':
             rotateAmount.x -= 0.05f;
            break;
        case 'd':
            rotateAmount.y += 0.05f;
            break;
        case 'c':
             rotateAmount.y -= 0.05f;
            break;
        case 'f':
            rotateAmount.z += 0.05f;
            break;
        case 'v':
             rotateAmount.z -= 0.05f;
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

     bDrawnAnything = true;

     if (nPts < MAX_N_PTS){
        pts[nPts].x = x;
        pts[nPts].y = y;
        nPts++;
     }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
     nPts = 0;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

void testApp::windowResized(int w, int h){

}
