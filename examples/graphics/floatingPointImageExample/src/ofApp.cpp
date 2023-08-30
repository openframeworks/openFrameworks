#include "ofApp.h"

//Some helper functions
//--------------------------------------------------------------
void ofApp::addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
	glm::vec3 normal = glm::normalize(glm::cross((b - a),(c - a)));
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void ofApp::addFace(ofMesh& mesh, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
glm::vec3 ofApp::getVertexFromImg(ofFloatImage& img, int x, int y) {
	return glm::vec3(x, y, 100 * img.getColor(x, y).getBrightness());
}

//--------------------------------------------------------------
void ofApp::setup(){

	//note: you can get nicer anti-aliased rendering ( with slower fps )
	//if you uncomment the appropriate line in main.cpp
	
	img.setUseTexture(false);
	img.load("nyc-small.exr");

	material.setShininess(120);
	material.setSpecularColor(ofColor(255, 255, 255, 255));
	light.enable();
	light.setPosition(+500, 0, 0);

	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 1;
	int width = img.getWidth();
	int height = img.getHeight();
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			glm::vec3 nw = getVertexFromImg(img, x, y);
			glm::vec3 ne = getVertexFromImg(img, x + skip, y);
			glm::vec3 sw = getVertexFromImg(img, x, y + skip);
			glm::vec3 se = getVertexFromImg(img, x + skip, y + skip);

			addFace(mesh, nw, ne, se, sw);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	easyCam.begin();
		material.begin();
		ofScale(1, -1, 1);
		ofRotateXDeg(60);
		ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2, 0);
		ofSetColor(255);
		ofEnableDepthTest();
		mesh.draw();
		ofDisableDepthTest();
		material.end();
	easyCam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
