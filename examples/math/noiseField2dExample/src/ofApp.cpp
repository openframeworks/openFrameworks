/*
 This example demonstrates how to use a two dimensional slice of a three
 dimensional noise field to guide particles that are flying around. It was
 originally based on the idea of simulating "pollen" being blown around by
 the wind, and implemented in the Processing:
 http://www.openprocessing.org/visuals/?visualID=2785
 */

#include "ofApp.h"

/*
 All these settings control the behavior of the app. In general it's a better
 idea to keep variables in the .h file, but this makes it easy to set them at
 the same time you declare them.
 */
int nPoints = 4096; // points to draw
float complexity = 6; // wind complexity
float pollenMass = .8; // pollen mass
float timeSpeed = .02; // wind variation speed
float phase = TWO_PI; // separate u-noise from v-noise
float windSpeed = 40; // wind vector magnitude for debug
int step = 10; // spatial sampling rate for debug
bool debugMode = false;

/*
 This is the magic method that samples a 2d slice of the 3d noise field. When
 you call this method with a position, it returns a direction (a 2d vector). The
 trick behind this method is that the u,v values for the field are taken from
 out-of-phase slices in the first dimension: t + phase for the u, and t - phase
 for the v.
 */
//--------------------------------------------------------------
glm::vec2 ofApp::getField(const glm::vec2& position) {
	float normx = ofNormalize(position.x, 0, ofGetWidth());
	float normy = ofNormalize(position.y, 0, ofGetHeight());
	float u = ofNoise(t + phase, normx * complexity + phase, normy * complexity + phase);
	float v = ofNoise(t - phase, normx * complexity - phase, normy * complexity + phase);
	return glm::vec2(u, v);
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true); // don't go too fast
	ofEnableAlphaBlending();
	
	// randomly allocate the points across the screen
  points.resize(nPoints);
  for(int i = 0; i < nPoints; i++) {
	points[i] = glm::vec2(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()));
  }
	
	// we'll be drawing the points into an ofMesh that is drawn as bunch of points
	cloud.clear();
	cloud.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void ofApp::update() {
	width = ofGetWidth(), height = ofGetHeight();
  t = ofGetFrameNum() * timeSpeed;
  for(int i = 0; i < nPoints; i++) {
		float x = points[i].x, y = points[i].y;
		glm::vec2 field = getField(points[i]); // get the field at this position
		// use the strength of the field to determine a speed to move
		// the speed is changing over time and velocity-space as well
	float speed = (1 + ofNoise(t, field.x, field.y)) / pollenMass;
		// add the velocity of the particle to its position 
	x += ofLerp(-speed, speed, field.x);
	y += ofLerp(-speed, speed, field.y);
		// if we've moved outside of the screen, reinitialize randomly
	if(x < 0 || x > width || y < 0 || y > height) {
	  x = ofRandom(0, width);
	  y = ofRandom(0, height);
	}
		// save the changes we made to the position
	points[i].x = x;
	points[i].y = y;
		// add the current point to our collection of drawn points
	  cloud.addVertex({x, y,0});
	}
} 

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(255);
  if(debugMode) {
	ofSetColor(0);
		// draw a vector field for the debug screen
	for(int i = 0; i < width; i += step) {
	  for(int j = 0; j < height; j += step) {
				glm::vec2 field = getField(glm::vec2(i, j));
		ofPushMatrix();
		ofTranslate(i, j);
				ofSetColor(0);
		ofDrawLine(0, 0, ofLerp(-windSpeed, windSpeed, field.x), ofLerp(-windSpeed, windSpeed, field.y));
		ofPopMatrix();
	  }
	}
		// draw the points as circles
		ofSetColor(ofColor::red);
		for(int i = 0; i < nPoints; i++) {
		ofDrawCircle(points[i], 2);
		}
  } else {
		// when not in debug mode, draw all the points to the screen
	ofSetColor(0, 10);
		cloud.draw();
	}
	
	ofDrawBitmapStringHighlight("click to reset\nhit any key for debug", 10, 10, ofColor::white, ofColor::black);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// when you hit a key, draw the debug screen
  debugMode = !debugMode;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	// when you click the mouse, reset all the points
  setup();
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
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
