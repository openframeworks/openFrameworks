#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // we need GL_TEXTURE_2D for our models coords.
    ofDisableArbTex();
    //ofSetVerticalSync(true);
	
    //model.loadModel("astroBoy_walk.dae");
	
    model.loadMeshes("astroBoy_walk.dae",meshes);

    //model.loadModel("astroBoy_walk.dae");
    //model.loadModel("rally.obj");
    //model.loadModel("CARGT.3DS");
    //model.loadModel("Poseidon_compiled.OBJ");
	
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    glEnable(GL_DEPTH_TEST);
    
    //some model / light stuff
	/*
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	 */
	
	cout << GL_POINTS << " " << GL_LINES << " " << GL_TRIANGLES << endl;
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
	glTranslatef(17,0,0);

	for (int i =0; i < meshes.size();i++){
		meshes[i].drawVertices();
		/*
		for (int j=0; j<meshes[i].mesh->vertices.size(); j++){
			glVertex3f(meshes[i].mesh->vertices[j].x,meshes[i].mesh->vertices[j].y,meshes[i].mesh->vertices[j].z);
		}
		 */
	}

    glPopMatrix();
    
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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

