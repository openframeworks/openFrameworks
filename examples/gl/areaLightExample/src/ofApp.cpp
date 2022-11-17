#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	areaLight.setup();
	areaLight.enable();
	areaLight.setAreaLight(400,120);
	areaLight.setAmbientColor(ofFloatColor(0.3,0.3,0.3));
	areaLight.setAttenuation(1.0,0.00003,0.00001);
	areaLight.setDiffuseColor(ofFloatColor(1,1,1));
	areaLight.setSpecularColor(ofFloatColor(1,1,1));

	ofBackground(0);
	plane.set(20000,20000,2,2);
	plane.rotateDeg(-90,glm::vec3(1,0,0));
	plane.move(glm::vec3(0,-300,0));
	materialPlane.setAmbientColor(ofFloatColor(0.1,0.1,0.1,1.0));
	materialPlane.setDiffuseColor(ofFloatColor(0.8,0.8,0.8,1.0));
	materialPlane.setSpecularColor(ofFloatColor(0.8,0.8,0.8,1.0));
	materialPlane.setShininess(10);


	camera.setFarClip(20000);
	camera.move(0,0,1000);
	
	logoMesh.load("oflogo.ply");
	materialMesh.setAmbientColor(ofFloatColor(0.85, 0.16, 0.43, 0.4));
	materialMesh.setDiffuseColor(ofFloatColor(0.85, 0.16, 0.43, 1.0));
	materialMesh.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
	materialMesh.setShininess(10);
	
	
}

//--------------------------------------------------------------
void ofApp::update(){
	float elapsedTime = ofGetElapsedTimef();
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 10000.0, 1.f / 5.f );
	
	if( mode == 0 ){
		float speed = ofMap( ofGetMouseX(), 100, ofGetWidth()-100, 0.1, 24, true );
		orbitSpeed += deltaTime * speed;
		areaLight.setPosition(cos(orbitSpeed)*-600,(sin(elapsedTime*1.1)*200), sin(orbitSpeed)*-600);
		areaLight.lookAt(glm::vec3(0, -150, 0));
		areaLight.setAmbientColor(ofFloatColor(0.3,0.3,0.3));
	}else if( mode == 1 ){
		float mousePercent = ofMap( ofGetMouseX(), 100, ofGetWidth()-100, 0, 1, true );
		highwaySpeed = ofMap( mousePercent, 0, 1, 500, 5000, true );
		areaLight.setAmbientColor(ofFloatColor(0.0,0.0,0.0));
		float zPosition = ofClamp(sin(elapsedTime), 0, 1) * 500.0+200;
		areaLight.setPosition( areaLight.getPosition().x + deltaTime * highwaySpeed, 350, zPosition );
		if( areaLight.getPosition().x > 1500 ) {
			areaLight.setPosition(-1500, areaLight.getPosition().y, zPosition );
		}
		areaLight.lookAt( glm::vec3(areaLight.getPosition().x, areaLight.getPosition().y-300, areaLight.getPosition().z) );
		areaLight.rollDeg(90);
		
		float noiseValue = ofSignedNoise(areaLight.getPosition().y * mousePercent * 0.001, areaLight.getPosition().x * mousePercent * 0.001, mousePercent * elapsedTime );
		if( noiseValue > 1.0 - mousePercent * 0.5 && bump > 1.0 ) {
			bump = 0.0;
			bumpHeight = ofRandom(10, 80);
		}
		bump += deltaTime * (3.0 + (mousePercent+0.2));
	}else if( mode == 2 ){
		float mousePercent = ofMap( ofGetMouseX(), 100, ofGetWidth()-100, 0, 1, true );
		areaLight.setPosition(cos(elapsedTime*2.0)*-600,fabs(sin(elapsedTime*2.0)*600)-200,sin(elapsedTime*3.4f)*200+400);
		areaLight.tiltDeg(deltaTime*(mousePercent*360.0*3.0+5.0f));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofEnableLighting();
	ofEnableDepthTest();
	camera.begin();
	materialPlane.begin();
	plane.draw();
	ofDrawSphere(0,-300,0,10000);
	materialPlane.end();
	
	materialMesh.begin();
	ofPushMatrix();
	ofTranslate(0,-300,0);
	if( mode == 1 ){
		if( bump < 1.0 ){
			ofTranslate(0.0, sin(bump * glm::pi<float>()) * bumpHeight, 0.0 );
		}
	}
	ofRotateXDeg(-90);
	logoMesh.draw();
	ofPopMatrix();
	materialMesh.end();
	
	
	areaLight.draw();
	camera.end();
	
	ofDisableDepthTest();
	ofDisableLighting();
	
	ofDrawBitmapStringHighlight("Mode (m): "+ofToString(mode,0), 15, 15);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 'm' ){
		mode ++;
		if( mode > 2 ){
			mode = 0;
		}
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
