#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackgroundHex(0x000000);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	// load the texure
	ofDisableArbTex();
	ofLoadImage(texture, "dot.png");
	
	// set the camera distance
	camDist  = 1605;
	camera.setDistance(camDist);
	
	// randomly add a point on a sphere
	int   num = 1000;
	float radius = 1000;
	for(int i = 0; i<num; i++ ) {
		
		float theta1 = ofRandom(0, TWO_PI);
		float theta2 = ofRandom(0, TWO_PI);
		
		ofVec3f p;
		p.x = cos( theta1 ) * cos( theta2 );
		p.y = sin( theta1 );
		p.z = cos( theta1 ) * sin( theta2 );
		p *= radius;
		
		addPoint(p.x, p.y, p.z);
    
	}
	
	// upload the data to the vbo
	int total = (int)points.size();
	vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
	vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
	
	
	// load the shader
	if(shader.load("shader")) {
		printf("Shader is loaded\n"); 
	}
}

//--------------------------------------------------------------
void testApp::update() {
}

//--------------------------------------------------------------
void testApp::draw() {

	glDepthMask(GL_FALSE);
	
	ofSetColor(255, 100, 90);
	
	// this makes everything look glowy :)
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofEnablePointSprites();
	
	// bind the shader and camera
	// everything inside this function
	// will be effected by the shader/camera
	shader.begin();
	camera.begin();
	
	// bind the texture so that when all the points 
	// are drawn they are replace with our dot image
	texture.bind();
	vbo.draw(GL_POINTS, 0, (int)points.size());
	texture.unbind();
	
	camera.end();
	shader.end();
	
	ofDisablePointSprites();
	ofDisableBlendMode();
	
	// check to see if the points are 
	// sizing to the right size
	ofEnableAlphaBlending();
	camera.begin();
	for (int i=0; i<points.size(); i++) {
		ofSetColor(255, 80);
		ofVec3f mid = points[i];
		mid.normalize();
		mid *= 300;
		ofLine(points[i], mid);
	} 
	camera.end();
	
	glDepthMask(GL_TRUE);
	
	ofSetColor(255, 100);
	ofRect(0, 0, 250, 90);
	ofSetColor(0);
	string info = "FPS "+ofToString(ofGetFrameRate(), 0) + "\n";
	info += "Total Points "+ofToString((int)points.size())+"\n";
	info += "Press 'a' to add more\n";
	info += "Press 'c' to remove all";
	
	ofDrawBitmapString(info, 20, 20);
	
}

//--------------------------------------------------------------
void testApp::addPoint(float x, float y, float z) {
	ofVec3f p(x, y, z);
	points.push_back(p);
	
	// we are passing the size in as a normal x position
	float size = ofRandom(5, 50);
	sizes.push_back(ofVec3f(size));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
	if(key == OF_KEY_UP) {
		camDist -= 10;
	}
	if(key == OF_KEY_DOWN) {
		camDist += 10;
	}
	camera.setDistance(camDist);
	
	
	// clear all the points
	if(key == 'c') {
		points.clear();
	}
	
	// add crazy amount
	if(key == 'a') {
		float theta1 = ofRandom(0, TWO_PI);
		float theta2 = ofRandom(0, TWO_PI);
		ofVec3f p;
		p.x = cos( theta1 ) * cos( theta2 );
		p.y = sin( theta1 );
		p.z = cos( theta1 ) * sin( theta2 );
		p  *= 800;
		addPoint(p.x, p.y, p.z);
		int total = (int)points.size();
		vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
		vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
		
	}
	
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