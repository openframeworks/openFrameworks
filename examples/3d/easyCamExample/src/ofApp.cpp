#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	// this uses depth information for occlusion
	// rather than always drawing things on top of each other
	ofEnableDepthTest();

	ofSetCircleResolution(64);
	bHelpText = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(20);
	cam.begin();
	ofSetConeResolution(20, 2);
	ofSetCylinderResolution(20, 2);
	ofEnableDepthTest();
	ofSetColor(ofColor::red);//RIGHT
	ofDrawCone(100, 0, 0, 50, 100);

	ofSetColor(ofColor::white);//LEFT
	ofDrawSphere(-100, 0, 0, 50);

	ofSetColor(ofColor::blue);//BOTTOM
	ofDrawBox(0, 100, 0, 100);

	ofSetColor(ofColor::cyan);//TOP
	ofDrawCylinder(0, -100, 0, 50, 100);

	ofSetColor(ofColor::yellow);//FRONT
	ofDrawBox(0, 0, 100, 100);

	ofSetColor(ofColor::magenta);//BACK
	ofDrawBox(0, 0, -100, 100);

	ofDrawGrid(20,10,true,true,true,true);
	ofDisableDepthTest();
	cam.end();
	drawInteractionArea();
	ofSetColor(255);

	if (bHelpText) {
		stringstream ss;
		ss << "FPS: " << ofToString(ofGetFrameRate(),0) <<endl<<endl;
		ss << "MODE: " << (cam.getOrtho()?"ORTHO":"PERSPECTIVE")<<endl;
		ss << "MOUSE INPUT ENABLED: " << (cam.getMouseInputEnabled()?"TRUE":"FALSE")<<endl;
		ss << "INERTIA ENABLED: " << (cam.getInertiaEnabled()?"TRUE":"FALSE")<<endl;
		ss << "ROTATION RELATIVE Y AXIS: " << (cam.getRelativeYAxis()?"TRUE":"FALSE")<<endl;
		ss << endl;
		ss << "Toogle camera projection mode (ORTHO or PERSPECTIVE):"<< endl;
		ss << "    press space bar."<< endl;
		ss << "Toggle mouse input:"<<endl;
		ss << "    press 'c' key."<< endl;
		ss << "Toggle camera inertia:"<<endl;
		ss << "    press 'i' key."<< endl;
		ss << "Toggle rotation relative Y axis:"<<endl;
		ss << "    press 'y' key."<< endl;
		ss << "Toggle this help:"<<endl;
		ss << "    press 'h' key."<< endl;
		ss << endl;
		ss << "camera x,y rotation:" <<endl;
		ss << "    LEFT MOUSE BUTTON DRAG inside yellow circle"<<endl;
		ss << endl;
		ss << "camera z rotation or roll"<<endl;
		ss << "    LEFT MOUSE BUTTON DRAG outside yellow circle"<<endl;

		ss << endl;
		ss << "move over x,y axis / truck and boom:"<<endl;
		ss << "    LEFT MOUSE BUTTON DRAG + m"<<endl;
		ss << "    MIDDLE MOUSE BUTTON PRESS"<<endl;
		ss << endl;
		ss << "move over z axis / dolly / zoom in or out:"<<endl;
		ss << "    RIGHT MOUSE BUTTON DRAG"<<endl;
		ss << "    VERTICAL SCROLL"<<endl<<endl;
		if (cam.getOrtho()) {
			ss << "    Notice that in ortho mode zoom will be centered at the mouse position." << endl;
		}
		ofDrawBitmapString(ss.str().c_str(), 20, 20);
	}

}
//--------------------------------------------------------------
void ofApp::drawInteractionArea(){
	ofRectangle vp = ofGetCurrentViewport();
	float r = std::min<float>(vp.width, vp.height) * 0.5f;
	float x = vp.width * 0.5f;
	float y = vp.height * 0.5f;

	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255, 255, 0);
	ofNoFill();
	glDepthMask(false);
	ofDrawCircle(x, y, r);
	glDepthMask(true);
	ofPopStyle();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
	  case ' ':
			cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
			break;
		case 'C':
		case 'c':
			cam.getMouseInputEnabled() ? cam.disableMouseInput() : cam.enableMouseInput();
			break;
		case 'F':
		case 'f':
			ofToggleFullscreen();
			break;
		case 'H':
		case 'h':
			bHelpText ^=true;
			break;
		case 'I':
		case 'i':
			cam.getInertiaEnabled() ? cam.disableInertia() : cam.enableInertia();
			break;
		case 'Y':
		case 'y':
			cam.setRelativeYAxis(!cam.getRelativeYAxis());
			break;
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
