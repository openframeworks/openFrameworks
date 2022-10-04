#include "ofApp.h"

/* Refer to the README.md in the example's root folder for more information on usage */

//--------------------------------------------------------------
void ofApp::setup(){
	light.setup();
	light.setPosition(-100, 200,100);
	light.setAmbientColor(ofFloatColor(0.4, 1.0));
	// we set up a plane on which the truck will move
	plane.set(10000, 10000);
	plane.rotateDeg(270, 1, 0 , 0);
	plane.move(0, -49, 0);

	ofFloatColor roadColor(0.7, 0.7);
	roadMaterial.setAmbientColor(roadColor);
	roadColor.a = 1.0;
	roadMaterial.setDiffuseColor(roadColor);
	roadMaterial.setShininess(0.01);
	truck.setup();

	// add the parameters from the truck
	gui.setup(truck.params);
	gui.loadFromFile("settings.xml");

	//Set our camera up in a nice location to view our awesome car
	cam.setPosition(-965, 586, -1084);
	cam.lookAt(truck.getNode(), {0.f, 1.f, 0.f});
	cam.setFarClip(10000);

}

//--------------------------------------------------------------
void ofApp::update(){
	truck.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	// draw a gradient in the background
	ofBackgroundGradient(ofColor(10), ofColor(50));

	ofEnableDepthTest();
	cam.begin();
		roadMaterial.begin();
			plane.draw();
		roadMaterial.end();
		truck.draw();
	cam.end();

	ofDisableDepthTest();

	gui.draw();
	ofDrawBitmapStringHighlight("Use your Arrow keys to move your \"truck\" around the scene. \nClick and drag your mouse to move the 3D scene.\nPress 'r' to reset the position", ofGetWidth()/2,20);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == 'r' ){
		truck.resetPosition();
	}
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
