/**
 *
 * OFDevCon Example Code Sprint
 * Model Distort Example
 *
 * This example loads a model and distorts it using noise
 *
 * The model is the open source and freely licensed balloon dog by Rob Myers, commissioned by furtherfield:
 * http://www.furtherfield.org/projects/balloon-dog-rob-myers
 *
 * Created by James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
 * Adapted during ofDevCon on 2/23/2012
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(50, 50, 50, 0);

	//we need to call this for textures to work on models
    ofDisableArbTex();
	
	//this makes sure that the back of the model doesn't show through the front
	ofEnableDepthTest();

	//now we load our model
	model.loadModel("dog/dog.3ds");
	model.setPosition(ofGetWidth()*.5, ofGetHeight() * 0.75, 0);
	
	light.enable();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255, 255, 255, 255);
	
	//first let's just draw the model with the model object
	//drawWithModel();
	
	//then we'll learn how to draw it manually so that we have more control over the data
	drawWithMesh();
}

//draw the model the built-in way
void testApp::drawWithModel(){
	
	//get the position of the model
	ofVec3f position = model.getPosition();

	//save the current view
	ofPushMatrix();
	
	//center ourselves there
	ofTranslate(position);
	ofRotate(-ofGetMouseX(), 0, 1, 0);
	ofRotate(90,1,0,0);
	ofTranslate(-position);
	
	//draw the model
	model.drawFaces();
	
	//restore the view position
    ofPopMatrix();	
}

//draw the model manually
void testApp::drawWithMesh(){

	//get the model attributes we need
	ofVec3f scale = model.getScale();
	ofVec3f position = model.getPosition();
	float normalizedScale = model.getNormalizedScale();
	ofVboMesh mesh = model.getMesh(0);
	ofTexture texture = model.getTextureForMesh(0);
	ofMaterial material = model.getMaterialForMesh(0);
	
    ofPushMatrix();
	
	//translate and scale based on the positioning. 
	ofTranslate(position);
	ofRotate(-ofGetMouseX(), 0, 1, 0);
	ofRotate(90,1,0,0);

	
	ofScale(normalizedScale, normalizedScale, normalizedScale);
	ofScale(scale.x,scale.y,scale.z);
	
	//modify mesh with some noise
	float liquidness = 5;
	float amplitude = mouseY/100.0;
	float speedDampen = 5;		
	vector<ofVec3f>& verts = mesh.getVertices();
	for(unsigned int i = 0; i < verts.size(); i++){
		verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
		verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
		verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
	}
		
	//draw the model manually
	texture.bind();
	material.begin();
	//mesh.drawWireframe(); //you can draw wireframe too
	mesh.drawFaces();
	material.end();
	texture.unbind();
	
	ofPopMatrix();

}

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
