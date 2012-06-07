#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofBackground(0, 0, 0);
	
	cameraRotation.set(0);
	zoom = -500;
	zoomTarget = 200;
	
	billboards.getVertices().resize(NUM_BILLBOARDS);
	billboards.getColors().resize(NUM_BILLBOARDS);
	billboards.getNormals().resize(NUM_BILLBOARDS,ofVec3f(0));
	
	// ------------------------- billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		billboardVels[i].set(ofRandomf(), -1.0, ofRandomf());
		billboards.getVertices()[i].set(ofRandom(-500, 500), 
													ofRandom(-500, 500), 
													ofRandom(-500, 500));
		
		billboards.getColors()[i].set(ofColor::fromHsb(ofRandom(96, 160), 255, 255));
	    billboardSizeTarget[i] = ofRandom(4, 64);
		
	}
	
	
	billboards.setUsage( GL_DYNAMIC_DRAW );
	billboards.setMode(OF_PRIMITIVE_POINTS);
	//billboardVbo.setVertexData(billboardVerts, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	//billboardVbo.setColorData(billboardColor, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader 
	// this is used to change the
	// size of the particle
	billboardShader.load("Billboard");
	
	// we need to disable ARB textures in order to use normalized texcoords
	ofDisableArbTex();
	texture.loadImage("dot.png");
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update() {
	
	float t = (ofGetElapsedTimef()) * 0.9f;
	float div = 250.0;
	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		// noise 
		ofVec3f vec(ofSignedNoise(t, billboards.getVertex(i).y/div, billboards.getVertex(i).z/div),
								ofSignedNoise(billboards.getVertex(i).x/div, t, billboards.getVertex(i).z/div),
								ofSignedNoise(billboards.getVertex(i).x/div, billboards.getVertex(i).y/div, t));
		
		vec *= 10 * ofGetLastFrameTime();
		billboardVels[i] += vec;
		billboards.getVertices()[i] += billboardVels[i]; 
		billboardVels[i] *= 0.94f; 
    	billboards.setNormal(i,ofVec3f(12 + billboardSizeTarget[i] * ofNoise(t+i),0,0));
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
	
	ofSetColor(255);
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
	ofRotate(cameraRotation.x, 1, 0, 0);
	ofRotate(cameraRotation.y, 0, 1, 0);
	ofRotate(cameraRotation.y, 0, 0, 1);
	
	// bind the shader so that wee can change the
	// size of the points via the vert shader
	billboardShader.begin();
	
	ofEnablePointSprites();
	texture.getTextureReference().bind();
	billboards.draw();
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
