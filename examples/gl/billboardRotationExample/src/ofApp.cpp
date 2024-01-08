#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0);
	ofSetVerticalSync(true);
	
	// billboard particles
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		pos[i].x = ofRandomWidth();
		pos[i].y = ofRandomHeight();
		vel[i].x = ofRandomf();
		vel[i].y = ofRandomf();
		home[i] = pos[i];
		pointSizes[i] = ofRandom(2, 40);
		rotations[i] = ofRandom(0, 360);
	}
	
	// set the vertex data
	vbo.setVertexData(pos, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	#ifdef TARGET_EMSCRIPTEN
		shader.load("shaderGLES3/Billboard");
	#else
		if(ofIsGLProgrammableRenderer()){
			shader.load("shaderGL3/Billboard");
		}else{
			shader.load("shaderGL2/Billboard");
		}
	#endif
	
	ofDisableArbTex();
	texture.load("snow.png");

	// we are getting the location of the point size attribute
	// we then set the pointSizes to the vertex attritbute
	// we then bind and then enable
	shader.begin();
	int pointAttLoc = shader.getAttributeLocation("pointSize");
	vbo.setAttributeData(pointAttLoc, pointSizes, 1, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);

	// rotate the snow based on the velocity
	int angleLoc = shader.getAttributeLocation("angle");
	vbo.setAttributeData(angleLoc, rotations, 1, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	shader.end();
}

//--------------------------------------------------------------
void ofApp::update() {
	glm::vec2 mouse(ofGetMouseX(), ofGetMouseY());
	glm::vec2 mouseVec(ofGetPreviousMouseX()-ofGetMouseX(), ofGetPreviousMouseY()-ofGetMouseY());
	glm::clamp(mouseVec, 0.0f, 10.0f);

	
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		ofSetRandomSeed(i);
		if(glm::distance(mouse,pos[i]) < ofRandom(100, 200)) {
			vel[i] -= mouseVec; 
		}
		
		pos[i] += vel[i];
		vel[i] *= 0.84f;
		
		if(pos[i].x < 0) pos[i].x = ofGetWidth();
		if(pos[i].x > ofGetWidth()) pos[i].x = 0;
		if(pos[i].y < 0) pos[i].y = ofGetHeight();
		if(pos[i].y > ofGetHeight()) pos[i].y = 0;
		
		glm::vec2 center(ofGetWidth()/2, ofGetHeight()/2);
		glm::vec2 frc = home[i] - pos[i];
		if(glm::length(frc) > 20.0) {
			frc = glm::normalize(frc);
			frc *= 0.84;
			vel[i] += frc;
		}
		
		// get the 2d heading
		float angle = (float)atan2(-vel[i].y, vel[i].x) + PI;
		rotations[i] = (angle * -1.0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofEnableAlphaBlending();
	ofSetColor(255);
	
	shader.begin();
	ofEnablePointSprites();
	
	
	texture.getTexture().bind();
	vbo.updateVertexData(pos, NUM_BILLBOARDS);

	// rotate the snow based on the velocity
	int angleLoc = shader.getAttributeLocation("angle");
	vbo.updateAttributeData(angleLoc, rotations, NUM_BILLBOARDS);

	vbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	texture.getTexture().unbind();

	ofDisablePointSprites();
	shader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		home[i].x = ofRandomWidth();
		home[i].y = ofRandomHeight();
	}
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
