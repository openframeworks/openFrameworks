#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(50, 50, 50, 0);
	
	ofEnableAlphaBlending();
	ofSetLogLevel(OF_LOG_VERBOSE);
		
	mesh.vertexData = new ofVertexData();
	mesh.setDrawType(GL_STREAM_DRAW_ARB);
	mesh.bEnableIndices = false;
	mesh.vertexData->setMode(OF_TRIANGLE_STRIP_MODE);
	
	glPointSize(3);
	ofSetLineWidth(3);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255, 0, 255, 155);
	mesh.drawFaces();
	ofSetColor(0, 255, 0, 255);
	mesh.drawWireframe();
    ofSetColor(255, 255, 255, 255);
	mesh.drawVertices();

    
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key== ' ') {
		lightsOn = !lightsOn;
	}
	
	if (key == 'c'){
		mesh.vertexData->clear();
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
	mesh.vertexData->addVertex(ofVec3f(x,y,0));
	mesh.vertexData->addIndex(mesh.vertexData->getNumVertices()-1);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

