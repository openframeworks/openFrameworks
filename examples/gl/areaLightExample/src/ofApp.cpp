#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableLighting();
	ofEnableDepthTest();
	areaLight.setup();
	areaLight.enable();
	areaLight.setAreaLight(120,400);
	//areaLight.setSpotlight(80,3);
	areaLight.setAmbientColor(ofFloatColor(0.1,0.1,0.1));
	areaLight.setAttenuation(1.0,0.0001,0.0001);
	areaLight.setDiffuseColor(ofFloatColor(1,1,1));
	areaLight.setSpecularColor(ofFloatColor(1,1,1));
	areaLight.rotate(-90,ofVec3f(1,0,0));
	areaLight.rotate(30,ofVec3f(0,0,1));
	areaLight.setPosition(0,-200,0);

	ofBackground(0);
	plane.set(20000,20000,2,2);
	plane.rotate(-90,ofVec3f(1,0,0));
	plane.move(ofVec3f(0,-300,0));
	materialPlane.setAmbientColor(ofFloatColor(0.1,0.1,0.1,1.0));
	materialPlane.setDiffuseColor(ofFloatColor(0.8,0.8,0.8,1.0));
	materialPlane.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
	materialPlane.setShininess(10);


	camera.setFarClip(20000);
	camera.move(0,0,3000);
}

//--------------------------------------------------------------
void ofApp::update(){
	//areaLight.setPosition(0,-200,0);
}

//--------------------------------------------------------------
void ofApp::draw(){
	camera.begin();
	materialPlane.begin();
	plane.draw();
	ofDrawSphere(0,-300,0,10000);
	materialPlane.end();
	areaLight.draw();
	camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
	case OF_KEY_UP:
		areaLight.move(0,10,0);
		break;
	case OF_KEY_DOWN:
		areaLight.move(0,-10,0);
		break;
	case OF_KEY_LEFT:
		areaLight.rotate(1,0,0,1);
		break;
	case OF_KEY_RIGHT:
		areaLight.rotate(-1,0,0,1);
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
