#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofEnableDepthTest();

	// turn on smooth lighting //
	ofSetSmoothLighting(true);
	
	int numLights = 5;
	float xdist = 1000.0;
	
	for( int i = 0; i < numLights; i++ ){
		auto pointLight = make_shared<ofLight>();
		pointLight->setup();
		pointLight->enable();
		pointLight->setPointLight();
		// Point lights emit light in all directions //
		// set the diffuse color, color reflected from the light source //
		pointLight->setDiffuseColor( ofColor(0.f, 255.f, 0.f));
		pointLight->setAmbientColor(ofFloatColor(0.1,0.1,0.1));
		// specular color, the highlight/shininess color //
		pointLight->setSpecularColor( ofColor(255.f, 255.f, 255.f));
		// space out the lights along the x axis
		float tx = ofMap( i, 0, numLights-1, -xdist * 0.5, xdist * 0.5f );
		pointLight->setPosition(tx, 300, 300);
		pointLight->setAttenuation(1.0,0.00001,0.00001);
		pointLights.push_back(pointLight);
	}
	
	plane.set(3000,3000,24,24);
	plane.rotateDeg(-90,glm::vec3(1,0,0));
	plane.move(glm::vec3(0,-300,0));
	
	camera.setFarClip(5000);
	camera.move(0,0,1000);

	colorHue = ofRandom(0, 250);

	lightColor.setBrightness( 180.f );
	lightColor.setSaturation( 150.f );
	
	logoMesh.load("oflogo.ply");
	materialMesh.setAmbientColor(ofFloatColor(1.0, 1.0, 1.0, 0.4));
	materialMesh.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0, 1.0));
	materialMesh.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
	materialMesh.setShininess(64);
}

//--------------------------------------------------------------
void ofApp::update(){
	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f/5000.f, 1.f/5.f);
	angle += deltaTime;
	
	colorHue += 1.f;
	if(colorHue >= 255){
		colorHue = 0.f;
	}
	lightColor.setHue(colorHue);
	
	for( int i = 0; i < (int)pointLights.size(); i++ ) {
		auto& plight = pointLights[i];
		lightColor.setHue( ofWrap(colorHue + (float)i * 12.5, 0, 255.f) );
		plight->setDiffuseColor(lightColor);
		float positionY = sin(angle+(float)i* cos(angle*0.05)*1.9f) * 300.0 + 100.0;
		plight->setPosition(plight->getX(), positionY, 200.0);
		
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(10), ofColor(50));
	ofEnableDepthTest();
	// enable lighting //
	ofEnableLighting();
	
	camera.begin();
	
	materialMesh.begin();
	plane.draw();
	ofPushMatrix();
	// move the mesh down a bit
	ofTranslate(0,-300,0);
	// rotate the mesh along the x axis to face the camera
	ofRotateXDeg(-90);
	logoMesh.draw();
	ofPopMatrix();
	materialMesh.end();
	
	
	
	// turn off lighting //
	ofDisableLighting();
	
	for( int i = 0; i < (int)pointLights.size(); i++ ){
		ofSetColor( pointLights[i]->getDiffuseColor() );
		if( !pointLights[i]->getIsEnabled() ){
			ofSetColor( 40 );
		}
		pointLights[i]->draw();
	}
	
	ofPushMatrix();
	ofTranslate(0, -299);
	ofRotateYDeg(90);
	ofRotateZDeg(90);
	ofSetColor( 180 );
	ofSetColor(40);
	ofDrawGridPlane(100, 14);
	ofPopMatrix();
	
	camera.end();
	
	ofSetColor(255, 255, 255);
	stringstream ss;
	for( size_t i = 0; i < pointLights.size(); i++ ){
		auto& plight = pointLights[i];
		ss << "Light enabled ("+ofToString(i+1,0)+"): " << plight->getIsEnabled();
		if( i < pointLights.size() -1 ){
			ss << endl;
		}
	}
	ofDrawBitmapString(ss.str(), 20, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	
	int lindex = -1;
	if( key == '1' ){
		lindex = 0;
	}
	if( key == '2' ){
		lindex = 1;
	}
	if( key == '3' ){
		lindex = 2;
	}
	if( key == '4'){
		lindex = 3;
	}
	if( key == '5' ){
		lindex = 4;
	}
	if( lindex > -1 && lindex < (int)pointLights.size() ){
		if(pointLights[lindex]->getIsEnabled()) {
			pointLights[lindex]->disable();
		} else {
			pointLights[lindex]->enable();
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
