#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    model.loadMeshes("astroBoy_walk.dae",meshes);
	
	
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50, 50, 50, 0);
	
    ofSetColor(255, 0, 255, 255);
    
    glPushMatrix();

	glScalef(30,30,30);
	glTranslatef(17,20,0);
	glRotatef(180,0,0,1);
	glRotatef(ofGetWidth()*.5 - mouseX,0,1,0);

	ofSetColor(255,0,255);
//	meshes[whichMesh]->drawFaces();
	ofSetColor(255,255,255);
//	meshes[whichMesh].drawVertices();
	ofSetColor(0,255,0);
//	meshes[whichMesh].drawWireframe();

    glPopMatrix();
    
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key= ' ') {
		whichMesh++;
		whichMesh%=meshes.size();
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
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

