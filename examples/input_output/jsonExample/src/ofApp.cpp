#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(255);
	ttf.load("mono.ttf", 8);
	path.setStrokeColor(0);
	path.setFilled(false);
	path.setStrokeWidth(1);

	ofFile file("drawing.json");
	if(file.exists()){
		file >> js;
		for(auto & stroke: js){
			if(!stroke.empty()){
				path.moveTo(stroke[0]["x"], stroke[0]["y"]);
				for(auto & p: stroke){
					path.lineTo(p["x"], p["y"]);
				}
			}
		}
		calculateText();
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	path.draw();

	ofSetColor(0);
	ofDrawRectangle(0,0,200,ofGetHeight());
	ttf.drawString("'s' saves to json", 210, 15);

	ofSetColor(255);
	ttf.drawString(text, 20, 20);

}

//--------------------------------------------------------------
void ofApp::calculateText(){
	auto total = js;
	if(!stroke.empty()){
		total.push_back(stroke);
	}
	size_t numlines = ofGetHeight() / ttf.getLineHeight();
	auto lines = ofSplitString(total.dump(2), "\n");
	std::vector<std::string> screenlines(lines.end() - std::min(lines.size(), numlines), lines.end());
	text = ofJoinString(screenlines, "\n");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 's'){
		ofSaveJson("drawing.json", js);
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
	ofJson pt;
	pt["x"] = x;
	pt["y"] = y;
	stroke.push_back(pt);
	path.lineTo(x,y);
	calculateText();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	path.moveTo(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	js.push_back(stroke);
	stroke.clear();
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
