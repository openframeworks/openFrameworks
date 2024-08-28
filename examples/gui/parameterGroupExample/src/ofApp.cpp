#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	renderer1.setup("renderer1");
	renderer2.setup("renderer2");

	parameters.setName("settings");
	parameters.add(vSync.set("vSync",true));
	vSync.addListener(this, &ofApp::vSyncChanged);
	parameters.add(renderer1.parameters);
	parameters.add(renderer2.parameters);

	gui.setup(parameters);

	gui.loadFromFile("settings.xml");

	font.load("frabk.ttf",9,true,true);
	ofEnableAlphaBlending();
}

void ofApp::vSyncChanged(bool & vSync){
	ofSetVerticalSync(vSync);
	ofLogNotice() << "vSync changed: " << vSync;
}


//--------------------------------------------------------------
void ofApp::update(){
	// frameNum is a readonly parameter so this will fail to compile
	// unless we are inside the CirclesRenderer class
	// renderer.frameNum = 5;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	renderer1.draw();
	renderer2.draw();
	ofSetColor(255);
	gui.draw();
	font.drawString("frame: " + ofToString(renderer1.frameNum),ofGetWidth()-150,20);
	font.drawString("fps: " + ofToString((int)ofGetFrameRate()),ofGetWidth()-150,40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key=='s'){
		ofSerialize(settings,parameters);
		settings.save("settings.xml");
	}
	if(key=='l'){
		settings.load("settings.xml");
		ofDeserialize(settings, parameters);
	}
	if(key=='o'){
		cout << renderer1.parameters;
		cout << renderer2.parameters;
	}
	if(key=='r'){
		renderer1.color = ofColor(127);
		renderer2.color = ofColor(127);
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
