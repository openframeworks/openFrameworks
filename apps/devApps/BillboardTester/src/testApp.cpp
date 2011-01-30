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
		pos[i].x = ofRandomWidth();
		pos[i].y = ofRandomHeight();
		pointSizes[i] = ofNextPow2(ofRandom(2, 64));
		rotations[i]  = ofRandom(0, 1);
	}
	
	// set the vertex data
	vbo.setVertexData(pos, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	
	// load the bilboard shader 
	// this is used to change the
	// size of the particle
	shader.setup("Billboard");
	
	// we need to disable ARB textures
	// because we are binding ourselfs
	ofDisableArbTex();
	texture.loadImage("dot.png");
}

//--------------------------------------------------------------
void testApp::update() {

	ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
	ofVec2f mouseVec(ofGetPreviousMouseX()-ofGetMouseX(), ofGetPreviousMouseY()-ofGetMouseY());
	mouseVec.limit(10.0);
	for (int i=0; i<NUM_BILLBOARDS; i++) {
		
		rotations[i]  += 0.003;
		
		if(mouse.distance(pos[i]) < 100.0) {
			vel[i] -= mouseVec;	
		}
		
		pos[i] += vel[i];
		vel[i] *= 0.94f;
		
		if(pos[i].x < 0) pos[i].x = ofGetWidth();
		if(pos[i].x > ofGetWidth()) pos[i].x = 0;
		if(pos[i].y < 0) pos[i].y = ofGetHeight();
		if(pos[i].y > ofGetHeight()) pos[i].y = 0;
	}
		
}

//--------------------------------------------------------------
void testApp::draw() {
	
	
	ofEnableAlphaBlending();
	ofSetHexColor(0xffffff);
	
	ofEnablePointSprites();
	shader.begin();
	
	// we are getting the location of the point size attribute
	// we then set the point of the pointSizes to the vertex att
	// we then bind and then enable
	int pointAttLocation = shader.getAttributeLocation("pointSize");
	glVertexAttribPointer(pointAttLocation, 1, GL_FLOAT, false, 0, pointSizes);
	glBindAttribLocation(shader.getProgram(), pointAttLocation, "pointSize");
	glEnableVertexAttribArray(pointAttLocation);

	//shader.setUniform2fv("rot", &vel[0].x, NUM_BILLBOARDS);
	shader.setUniform1fv("timer", rotations, NUM_BILLBOARDS);
	
	// bind the texture for point replace
	texture.getTextureReference().bind();
	
	// draw the vbo as points
	vbo.bind();
	vbo.setVertexData(pos, NUM_BILLBOARDS, GL_DYNAMIC_DRAW);
	vbo.draw(GL_POINTS, 0, NUM_BILLBOARDS);
	vbo.unbind();

	// unbind the texture
	texture.getTextureReference().unbind();
	
	shader.end();
	
	ofDisablePointSprites();
	glDisableVertexAttribArray(pointAttLocation); // disable to point array
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

