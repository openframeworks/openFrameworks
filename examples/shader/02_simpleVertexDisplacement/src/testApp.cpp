#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    shader.load("gl3/shader.vert", "gl3/shader.frag");
    plane.set(800, 600, 50, 50);
    
    drawWires = false;
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    shader.begin();
    shader.setUniform1f("time", ofGetElapsedTimef());
    
    ofRotate(rotation, 1, 0, 0);
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    if(drawWires) {
        plane.drawWireframe();
    } else {
        plane.draw();
    }
    
    shader.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    drawWires = !drawWires;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
    rotation = 45 + (-60 * ( (float) y / ofGetHeight()));
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}