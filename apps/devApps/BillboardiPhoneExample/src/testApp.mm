#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){	
	
	
	ofRegisterTouchEvents(this);
	ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);

	ofDisableArbTex();
	texture.loadImage("dot.png");
	   
	zoom = 0;
	int worldSize = 200;
	
	for (int i=0; i<NUM_PTS; i++) {
		pts[i].x = ofRandom(-worldSize, worldSize);
		pts[i].y = ofRandom(-worldSize, worldSize);
		pts[i].z = ofRandom(-worldSize, worldSize);
	}
	
	vbo.setVertexData(pts, NUM_PTS, GL_DYNAMIC_DRAW);
	
}


//--------------------------------------------------------------
void testApp::update() {
	
	rot += vel;
	vel *= 0.96f;
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableAlphaBlending();	
	ofSetColor(255, 0, 255);
	
	
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2, zoom);
	ofRotateX(rot.y);
	ofRotateY(rot.x);
	
	ofEnablePointSprites();

	glPointSize(10);
	texture.getTextureReference().bind();
	vbo.draw(GL_POINTS, 0, NUM_PTS);
	texture.getTextureReference().unbind();
	
	ofDisablePointSprites();
	
	ofPopMatrix();
	
	
	ofSetColor(200);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 1), 5, 20);
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch) {
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	
	if(touch.id == 1) {
		zoom = ofMap(touch.x, 0.0, ofGetWidth(), -200, 200);		
	}
	else {
		ofVec2f pt(touch.x, touch.y);
		vel = finger - pt;
		vel.limit(10.0);
		finger.set(touch.x, touch.y); 
	}
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs &touch){

}
