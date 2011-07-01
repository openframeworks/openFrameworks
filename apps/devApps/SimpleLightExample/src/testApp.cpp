#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(10, 10, 10);
	glEnable(GL_DEPTH_TEST);
	
	radius		= 100.f;
	center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
	
	pointLight.setDiffuseColor( 0, 255, 0);
	pointLight.setSpecularColor(255, 255, 255);
	pointLight.setPointLight();
	
	spotLight.setDiffuseColor(255, 0, 0);
	spotLight.setSpecularColor(255, 255, 255);
	spotLight.setSpotlight(40, 6);
	
	directionalLight.setDiffuseColor(0, 0, 255);
	directionalLight.setSpecularColor(255, 255, 255);
	directionalLight.setDirectional();
	directionalLight.setOrientation( ofVec3f(0, 90, 0) );
	
	bShiny = true;
	keyPressed('s');
	material.setSpecularColor(ofColor(255, 255, 255, 255));
	
	ofSetSmoothLighting(true);
	
	bPointLight = bSpotLight = bDirLight = true;
}

//--------------------------------------------------------------
void testApp::update() {
	pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x, 
						   sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y, 
						   -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
	spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);
	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableLighting();
	material.begin();
	if (bPointLight) pointLight.enable();
	if (bSpotLight) spotLight.enable();
	if (bDirLight) directionalLight.enable();
	ofSetColor(255, 255, 255, 255);
	ofSphere( center.x, center.y, center.z, radius);
	
	glPushMatrix();
	glTranslatef(300, 300, cos(ofGetElapsedTimef()*1.4) * 300.f);
	glRotatef(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
	glRotatef(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
	ofBox(0, 0, 0, 60);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(center.x, center.y, -900);
	glRotatef(ofGetElapsedTimef() * .2 * RAD_TO_DEG, 0, 1, 0);
	ofBox( 0, 0, 0, 850);
	glPopMatrix();
	
	pointLight.disable();
	spotLight.disable();
	directionalLight.disable();
	material.end();
	ofDisableLighting();
	ofSetColor( pointLight.getDiffuseColor() );
	if(bPointLight) pointLight.draw();
	ofSetColor( spotLight.getDiffuseColor() );
	if(bSpotLight) spotLight.draw();
	
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Point Light On (1) : "+ofToString(bPointLight) +"\n"+
					   "Spot Light On (2) : "+ofToString(bSpotLight) +"\n"+
					   "Directional Light On (3) : "+ofToString(bDirLight)+"\n"+
					   "Shiny Objects On (s) : "+ofToString(bShiny) , 
					   20, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	switch (key) {
		case '1':
			bPointLight = !bPointLight;
			break;
		case '2':
			bSpotLight = !bSpotLight;
			break;
		case '3':
			bDirLight = !bDirLight;
			break;
		case 's':
			bShiny	= !bShiny;
			if (bShiny) material.setShininess( 120 );
			else material.setShininess( 30 );
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

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