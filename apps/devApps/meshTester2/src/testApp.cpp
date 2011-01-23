#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	ofEnableNormalizedTexCoords();
	
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	mesh.meshElement = new ofMeshElement();
	mesh.setDrawType(GL_STREAM_DRAW_ARB);
	mesh.meshElement->setMode(OF_TRIANGLE_STRIP_ELEMENT);
    
    glEnable(GL_DEPTH_TEST);
	
	lightsOn = false;
	glEnable(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(4);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	lightsOn ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	ofBackground(50, 50, 50, 0);
	
    ofSetColor(255, 255, 255, 255);
    
    glPushMatrix();
	mesh.drawFaces();
    glPopMatrix();
    
	glDisable(GL_LIGHTING);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key= ' ') {
		lightsOn = !lightsOn;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	mesh.meshElement->addVertex(ofVec3f(x,y,0));
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

