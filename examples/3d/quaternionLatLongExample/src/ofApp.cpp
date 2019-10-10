/**
 *
 * OFDevCon Example Code Sprint
 * Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by James George on 2/23/2012
 */

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(30);
	ofEnableAlphaBlending();
	ofNoFill();

	// create little objects for each city.
	// A Lat/Lon like this:
	// Lewiston, Idaho 	W 46 24' N 117 2'
	// This translate to angles and degrees (1 degree = 1/60. of an angle)
	// West and South are negative values

	// here is a list of big cities and their positions
	// http://www.infoplease.com/ipa/A0001796.html

	City newyork = { "new york", 40+47/60., -73 + 58/60. };
	City tokyo = { "tokyo", 35 + 40./60, 139 + 45/60. };
	City london = { "london", 51 + 32/60., -5./60. };
	City shanghai = { "shanghai", 31 + 10/60., 121 + 28/60. };
	City buenosaires = { "buenos aires", -34 + 35/60., -58 + 22/60. };
	City melbourne = { "melbourne" , -37 + 47/60., 144 + 58/60. };
	City detroit = { "detroit", 42 + 19/60., -83 + 2 / 60. };

	cities.push_back( newyork );
	cities.push_back( tokyo );
	cities.push_back( london );
	cities.push_back( shanghai );
	cities.push_back( buenosaires );
	cities.push_back( melbourne );
	cities.push_back( detroit );
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofPushMatrix();

	//translate so that the center of the screen is 0,0
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	ofSetColor(255, 255, 255, 20);

	//draw a translucent wireframe sphere (ofNoFill() is on)
	ofPushMatrix();
	//add an extra spin at the rate of 1 degree per frame
	ofRotateDeg(ofGetFrameNum(), 0, 1, 0);
	ofDrawSphere(0, 0, 0, 300);
	ofPopMatrix();

	ofSetColor(255);
	for(unsigned int i = 0; i < cities.size(); i++){

		//three rotations
		//two to represent the latitude and lontitude of the city
		//a third so that it spins along with the spinning sphere
		glm::quat latRot, longRot, spinQuat;
		latRot = glm::angleAxis(ofDegToRad(cities[i].latitude), glm::vec3(1, 0, 0));
		longRot = glm::angleAxis(ofDegToRad(cities[i].longitude), glm::vec3(0, 1, 0));
		
		spinQuat = glm::angleAxis(ofDegToRad(ofGetFrameNum()), glm::vec3(0, 1, 0));

		//our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
		glm::vec3 center = glm::vec3(0,0,300);
		//multiplying a quat with another quat combines their rotations into one quat
		//multiplying a quat to a vector applies the quat's rotation to that vector
		//so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
		glm::vec3 worldPoint =  spinQuat * longRot * latRot * center;

		//draw it and label it
		ofDrawLine(glm::vec3(0,0,0), worldPoint);

		//set the bitmap text mode billboard so the points show up correctly in 3d
		ofDrawBitmapString(cities[i].name, worldPoint );
	}

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
