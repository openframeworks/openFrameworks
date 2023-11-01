#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	snapCounter = 0;
	bSnapshot = false;
	cooper.load("cooperBlack.ttf", 50);
	phase = 0;
}


//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(255,255,255);
}


//--------------------------------------------------------------
void ofApp::draw(){

	phase += 0.35f;

	ofSetHexColor(0xDDDDFF);
	ofDrawRectangle(200,200,300,180);


	ofSetColor(255,255,255,128);
	ofPushMatrix();
		float width = cooper.stringWidth("catch me\nif you can!");
		ofTranslate(350,290,0);
		ofRotateDeg(phase*3, 0,0,1);
		ofScale(1 + 0.5f * sin(phase/10.0f), 1 + 0.5f * sin(phase/10.0f), 1);
		cooper.drawString("catch me\nif you can!", -width/2,20);
	ofPopMatrix();

	ofSetColor(255,150,140,128);
	ofPushMatrix();
		ofTranslate(330,280,0);
		ofRotateDeg(phase*5, 0,0,1);
		ofDrawRectangle(-25,-25,50,50);
	ofPopMatrix();

	if (bSnapshot == true){
		// grab a rectangle at 200,200, width and height of 300,180
		img.grabScreen(200,200,300,180);
		
		string fileName = "snapshot_"+ofToString(snapCounter, 5, '0')+".png";
		img.save(fileName);
		snapString = "saved " + fileName;
		snapCounter++;
		bSnapshot = false;
	}

	ofSetHexColor(0x000000);
	ofDrawBitmapString("press 'x' to capture screen \n", 200,460);
	ofDrawBitmapString(snapString, 600,460);


	ofSetHexColor(0xFFFFFF);
	if(snapCounter > 0) {
		img.draw(600,200,300,180);
	}


}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if (key == 'x'){
		bSnapshot = true;
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
