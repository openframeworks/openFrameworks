#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	ofBackground(0);
	ofSetColor(255);

	svg.load("tiger.svg");
	for (ofPath p: svg.getPaths()){
		// svg defaults to non zero winding which doesn't look so good as contours
		p.setPolyWindingMode(OF_POLY_WINDING_ODD);
		const vector<ofPolyline>& lines = p.getOutline();
		for(const ofPolyline & line: lines){
			outlines.push_back(line.getResampledBySpacing(1));
		}
	}
}


//--------------------------------------------------------------
void ofApp::update(){
	step += 0.001;
	if (step > 1) {
		step -= 1;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofRotateDeg(mouseX);
	float scale = ofMap(mouseY, 0, ofGetHeight(), .5, 10);
	ofScale(scale, scale, scale);
	ofTranslate(-250, -250);
	if(ofGetMousePressed()) {
		ofNoFill();
		for (ofPolyline & line: outlines){
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
