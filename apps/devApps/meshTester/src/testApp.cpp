#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    model.loadMeshes("astroBoy_walk.dae",meshes);

	for(int i =0; i < meshes.size();i++){
		vboMeshes.push_back(ofVboMesh());
		vboMeshes.back().meshElement = &meshes[i];
	}
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
	
	whichMesh = 0;
	
	glPointSize(3);
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

	for (int i =0; i < vboMeshes.size(); i++){
		ofSetColor(i*255.0/(vboMeshes.size()-1),0,255 - i*255.0/(vboMeshes.size()-1));
		vboMeshes[i].drawFaces();

		ofSetColor(120,i*255.0/(vboMeshes.size()-1),i*255.0/(vboMeshes.size()-1));
		vboMeshes[i].drawVertices();

		ofSetColor(i*255.0/(vboMeshes.size()-1),i*255.0/(vboMeshes.size()-1),0);
		vboMeshes[i].drawWireframe();
	}

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

