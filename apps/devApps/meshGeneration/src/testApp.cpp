#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(50, 50, 50, 0);
	
	ofSetLogLevel(OF_LOG_VERBOSE);
		
	mesh.meshElement = new ofMeshElement();
	mesh.setDrawType(GL_STREAM_DRAW_ARB);
	mesh.meshElement->setMode(OF_TRIANGLE_STRIP_ELEMENT);
	
	glPointSize(4);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255, 255);
    glPushMatrix();
	mesh.drawFaces();
    glPopMatrix();
    
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
	mesh.meshElement->addIndex(mesh.meshElement->getNumVertices()-1);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

