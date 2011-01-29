#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetLineWidth(3.0);
	
	background.loadImage("bg.png");
	
	
	// ------------------------- billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboardVerts[i].set(ofRandom(-500, 500), 
							  ofRandom(-500, 500), 
							  ofRandom(-500, 500));
		
		billboardColor[i] = 1;
		billboardSize[i] = ofNextPow2(ofRandom(4, 24));	// faster to have a power of 2
	}
	
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader 
	// this is used to change the
	// size of the particle
	billboardShader.setup("Billboard");
	
	// we need to disable ARB textures
	// because we are binding ourselfs
	ofDisableArbTex();
	texture.loadImage("dot.png");
}

//--------------------------------------------------------------
void testApp::update() {
	
	float t   = (ofGetElapsedTimef()) * 0.9f;
	float div = 400.0;
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		ofVec3f vec(ofSignedNoise(t, billboardVerts[i].y/div, billboardVerts[i].z/div),
					ofSignedNoise(billboardVerts[i].x/div, t, billboardVerts[i].z/div),
					ofSignedNoise(billboardVerts[i].x/div, billboardVerts[i].y/div, t));
		
		vec *= 0.3f;
		billboardVels[i] *= 0.4f;
		billboardVerts[i] += billboardVels[i] + vec;	
	
		
		billboardColor[i].a -= 0.2;
		
		if(billboardColor[i].a <= 0.0) {
			billboardColor[i].a = 1;
			billboardVerts[i].set(ofRandom(-500, 500), 
								  ofRandom(-500, 500), 
								  ofRandom(-500, 500));
		}
		 
	}

}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	background.draw(0, 0);
	
	
	// we read the values in the depth buffer and test against them.
	// We are also *writing* to the depth buffer when we draw
	glEnable(GL_DEPTH_TEST);
	
	
	
	ofEnableAlphaBlending();
	
	// move the camera around
	ofVec2f des(ofGetMouseX(), ofGetMouseY());
	//cameraRotation += (des-cameraRotation) * 0.03;
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 200);
	ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.y, 0, 1, 0);
	ofRotate(cameraRotation.y, 0, 0, 1);
	
	// Now we *Still test against depth*, but we *no longer write* to the depth buffer
	glDepthMask(GL_FALSE);
	
	billboardShader.begin();
	billboardShader.setUniform1fv("particleSize", billboardSize);		
	billboardShader.setUniform4fv("theColor", &billboardColor[0].r);
	
	ofEnablePointSprites();
	ofEnableAlphaBlending();
	texture.getTextureReference().bind();
	billboardVbo.bind();
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	
	billboardVbo.unbind();
	texture.getTextureReference().unbind();
	ofDisablePointSprites();
	
	billboardShader.end();
	
	ofPopMatrix();
	
	
	
	// re0enable writing to the depth buffer, so the next drame is ok.
	// We need to do this because GL is a state machine, and its a dumb mother fucker.
	glDepthMask(GL_TRUE);
	
	
	
	
	
	
	
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	texture.draw(20, 100);
	ofDrawBitmapString("<-- This is your billboard texture", 50, 120);
	
	// ------------------------- 
	// debug info
	ofSetColor(255, 255, 255);
	string info = ofToString(ofGetFrameRate(), 2)+"\n";
	info += "Particle Size: "+ofToString(NUM_BILLBOARDS)+"\n";
	ofDrawBitmapString(info, 30, 30);
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
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

