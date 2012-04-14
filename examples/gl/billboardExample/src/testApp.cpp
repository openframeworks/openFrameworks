#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0, 0, 0);
	
	cameraRotation = 0;
	zoom = -500;
	zoomTarget = 200;
	
	// ------------------------- billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboardVerts[i].set(ofRandom(-500, 500), 
													ofRandom(-500, 500), 
													ofRandom(-500, 500));
		
		billboardColor[i].set(ofColor::fromHsb(ofRandom(96, 160), 255, 255));
		
		billboardSize[i] = 0;
		billboardSizeTarget[i] = ofRandom(4, 64);
	}
	
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader 
	// this is used to change the
	// size of the particle
	billboardShader.load("Billboard");
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("dot.png");
}

//--------------------------------------------------------------
void testApp::update() {
	
	float t = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// noise 
		ofVec3f vec(ofSignedNoise(t, billboardVerts[i].y/div, billboardVerts[i].z/div),
								ofSignedNoise(billboardVerts[i].x/div, t, billboardVerts[i].z/div),
								ofSignedNoise(billboardVerts[i].x/div, billboardVerts[i].y/div, t));
		
		vec *= 10 * ofGetLastFrameTime();
		billboardVels[i] += vec;
		billboardVerts[i] += billboardVels[i]; 
		billboardVels[i] *= 0.94f; 
		billboardSize[i] = 12 + billboardSizeTarget[i] * ofNoise(t + i);
	}
	
	
	// move the camera around
	float mx = (float)mouseX/(float)ofGetWidth();
	float my = (float)mouseY/(float)ofGetHeight();
	ofVec3f des(mx * 360.0, my * 360.0, 0);
	cameraRotation += (des-cameraRotation) * 0.03;
	zoom += (zoomTarget - zoom) * 0.03;
	
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackgroundGradient(ofColor(255), ofColor(230, 240, 255));
	
	string info = ofToString(ofGetFrameRate(), 2)+"\n";
	info += "Particle Count: "+ofToString(NUM_BILLBOARDS);
	ofDrawBitmapStringHighlight(info, 30, 30);
	
	ofEnableAlphaBlending();
	ofSetColor(255);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
	ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.y, 0, 1, 0);
	ofRotate(cameraRotation.y, 0, 0, 1);
	
	// bind the shader so that wee can change the
	// size of the points via the vert shader
	billboardShader.begin();
	billboardShader.setUniform1fv("pointSize", billboardSize, NUM_BILLBOARDS); 
	
	ofEnablePointSprites();
	texture.getTextureReference().bind();
	billboardVbo.bind();
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	
	billboardVbo.unbind();
	texture.getTextureReference().unbind();
	ofDisablePointSprites();
	
	billboardShader.end();
	
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	if(key == OF_KEY_UP) zoomTarget +=10;
	if(key == OF_KEY_DOWN) zoomTarget -=10;
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
