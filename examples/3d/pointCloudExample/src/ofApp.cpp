#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);

	// load an image from disk
	img.load("linzer.png");

	//for everything but web
	#ifndef TARGET_EMSCRIPTEN
		mesh.setMode(OF_PRIMITIVE_POINTS); // we're going to load a ton of points into an ofMesh

		glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
		glPointSize(3); // make the points bigger

		// loop through the image in the x and y axes
		int skip = 3; // load a subset of the points
		for(int y = 0; y < img.getHeight(); y += skip) {
			for(int x = 0; x < img.getWidth(); x += skip) {
				ofColor cur = img.getColor(x, y);
				if(cur.a > 0) {
					// the alpha value encodes depth, let's remap it to a good depth range
					float z = ofMap(cur.a, 0, 255, -300, 300);
					cur.a = 255;
					mesh.addColor(cur);
					glm::vec3 pos(x, y, z);
					mesh.addVertex(pos);
				}
			}
		}

		mesh.setupIndicesAuto();
	#else
		//for web we have to make tiny square meshes out of two triangles 
		//OF should maybe do this on the backend for emscripten but for now we'll do it manually 
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);

		//play with this number to make the pixel/point size bigger
		float hPointSize = 1;

		//these are the offsets for the four corners of the square 
		std::vector<glm::vec3> offsets = {glm::vec3(-hPointSize, -hPointSize, 0.0), glm::vec3(hPointSize, -hPointSize, 0.0), glm::vec3(hPointSize, hPointSize, 0.0), glm::vec3(-hPointSize, hPointSize, 0.0) };

		//we make the square out of two triangles so 0,1,2 is a triangle made out of the first 3 points above and 2,3,0 is a triangle made out of the last 3 points above 
		std::vector<ofIndexType> indices = {0, 1, 2, 2, 3, 0};

		// loop through the image in the x and y axes
		int skip = 3; // load a subset of the points
		int pixelCount = 0; 
		for(int y = 0; y < img.getHeight(); y += skip) {
			for(int x = 0; x < img.getWidth(); x += skip) {
				ofColor cur = img.getColor(x, y);
				if(cur.a > 0) {
					// the alpha value encodes depth, let's remap it to a good depth range
					float z = ofMap(cur.a, 0, 255, -300, 300);
					cur.a = 255;
					glm::vec3 pos(x, y, z);
					//build our pixel/point as a small square from the offsets above 
					for( auto & offset : offsets){
						mesh.addColor(cur);
						mesh.addVertex(offset + pos);
					}
					//add the index so the triangles can be made out of the vertex data 
					for( auto & index : indices ){
						mesh.addIndex(pixelCount + index);
					}
					pixelCount+=4;
				}
			}
		}
	#endif 

	ofEnableDepthTest();
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

	// even points can overlap with each other, let's avoid that
	cam.begin();
	ofScale(2, -2, 2); // flip the y axis and zoom in a bit
	ofRotateYDeg(90);
	ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
	mesh.draw();
	cam.end();
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
