#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetLogLevel(OF_LOG_VERBOSE);
    model.loadMeshes("astroBoy_walk.dae",meshes);
	
	for(int i =0; i < meshes.size();i++){
		vboMeshes.push_back(ofVboMesh());
		vboMeshes2.push_back(ofVboMesh());
		vboMeshes.back().mesh = &meshes[i];
		vboMeshes2.back().mesh = &meshes[i];
	}
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    //glEnable(GL_DEPTH_TEST);
	
	whichMesh = 0;
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

	/*
	glBegin(GL_POINTS);
	for (int i =0; i < vboMeshes[whichMesh].mesh->vertices.size();i++){
		ofVec3f& curVert = vboMeshes[whichMesh].mesh->vertices.at(i);
		glVertex3f(curVert.x,curVert.y,curVert.z);
	}
	glEnd();
	 */
	
//	vboMeshes[0].drawFaces();
	ofSetColor(255,0,255);
	vboMeshes[whichMesh].drawFaces();
	ofSetColor(255,255,255);
	vboMeshes[whichMesh].drawVertices();
	ofSetColor(0,255,0);
	vboMeshes2[whichMesh].drawWireframe();

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

