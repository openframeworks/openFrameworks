#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	ofBackground(0);
	ofSetColor(255);
	
	svg.load("tiger.svg");
	for (int i = 0; i < svg.getNumPath(); i++){
		ofPath p = svg.getPathAt(i);
		// svg defaults to non zero winding which doesn't look so good as contours
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		vector<ofPolyline>& lines = p.getOutline();
		for(int j=0;j<(int)lines.size();j++){
			outlines.push_back(lines[j].getResampledBySpacing(1));
		}
	}
}


//--------------------------------------------------------------
void testApp::update(){
	step += 0.001;
	if (step > 1) {
		step -= 1;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotate(mouseX);
	float scale = ofMap(mouseY, 0, ofGetHeight(), .5, 10);
	ofScale(scale, scale, scale);
	ofTranslate(-250, -250);
	if(ofGetMousePressed()) {
		ofNoFill();
		for (int i = 0; i < (int)outlines.size(); i++){
			ofPolyline & line = outlines[i];

			int num = step * line.size();

			ofBeginShape();
			for (int j = 0; j < num; j++){
				ofVertex(line[j]);
			}
			ofEndShape();
		}
	} else {
		svg.draw();
	}
	
	ofPopMatrix();
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
