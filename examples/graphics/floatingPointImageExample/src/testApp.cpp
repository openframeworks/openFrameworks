#include "testApp.h"

//Some helper functions
//--------------------------------------------------------------
void testApp::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void testApp::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
ofVec3f testApp::getVertexFromImg(ofFloatImage& img, int x, int y) {
	return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
}

//--------------------------------------------------------------
void testApp::setup(){

	//note: you can get nicer anti-aliased rendering ( with slower fps ) 
	//if you uncomment the appropriate line in main.cpp

	img.loadImage("nyc-small.exr");
	
	light.enable();
	light.setPosition(+500, 0, 0);
	
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int skip = 1;	
	int width = img.getWidth();
	int height = img.getHeight();
	for(int y = 0; y < height - skip; y += skip) {
		for(int x = 0; x < width - skip; x += skip) {
			ofVec3f nw = getVertexFromImg(img, x, y);
			ofVec3f ne = getVertexFromImg(img, x + skip, y);
			ofVec3f sw = getVertexFromImg(img, x, y + skip);
			ofVec3f se = getVertexFromImg(img, x + skip, y + skip);
			
			addFace(mesh, nw, ne, se, sw);
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
	
	easyCam.begin();
		ofScale(1, -1, 1);
		ofRotateX(60);
		ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2, 0);
		ofSetColor(255);
		ofEnableDepthTest();
		mesh.draw();
		ofDisableDepthTest();
	easyCam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
