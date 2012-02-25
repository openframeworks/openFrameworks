#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofSetLineWidth(3.0);
	
	cameraRotation = 0;
	zoom		   = -500;
	zoomTarget     = 200;
	
	// ------------------------- billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboardVerts[i].set(ofRandom(-500, 500), 
							  ofRandom(-500, 500), 
							  ofRandom(-500, 500));
		
		billboardColor[i].set(1, 0, 0, ofRandomuf());
	
		billboardSize[i]	    = 0;
		billboardSizeTarget[i]  = ofNextPow2(ofRandom(4, 24));	// faster to have a power of 2
	}
	
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader 
	// this is used to change the
	// size of the particle
	billboardShader.load("Billboard");
	
	// we need to disable ARB textures
	// because we are binding ourselfs
	ofDisableArbTex();
	texture.loadImage("dot.png");
}

//--------------------------------------------------------------
void testApp::update() {

	float t   = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// noise 
		ofVec3f vec(ofSignedNoise(t, billboardVerts[i].y/div, billboardVerts[i].z/div),
					ofSignedNoise(billboardVerts[i].x/div, t, billboardVerts[i].z/div),
					ofSignedNoise(billboardVerts[i].x/div, billboardVerts[i].y/div, t));
		
		vec *= 0.3f;
		billboardVels[i] += vec;
		billboardVerts[i] += billboardVels[i];	
		billboardVels[i]  *= 0.94f;	
		
		billboardColor[i].a -= 0.002;
		billboardSize[i] += (billboardSizeTarget[i]-billboardSize[i]) * 0.03;
		
		if(billboardColor[i].a <= 0.0) {
			billboardColor[i].a = 1;
			billboardVerts[i].set(ofRandom(-500, 500), 
								  ofRandom(-500, 500), 
								  ofRandom(-500, 500));
			
			// faster to have a power of 2
			billboardSize[i] = 0;
			billboardSizeTarget[i]  = ofNextPow2(ofRandom(4, 64));	

		}
		 
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
	
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	
	
	// we read the values in the depth buffer and test against them.
	// We are also *writing* to the depth buffer when we draw
	glEnable(GL_DEPTH_TEST);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
	ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.y, 0, 1, 0);
	ofRotate(cameraRotation.y, 0, 0, 1);
	
	// Now we *Still test against depth*, 
	// but we *no longer write* to the depth buffer
	glDepthMask(GL_FALSE);
	
	// bind the shader so that wee can change the
	// size of the points via the vert shader
	billboardShader.begin();
	billboardShader.setUniform1fv("pointSize", billboardSize, NUM_BILLBOARDS);		
	
	ofEnableAlphaBlending();
	ofEnablePointSprites();
	texture.getTextureReference().bind();
	billboardVbo.bind();
	billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	billboardVbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	
	billboardVbo.unbind();
	texture.getTextureReference().unbind();
	ofDisablePointSprites();
	
	billboardShader.end();
	
	ofPopMatrix();
	
	
	
	// re0enable writing to the depth buffer, so the next drame is ok.
	// We need to do this because GL is a state machine, and its a dumb mother fucker.
	glDepthMask(GL_TRUE);
	
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	texture.draw(30, 80);
	
	//ofSetHexColor(0xff0000);
	//ofNoFill();
	//ofRect(20, 100, texture.getWidth(), texture.getHeight());
	ofSetHexColor(0xffffff);
	ofDrawBitmapString("<-- This is your billboard texture\n    the texture needs to a power of\n    2 for proper rendering.", 95, 32+texture.getHeight());
	
	// ------------------------- 
	// debug info
	ofSetColor(255, 255, 255);
	string info = ofToString(ofGetFrameRate(), 2)+"\n";
	info += "Particle Count: "+ofToString(NUM_BILLBOARDS)+"\n";
	ofDrawBitmapString(info, 30, 30);
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if(key == 'f') ofToggleFullscreen();
	if(key == OF_KEY_UP) 	zoomTarget +=10;
	if(key == OF_KEY_DOWN) 	zoomTarget -=10;
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

